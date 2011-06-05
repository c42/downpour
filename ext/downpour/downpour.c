#include "downpour.h"

VALUE DownpourModule;
VALUE DrizzleStatus;
VALUE DrizzleConnection;
VALUE DrizzleBufferedResult;
VALUE DrizzleQuery;
VALUE DrizzleColumn;

void init_downpour_module();
void init_drizzle_status();
void init_drizzle_connection();
void init_drizzle_buffered_result();
void init_drizzle_query();
void init_drizzle_column();

void Init_downpour()
{
  init_downpour_module();
  init_drizzle_status();
  init_drizzle_connection();
  init_drizzle_buffered_result();
  init_drizzle_query();
  init_drizzle_column();
}
