#include "downpour.h"
#include "result.h"

#define SELF_TYPE drizzle_result_st
#define RUBY_CLASS DrizzleResult

#define attr(foo, conversion) static VALUE attr_##foo(VALUE self)\
{\
  read_self_ptr();\
  return conversion(drizzle_result_##foo(self_ptr));\
}

static VALUE to_rb_string(char *str)
{
  return str == NULL ? Qnil : rb_str_new2(str);
}

static VALUE to_results_array(char **array, long count)
{
  VALUE ary = rb_ary_new2(count);

  long i;
  for(i = 0; i < count; i++)
    rb_ary_push(ary, to_rb_string(array[i]));

  return ary;
}

VALUE downpour_wrap_row(VALUE self, drizzle_result_st *self_ptr, drizzle_row_t row)
{
  // No more rows to read :-)
  if(row == NULL)
    return Qnil;

  return to_results_array(row, drizzle_result_column_count(self_ptr));
}

bool downpour_is_buffered(drizzle_result_st *self_ptr)
{
  return self_ptr->options & (DRIZZLE_RESULT_BUFFER_ROW);
}

static VALUE is_buffered(VALUE self)
{
  read_self_ptr();

  return downpour_is_buffered(self_ptr) ? Qtrue : Qfalse;
}

static void buffer_column_if_needed(drizzle_result_st *self_ptr)
{
  if(self_ptr->options & DRIZZLE_RESULT_BUFFER_COLUMN)
    return;
  CHECK_OK(drizzle_column_buffer(self_ptr));
}

static drizzle_column_st *next_column(drizzle_result_st *self_ptr)
{
  return drizzle_column_next(self_ptr);
}

typedef struct ResultExtraInfo {
  drizzle_column_st **columns;
} ResultExtraInfo;

static ResultExtraInfo *extra_info(drizzle_result_st *self_ptr)
{
  buffer_column_if_needed(self_ptr);

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
attr(insert_id, UINT2NUM);
attr(error_code, UINT2NUM);
attr(affected_rows, UINT2NUM);
attr(warning_count, UINT2NUM);
attr_string(sqlstate);
attr_string(info);
attr_string(error);

VALUE downpour_include_result_module(drizzle_result_st *self_ptr, VALUE result)
{
  ResultExtraInfo *info = extra_info(self_ptr);
  set_extra_pointer(result, info, free_extra_info);
  rb_iv_set(result, "@columns", get_columns_in_array(self_ptr, result, info));
  return result;
}

void init_drizzle_result()
{
  DrizzleResult = rb_define_module_under(DownpourModule, "Result");
  rb_define_method(DrizzleResult, "buffered?", is_buffered, 0);
  define_attr(column_count);
  define_attr(insert_id);
  define_attr(error_code);
  define_attr(affected_rows);
  define_attr(warning_count);
  define_attr(sqlstate);
  define_attr(info);
  define_attr(error);
}
