#include "drizzle.h"

static VALUE clone(VALUE self)
{
  drizzle_st *ptr;
  Data_Get_Struct(self, drizzle_st, ptr);
  drizzle_st *cloned = drizzle_clone(NULL, ptr);
  return Data_Wrap_Struct(DrizzleStatus, NULL, drizzle_free, cloned);
}

void init_drizzle_status()
{
  DrizzleStatus = drizzle_gem_create_class_with_private_constructor("Status", rb_cObject);
  rb_define_method(DrizzleStatus, "clone", clone, 0);
}
