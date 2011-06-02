#ifndef DRIZZLE_H
#define DRIZZLE_H

#include "ruby.h"
#include <libdrizzle/drizzle.h>
#include <libdrizzle/drizzle_client.h>

extern VALUE DownpourModule;
extern VALUE DrizzleStatus;
extern VALUE DrizzleConnection;
extern VALUE DrizzleResult;
extern VALUE DrizzleQuery;

// All mark and release methods
typedef void (*FREE_METHOD)(void* ptr);
typedef void (*SET_CONTEXT)(void* ptr, void *context);
void *downpour_from_ruby_object(VALUE value);
VALUE downpour_to_ruby_object(void *ptr, VALUE klass, VALUE parent, FREE_METHOD free_method, SET_CONTEXT set_context);
VALUE downpour_get_ruby_object(void *ptr);

// All Constructors
VALUE downpour_constructor(drizzle_st *self_ptr);
VALUE downpour_connection_constructor(drizzle_con_st *self_ptr, VALUE status);
VALUE downpour_result_constructor(drizzle_result_st *self_ptr, VALUE connection);
VALUE downpour_query_constructor(drizzle_query_st *self_ptr, VALUE connection);

VALUE drizzle_gem_create_class_with_private_constructor(const char *name, VALUE super);
void drizzle_gem_assert_value_is_ok(drizzle_return_t value);
VALUE drizzle_gem_to_string_array(char **array, long count);
const char *drizzle_gem_read_string_with_default(VALUE string, const char *default_value);

#define convert_to_struct(datatype, var_name, value) datatype *var_name; var_name = downpour_from_ruby_object(value)
#define read_self_ptr() convert_to_struct(SELF_TYPE, self_ptr, self)
#define CHECK_OK(value) drizzle_gem_assert_value_is_ok(value)
#define rb_call(self, string) rb_funcall(self, rb_intern(string), 0)
#define read_string(value, default_value) drizzle_gem_read_string_with_default(value, default_value)
#define drizzle_alloc(type) ((type *) malloc(sizeof(type)))
#define to_ruby_object(ptr, klass, parent, free_method, set_context) downpour_to_ruby_object(ptr, klass, parent, (FREE_METHOD) (free_method), (SET_CONTEXT) (set_context))

#endif
