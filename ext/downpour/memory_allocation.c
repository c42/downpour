#include "downpour.h"

typedef struct DownpourWrapper {
  int reference_count;
  void *ptr;
  struct DownpourWrapper *parent;
  FREE_METHOD free_method;
  VALUE rb_object;
} DownpourWrapper;

static void downpour_mark(DownpourWrapper *wrapper)
{
  if(wrapper == NULL)
    return;

  wrapper->reference_count++;
}

static void downpour_release(DownpourWrapper *wrapper)
{
  if(wrapper == NULL)
    return;

  wrapper->reference_count--;

  if(wrapper->reference_count == 0) {
    wrapper->free_method(wrapper->ptr);
    downpour_release(wrapper->parent);
    free(wrapper);
  }
}

static DownpourWrapper *downpour_wrap_pointer(void *ptr, DownpourWrapper *parent, FREE_METHOD free_method)
{
  DownpourWrapper *wrapper = drizzle_alloc(DownpourWrapper);
  wrapper->ptr = ptr;
  wrapper->parent = parent;
  wrapper->free_method = free_method;
  wrapper->reference_count = 0;

  downpour_mark(parent);
  downpour_mark(wrapper);
  return wrapper;
}

static void mark_for_ruby_gc(DownpourWrapper *wrapper)
{
  if(wrapper == NULL)
    return;

  rb_gc_mark(wrapper->rb_object);

  mark_for_ruby_gc(wrapper->parent);
}

static DownpourWrapper *get_wrapper_from_object(VALUE value)
{
  if(value == Qnil)
    return NULL;

  DownpourWrapper *wrapper = NULL;
  Data_Get_Struct(value, DownpourWrapper, wrapper);
  return wrapper;
}

VALUE downpour_get_ruby_object(void *ptr)
{
  if(ptr == NULL)
    return Qnil;

  DownpourWrapper *wrapper = (DownpourWrapper *)ptr;
  return wrapper->rb_object;
}

void *downpour_from_ruby_object(VALUE value)
{
  DownpourWrapper *wrapper;
  wrapper = get_wrapper_from_object(value);
  return wrapper == NULL? NULL : wrapper->ptr;
}

VALUE downpour_to_ruby_object(void *ptr, VALUE klass, VALUE parent, FREE_METHOD free_method, SET_CONTEXT set_context)
{
  if(ptr == NULL)
    return Qnil;

  DownpourWrapper *wrapper = downpour_wrap_pointer(ptr, get_wrapper_from_object(parent), free_method);

  if(set_context != NULL)
    set_context(ptr, wrapper);

  return wrapper->rb_object = Data_Wrap_Struct(klass, mark_for_ruby_gc, downpour_release, wrapper);
}

VALUE downpour_get_parent(VALUE self)
{
  DownpourWrapper *wrapper = get_wrapper_from_object(self);

  if(wrapper == NULL || wrapper->parent == NULL)
    return Qnil;

  return wrapper->parent->rb_object;
}
