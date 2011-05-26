#include "drizzle.h"

static VALUE version(VALUE self)
{
  return rb_str_new2(drizzle_version());
}

static VALUE create(VALUE self)
{
  drizzle_st *ptr = drizzle_create(drizzle_alloc(drizzle_st));
  return downpour_constructor(ptr);
}

void init_drizzle_module()
{
  DrizzleModule = rb_define_module("Drizzle");
  rb_define_singleton_method(DrizzleModule, "version", version, 0);
  rb_define_singleton_method(DrizzleModule, "create", create, 0);
}
