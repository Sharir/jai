#!/bin/bash

NAME='jai'

BUILD=false
RUN=false
DEBUG=false
TEST='test/hello.jai'

while getopts 'brdt:' opts; do
	case $opts in
		b) BUILD=true;;
		r) RUN=true;;
		d) DEBUG=true;;
		t) TEST=$OPTARG;;
	esac
done

run() {
	if [ $RUN = true ]; then
		./bin/$NAME $TEST
		EC=$?

		if [ $EC -ne 0 ]; then
			echo "Exit code $EC"
		fi
	fi
}

if [ ! -d ./bin ]; then
	mkdir bin
fi

rm $NAME.stackdump >/dev/null 2>&1
rm bin/$NAME.stackdump >/dev/null 2>&1

if [ $BUILD = true ]; then
	POSTFIX=''
	if [ $DEBUG = true ]; then
		POSTFIX='-g'
	fi

	g++ -std=c++11 -Wall `find ./csrc -name \\*.c -o -name \\*.h -o -name \\*.cpp -o -name \\*.c++ -o -name \\*.hpp | tr '\n' ' '` -o bin/temp $POSTFIX

	if [ $? -eq 0 ]; then
		mv bin/temp bin/$NAME
		run
	else
		rm bin/temp > /dev/null 2>&1
	fi
else
	run
fi
