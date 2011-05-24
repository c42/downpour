#ifndef DRIZZLE_H
#define DRIZZLE_H

#include "ruby.h"
#include <libdrizzle/drizzle.h>
#include <libdrizzle/drizzle_client.h>

extern VALUE DrizzleModule;
extern VALUE DrizzleStatus;
extern VALUE DrizzleConnection;
extern VALUE DrizzleResult;

VALUE drizzle_gem_create_class_with_private_constructor(const char *name, VALUE super);

#define convert_to_struct(datatype, var_name, value) datatype *var_name; Data_Get_Struct(value, datatype, var_name)
#define read_self_ptr() convert_to_struct(SELF_TYPE, self_ptr, self)

#endif
