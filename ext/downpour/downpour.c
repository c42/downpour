#include "downpour.h"

VALUE DownpourModule;
VALUE DrizzleStatus;
VALUE DrizzleConnection;
VALUE DrizzleResult;
VALUE DrizzleQuery;

void init_downpour_module();
void init_drizzle_status();
void init_drizzle_connection();
void init_drizzle_result();
void init_drizzle_query();

void Init_downpour()
{
  init_downpour_module();
  init_drizzle_status();
  init_drizzle_connection();
  init_drizzle_result();
  init_drizzle_query();
}
