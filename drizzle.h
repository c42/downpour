#ifndef DRIZZLE_H
#define DRIZZLE_H

#include "ruby.h"
#include <libdrizzle/drizzle.h>

extern VALUE DrizzleModule;
extern VALUE DrizzleStatus;

VALUE drizzle_gem_create_class_with_private_constructor(const char *name, VALUE super);

#endif
