#include <stdio.h>
#include <assert.h>

#if MINE
#include "ft_getopt.h"
#define GETOPT_LONG ft_getopt_long
#define OPTIND ft_optind
#define OPTERR ft_opterr
#define OPTARG ft_optarg
#else
#include <unistd.h>
#include <getopt.h>
#define GETOPT_LONG getopt_long
#define OPTIND optind
#define OPTERR opterr
#define OPTARG optarg
#endif

extern int OPTIND;
extern int OPTERR;
extern char* OPTARG;

#ifndef OPTERR_VAL
#define OPTERR_VAL 0
#endif

static void test(int argc, char **argv, const char *optstring, const struct option *longopts)
{
	printf("argc: %i\n", argc);
	OPTIND = 0;
	OPTERR = OPTERR_VAL;
	int c, longindex = -1;
	while ((c = GETOPT_LONG(argc, argv, optstring, longopts, &longindex)) != -1) {
		printf("ret: %x arg: \"%s\" longindex: %i \n", (unsigned) c, OPTARG, longindex);
		for (size_t i = 0; longopts[i].name; ++i) {
			if (longopts[i].flag)
				printf("%zu: flag: %i \n", i, *longopts[i].flag);
		}
		OPTARG = NULL;
		longindex = -1;
	}
	printf("optind at end: %i\n", OPTIND);
	printf("remaining arguments:\n");
	for (; OPTIND < argc; ++OPTIND)
		printf("%i: \"%s\"\n", OPTIND, argv[OPTIND]);
	printf("end\n");
}

int main(int argc, char **argv)
{
	int i = 0;
	struct option longopts[] = {
		{ "vlag", 0, NULL, 0 },
		{ "similarflag", 0, NULL, 1 },
		{ "similarvlag", 0, NULL, 2 },
		{ "0ptarg", optional_argument, NULL, 3 },
		{ "reqarg", required_argument, NULL, 4 },
		{ "setflag", 0, &i, 5 },
		{ NULL, 0, NULL, 0 }
	};
	test(argc, argv, OPTSTRING, longopts);
	return 0;
}
