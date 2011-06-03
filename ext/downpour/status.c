#include "downpour.h"

#define SELF_TYPE drizzle_st

static in_port_t get_port(VALUE port, in_port_t default_port)
{
  if(port == Qnil)
    return default_port;
  Check_Type(port, T_FIXNUM);
  return NUM2UINT(port);
}

static VALUE add_tcp_connection_with_defaults(int argc, VALUE *argv, VALUE self, in_port_t default_port, drizzle_con_options_t default_options)
{
  read_self_ptr();

  VALUE host, port, user, passwd, db, options;

  //TODO: We ignore options
  rb_scan_args(argc, argv, "06", &host, &user, &passwd, &db, &port, &options);

  drizzle_con_st *connection = drizzle_con_add_tcp(self_ptr, NULL, 
                                                    read_string(host, "localhost"), 
                                                    get_port(port, default_port),
                                                    read_string(user, ""),
                                                    read_string(passwd, ""),
                                                    read_string(db, "test"),
                                                    default_options);
  return downpour_connection_constructor(connection, self);
}

static VALUE add_tcp_connection(int argc, VALUE *argv, VALUE self)
{
  return add_tcp_connection_with_defaults(argc, argv, self, DRIZZLE_DEFAULT_TCP_PORT, DRIZZLE_CON_NONE);
}

static VALUE add_mysql_tcp_connection(int argc, VALUE *argv, VALUE self)
{
  return add_tcp_connection_with_defaults(argc, argv, self, 3306, DRIZZLE_CON_MYSQL);
}

static VALUE error(VALUE self)
{
  read_self_ptr();

  return rb_str_new2(drizzle_error(self_ptr));
}

static VALUE set_verbose(VALUE self, VALUE newVerbocity)
{
  read_self_ptr();

  int verbocity = NUM2INT(newVerbocity);
  drizzle_set_verbose(self_ptr, verbocity);

  return newVerbocity;
}

static VALUE get_verbose(VALUE self)
{
  read_self_ptr();

  return UINT2NUM(drizzle_verbose(self_ptr));
}

static VALUE verbose_name(VALUE self)
{
  read_self_ptr();

  return rb_str_new2(drizzle_verbose_name(drizzle_verbose(self_ptr)));
}

static VALUE add_query(VALUE self, VALUE connection, VALUE query)
{
  read_self_ptr();
  convert_to_struct(drizzle_con_st, connection_ptr, connection);
  Check_Type(query, T_STRING);

  drizzle_query_st *query_ptr = drizzle_query_add(self_ptr, NULL, connection_ptr, NULL, 
                                                    RSTRING_PTR(query), RSTRING_LEN(query), 0, NULL);

  return downpour_query_constructor(query_ptr, connection);
}

static VALUE run_all(VALUE self)
{
  read_self_ptr();
  CHECK_OK(drizzle_query_run_all(self_ptr));
  return Qnil;
}

static VALUE run_one(VALUE self)
{
  read_self_ptr();
  drizzle_return_t ret;
  drizzle_query_st *query = drizzle_query_run(self_ptr, &ret);
  CHECK_OK(ret);

  if(query == NULL)
    return Qnil;

  return downpour_get_ruby_object(drizzle_query_context(query));
}

VALUE downpour_constructor(drizzle_st *self_ptr)
{
  return to_ruby_object(self_ptr, DrizzleStatus, Qnil, drizzle_free, drizzle_set_context);
}

void init_drizzle_status()
{
  DrizzleStatus = drizzle_gem_create_class_with_private_constructor("Status", rb_cObject);
  rb_define_method(DrizzleStatus, "add_tcp_connection", add_tcp_connection, -1);
  rb_define_method(DrizzleStatus, "add_mysql_tcp_connection", add_mysql_tcp_connection, -1);
  rb_define_method(DrizzleStatus, "error", error, 0);
  rb_define_method(DrizzleStatus, "verbose=", set_verbose, 1);
  rb_define_method(DrizzleStatus, "verbose", get_verbose, 0);
  rb_define_method(DrizzleStatus, "verbose_name", verbose_name, 0);
  rb_define_private_method(DrizzleStatus, "_add_query", add_query, 2);
  rb_define_private_method(DrizzleStatus, "_run_all!", run_all, 0);
  rb_define_private_method(DrizzleStatus, "_run!", run_one, 0);
}
