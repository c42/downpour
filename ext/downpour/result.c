#include "downpour.h"

#define SELF_TYPE drizzle_result_st
#define RUBY_CLASS DrizzleResult

#define attr(foo, conversion) static VALUE attr_##foo(VALUE self)\
{\
  read_self_ptr();\
  return conversion(drizzle_result_##foo(self_ptr));\
}

static uint64_t do_column_count(drizzle_result_st *self_ptr)
{
  return drizzle_result_column_count(self_ptr);
}

static bool is_buffered(drizzle_result_st *self_ptr)
{
  return self_ptr->options & (DRIZZLE_RESULT_BUFFER_ROW);
}

static void buffer_if_needed(drizzle_result_st *self_ptr)
{
  // Only buffer once
  if(is_buffered(self_ptr))
    return;

  CHECK_OK(drizzle_result_buffer(self_ptr));
}

static VALUE wrap_row(drizzle_result_st *self_ptr, drizzle_row_t row)
{
  // No more rows to read :-)
  if(row == NULL)
    return Qnil;

  return drizzle_gem_to_string_array(row, do_column_count(self_ptr));
}

static void buffer_column_if_needed(drizzle_result_st *self_ptr)
{
  if(self_ptr->options & DRIZZLE_RESULT_BUFFER_COLUMN)
    return;
  CHECK_OK(drizzle_column_buffer(self_ptr));
}

static VALUE next_row(VALUE self)
{
  read_self_ptr();

  drizzle_row_t result = drizzle_row_next(self_ptr);
  return wrap_row(self_ptr, result);
}

static drizzle_column_st *next_column(drizzle_result_st *self_ptr)
{
  buffer_column_if_needed(self_ptr);
  return drizzle_column_next(self_ptr);
}

typedef struct ResultExtraInfo {
  drizzle_column_st **columns;
} ResultExtraInfo;

static ResultExtraInfo *extra_info(drizzle_result_st *self_ptr)
{
  ResultExtraInfo *result = drizzle_alloc(ResultExtraInfo);
  int i, number_of_columns = drizzle_result_column_count(self_ptr);
  result->columns = malloc(sizeof(drizzle_column_st *) * (number_of_columns + 1));

  for (i = 0; i < number_of_columns; i++) {
    result->columns[i] = next_column(self_ptr);
  }
  result->columns[number_of_columns] = NULL;
  return result;
}

static void free_extra_info(ResultExtraInfo *extra_info)
{
  int i;
  for (i = 0; extra_info->columns[i] != NULL; i++) {
    drizzle_column_free(extra_info->columns[i]);
  }
  free(extra_info->columns);
  free(extra_info);
}

static VALUE get_columns_in_array(drizzle_result_st *self_ptr, VALUE self, ResultExtraInfo *extra_info)
{
  int i, number_of_columns = drizzle_result_column_count(self_ptr);
  VALUE array = rb_ary_new2(number_of_columns);

  for (i = 0; i < number_of_columns; i++)
    rb_ary_push(array, downpour_column_constructor(extra_info->columns[i], self));

  return array;
}

attr(column_count, UINT2NUM);
attr(row_count, UINT2NUM);
attr(insert_id, UINT2NUM);
attr(error_code, UINT2NUM);
attr(affected_rows, UINT2NUM);
attr(warning_count, UINT2NUM);
attr_string(sqlstate);
attr_string(info);
attr_string(error);

VALUE downpour_result_constructor(drizzle_result_st *self_ptr, VALUE connection)
{
  buffer_if_needed(self_ptr);
  VALUE ret = to_ruby_object(self_ptr, DrizzleResult, connection, drizzle_result_free, NULL);
  ResultExtraInfo *info = extra_info(self_ptr);
  set_extra_pointer(ret, info, free_extra_info);
  rb_iv_set(ret, "@columns", get_columns_in_array(self_ptr, ret, info));
  return ret;
}

void init_drizzle_result()
{
  DrizzleResult = drizzle_gem_create_class_with_private_constructor("Result", rb_cObject);
  rb_define_method(DrizzleResult, "next_row", next_row, 0);
  define_attr(row_count);
  define_attr(column_count);
  define_attr(insert_id);
  define_attr(error_code);
  define_attr(affected_rows);
  define_attr(warning_count);
  define_attr(sqlstate);
  define_attr(info);
  define_attr(error);
}
