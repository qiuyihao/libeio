#!/bin/bash

case $1 in
"clean")
	rm -f *.o
	echo "# clean libeio.a temporary file ok";
	;;
*)
	echo "# building libeio.a";
	gcc -c *.cpp
	ar -rcv libeio.a *.o
	rm -f *.o
	echo "# building libeio.a ok";
	exit 1
esac
