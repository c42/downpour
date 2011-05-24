#include "drizzle.h"

#define SELF_TYPE drizzle_st

static VALUE clone(VALUE self)
{
  read_self_ptr();
  drizzle_st *cloned = drizzle_clone(NULL, self_ptr);
  return Data_Wrap_Struct(DrizzleStatus, NULL, drizzle_free, cloned);
}

static VALUE add_tcp_connection(VALUE self)
{
  read_self_ptr();

  //TODO: Set real values here
  drizzle_con_st *connection = drizzle_con_add_tcp(self_ptr, NULL, "localhost", DRIZZLE_DEFAULT_TCP_PORT, "", "", "test", DRIZZLE_CON_ALLOCATED);

  VALUE val = Data_Wrap_Struct(DrizzleConnection, NULL, NULL, connection);
  rb_iv_set(val, "@status", self);
  return val;
}

void init_drizzle_status()
{
  DrizzleStatus = drizzle_gem_create_class_with_private_constructor("Status", rb_cObject);
  rb_define_method(DrizzleStatus, "clone", clone, 0);
  rb_define_method(DrizzleStatus, "add_tcp_connection", add_tcp_connection, 0);
}
