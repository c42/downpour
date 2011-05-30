require 'mkmf'

libdrizzle = 'libdrizzle-1.0'

extension_name = 'downpour'
dir_config(extension_name)

if(!system("pkg-config #{libdrizzle}"))
  raise "Cannot find pkg-config or #{libdrizzle}"
end

$LDFLAGS += " " + `pkg-config --libs #{libdrizzle}`
$CFLAGS += " " + `pkg-config --cflags #{libdrizzle}`

create_makefile(extension_name)
