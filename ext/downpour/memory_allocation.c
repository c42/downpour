#include "downpour.h"

typedef struct DownpourWrapper {
  int reference_count;
  void *ptr;
  struct DownpourWrapper *parent;
  void (*free_method)(void *ptr);
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
    free_method(wrapper->ptr);
    downpour_release(wrapper->parent);
    free(wrapper);
  }
}

static DownpourWrapper *downpour_wrap_pointer(void *ptr, DownpourWrapper *parent, void (*free_method)(void *ptr))
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
  convert_to_struct(DownpourWrapper, wrapper, value);
  return wrapper->ptr;
}

VALUE downpour_to_ruby_object(void *ptr, VALUE klass, VALUE parent, void (*free_method)(void *ptr))
{
  DownpourWrapper *parent_ptr = NULL;
  if(parent != Qnil)
    Data_Get_Struct(parent, DownpourWrapper, parent_ptr);
  DownpourWrapper *wrapper = downpour_wrap_pointer(ptr, parent_ptr, free_method);
  return Data_Wrap_Struct(klass, NULL, downpour_release, wrapper);
}
