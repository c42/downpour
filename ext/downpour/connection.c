#include "downpour.h"

#define SELF_TYPE drizzle_con_st

static VALUE query(VALUE self, VALUE query)
{
  read_self_ptr();

  Check_Type(query, T_STRING);
  
  drizzle_return_t retptr;
  drizzle_result_st *result = drizzle_query(self_ptr, NULL, RSTRING_PTR(query), RSTRING_LEN(query), &retptr);

  CHECK_OK(retptr);

  return downpour_result_constructor(result, self);
}

VALUE downpour_connection_constructor(drizzle_con_st *self_ptr, VALUE status)
{
  VALUE ret = Data_Wrap_Struct(DrizzleConnection, NULL, drizzle_con_free, self_ptr);
  rb_iv_set(ret, "@status", status);
  return ret;
}

void init_drizzle_connection()
{
  DrizzleConnection = drizzle_gem_create_class_with_private_constructor("Connection", rb_cObject);
  rb_define_method(DrizzleConnection, "query", query, 1);
}
