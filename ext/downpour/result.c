#include "downpour.h"

#define SELF_TYPE drizzle_result_st

static uint64_t do_column_count(drizzle_result_st *self_ptr)
{
  return drizzle_result_column_count(self_ptr);
}

static bool is_buffered_bool(VALUE self)
{
  read_self_ptr();
  return self_ptr->options & (DRIZZLE_RESULT_BUFFER_ROW);
}

static VALUE is_buffered(VALUE self)
{
  return is_buffered_bool(self) ? Qtrue : Qfalse;
}

static VALUE buffer_if_needed(VALUE self)
{
  read_self_ptr();

  // Only buffer once
  if(is_buffered_bool(self))
    return Qfalse;

  CHECK_OK(drizzle_result_buffer(self_ptr));
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

static void buffer_column_if_needed(drizzle_result_st *self_ptr)
{
  if(self_ptr->options & DRIZZLE_RESULT_BUFFER_COLUMN)
    return;
  CHECK_OK(drizzle_column_buffer(self_ptr));
}

static drizzle_row_t do_drizzle_row_buffer(drizzle_result_st *self_ptr)
{
  drizzle_return_t ret;
  drizzle_row_t result = drizzle_row_buffer(self_ptr, &ret);
  CHECK_OK(ret);
  return result;
}

static VALUE next_row_unbuffered(drizzle_result_st *self_ptr)
{
  buffer_column_if_needed(self_ptr);
  drizzle_row_t result = do_drizzle_row_buffer(self_ptr);
  VALUE parsed = wrap_row(self_ptr, result);
  if(result)
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

static VALUE insert_id(VALUE self)
{
  read_self_ptr();

  return UINT2NUM(drizzle_result_insert_id(self_ptr));
}

static VALUE error_code(VALUE self)
{
  read_self_ptr();
  return UINT2NUM(drizzle_result_error_code(self_ptr));
}

VALUE downpour_result_constructor(drizzle_result_st *self_ptr, VALUE connection)
{
  return downpour_to_ruby_object(self_ptr, DrizzleResult, connection, drizzle_result_free);
}

void init_drizzle_result()
{
  DrizzleResult = drizzle_gem_create_class_with_private_constructor("Result", rb_cObject);
  rb_define_method(DrizzleResult, "row_count", row_count, 0);
  rb_define_method(DrizzleResult, "column_count", column_count, 0);
  rb_define_method(DrizzleResult, "buffer!", buffer_if_needed, 0);
  rb_define_method(DrizzleResult, "buffered?", is_buffered, 0);
  rb_define_method(DrizzleResult, "next_row", next_row, 0);
  rb_define_method(DrizzleResult, "insert_id", insert_id, 0);
  rb_define_method(DrizzleResult, "error_code", error_code, 0);
}
