require 'mkmf'

extension_name = 'drizzle'
dir_config(extension_name)

have_library("drizzle")
$CFLAGS += " -I/usr/local/include/libdrizzle-1.0"

create_makefile(extension_name)
