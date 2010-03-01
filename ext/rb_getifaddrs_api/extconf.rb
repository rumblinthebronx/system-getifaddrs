require 'mkmf'

dir_config "ifaddrs_api"

$CFLAGS = "-D_GNU_SOURCE -Wall"

have_header('arpa/inet.h')
have_header('sys/socket.h')
have_header('sys/types.h')
have_header('netdb.h')
have_header('ifaddrs.h')
have_header('stdio.h')
have_header('stdlib.h')
have_header('unistd.h')

create_makefile("ifaddrs_api")