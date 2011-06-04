#include "downpour.h"

#define SELF_TYPE drizzle_column_st
#define RUBY_CLASS DrizzleColumn

#define attr(foo, conversion) static VALUE attr_##foo(VALUE self)\
{\
  read_self_ptr();\
  return conversion(drizzle_column_##foo(self_ptr));\
}

attr_string(name);

VALUE downpour_column_constructor(drizzle_column_st *self_ptr, VALUE result)
{
  return to_ruby_object(self_ptr, DrizzleColumn, result, drizzle_column_free, NULL);
}

void init_drizzle_column()
{
  DrizzleColumn = drizzle_gem_create_class_with_private_constructor("Column", rb_cObject);
  define_attr(name);
}
