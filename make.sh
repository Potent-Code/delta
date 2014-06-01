#!/bin/sh

# if no arguments are given, we just want to build
if [ "$1" == "" ]; then
	echo "Building Delta..."
	bam $BAMFLAGS -a Mode=Debug
elif [ "$1" == "install" ]; then
	cp libdelta.a /usr/local/lib/ &&
	cp libdelta.so /usr/local/lib/ &&
	cp src/include/delta.h /usr/local/include/ || exit 1
	echo "libdelta.a -> /usr/local/lib/"
	echo "libdelta.so -> /usr/local/lib/"
	echo "src/include/delta.h -> /usr/local/include/"
elif [ "$1" == "clean" ]; then
	echo "removing obj/"
	echo "removing libdelta.*"
	rm -fR obj libdelta.*
else
	echo "Usage:"
	echo -e "\t$0\t\tBuild"
	echo -e "\t$0 install\tInstall to /usr/local/lib and /usr/local/include"
	echo -e "\t$0 clean\t\tClean build outputs"
fi
