#include "drizzle.h"

#define SELF_TYPE drizzle_con_st

static VALUE query(VALUE self, VALUE query)
{
  read_self_ptr();

  Check_Type(query, T_STRING);
  
  drizzle_return_t retptr;
  drizzle_result_st *result = drizzle_query(self_ptr, NULL, RSTRING_PTR(query), RSTRING_LEN(query), &retptr);

  CHECK_OK(retptr);

  return Data_Wrap_Struct(DrizzleResult, NULL, NULL, result);
}

void init_drizzle_connection()
{
  DrizzleConnection = drizzle_gem_create_class_with_private_constructor("Connection", rb_cObject);
  rb_define_method(DrizzleConnection, "query", query, 1);
}
