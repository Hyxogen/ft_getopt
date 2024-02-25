#include <stdio.h>

#if MINE
#include "ft_getopt.h"
#define GETOPT ft_getopt
#define OPTIND ft_optind
#define OPTERR ft_opterr
#define OPTARG ft_optarg
#else
#include <unistd.h>
#include <getopt.h>
#define GETOPT getopt
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


int main(int argc, char **argv)
{
	printf("argc: %i\n", argc);
	OPTERR = OPTERR_VAL;
	int c;
	while ((c = GETOPT(argc, argv, OPTSTRING)) != -1) {
		printf("ret: %x arg: \"%s\"\n", (unsigned) c, OPTARG);
		OPTARG = NULL;
	}
	printf("remaining arguments:\n");
	for (; OPTIND < argc; ++OPTIND)
		printf("%i: \"%s\"\n", OPTIND, argv[OPTIND]);
	printf("end\n");
}
