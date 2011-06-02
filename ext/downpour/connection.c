#include "downpour.h"

#define SELF_TYPE drizzle_con_st
#define attr(foo, conversion) static VALUE attr_##foo(VALUE self)\
{\
  read_self_ptr();\
  return conversion(drizzle_con_##foo(self_ptr));\
}
#define settr(foo) static VALUE settr_##foo(VALUE self, VALUE newValue)\
{\
  read_self_ptr();\
  Check_Type(newValue, T_STRING);\
  drizzle_con_set_##foo(self_ptr, RSTRING_PTR(newValue));\
  return newValue;\
}
#define attr_string(foo) attr(foo, rb_str_new2)
#define prop(foo) attr_string(foo) settr(foo)

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

static VALUE connection_close(VALUE self)
{
  read_self_ptr();
  drizzle_con_close(self_ptr);
  return Qnil;
}

attr_string(error);
attr(errno, INT2NUM);
attr(error_code, UINT2NUM);
attr_string(sqlstate);
attr_string(host);
attr(port, UINT2NUM);
prop(uds);
attr_string(user);
attr_string(password);
prop(db);
attr(protocol_version, UINT2NUM);
attr_string(server_version);
attr(server_version_number, UINT2NUM);
attr(thread_id, UINT2NUM);
attr(max_packet_size, UINT2NUM);

VALUE downpour_connection_constructor(drizzle_con_st *self_ptr, VALUE status)
{
  return to_ruby_object(self_ptr, DrizzleConnection, status, drizzle_con_free, drizzle_con_set_context);
}

void init_drizzle_connection()
{
  DrizzleConnection = drizzle_gem_create_class_with_private_constructor("Connection", rb_cObject);
  rb_define_method(DrizzleConnection, "query", query, 1);
  define_attr(error);
  define_attr(errno);
  define_attr(error_code);
  define_attr(sqlstate);
  define_attr(host);
  define_attr(port);
  define_prop(uds);
  define_attr(user);
  define_attr(password);
  define_prop(db);
  define_attr(protocol_version);
  define_attr(server_version);
  define_attr(server_version_number);
  define_attr(thread_id);
  define_attr(max_packet_size);
  rb_define_method(DrizzleConnection, "close!", connection_close, 0);
}
