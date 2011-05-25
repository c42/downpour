#include "drizzle.h"

VALUE DrizzleModule;
VALUE DrizzleStatus;
VALUE DrizzleConnection;
VALUE DrizzleResult;

void init_drizzle_module();
void init_drizzle_status();
void init_drizzle_connection();
void init_drizzle_result();

static VALUE do_not_use_this_constructor(VALUE self)
{
  rb_raise(rb_eNameError, "private method `new' called for %s:Class.", rb_class2name(CLASS_OF(self)));
}

VALUE drizzle_gem_create_class_with_private_constructor(const char *name, VALUE super)
{
  VALUE ret = rb_define_class_under(DrizzleModule, name, super);
  rb_define_method(ret, "initialize", do_not_use_this_constructor, 0);
  return ret;
}

void drizzle_gem_assert_value_is_ok(drizzle_return_t ret)
{
  if(ret != DRIZZLE_RETURN_OK)
    rb_raise(rb_eIOError, "Action Failed with error code: %d", ret);
}

void Init_drizzle()
{
  init_drizzle_module();
  init_drizzle_status();
  init_drizzle_connection();
  init_drizzle_result();
}
