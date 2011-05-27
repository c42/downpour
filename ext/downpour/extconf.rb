require 'mkmf'

extension_name = 'downpour'
dir_config(extension_name)

have_library("drizzle", "drizzle_create")
$CFLAGS += " -I/usr/local/include/libdrizzle-1.0 -I/usr/include/libdrizzle-1.0"

create_makefile(extension_name)
