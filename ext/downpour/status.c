#include "downpour.h"

#define SELF_TYPE drizzle_st

static in_port_t get_port(VALUE port)
{
  if(port == Qnil)
    return DRIZZLE_DEFAULT_TCP_PORT;
  Check_Type(port, T_FIXNUM);
  return NUM2UINT(port);
}

static VALUE add_tcp_connection(int argc, VALUE *argv, VALUE self)
{
  read_self_ptr();

  VALUE host, port, user, passwd, db, options;

  //TODO: We ignore options
  rb_scan_args(argc, argv, "06", &host, &user, &passwd, &db, &port, &options);

  drizzle_con_st *connection = drizzle_con_add_tcp(self_ptr, drizzle_alloc(drizzle_con_st), 
                                                    read_string(host, "localhost"), 
                                                    get_port(port),
                                                    read_string(user, ""),
                                                    read_string(passwd, ""),
                                                    read_string(db, "test"),
                                                    DRIZZLE_CON_NONE);
  return downpour_connection_constructor(connection);
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

static void downpour_destructor(drizzle_st *self_ptr)
{
  drizzle_free(self_ptr);
  free(self_ptr);
}

VALUE downpour_constructor(drizzle_st *self_ptr)
{
  return Data_Wrap_Struct(DrizzleStatus, NULL, downpour_destructor, self_ptr);
}

void init_drizzle_status()
{
  DrizzleStatus = drizzle_gem_create_class_with_private_constructor("Status", rb_cObject);
  rb_define_method(DrizzleStatus, "add_tcp_connection", add_tcp_connection, -1);
  rb_define_method(DrizzleStatus, "error", error, 0);
  rb_define_method(DrizzleStatus, "verbose=", set_verbose, 1);
}
