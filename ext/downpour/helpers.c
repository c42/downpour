#include "downpour.h"

static VALUE do_not_use_this_constructor(VALUE self)
{
  rb_raise(rb_eNameError, "private method `new' called for %s:Class.", rb_class2name(CLASS_OF(self)));
}

VALUE drizzle_gem_to_string_array(char **array, long count)
{
  VALUE ary = rb_ary_new2(count);

  long i;
  for(i = 0; i < count; i++)
    rb_ary_push(ary, rb_str_new2(array[i]));

  return ary;
}

VALUE drizzle_gem_create_class_with_private_constructor(const char *name, VALUE super)
{
  VALUE ret = rb_define_class_under(DownpourModule, name, super);
  rb_define_method(ret, "initialize", do_not_use_this_constructor, 0);
  return ret;
}

void drizzle_gem_assert_value_is_ok(drizzle_return_t ret)
{
  if(ret != DRIZZLE_RETURN_OK)
    rb_raise(rb_eIOError, "Action Failed with error code: %d. Please check the error string on status object for more information", ret);
}

const char *drizzle_gem_read_string_with_default(VALUE string, const char *default_value)
{
  if(string == Qnil)
    return default_value;

  Check_Type(string, T_STRING);
  return RSTRING_PTR(string);
}
