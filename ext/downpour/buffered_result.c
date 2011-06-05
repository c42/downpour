#include "downpour.h"
#include "result.h"

#define SELF_TYPE drizzle_result_st
#define RUBY_CLASS DrizzleBufferedResult

#define attr(foo, conversion) static VALUE attr_##foo(VALUE self)\
{\
  read_self_ptr();\
  return conversion(drizzle_result_##foo(self_ptr));\
}

static void buffer_if_needed(drizzle_result_st *self_ptr)
{
  // Only buffer once
  if(downpour_is_buffered(self_ptr))
    return;

  CHECK_OK(drizzle_result_buffer(self_ptr));
}

static VALUE wrap_row(drizzle_result_st *self_ptr, drizzle_row_t row)
{
  // No more rows to read :-)
  if(row == NULL)
    return Qnil;

  return drizzle_gem_to_string_array(row, drizzle_result_column_count(self_ptr));
}

static VALUE next_row(VALUE self)
{
  read_self_ptr();

  drizzle_row_t result = drizzle_row_next(self_ptr);
  return wrap_row(self_ptr, result);
}

attr(row_count, UINT2NUM);

VALUE downpour_result_constructor(drizzle_result_st *self_ptr, VALUE connection)
{
  buffer_if_needed(self_ptr);
  VALUE ret = to_ruby_object(self_ptr, DrizzleBufferedResult, connection, drizzle_result_free, NULL);
  return downpour_include_result_module(self_ptr, ret);
}

void init_drizzle_buffered_result()
{
  DrizzleBufferedResult = drizzle_gem_create_class_with_private_constructor("BufferedResult", rb_cObject);
  rb_include_module(DrizzleBufferedResult, DrizzleResult);
  rb_define_method(DrizzleBufferedResult, "next_row", next_row, 0);
  define_attr(row_count);
}
