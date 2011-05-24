#include "drizzle.h"

VALUE DrizzleModule;
VALUE DrizzleStatus;
VALUE DrizzleConnection;
VALUE DrizzleResult;

void init_drizzle_module();
void init_drizzle_status();
void init_drizzle_connection();
void init_drizzle_result();

void Init_drizzle()
{
  init_drizzle_module();
  init_drizzle_status();
  init_drizzle_connection();
  init_drizzle_result();
}
