#!/bin/sh
#
# Build and test Git
#

. ${0%/*}/lib-travisci.sh

ln -s "$cache_dir/.prove" t/.prove

make show-cc-version

time make --jobs=2 -k
mkfifo .git/prove-output
cat .git/prove-output &
{
	time make test ${TEST_SELECTION:+T="$TEST_SELECTION"}

	case "$jobname" in
	linux-gcc)
		export GIT_TEST_SPLIT_INDEX=yes
		export GIT_TEST_FULL_IN_PACK_ARRAY=true
		export GIT_TEST_OE_SIZE=10
		export GIT_TEST_OE_DELTA_SIZE=5
		export GIT_TEST_COMMIT_GRAPH=1
		export GIT_TEST_MULTI_PACK_INDEX=1
		time make test ${TEST_SELECTION:+T="$TEST_SELECTION"}
		;;
	GETTEXT_POISON)
		export GIT_GETTEXT_POISON=YesPlease
		time make test ${TEST_SELECTION:+T="$TEST_SELECTION"}
		;;
	esac
} >.git/prove-output

check_unignored_build_artifacts

save_good_tree
