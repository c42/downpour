#include "downpour.h"

typedef struct DownpourWrapper {
  int reference_count;
  void *ptr;
  struct DownpourWrapper *parent;
  FREE_METHOD free_method;
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

void *downpour_from_ruby_object(VALUE value)
{
  DownpourWrapper *wrapper = NULL;
  Data_Get_Struct(value, DownpourWrapper, wrapper);
  return wrapper->ptr;
}

VALUE downpour_to_ruby_object(void *ptr, VALUE klass, VALUE parent, FREE_METHOD free_method, SET_CONTEXT set_context)
{
  DownpourWrapper *parent_ptr = NULL;
  if(parent != Qnil)
    Data_Get_Struct(parent, DownpourWrapper, parent_ptr);
  DownpourWrapper *wrapper = downpour_wrap_pointer(ptr, parent_ptr, free_method);

  if(set_context != NULL)
    set_context(ptr, wrapper);

  return Data_Wrap_Struct(klass, NULL, downpour_release, wrapper);
}
