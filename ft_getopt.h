#ifndef FT_GETOPT_H
#define FT_GETOPT_H

#include <getopt.h>

int ft_getopt(int argc, char **argv, const char *optstring);
int ft_getopt_long(int argc, char **argv, const char *optstring,
		   const struct option *longopts, int *longindex);

#endif // FT_GETOPT_H
