#!/bin/bash

CC=cc
CFLAGS="-fsanitize=address,undefined -Wall -Wextra -fsanitize-trap=all"
DRIVER=test.c

escape() {
	echo "$@" | sed 's/"/\\"/g'
}

compile_cmd() {
	OPTERR=$1
	shift

	OPTSTRING=\"$1\"
	shift

	echo "$CC $CFLAGS $DRIVER -DOPTERR_VAL=$OPTERR -DOPTSTRING=$OPTSTRING getopt.c"
}

diff_test() {
	MINE_STDOUT=$(mktemp)
	MINE_STDERR=$(mktemp)

	THEIRS_STDOUT=$(mktemp)
	THEIRS_STDERR=$(mktemp)

	export CMD=$(compile_cmd "$@")
	shift 2

	$CMD -DMINE=1
	timeout 1s ./a.out "$@" > $MINE_STDOUT 2> $MINE_STDERR
	MY_RES=$?
	$CMD -DMINE=0
	./a.out "$@" > $THEIRS_STDOUT 2> $THEIRS_STDERR
	THEIR_RES=$?

	if [ $MY_RES != 0 ]; then
		echo "KO (exit val): $(escape $CMD) -DMINE=1"
		echo "run: ./a.out $@"
		return
	fi

	diff -q $MINE_STDOUT $THEIRS_STDOUT >/dev/null
	if [ $? != 0 ]; then
		echo "KO"
		echo "< mine"
		echo "> theirs"
		echo "compiled with: $(escape $CMD)"
		echo "run: ./a.out $@"

		diff $MINE_STDOUT $THEIRS_STDOUT --color
	fi

	diff -q $MINE_STDERR $THEIRS_STDERR >/dev/null
	if [ $? != 0 ]; then
		echo "KO"
		echo "< mine"
		echo "> theirs"
		echo "compiled with: $(escape $CMD)"
		echo "run: ./a.out $@"

		diff $MINE_STDERR $THEIRS_STDERR --color
	fi
	echo "OK"
}

diff_test_both() {
	diff_test 0 "$@"
	diff_test 1 "$@"
}

run_tests() {
	
	diff_test_both ':zo:' kaas -zo
	diff_test_both ':zo:' kaas -zo boter
	diff_test_both ':zo:' -zokaas
	diff_test_both ':abcdef:o:' a b -fyes
	diff_test_both '' -a -b -c -d
	diff_test_both ':f' -f
	diff_test_both 'abcd' -a -b -c -d
	diff_test_both 'abcd' -abd
	diff_test_both 'abcd' -a -b -c -d kaas
	diff_test_both ':a:bcd' -a -b -c -d kaas
	diff_test_both ':f:o:y' -f in.txt -o out.txt -y
	diff_test_both ':f:o:y' -y -f in.txt -o out.txt
	diff_test_both 'f:o:y' -f in.txt -o out.txt -y
	diff_test_both 'f:o:yz' kaas -f in.txt -zoout.txt boter
	diff_test_both ':f:o:y' -y -f in.txt -o out.txt
	diff_test_both ':f:o:y' -y -f in.txt -o out.txt -x
	diff_test_both ':f:o:y' --y -f in.txt -o out.txt -x
	diff_test_both ':f:o:y' --lol -f in.txt -o out.txt -x
	diff_test_both ':f:o:yn' --lol -nfin.txt -o out.txt -x boter kaas en eieren
	diff_test_both ':f:o:yn' -- --lol -nfin.txt -o out.txt -x boter kaas en eieren
	diff_test_both ':f:o:yn' --lol -nfin.txt -- -o out.txt -x boter kaas en eieren
}

run_tests_long() {
	DRIVER=testlong.c
	run_tests
	
	diff_test_both '' a b d --reqarg kaas boter en ei

	diff_test_both ':abcdef:o:' a b -fyes
	diff_test_both ':abcdef:o:' a b -cdfyes --vl -abc -similarv -a 4 --reqarg=x --setflag f
	diff_test_both ':abcdef:o:' -a --setflag
	diff_test_both ':abcdef:o:' a -similarv

	diff_test_both '' --vlag
	diff_test_both '' --vlag=no
	diff_test_both '' --vl
	diff_test_both '' --similarv
	diff_test_both '' --similarflag
	diff_test_both '' --0ptarg
	diff_test_both '' --0ptarg a
	diff_test_both '' --0ptarg=a
	diff_test_both '' --reqarg
	diff_test_both '' --reqarg a
	diff_test_both '' --reqarg=a
	diff_test_both '' --setflag
	diff_test_both '' --setflag=a

	diff_test_both '' a --vlag
	diff_test_both '' --vlag a

	diff_test_both '' a --vlag b c d --similarv
	diff_test_both '' a --vlag b --similarv d
	diff_test_both '' a --vlag --similarv d
	diff_test_both '' a --vla=no --similarv d

	diff_test_both '' --0ptarg --vlag

	diff_test_both 'abcdef:o:' a b -cdfyes --vl -abc -similarv -a 4 --reqarg=x --setflag f
	diff_test_both 'abcdef:o:' a b -cdfyes --vl -abc -similarv --similarv -a 4 --reqarg=x --setflag f
}


if [ "$#" = 0 ]; then
	run_tests
	run_tests_long
else
	diff_test $@
	#run $@
fi
