require 'mkmf'

extension_name = 'drizzle'
dir_config(extension_name)

have_library("drizzle")

create_makefile(extension_name)
