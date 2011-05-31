#include "downpour.h"

#define SELF_TYPE drizzle_con_st
#define attr(foo) static VALUE attr_##foo(VALUE self)\
{\
  read_self_ptr();\
  return rb_str_new2(drizzle_con_##foo(self_ptr));\
}
#define settr(foo) static VALUE settr_##foo(VALUE self, VALUE newValue)\
{\
  read_self_ptr();\
  Check_Type(newValue, T_STRING);\
  drizzle_con_set_##foo(self_ptr, RSTRING_PTR(newValue));\
  return newValue;\
}
#define prop(foo) attr(foo) settr(foo)

#define define_attr(foo) rb_define_method(DrizzleConnection, #foo, attr_##foo, 0)
#define define_settr(foo) rb_define_method(DrizzleConnection, #foo "=", settr_##foo, 1)
#define define_prop(foo) define_attr(foo); define_settr(foo)

static VALUE query(VALUE self, VALUE query)
{
  read_self_ptr();

  Check_Type(query, T_STRING);
  
  drizzle_return_t retptr;
  drizzle_result_st *result = drizzle_query(self_ptr, NULL, RSTRING_PTR(query), RSTRING_LEN(query), &retptr);

  CHECK_OK(retptr);

  return downpour_result_constructor(result, self);
}

static VALUE attr_port(VALUE self)
{
  read_self_ptr();
  return UINT2NUM(drizzle_con_port(self_ptr));
}

prop(db);
attr(user);
attr(password);
attr(host);
prop(uds);

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
  rb_define_method(DrizzleConnection, "port", attr_port, 0);
  define_prop(db);
  define_attr(user);
  define_attr(password);
  define_attr(host);
  define_prop(uds);
}
