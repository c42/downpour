#include "drizzle.h"

VALUE DrizzleModule;
VALUE DrizzleStatus;

void init_drizzle_module();
void init_drizzle_status();

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

void Init_drizzle()
{
  init_drizzle_module();
  init_drizzle_status();
}
