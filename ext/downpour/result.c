#include "drizzle.h"

#define SELF_TYPE drizzle_result_st

static uint64_t do_column_count(drizzle_result_st *self_ptr)
{
  return drizzle_result_column_count(self_ptr);
}

static VALUE is_buffered(VALUE self)
{
  VALUE buffered = rb_iv_get(self, "@buffered");

  if(buffered == Qnil || buffered == Qfalse)
    return Qfalse;

  return buffered;
}

static bool is_buffered_bool(VALUE self)
{
  return RTEST(rb_call(self, "buffered?"));
}

static VALUE buffer_if_needed(VALUE self)
{
  read_self_ptr();

  // Only buffer once
  if(is_buffered_bool(self))
    return Qfalse;

  CHECK_OK(drizzle_result_buffer(self_ptr));
  rb_iv_set(self, "@buffered", Qtrue);
  return Qtrue;
}

static VALUE row_count(VALUE self)
{
  rb_call(self, "buffer!");
  read_self_ptr();
  return UINT2NUM(drizzle_result_row_count(self_ptr));
}

static VALUE wrap_row(drizzle_result_st *self_ptr, drizzle_row_t row)
{
  // No more rows to read :-)
  if(row == NULL)
    return Qnil;

  return drizzle_gem_to_string_array(row, do_column_count(self_ptr));
}

static VALUE next_row_buffered(drizzle_result_st *self_ptr)
{
  drizzle_row_t result = drizzle_row_next(self_ptr);
  return wrap_row(self_ptr, result);
}

static VALUE next_row_unbuffered(drizzle_result_st *self_ptr)
{
  drizzle_return_t ret;
  drizzle_row_t result = drizzle_row_buffer(self_ptr, &ret);
  CHECK_OK(ret);
  VALUE parsed = wrap_row(self_ptr, result);
  drizzle_row_free(self_ptr, result);
  return parsed;
}

static VALUE next_row(VALUE self)
{
  read_self_ptr();

  if(is_buffered_bool(self))
    return next_row_buffered(self_ptr);

  return next_row_unbuffered(self_ptr);
}

static VALUE column_count(VALUE self)
{
  read_self_ptr();
  return UINT2NUM(do_column_count(self_ptr));
}

static void downpour_result_destructor(drizzle_result_st *self_ptr)
{
  drizzle_result_free(self_ptr);
  free(self_ptr);
}

VALUE downpour_result_constructor(drizzle_result_st *self_ptr)
{
  return Data_Wrap_Struct(DrizzleResult, NULL, NULL, self_ptr);
}

void init_drizzle_result()
{
  DrizzleResult = drizzle_gem_create_class_with_private_constructor("Result", rb_cObject);
  rb_define_method(DrizzleResult, "row_count", row_count, 0);
  rb_define_method(DrizzleResult, "column_count", column_count, 0);
  rb_define_method(DrizzleResult, "buffer!", buffer_if_needed, 0);
  rb_define_method(DrizzleResult, "buffered?", is_buffered, 0);
  rb_define_method(DrizzleResult, "next_row", next_row, 0);
}
