#include "downpour.h"

#define SELF_TYPE drizzle_query_st

static VALUE create_results(VALUE self)
{
  read_self_ptr();

  drizzle_result_st *result = drizzle_query_result(self_ptr);
  VALUE connection = downpour_get_parent(self);
  return downpour_result_constructor(result, connection);
}

VALUE downpour_query_constructor(drizzle_query_st *self_ptr, VALUE connection)
{
  return to_ruby_object(self_ptr, DrizzleQuery, connection, drizzle_query_free, drizzle_query_set_context);
}

void init_drizzle_query()
{
  DrizzleQuery = drizzle_gem_create_class_with_private_constructor("Query", rb_cObject);
  rb_define_private_method(DrizzleQuery, "create_results", create_results, 0);
}
