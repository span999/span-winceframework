/*




*/
#ifndef __LIBSIO_HELP_H__
#define __LIBSIO_HELP_H__

#if 0
static char helpstr[] = "12346789";
#else
static char helpstr[] = "\
usage: libsIO <subcommand> [options] [args]\r\n\
libsIO command-line tool, version 0.0.6.\r\n\
Type 'libsIO help' for this help.\r\n\
\r\n\
Available subcommands:\r\n\
   help\r\n\
   cpu\r\n\
   mem\r\n\
   iic\r\n\
   gpio(not yet)\r\n\
\r\n\
Available options for iic:\r\n\
   search (search all available device on all bus)\r\n\
   list (list all available device on all bus)\r\n\
   dump\r\n\
   set (set data on specified index of specified device)\r\n\
      (ex: libsIO iic set 1 2c 1a 7f7f - set value 7f7f to index 0x1a of address 0x2c on bus1. )\r\n\
   get (get data on specified index of specified device)\r\n\
      (ex: libsIO iic get 1 2c 1a - get value from index 0x1a of address 0x2c on bus1. )\r\n\
\r\n\
libsIO is a tool for low level accessing.\r\n\
For additional information, please mail spanliu@gmail.com\r\n\
";
#endif


#endif /* __LIBSIO_HELP_H__ */
