#include "downpour.h"

#define SELF_TYPE drizzle_column_st
#define RUBY_CLASS DrizzleColumn

#define attr(foo, conversion) static VALUE attr_##foo(VALUE self)\
{\
  read_self_ptr();\
  return conversion(drizzle_column_##foo(self_ptr));\
}

attr_string(catalog);
attr_string(db);
attr_string(table);
attr_string(orig_table);
attr_string(name);
attr_string(orig_name);
// charset
attr(column_size, UINT2NUM);
attr(max_size, INT2NUM);
//column type
//column_type_drizzle
//flags
attr(decimals, UINT2NUM);
//default_value

VALUE downpour_column_constructor(drizzle_column_st *self_ptr, VALUE result)
{
  return to_ruby_object(self_ptr, DrizzleColumn, result, drizzle_column_free, NULL);
}

void init_drizzle_column()
{
  DrizzleColumn = drizzle_gem_create_class_with_private_constructor("Column", rb_cObject);
  define_attr(catalog);
  define_attr(db);
  define_attr(table);
  define_attr(orig_table);
  define_attr(name);
  define_attr(orig_name);
  // charset
  define_attr(column_size);
  define_attr(max_size);
  //column type
  //column_type_drizzle
  //flags
  define_attr(decimals);
  //default_value
}
