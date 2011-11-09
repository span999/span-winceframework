/*




*/
#ifndef __LIBSIO_HELP_H__
#define __LIBSIO_HELP_H__

#if 0
static char helpstr[] = "12346789";
#else
static char helpstr[] = "\
usage: libsIO <subcommand> [options] [args]\
libsIO command-line tool, version 0.0.6.\
Type 'libsIO help' for this help.\
\
Available subcommands:\
   help\
   cpu\
   mem\
   iic\
   gpio(not yet)\
\
Available options for iic:\
   search\
   list\
   dump\
   set\
   get\
\
libsIO is a tool for low level accessing.\
For additional information, please mail spanliu@gmail.com\
";
#endif


#endif /* __LIBSIO_HELP_H__ */
