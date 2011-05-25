#include "downpour.h"

VALUE DownpourModule;
VALUE DrizzleStatus;
VALUE DrizzleConnection;
VALUE DrizzleResult;

void init_downpour_module();
void init_drizzle_status();
void init_drizzle_connection();
void init_drizzle_result();

void Init_downpour()
{
  init_downpour_module();
  init_drizzle_status();
  init_drizzle_connection();
  init_drizzle_result();
}
