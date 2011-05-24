#include "drizzle.h"

#define SELF_TYPE drizzle_result_st

void init_drizzle_result()
{
  DrizzleResult = drizzle_gem_create_class_with_private_constructor("Result", rb_cObject);
}
