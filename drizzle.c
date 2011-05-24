#include "ruby.h"
#include <libdrizzle/drizzle.h>

void Init_drizzle();

static VALUE version(VALUE self)
{
  return rb_str_new2(drizzle_version());
}

void Init_drizzle()
{
  VALUE Drizzle = rb_define_class("Drizzle", rb_cObject);
  rb_define_singleton_method(Drizzle, "version", version, 0);
}
