#!/bin/bash

cmake_cmd=""

do_build()
{
	rm -rf build >> /dev/null
	mkdir build
	pushd build
	cmake .. ${cmake_cmd}
	make
	popd
}

unset OPTIND
while getopts ":te" opt; do
	case ${opt} in
		t)
			cmake_cmd="$cmake_cmd -DBUILD_TEST=ON "
			;;
		e)
			cmake_cmd="$cmake_cmd -DBUILD_EXAMPLE=ON "
			;;
		:)
			err "Missing argument for ${OPTARG}"
			exit 1 ;;
		*) exit 1;;
	esac
done

do_build

