#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include "ft_getopt.h"

int ft_optind = 1, ft_opterr = 0, ft_optopt = 0;
char *ft_optarg = NULL;

__attribute__((format(printf, 3, 4))) static void
print_error(int argc, char *const *argv, const char *restrict fmt, ...)
{
	va_list list;
	va_start(list, fmt);

	if (argc > 0)
		fprintf(stderr, "%s: ", argv[0]);
	vfprintf(stderr, fmt, list);
}

static int do_getopt_short(int argc, char *const *argv, const char *optstring)
{
	int colon = optstring[0] == ':';
	if (colon)
		++optstring;

	char *cur = argv[ft_optind++];

	for (; *optstring; ++optstring) {
		if (*optstring == ':' || cur[1] != *optstring)
			continue;

		if (optstring[1] == ':') {
			if (cur[2]) {
				ft_optarg = &cur[2];
			} else if (ft_optind < argc) {
				ft_optarg = argv[ft_optind++];
			} else if (colon) {
				ft_optopt = *optstring;
				return ':';
			} else if (ft_opterr) {
				ft_optopt = *optstring;
				print_error(
					argc, argv,
					"option requires an argument -- '%c'\n",
					ft_optopt);
				break;
			}
		}
		return *optstring;
	}

	if (!*optstring && !colon && ft_opterr) {
		ft_optopt = cur[1];
		print_error(argc, argv, "invalid option -- '%c'\n", ft_optopt);
	}
	return '?';
}

static void permute(char **argv, size_t dest, size_t src)
{
	char *tmp = argv[src];
	for (; dest != src; --src) {
		argv[src] = argv[src - 1];
	}
	argv[dest] = tmp;
}

int ft_getopt(int argc, char **argv, const char *optstring)
{
	return ft_getopt_long(argc, argv, optstring, NULL, NULL);
}

int ft_getopt_long(int argc, char **argv, const char *optstring,
		   const struct option *longopts, int *longindex)
{
	(void)longopts;
	(void)longindex;
	int saved = ft_optind;
	while (ft_optind < argc &&
	       (!argv[ft_optind] || argv[ft_optind][0] != '-'))
		++ft_optind;
	if (ft_optind >= argc) {
		ft_optind = saved;
		return -1;
	}

	int resumed = ft_optind;
	int ret = do_getopt_short(argc, argv, optstring);

	if (ret != -1 && resumed > saved) {
		for (int i = ft_optind - resumed; i > 0; --i)
			permute(argv, saved, ft_optind - 1);
		ft_optind -= resumed - saved;
	}

	return ret;
}
