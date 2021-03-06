#include "downpour.h"

static VALUE version(VALUE self)
{
  return rb_str_new2(drizzle_version());
}

static VALUE create(VALUE self)
{
  drizzle_st *ptr = drizzle_create(NULL);
  drizzle_remove_options(ptr, DRIZZLE_FREE_OBJECTS);
  return downpour_constructor(ptr);
}

void init_downpour_module()
{
  DownpourModule = rb_define_module("Downpour");
  rb_define_singleton_method(DownpourModule, "drizzle_version", version, 0);
  rb_define_singleton_method(DownpourModule, "create", create, 0);
}
