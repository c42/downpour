#ifndef RESULTS_H
#define RESULTS_H

#include "downpour.h"

bool downpour_is_buffered(drizzle_result_st *self_ptr);
VALUE downpour_wrap_row(VALUE self, drizzle_result_st *self_ptr, drizzle_row_t row);

#endif
