#!/bin/sh
#
# Perform various static code analysis checks
#

. ${0%/*}/lib.sh

if test -n "$1"
then
	HOST_UID_GID=$1
else
	HOST_UID_GID=$(id -u):$(id -g)
fi

docker run \
	--interactive --tty \
	--env MAKEFLAGS \
	--volume "${PWD}:/src" \
	--user $HOST_UID_GID \
	szeder/coccinelle:1.0.8-2

set +x

fail=
for cocci_patch in contrib/coccinelle/*.patch
do
	if test -s "$cocci_patch"
	then
		echo "$(tput setaf 1)Coccinelle suggests the following changes in '$cocci_patch':$(tput sgr0)"
		cat "$cocci_patch"
		fail=UnfortunatelyYes
	fi
done

if test -n "$fail"
then
	echo "$(tput setaf 1)error: Coccinelle suggested some changes$(tput sgr0)"
	exit 1
fi

make -k hdr-check ||
exit 1

save_good_tree
