#include "ruby.h"

VALUE Drizzle = Qnil;

void Init_drizzle();

VALUE method_test1(VALUE self);

void Init_drizzle() {
	Drizzle = rb_define_module("Drizzle");
	rb_define_method(Drizzle, "test1", method_test1, 0);
}

VALUE method_test1(VALUE self) {
	int x = 10;
	return INT2NUM(x);
}
