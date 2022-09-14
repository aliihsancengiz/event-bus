#!/bin/bash

do_install()
{
	pushd build
	make install
	popd
}

./build.sh
do_install
