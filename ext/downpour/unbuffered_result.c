#include "downpour.h"
#include "result.h"

#define SELF_TYPE drizzle_result_st
#define RUBY_CLASS DrizzleUnBufferedResult

#define attr(foo, conversion) static VALUE attr_##foo(VALUE self)\
{\
  read_self_ptr();\
  return conversion(drizzle_result_##foo(self_ptr));\
}

static drizzle_row_t do_drizzle_row_buffer(drizzle_result_st *self_ptr)
{
  drizzle_return_t ret;
  drizzle_row_t result = drizzle_row_buffer(self_ptr, &ret);
  CHECK_OK(ret);
  return result;
}

static VALUE next_row(VALUE self)
{
  read_self_ptr();

  drizzle_row_t result = do_drizzle_row_buffer(self_ptr);
  VALUE parsed = downpour_wrap_row(self_ptr, result);
  if(result)
    drizzle_row_free(self_ptr, result);
  return parsed;
}

// Improve this method
static VALUE end_result(VALUE self)
{
  read_self_ptr();
  while(do_drizzle_row_buffer(self_ptr) != NULL)
    ; // Do Nothing
  return Qnil;
}

VALUE downpour_unbuffered_result_constructor(drizzle_result_st *self_ptr, VALUE connection)
{
  VALUE ret = to_ruby_object(self_ptr, DrizzleUnBufferedResult, connection, drizzle_result_free, NULL);
  return downpour_include_result_module(self_ptr, ret);
}

void init_drizzle_unbuffered_result()
{
  DrizzleUnBufferedResult = drizzle_gem_create_class_with_private_constructor("UnBufferedResult", rb_cObject);
  rb_include_module(DrizzleUnBufferedResult, DrizzleResult);
  rb_define_method(DrizzleUnBufferedResult, "next_row", next_row, 0);
  rb_define_method(DrizzleUnBufferedResult, "close!", end_result, 0);
}
