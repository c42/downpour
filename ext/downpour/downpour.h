#ifndef DRIZZLE_H
#define DRIZZLE_H

#include "ruby.h"
#include <libdrizzle/drizzle.h>
#include <libdrizzle/drizzle_client.h>

extern VALUE DownpourModule;
extern VALUE DrizzleStatus;
extern VALUE DrizzleConnection;
extern VALUE DrizzleResult;
extern VALUE DrizzleBufferedResult;
extern VALUE DrizzleUnBufferedResult;
extern VALUE DrizzleQuery;
extern VALUE DrizzleColumn;

// All mark and release methods
typedef void (*FREE_METHOD)(void* ptr);
typedef void (*SET_CONTEXT)(void* ptr, void *context);
void *downpour_from_ruby_object(VALUE value);
VALUE downpour_to_ruby_object(void *ptr, VALUE klass, VALUE parent, FREE_METHOD free_method, SET_CONTEXT set_context);
VALUE downpour_get_ruby_object(void *ptr);
VALUE downpour_get_parent(VALUE self);
void downpour_set_extra_pointer(VALUE self, void *extra_pointer, FREE_METHOD free_extra_pointer);
void *downpour_get_extra_pointer(VALUE self);

// All Constructors
VALUE downpour_constructor(drizzle_st *self_ptr);
VALUE downpour_connection_constructor(drizzle_con_st *self_ptr, VALUE status);
VALUE downpour_buffered_result_constructor(drizzle_result_st *self_ptr, VALUE connection);
VALUE downpour_unbuffered_result_constructor(drizzle_result_st *self_ptr, VALUE connection);
VALUE downpour_query_constructor(drizzle_query_st *self_ptr, VALUE connection);
VALUE downpour_column_constructor(drizzle_column_st *self_ptr, VALUE result);
VALUE downpour_include_result_module(drizzle_result_st *self_ptr, VALUE self);

VALUE drizzle_gem_create_class_with_private_constructor(const char *name, VALUE super);
void drizzle_gem_assert_value_is_ok(drizzle_return_t value);
const char *drizzle_gem_read_string_with_default(VALUE string, const char *default_value);

#define convert_to_struct(datatype, var_name, value) datatype *var_name = downpour_from_ruby_object(value)
#define read_self_ptr() convert_to_struct(SELF_TYPE, self_ptr, self)
#define CHECK_OK(value) drizzle_gem_assert_value_is_ok(value)
#define rb_call(self, string) rb_funcall(self, rb_intern(string), 0)
#define read_string(value, default_value) drizzle_gem_read_string_with_default(value, default_value)
#define drizzle_alloc(type) ((type *) malloc(sizeof(type)))
#define to_ruby_object(ptr, klass, parent, free_method, set_context) downpour_to_ruby_object(ptr, klass, parent, (FREE_METHOD) (free_method), (SET_CONTEXT) (set_context))
#define set_extra_pointer(value, ptr, free_method) downpour_set_extra_pointer(value, ptr, (FREE_METHOD) free_method)

// Property Macros -> define attr(foo, conversion) and settr_string to use them
#define attr_string(foo) attr(foo, rb_str_new2)
#define prop_string(foo) attr_string(foo) settr_string(foo)
#define prop_int(foo, conversion) attr(foo, conversion) settr_int(foo)

#define define_attr(foo) rb_define_method(RUBY_CLASS, #foo, attr_##foo, 0)
#define define_settr(foo) rb_define_method(RUBY_CLASS, #foo "=", settr_##foo, 1)
#define define_prop(foo) define_attr(foo); define_settr(foo)

#endif
