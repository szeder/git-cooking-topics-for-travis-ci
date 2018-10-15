#!/bin/sh
#
# Build and test Git
#

. ${0%/*}/lib-travisci.sh

ln -s "$cache_dir/.prove" t/.prove

make --jobs=2
mkfifo .git/prove-output
cat .git/prove-output &
{
	make --quiet test

	case "$jobname" in
	linux-gcc)
		export GIT_TEST_SPLIT_INDEX=yes
		export GIT_TEST_FULL_IN_PACK_ARRAY=true
		export GIT_TEST_OE_SIZE=10
		export GIT_TEST_OE_DELTA_SIZE=5
		make --quiet test
		;;
	GETTEXT_POISON)
		GIT_GETTEXT_POISON=YesPlease make --quiet test
		;;
	esac
} >.git/prove-output

check_unignored_build_artifacts

save_good_tree
