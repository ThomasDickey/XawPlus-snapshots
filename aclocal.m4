dnl $XTermId: aclocal.m4,v 1.4 2015/01/03 01:39:35 tom Exp $
dnl
dnl ---------------------------------------------------------------------------
dnl
dnl Copyright 2015 by Thomas E. Dickey
dnl
dnl                         All Rights Reserved
dnl
dnl Permission is hereby granted, free of charge, to any person obtaining a
dnl copy of this software and associated documentation files (the
dnl "Software"), to deal in the Software without restriction, including
dnl without limitation the rights to use, copy, modify, merge, publish,
dnl distribute, sublicense, and/or sell copies of the Software, and to
dnl permit persons to whom the Software is furnished to do so, subject to
dnl the following conditions:
dnl
dnl The above copyright notice and this permission notice shall be included
dnl in all copies or substantial portions of the Software.
dnl
dnl THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
dnl OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
dnl MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
dnl IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
dnl CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
dnl TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
dnl SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
dnl
dnl Except as contained in this notice, the name(s) of the above copyright
dnl holders shall not be used in advertising or otherwise to promote the
dnl sale, use or other dealings in this Software without prior written
dnl authorization.
dnl
dnl ---------------------------------------------------------------------------
dnl See
dnl		http://invisible-island.net/autoconf/autoconf.html
dnl ---------------------------------------------------------------------------
dnl ---------------------------------------------------------------------------
dnl CF_ACVERSION_CHECK version: 5 updated: 2014/06/04 19:11:49
dnl ------------------
dnl Conditionally generate script according to whether we're using a given autoconf.
dnl
dnl $1 = version to compare against
dnl $2 = code to use if AC_ACVERSION is at least as high as $1.
dnl $3 = code to use if AC_ACVERSION is older than $1.
define([CF_ACVERSION_CHECK],
[
ifdef([AC_ACVERSION], ,[ifdef([AC_AUTOCONF_VERSION],[m4_copy([AC_AUTOCONF_VERSION],[AC_ACVERSION])],[m4_copy([m4_PACKAGE_VERSION],[AC_ACVERSION])])])dnl
ifdef([m4_version_compare],
[m4_if(m4_version_compare(m4_defn([AC_ACVERSION]), [$1]), -1, [$3], [$2])],
[CF_ACVERSION_COMPARE(
AC_PREREQ_CANON(AC_PREREQ_SPLIT([$1])),
AC_PREREQ_CANON(AC_PREREQ_SPLIT(AC_ACVERSION)), AC_ACVERSION, [$2], [$3])])])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ACVERSION_COMPARE version: 3 updated: 2012/10/03 18:39:53
dnl --------------------
dnl CF_ACVERSION_COMPARE(MAJOR1, MINOR1, TERNARY1,
dnl                      MAJOR2, MINOR2, TERNARY2,
dnl                      PRINTABLE2, not FOUND, FOUND)
define([CF_ACVERSION_COMPARE],
[ifelse(builtin([eval], [$2 < $5]), 1,
[ifelse([$8], , ,[$8])],
[ifelse([$9], , ,[$9])])])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ADD_CFLAGS version: 11 updated: 2014/07/22 05:32:57
dnl -------------
dnl Copy non-preprocessor flags to $CFLAGS, preprocessor flags to $CPPFLAGS
dnl The second parameter if given makes this macro verbose.
dnl
dnl Put any preprocessor definitions that use quoted strings in $EXTRA_CPPFLAGS,
dnl to simplify use of $CPPFLAGS in compiler checks, etc., that are easily
dnl confused by the quotes (which require backslashes to keep them usable).
AC_DEFUN([CF_ADD_CFLAGS],
[
cf_fix_cppflags=no
cf_new_cflags=
cf_new_cppflags=
cf_new_extra_cppflags=

for cf_add_cflags in $1
do
case $cf_fix_cppflags in
no)
	case $cf_add_cflags in #(vi
	-undef|-nostdinc*|-I*|-D*|-U*|-E|-P|-C) #(vi
		case $cf_add_cflags in
		-D*)
			cf_tst_cflags=`echo ${cf_add_cflags} |sed -e 's/^-D[[^=]]*='\''\"[[^"]]*//'`

			test "x${cf_add_cflags}" != "x${cf_tst_cflags}" \
				&& test -z "${cf_tst_cflags}" \
				&& cf_fix_cppflags=yes

			if test $cf_fix_cppflags = yes ; then
				cf_new_extra_cppflags="$cf_new_extra_cppflags $cf_add_cflags"
				continue
			elif test "${cf_tst_cflags}" = "\"'" ; then
				cf_new_extra_cppflags="$cf_new_extra_cppflags $cf_add_cflags"
				continue
			fi
			;;
		esac
		case "$CPPFLAGS" in
		*$cf_add_cflags) #(vi
			;;
		*) #(vi
			case $cf_add_cflags in #(vi
			-D*)
				cf_tst_cppflags=`echo "x$cf_add_cflags" | sed -e 's/^...//' -e 's/=.*//'`
				CF_REMOVE_DEFINE(CPPFLAGS,$CPPFLAGS,$cf_tst_cppflags)
				;;
			esac
			cf_new_cppflags="$cf_new_cppflags $cf_add_cflags"
			;;
		esac
		;;
	*)
		cf_new_cflags="$cf_new_cflags $cf_add_cflags"
		;;
	esac
	;;
yes)
	cf_new_extra_cppflags="$cf_new_extra_cppflags $cf_add_cflags"

	cf_tst_cflags=`echo ${cf_add_cflags} |sed -e 's/^[[^"]]*"'\''//'`

	test "x${cf_add_cflags}" != "x${cf_tst_cflags}" \
		&& test -z "${cf_tst_cflags}" \
		&& cf_fix_cppflags=no
	;;
esac
done

if test -n "$cf_new_cflags" ; then
	ifelse([$2],,,[CF_VERBOSE(add to \$CFLAGS $cf_new_cflags)])
	CFLAGS="$CFLAGS $cf_new_cflags"
fi

if test -n "$cf_new_cppflags" ; then
	ifelse([$2],,,[CF_VERBOSE(add to \$CPPFLAGS $cf_new_cppflags)])
	CPPFLAGS="$CPPFLAGS $cf_new_cppflags"
fi

if test -n "$cf_new_extra_cppflags" ; then
	ifelse([$2],,,[CF_VERBOSE(add to \$EXTRA_CPPFLAGS $cf_new_extra_cppflags)])
	EXTRA_CPPFLAGS="$cf_new_extra_cppflags $EXTRA_CPPFLAGS"
fi

AC_SUBST(EXTRA_CPPFLAGS)

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ADD_LIB version: 2 updated: 2010/06/02 05:03:05
dnl ----------
dnl Add a library, used to enforce consistency.
dnl
dnl $1 = library to add, without the "-l"
dnl $2 = variable to update (default $LIBS)
AC_DEFUN([CF_ADD_LIB],[CF_ADD_LIBS(-l$1,ifelse($2,,LIBS,[$2]))])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ADD_LIBS version: 2 updated: 2014/07/13 14:33:27
dnl -----------
dnl Add one or more libraries, used to enforce consistency.  Libraries are
dnl prepended to an existing list, since their dependencies are assumed to
dnl already exist in the list.
dnl
dnl $1 = libraries to add, with the "-l", etc.
dnl $2 = variable to update (default $LIBS)
AC_DEFUN([CF_ADD_LIBS],[
cf_add_libs="$1"
# Filter out duplicates - this happens with badly-designed ".pc" files...
for cf_add_1lib in [$]ifelse($2,,LIBS,[$2])
do
	for cf_add_2lib in $cf_add_libs
	do
		if test "x$cf_add_1lib" = "x$cf_add_2lib"
		then
			cf_add_1lib=
			break
		fi
	done
	test -n "$cf_add_1lib" && cf_add_libs="$cf_add_libs $cf_add_1lib"
done
ifelse($2,,LIBS,[$2])="$cf_add_libs"
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ADD_LIB_AFTER version: 3 updated: 2013/07/09 21:27:22
dnl ----------------
dnl Add a given library after another, e.g., following the one it satisfies a
dnl dependency for.
dnl
dnl $1 = the first library
dnl $2 = its dependency
AC_DEFUN([CF_ADD_LIB_AFTER],[
CF_VERBOSE(...before $LIBS)
LIBS=`echo "$LIBS" | sed -e "s/[[ 	]][[ 	]]*/ /g" -e "s%$1 %$1 $2 %" -e 's%  % %g'`
CF_VERBOSE(...after  $LIBS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ARG_DISABLE version: 3 updated: 1999/03/30 17:24:31
dnl --------------
dnl Allow user to disable a normally-on option.
AC_DEFUN([CF_ARG_DISABLE],
[CF_ARG_OPTION($1,[$2],[$3],[$4],yes)])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ARG_ENABLE version: 3 updated: 1999/03/30 17:24:31
dnl -------------
dnl Allow user to enable a normally-off option.
AC_DEFUN([CF_ARG_ENABLE],
[CF_ARG_OPTION($1,[$2],[$3],[$4],no)])dnl
dnl ---------------------------------------------------------------------------
dnl CF_ARG_OPTION version: 4 updated: 2010/05/26 05:38:42
dnl -------------
dnl Restricted form of AC_ARG_ENABLE that ensures user doesn't give bogus
dnl values.
dnl
dnl Parameters:
dnl $1 = option name
dnl $2 = help-string
dnl $3 = action to perform if option is not default
dnl $4 = action if perform if option is default
dnl $5 = default option value (either 'yes' or 'no')
AC_DEFUN([CF_ARG_OPTION],
[AC_ARG_ENABLE([$1],[$2],[test "$enableval" != ifelse([$5],no,yes,no) && enableval=ifelse([$5],no,no,yes)
  if test "$enableval" != "$5" ; then
ifelse([$3],,[    :]dnl
,[    $3]) ifelse([$4],,,[
  else
    $4])
  fi],[enableval=$5 ifelse([$4],,,[
  $4
])dnl
  ])])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CC_ENV_FLAGS version: 1 updated: 2012/10/03 05:25:49
dnl ---------------
dnl Check for user's environment-breakage by stuffing CFLAGS/CPPFLAGS content
dnl into CC.  This will not help with broken scripts that wrap the compiler with
dnl options, but eliminates a more common category of user confusion.
AC_DEFUN([CF_CC_ENV_FLAGS],
[
# This should have been defined by AC_PROG_CC
: ${CC:=cc}

AC_MSG_CHECKING(\$CC variable)
case "$CC" in #(vi
*[[\ \	]]-[[IUD]]*)
	AC_MSG_RESULT(broken)
	AC_MSG_WARN(your environment misuses the CC variable to hold CFLAGS/CPPFLAGS options)
	# humor him...
	cf_flags=`echo "$CC" | sed -e 's/^[[^ 	]]*[[ 	]]//'`
	CC=`echo "$CC" | sed -e 's/[[ 	]].*//'`
	CF_ADD_CFLAGS($cf_flags)
	;;
*)
	AC_MSG_RESULT(ok)
	;;
esac
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CHECK_CACHE version: 12 updated: 2012/10/02 20:55:03
dnl --------------
dnl Check if we're accidentally using a cache from a different machine.
dnl Derive the system name, as a check for reusing the autoconf cache.
dnl
dnl If we've packaged config.guess and config.sub, run that (since it does a
dnl better job than uname).  Normally we'll use AC_CANONICAL_HOST, but allow
dnl an extra parameter that we may override, e.g., for AC_CANONICAL_SYSTEM
dnl which is useful in cross-compiles.
dnl
dnl Note: we would use $ac_config_sub, but that is one of the places where
dnl autoconf 2.5x broke compatibility with autoconf 2.13
AC_DEFUN([CF_CHECK_CACHE],
[
if test -f $srcdir/config.guess || test -f $ac_aux_dir/config.guess ; then
	ifelse([$1],,[AC_CANONICAL_HOST],[$1])
	system_name="$host_os"
else
	system_name="`(uname -s -r) 2>/dev/null`"
	if test -z "$system_name" ; then
		system_name="`(hostname) 2>/dev/null`"
	fi
fi
test -n "$system_name" && AC_DEFINE_UNQUOTED(SYSTEM_NAME,"$system_name",[Define to the system name.])
AC_CACHE_VAL(cf_cv_system_name,[cf_cv_system_name="$system_name"])

test -z "$system_name" && system_name="$cf_cv_system_name"
test -n "$cf_cv_system_name" && AC_MSG_RESULT(Configuring for $cf_cv_system_name)

if test ".$system_name" != ".$cf_cv_system_name" ; then
	AC_MSG_RESULT(Cached system name ($system_name) does not agree with actual ($cf_cv_system_name))
	AC_MSG_ERROR("Please remove config.cache and try again.")
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CHECK_CFLAGS version: 3 updated: 2014/07/22 05:32:57
dnl ---------------
dnl Conditionally add to $CFLAGS and $CPPFLAGS values which are derived from
dnl a build-configuration such as imake.  These have the pitfall that they
dnl often contain compiler-specific options which we cannot use, mixed with
dnl preprocessor options that we usually can.
AC_DEFUN([CF_CHECK_CFLAGS],
[
CF_VERBOSE(checking additions to CFLAGS)
cf_check_cflags="$CFLAGS"
cf_check_cppflags="$CPPFLAGS"
CF_ADD_CFLAGS($1,yes)
if test "x$cf_check_cflags" != "x$CFLAGS" ; then
AC_TRY_LINK([#include <stdio.h>],[printf("Hello world");],,
	[CF_VERBOSE(test-compile failed.  Undoing change to \$CFLAGS)
	 if test "x$cf_check_cppflags" != "x$CPPFLAGS" ; then
		 CF_VERBOSE(but keeping change to \$CPPFLAGS)
	 fi
	 CFLAGS="$cf_check_flags"])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CHECK_LIBTOOL_VERSION version: 1 updated: 2013/04/06 18:03:09
dnl ------------------------
dnl Show the version of libtool
dnl
dnl Save the version in a cache variable - this is not entirely a good thing,
dnl but the version string from libtool is very ugly, and for bug reports it
dnl might be useful to have the original string.
AC_DEFUN([CF_CHECK_LIBTOOL_VERSION],[
if test -n "$LIBTOOL" && test "$LIBTOOL" != none
then
	AC_MSG_CHECKING(version of $LIBTOOL)
	CF_LIBTOOL_VERSION
	AC_MSG_RESULT($cf_cv_libtool_version)
	if test -z "$cf_cv_libtool_version" ; then
		AC_MSG_ERROR(This is not GNU libtool)
	fi
else
	AC_MSG_ERROR(GNU libtool has not been found)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_CLANG_COMPILER version: 2 updated: 2013/11/19 19:23:35
dnl -----------------
dnl Check if the given compiler is really clang.  clang's C driver defines
dnl __GNUC__ (fooling the configure script into setting $GCC to yes) but does
dnl not ignore some gcc options.
dnl
dnl This macro should be run "soon" after AC_PROG_CC or AC_PROG_CPLUSPLUS, to
dnl ensure that it is not mistaken for gcc/g++.  It is normally invoked from
dnl the wrappers for gcc and g++ warnings.
dnl
dnl $1 = GCC (default) or GXX
dnl $2 = CLANG_COMPILER (default)
dnl $3 = CFLAGS (default) or CXXFLAGS
AC_DEFUN([CF_CLANG_COMPILER],[
ifelse([$2],,CLANG_COMPILER,[$2])=no

if test "$ifelse([$1],,[$1],GCC)" = yes ; then
	AC_MSG_CHECKING(if this is really Clang ifelse([$1],GXX,C++,C) compiler)
	cf_save_CFLAGS="$ifelse([$3],,CFLAGS,[$3])"
	ifelse([$3],,CFLAGS,[$3])="$ifelse([$3],,CFLAGS,[$3]) -Qunused-arguments"
	AC_TRY_COMPILE([],[
#ifdef __clang__
#else
make an error
#endif
],[ifelse([$2],,CLANG_COMPILER,[$2])=yes
cf_save_CFLAGS="$cf_save_CFLAGS -Qunused-arguments"
],[])
	ifelse([$3],,CFLAGS,[$3])="$cf_save_CFLAGS"
	AC_MSG_RESULT($ifelse([$2],,CLANG_COMPILER,[$2]))
fi
])
dnl ---------------------------------------------------------------------------
dnl CF_DISABLE_ECHO version: 12 updated: 2012/10/06 16:30:28
dnl ---------------
dnl You can always use "make -n" to see the actual options, but it's hard to
dnl pick out/analyze warning messages when the compile-line is long.
dnl
dnl Sets:
dnl	ECHO_LT - symbol to control if libtool is verbose
dnl	ECHO_LD - symbol to prefix "cc -o" lines
dnl	RULE_CC - symbol to put before implicit "cc -c" lines (e.g., .c.o)
dnl	SHOW_CC - symbol to put before explicit "cc -c" lines
dnl	ECHO_CC - symbol to put before any "cc" line
dnl
AC_DEFUN([CF_DISABLE_ECHO],[
AC_MSG_CHECKING(if you want to see long compiling messages)
CF_ARG_DISABLE(echo,
	[  --disable-echo          do not display "compiling" commands],
	[
    ECHO_LT='--silent'
    ECHO_LD='@echo linking [$]@;'
    RULE_CC='@echo compiling [$]<'
    SHOW_CC='@echo compiling [$]@'
    ECHO_CC='@'
],[
    ECHO_LT=''
    ECHO_LD=''
    RULE_CC=''
    SHOW_CC=''
    ECHO_CC=''
])
AC_MSG_RESULT($enableval)
AC_SUBST(ECHO_LT)
AC_SUBST(ECHO_LD)
AC_SUBST(RULE_CC)
AC_SUBST(SHOW_CC)
AC_SUBST(ECHO_CC)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_DISABLE_LEAKS version: 7 updated: 2012/10/02 20:55:03
dnl ----------------
dnl Combine no-leak checks with the libraries or tools that are used for the
dnl checks.
AC_DEFUN([CF_DISABLE_LEAKS],[

AC_REQUIRE([CF_WITH_DMALLOC])
AC_REQUIRE([CF_WITH_DBMALLOC])
AC_REQUIRE([CF_WITH_VALGRIND])

AC_MSG_CHECKING(if you want to perform memory-leak testing)
AC_ARG_ENABLE(leaks,
	[  --disable-leaks         test: free permanent memory, analyze leaks],
	[if test "x$enableval" = xno; then with_no_leaks=yes; else with_no_leaks=no; fi],
	: ${with_no_leaks:=no})
AC_MSG_RESULT($with_no_leaks)

if test "$with_no_leaks" = yes ; then
	AC_DEFINE(NO_LEAKS,1,[Define to 1 if you want to perform memory-leak testing.])
	AC_DEFINE(YY_NO_LEAKS,1,[Define to 1 if you want to perform memory-leak testing.])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_DISABLE_LIBTOOL_VERSION version: 2 updated: 2014/11/15 19:05:29
dnl --------------------------
dnl Check if we should use the libtool 1.5 feature "-version-number" instead of
dnl the older "-version-info" feature.  The newer feature allows us to use
dnl version numbering on shared libraries which make them compatible with
dnl various systems.
AC_DEFUN([CF_DISABLE_LIBTOOL_VERSION],
[
AC_MSG_CHECKING(if libtool -version-number should be used)
CF_ARG_DISABLE(libtool-version,
	[  --disable-libtool-version  enable to use libtool's incompatible naming scheme],
	[cf_libtool_version=no],
	[cf_libtool_version=yes])
AC_MSG_RESULT($cf_libtool_version)

if test "$cf_libtool_version" = yes ; then
	LIBTOOL_VERSION="-version-number"
else
	LIBTOOL_VERSION="-version-info"
	case "x$VERSION" in #(vi
	x) #(vi
		AC_MSG_WARN(VERSION was not set)
		;;
	x*.*.*)
		ABI_VERSION="$VERSION"
		CF_VERBOSE(ABI_VERSION: $ABI_VERSION)
		;;
	x*:*:*)
		ABI_VERSION=`echo "$VERSION" | sed -e 's/:/./g'`
		CF_VERBOSE(ABI_VERSION: $ABI_VERSION)
		;;
	*)
		AC_MSG_WARN(unexpected VERSION value: $VERSION)
		;;
	esac
fi

AC_SUBST(ABI_VERSION)
AC_SUBST(LIBTOOL_VERSION)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_DISABLE_RPATH_HACK version: 2 updated: 2011/02/13 13:31:33
dnl ---------------------
dnl The rpath-hack makes it simpler to build programs, particularly with the
dnl *BSD ports which may have essential libraries in unusual places.  But it
dnl can interfere with building an executable for the base system.  Use this
dnl option in that case.
AC_DEFUN([CF_DISABLE_RPATH_HACK],
[
AC_MSG_CHECKING(if rpath-hack should be disabled)
CF_ARG_DISABLE(rpath-hack,
	[  --disable-rpath-hack    don't add rpath options for additional libraries],
	[cf_disable_rpath_hack=yes],
	[cf_disable_rpath_hack=no])
AC_MSG_RESULT($cf_disable_rpath_hack)
if test "$cf_disable_rpath_hack" = no ; then
	CF_RPATH_HACK
fi
])
dnl ---------------------------------------------------------------------------
dnl CF_ENABLE_RPATH version: 2 updated: 2010/03/27 18:39:42
dnl ---------------
dnl Check if the rpath option should be used, setting cache variable
dnl cf_cv_enable_rpath if so.
AC_DEFUN([CF_ENABLE_RPATH],
[
AC_MSG_CHECKING(if rpath option should be used)
AC_ARG_ENABLE(rpath,
[  --enable-rpath          use rpath option when generating shared libraries],
[cf_cv_enable_rpath=$enableval],
[cf_cv_enable_rpath=no])
AC_MSG_RESULT($cf_cv_enable_rpath)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_FORGET_TOOL version: 1 updated: 2013/04/06 18:03:09
dnl --------------
dnl Forget that we saw the given tool.
AC_DEFUN([CF_FORGET_TOOL],[
unset ac_cv_prog_ac_ct_$1
unset ac_ct_$1
unset $1
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GCC_ATTRIBUTES version: 16 updated: 2012/10/02 20:55:03
dnl -----------------
dnl Test for availability of useful gcc __attribute__ directives to quiet
dnl compiler warnings.  Though useful, not all are supported -- and contrary
dnl to documentation, unrecognized directives cause older compilers to barf.
AC_DEFUN([CF_GCC_ATTRIBUTES],
[
if test "$GCC" = yes
then
cat > conftest.i <<EOF
#ifndef GCC_PRINTF
#define GCC_PRINTF 0
#endif
#ifndef GCC_SCANF
#define GCC_SCANF 0
#endif
#ifndef GCC_NORETURN
#define GCC_NORETURN /* nothing */
#endif
#ifndef GCC_UNUSED
#define GCC_UNUSED /* nothing */
#endif
EOF
if test "$GCC" = yes
then
	AC_CHECKING([for $CC __attribute__ directives])
cat > conftest.$ac_ext <<EOF
#line __oline__ "${as_me:-configure}"
#include "confdefs.h"
#include "conftest.h"
#include "conftest.i"
#if	GCC_PRINTF
#define GCC_PRINTFLIKE(fmt,var) __attribute__((format(printf,fmt,var)))
#else
#define GCC_PRINTFLIKE(fmt,var) /*nothing*/
#endif
#if	GCC_SCANF
#define GCC_SCANFLIKE(fmt,var)  __attribute__((format(scanf,fmt,var)))
#else
#define GCC_SCANFLIKE(fmt,var)  /*nothing*/
#endif
extern void wow(char *,...) GCC_SCANFLIKE(1,2);
extern void oops(char *,...) GCC_PRINTFLIKE(1,2) GCC_NORETURN;
extern void foo(void) GCC_NORETURN;
int main(int argc GCC_UNUSED, char *argv[[]] GCC_UNUSED) { return 0; }
EOF
	cf_printf_attribute=no
	cf_scanf_attribute=no
	for cf_attribute in scanf printf unused noreturn
	do
		CF_UPPER(cf_ATTRIBUTE,$cf_attribute)
		cf_directive="__attribute__(($cf_attribute))"
		echo "checking for $CC $cf_directive" 1>&AC_FD_CC

		case $cf_attribute in #(vi
		printf) #(vi
			cf_printf_attribute=yes
			cat >conftest.h <<EOF
#define GCC_$cf_ATTRIBUTE 1
EOF
			;;
		scanf) #(vi
			cf_scanf_attribute=yes
			cat >conftest.h <<EOF
#define GCC_$cf_ATTRIBUTE 1
EOF
			;;
		*) #(vi
			cat >conftest.h <<EOF
#define GCC_$cf_ATTRIBUTE $cf_directive
EOF
			;;
		esac

		if AC_TRY_EVAL(ac_compile); then
			test -n "$verbose" && AC_MSG_RESULT(... $cf_attribute)
			cat conftest.h >>confdefs.h
			case $cf_attribute in #(vi
			noreturn) #(vi
				AC_DEFINE_UNQUOTED(GCC_NORETURN,$cf_directive,[Define to noreturn-attribute for gcc])
				;;
			printf) #(vi
				cf_value='/* nothing */'
				if test "$cf_printf_attribute" != no ; then
					cf_value='__attribute__((format(printf,fmt,var)))'
					AC_DEFINE(GCC_PRINTF,1,[Define to 1 if the compiler supports gcc-like printf attribute.])
				fi
				AC_DEFINE_UNQUOTED(GCC_PRINTFLIKE(fmt,var),$cf_value,[Define to printf-attribute for gcc])
				;;
			scanf) #(vi
				cf_value='/* nothing */'
				if test "$cf_scanf_attribute" != no ; then
					cf_value='__attribute__((format(scanf,fmt,var)))'
					AC_DEFINE(GCC_SCANF,1,[Define to 1 if the compiler supports gcc-like scanf attribute.])
				fi
				AC_DEFINE_UNQUOTED(GCC_SCANFLIKE(fmt,var),$cf_value,[Define to sscanf-attribute for gcc])
				;;
			unused) #(vi
				AC_DEFINE_UNQUOTED(GCC_UNUSED,$cf_directive,[Define to unused-attribute for gcc])
				;;
			esac
		fi
	done
else
	fgrep define conftest.i >>confdefs.h
fi
rm -rf conftest*
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GCC_VERSION version: 7 updated: 2012/10/18 06:46:33
dnl --------------
dnl Find version of gcc
AC_DEFUN([CF_GCC_VERSION],[
AC_REQUIRE([AC_PROG_CC])
GCC_VERSION=none
if test "$GCC" = yes ; then
	AC_MSG_CHECKING(version of $CC)
	GCC_VERSION="`${CC} --version 2>/dev/null | sed -e '2,$d' -e 's/^.*(GCC[[^)]]*) //' -e 's/^.*(Debian[[^)]]*) //' -e 's/^[[^0-9.]]*//' -e 's/[[^0-9.]].*//'`"
	test -z "$GCC_VERSION" && GCC_VERSION=unknown
	AC_MSG_RESULT($GCC_VERSION)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GCC_WARNINGS version: 31 updated: 2013/11/19 19:23:35
dnl ---------------
dnl Check if the compiler supports useful warning options.  There's a few that
dnl we don't use, simply because they're too noisy:
dnl
dnl	-Wconversion (useful in older versions of gcc, but not in gcc 2.7.x)
dnl	-Wredundant-decls (system headers make this too noisy)
dnl	-Wtraditional (combines too many unrelated messages, only a few useful)
dnl	-Wwrite-strings (too noisy, but should review occasionally).  This
dnl		is enabled for ncurses using "--enable-const".
dnl	-pedantic
dnl
dnl Parameter:
dnl	$1 is an optional list of gcc warning flags that a particular
dnl		application might want to use, e.g., "no-unused" for
dnl		-Wno-unused
dnl Special:
dnl	If $with_ext_const is "yes", add a check for -Wwrite-strings
dnl
AC_DEFUN([CF_GCC_WARNINGS],
[
AC_REQUIRE([CF_GCC_VERSION])
CF_INTEL_COMPILER(GCC,INTEL_COMPILER,CFLAGS)
CF_CLANG_COMPILER(GCC,CLANG_COMPILER,CFLAGS)

cat > conftest.$ac_ext <<EOF
#line __oline__ "${as_me:-configure}"
int main(int argc, char *argv[[]]) { return (argv[[argc-1]] == 0) ; }
EOF

if test "$INTEL_COMPILER" = yes
then
# The "-wdXXX" options suppress warnings:
# remark #1419: external declaration in primary source file
# remark #1683: explicit conversion of a 64-bit integral type to a smaller integral type (potential portability problem)
# remark #1684: conversion from pointer to same-sized integral type (potential portability problem)
# remark #193: zero used for undefined preprocessing identifier
# remark #593: variable "curs_sb_left_arrow" was set but never used
# remark #810: conversion from "int" to "Dimension={unsigned short}" may lose significant bits
# remark #869: parameter "tw" was never referenced
# remark #981: operands are evaluated in unspecified order
# warning #279: controlling expression is constant

	AC_CHECKING([for $CC warning options])
	cf_save_CFLAGS="$CFLAGS"
	EXTRA_CFLAGS="-Wall"
	for cf_opt in \
		wd1419 \
		wd1683 \
		wd1684 \
		wd193 \
		wd593 \
		wd279 \
		wd810 \
		wd869 \
		wd981
	do
		CFLAGS="$cf_save_CFLAGS $EXTRA_CFLAGS -$cf_opt"
		if AC_TRY_EVAL(ac_compile); then
			test -n "$verbose" && AC_MSG_RESULT(... -$cf_opt)
			EXTRA_CFLAGS="$EXTRA_CFLAGS -$cf_opt"
		fi
	done
	CFLAGS="$cf_save_CFLAGS"

elif test "$GCC" = yes
then
	AC_CHECKING([for $CC warning options])
	cf_save_CFLAGS="$CFLAGS"
	EXTRA_CFLAGS=
	cf_warn_CONST=""
	test "$with_ext_const" = yes && cf_warn_CONST="Wwrite-strings"
	cf_gcc_warnings="Wignored-qualifiers Wlogical-op Wvarargs"
	test "x$CLANG_COMPILER" = xyes && cf_gcc_warnings=
	for cf_opt in W Wall \
		Wbad-function-cast \
		Wcast-align \
		Wcast-qual \
		Wdeclaration-after-statement \
		Wextra \
		Winline \
		Wmissing-declarations \
		Wmissing-prototypes \
		Wnested-externs \
		Wpointer-arith \
		Wshadow \
		Wstrict-prototypes \
		Wundef $cf_gcc_warnings $cf_warn_CONST $1
	do
		CFLAGS="$cf_save_CFLAGS $EXTRA_CFLAGS -$cf_opt"
		if AC_TRY_EVAL(ac_compile); then
			test -n "$verbose" && AC_MSG_RESULT(... -$cf_opt)
			case $cf_opt in #(vi
			Wcast-qual) #(vi
				CPPFLAGS="$CPPFLAGS -DXTSTRINGDEFINES"
				;;
			Winline) #(vi
				case $GCC_VERSION in
				[[34]].*)
					CF_VERBOSE(feature is broken in gcc $GCC_VERSION)
					continue;;
				esac
				;;
			Wpointer-arith) #(vi
				case $GCC_VERSION in
				[[12]].*)
					CF_VERBOSE(feature is broken in gcc $GCC_VERSION)
					continue;;
				esac
				;;
			esac
			EXTRA_CFLAGS="$EXTRA_CFLAGS -$cf_opt"
		fi
	done
	CFLAGS="$cf_save_CFLAGS"
fi
rm -rf conftest*

AC_SUBST(EXTRA_CFLAGS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_GNU_SOURCE version: 6 updated: 2005/07/09 13:23:07
dnl -------------
dnl Check if we must define _GNU_SOURCE to get a reasonable value for
dnl _XOPEN_SOURCE, upon which many POSIX definitions depend.  This is a defect
dnl (or misfeature) of glibc2, which breaks portability of many applications,
dnl since it is interwoven with GNU extensions.
dnl
dnl Well, yes we could work around it...
AC_DEFUN([CF_GNU_SOURCE],
[
AC_CACHE_CHECK(if we must define _GNU_SOURCE,cf_cv_gnu_source,[
AC_TRY_COMPILE([#include <sys/types.h>],[
#ifndef _XOPEN_SOURCE
make an error
#endif],
	[cf_cv_gnu_source=no],
	[cf_save="$CPPFLAGS"
	 CPPFLAGS="$CPPFLAGS -D_GNU_SOURCE"
	 AC_TRY_COMPILE([#include <sys/types.h>],[
#ifdef _XOPEN_SOURCE
make an error
#endif],
	[cf_cv_gnu_source=no],
	[cf_cv_gnu_source=yes])
	CPPFLAGS="$cf_save"
	])
])
test "$cf_cv_gnu_source" = yes && CPPFLAGS="$CPPFLAGS -D_GNU_SOURCE"
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_HELP_MESSAGE version: 3 updated: 1998/01/14 10:56:23
dnl ---------------
dnl Insert text into the help-message, for readability, from AC_ARG_WITH.
AC_DEFUN([CF_HELP_MESSAGE],
[AC_DIVERT_HELP([$1])dnl
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_INTEL_COMPILER version: 6 updated: 2014/03/17 13:13:07
dnl -----------------
dnl Check if the given compiler is really the Intel compiler for Linux.  It
dnl tries to imitate gcc, but does not return an error when it finds a mismatch
dnl between prototypes, e.g., as exercised by CF_MISSING_CHECK.
dnl
dnl This macro should be run "soon" after AC_PROG_CC or AC_PROG_CPLUSPLUS, to
dnl ensure that it is not mistaken for gcc/g++.  It is normally invoked from
dnl the wrappers for gcc and g++ warnings.
dnl
dnl $1 = GCC (default) or GXX
dnl $2 = INTEL_COMPILER (default) or INTEL_CPLUSPLUS
dnl $3 = CFLAGS (default) or CXXFLAGS
AC_DEFUN([CF_INTEL_COMPILER],[
AC_REQUIRE([AC_CANONICAL_HOST])
ifelse([$2],,INTEL_COMPILER,[$2])=no

if test "$ifelse([$1],,[$1],GCC)" = yes ; then
	case $host_os in
	linux*|gnu*)
		AC_MSG_CHECKING(if this is really Intel ifelse([$1],GXX,C++,C) compiler)
		cf_save_CFLAGS="$ifelse([$3],,CFLAGS,[$3])"
		ifelse([$3],,CFLAGS,[$3])="$ifelse([$3],,CFLAGS,[$3]) -no-gcc"
		AC_TRY_COMPILE([],[
#ifdef __INTEL_COMPILER
#else
make an error
#endif
],[ifelse([$2],,INTEL_COMPILER,[$2])=yes
cf_save_CFLAGS="$cf_save_CFLAGS -we147"
],[])
		ifelse([$3],,CFLAGS,[$3])="$cf_save_CFLAGS"
		AC_MSG_RESULT($ifelse([$2],,INTEL_COMPILER,[$2]))
		;;
	esac
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_LD_RPATH_OPT version: 5 updated: 2011/07/17 14:48:41
dnl ---------------
dnl For the given system and compiler, find the compiler flags to pass to the
dnl loader to use the "rpath" feature.
AC_DEFUN([CF_LD_RPATH_OPT],
[
AC_REQUIRE([CF_CHECK_CACHE])

LD_RPATH_OPT=
AC_MSG_CHECKING(for an rpath option)
case $cf_cv_system_name in #(vi
irix*) #(vi
	if test "$GCC" = yes; then
		LD_RPATH_OPT="-Wl,-rpath,"
	else
		LD_RPATH_OPT="-rpath "
	fi
	;;
linux*|gnu*|k*bsd*-gnu) #(vi
	LD_RPATH_OPT="-Wl,-rpath,"
	;;
openbsd[[2-9]].*|mirbsd*) #(vi
	LD_RPATH_OPT="-Wl,-rpath,"
	;;
dragonfly*|freebsd*) #(vi
	LD_RPATH_OPT="-rpath "
	;;
netbsd*) #(vi
	LD_RPATH_OPT="-Wl,-rpath,"
	;;
osf*|mls+*) #(vi
	LD_RPATH_OPT="-rpath "
	;;
solaris2*) #(vi
	LD_RPATH_OPT="-R"
	;;
*)
	;;
esac
AC_MSG_RESULT($LD_RPATH_OPT)

case "x$LD_RPATH_OPT" in #(vi
x-R*)
	AC_MSG_CHECKING(if we need a space after rpath option)
	cf_save_LIBS="$LIBS"
	CF_ADD_LIBS(${LD_RPATH_OPT}$libdir)
	AC_TRY_LINK(, , cf_rpath_space=no, cf_rpath_space=yes)
	LIBS="$cf_save_LIBS"
	AC_MSG_RESULT($cf_rpath_space)
	test "$cf_rpath_space" = yes && LD_RPATH_OPT="$LD_RPATH_OPT "
	;;
esac
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_LIBTOOL_VERSION version: 1 updated: 2013/04/06 18:03:09
dnl ------------------
AC_DEFUN([CF_LIBTOOL_VERSION],[
if test -n "$LIBTOOL" && test "$LIBTOOL" != none
then
	cf_cv_libtool_version=`$LIBTOOL --version 2>&1 | sed -e '/^$/d' |sed -e '2,$d' -e 's/([[^)]]*)//g' -e 's/^[[^1-9]]*//' -e 's/[[^0-9.]].*//'`
else
	cf_cv_libtool_version=
fi
test -z "$cf_cv_libtool_version" && unset cf_cv_libtool_version
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_LIB_PREFIX version: 9 updated: 2012/01/21 19:28:10
dnl -------------
dnl Compute the library-prefix for the given host system
dnl $1 = variable to set
define([CF_LIB_PREFIX],
[
	case $cf_cv_system_name in #(vi
	OS/2*|os2*) #(vi
        LIB_PREFIX=''
        ;;
	*)	LIB_PREFIX='lib'
        ;;
	esac
ifelse($1,,,[$1=$LIB_PREFIX])
	AC_SUBST(LIB_PREFIX)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_LIB_SUFFIX version: 23 updated: 2014/06/21 17:47:12
dnl -------------
dnl Compute the library file-suffix from the given model name
dnl $1 = model name
dnl $2 = variable to set (the nominal library suffix)
dnl $3 = dependency variable to set (actual filename)
dnl The variable $LIB_SUFFIX, if set, prepends the variable to set.
AC_DEFUN([CF_LIB_SUFFIX],
[
	case X$1 in #(vi
	Xlibtool) #(vi
		$2='.la'
		$3=[$]$2
		;;
	Xdebug) #(vi
		$2='_g.a'
		$3=[$]$2
		;;
	Xprofile) #(vi
		$2='_p.a'
		$3=[$]$2
		;;
	Xshared) #(vi
		case $cf_cv_system_name in
		aix[[5-7]]*) #(vi
			$2='.so'
			$3=[$]$2
			;;
		cygwin*|msys*|mingw*) #(vi
			$2='.dll'
			$3='.dll.a'
			;;
		darwin*) #(vi
			$2='.dylib'
			$3=[$]$2
			;;
		hpux*) #(vi
			case $target in
			ia64*) #(vi
				$2='.so'
				$3=[$]$2
				;;
			*) #(vi
				$2='.sl'
				$3=[$]$2
				;;
			esac
			;;
		*) #(vi
			$2='.so'
			$3=[$]$2
			;;
		esac
		;;
	*)
		$2='.a'
		$3=[$]$2
		;;
	esac
	test -n "$LIB_SUFFIX" && $2="${LIB_SUFFIX}[$]{$2}"
	test -n "$LIB_SUFFIX" && $3="${LIB_SUFFIX}[$]{$3}"
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MAKEFLAGS version: 15 updated: 2014/05/10 16:43:23
dnl ------------
dnl Some 'make' programs support ${MAKEFLAGS}, some ${MFLAGS}, to pass 'make'
dnl options to lower-levels.  It's very useful for "make -n" -- if we have it.
dnl (GNU 'make' does both, something POSIX 'make', which happens to make the
dnl ${MAKEFLAGS} variable incompatible because it adds the assignments :-)
AC_DEFUN([CF_MAKEFLAGS],
[
AC_CACHE_CHECK(for makeflags variable, cf_cv_makeflags,[
	cf_cv_makeflags=''
	for cf_option in '-${MAKEFLAGS}' '${MFLAGS}'
	do
		cat >cf_makeflags.tmp <<CF_EOF
SHELL = /bin/sh
all :
	@ echo '.$cf_option'
CF_EOF
		cf_result=`${MAKE:-make} -k -f cf_makeflags.tmp 2>/dev/null | fgrep -v "ing directory" | sed -e 's,[[ 	]]*$,,'`
		case "$cf_result" in
		.*k|.*kw)
			cf_result=`${MAKE:-make} -k -f cf_makeflags.tmp CC=cc 2>/dev/null`
			case "$cf_result" in
			.*CC=*)	cf_cv_makeflags=
				;;
			*)	cf_cv_makeflags=$cf_option
				;;
			esac
			break
			;;
		.-)	;;
		*)	echo "given option \"$cf_option\", no match \"$cf_result\""
			;;
		esac
	done
	rm -f cf_makeflags.tmp
])

AC_SUBST(cf_cv_makeflags)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MAKE_TAGS version: 6 updated: 2010/10/23 15:52:32
dnl ------------
dnl Generate tags/TAGS targets for makefiles.  Do not generate TAGS if we have
dnl a monocase filesystem.
AC_DEFUN([CF_MAKE_TAGS],[
AC_REQUIRE([CF_MIXEDCASE_FILENAMES])

AC_CHECK_PROGS(CTAGS, exctags ctags)
AC_CHECK_PROGS(ETAGS, exetags etags)

AC_CHECK_PROG(MAKE_LOWER_TAGS, ${CTAGS:-ctags}, yes, no)

if test "$cf_cv_mixedcase" = yes ; then
	AC_CHECK_PROG(MAKE_UPPER_TAGS, ${ETAGS:-etags}, yes, no)
else
	MAKE_UPPER_TAGS=no
fi

if test "$MAKE_UPPER_TAGS" = yes ; then
	MAKE_UPPER_TAGS=
else
	MAKE_UPPER_TAGS="#"
fi

if test "$MAKE_LOWER_TAGS" = yes ; then
	MAKE_LOWER_TAGS=
else
	MAKE_LOWER_TAGS="#"
fi

AC_SUBST(CTAGS)
AC_SUBST(ETAGS)

AC_SUBST(MAKE_UPPER_TAGS)
AC_SUBST(MAKE_LOWER_TAGS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MIXEDCASE_FILENAMES version: 6 updated: 2013/10/08 17:47:05
dnl ----------------------
dnl Check if the file-system supports mixed-case filenames.  If we're able to
dnl create a lowercase name and see it as uppercase, it doesn't support that.
AC_DEFUN([CF_MIXEDCASE_FILENAMES],
[
AC_CACHE_CHECK(if filesystem supports mixed-case filenames,cf_cv_mixedcase,[
if test "$cross_compiling" = yes ; then
	case $target_alias in #(vi
	*-os2-emx*|*-msdosdjgpp*|*-cygwin*|*-msys*|*-mingw*|*-uwin*) #(vi
		cf_cv_mixedcase=no
		;;
	*)
		cf_cv_mixedcase=yes
		;;
	esac
else
	rm -f conftest CONFTEST
	echo test >conftest
	if test -f CONFTEST ; then
		cf_cv_mixedcase=no
	else
		cf_cv_mixedcase=yes
	fi
	rm -f conftest CONFTEST
fi
])
test "$cf_cv_mixedcase" = yes && AC_DEFINE(MIXEDCASE_FILENAMES,1,[Define to 1 if filesystem supports mixed-case filenames.])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_MSG_LOG version: 5 updated: 2010/10/23 15:52:32
dnl ----------
dnl Write a debug message to config.log, along with the line number in the
dnl configure script.
AC_DEFUN([CF_MSG_LOG],[
echo "${as_me:-configure}:__oline__: testing $* ..." 1>&AC_FD_CC
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NO_LEAKS_OPTION version: 5 updated: 2012/10/02 20:55:03
dnl ------------------
dnl see CF_WITH_NO_LEAKS
AC_DEFUN([CF_NO_LEAKS_OPTION],[
AC_MSG_CHECKING(if you want to use $1 for testing)
AC_ARG_WITH($1,
	[$2],
	[AC_DEFINE_UNQUOTED($3,1,"Define to 1 if you want to use $1 for testing.")ifelse([$4],,[
	 $4
])
	: ${with_cflags:=-g}
	: ${with_no_leaks:=yes}
	 with_$1=yes],
	[with_$1=])
AC_MSG_RESULT(${with_$1:-no})

case .$with_cflags in #(vi
.*-g*)
	case .$CFLAGS in #(vi
	.*-g*) #(vi
		;;
	*)
		CF_ADD_CFLAGS([-g])
		;;
	esac
	;;
esac
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_NUMBER_SYNTAX version: 1 updated: 2003/09/20 18:12:49
dnl ----------------
dnl Check if the given variable is a number.  If not, report an error.
dnl $1 is the variable
dnl $2 is the message
AC_DEFUN([CF_NUMBER_SYNTAX],[
if test -n "$1" ; then
  case $1 in #(vi
  [[0-9]]*) #(vi
 	;;
  *)
	AC_MSG_ERROR($2 is not a number: $1)
 	;;
  esac
else
  AC_MSG_ERROR($2 value is empty)
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PATH_SYNTAX version: 14 updated: 2012/06/19 20:58:54
dnl --------------
dnl Check the argument to see that it looks like a pathname.  Rewrite it if it
dnl begins with one of the prefix/exec_prefix variables, and then again if the
dnl result begins with 'NONE'.  This is necessary to work around autoconf's
dnl delayed evaluation of those symbols.
AC_DEFUN([CF_PATH_SYNTAX],[
if test "x$prefix" != xNONE; then
  cf_path_syntax="$prefix"
else
  cf_path_syntax="$ac_default_prefix"
fi

case ".[$]$1" in #(vi
.\[$]\(*\)*|.\'*\'*) #(vi
  ;;
..|./*|.\\*) #(vi
  ;;
.[[a-zA-Z]]:[[\\/]]*) #(vi OS/2 EMX
  ;;
.\[$]{*prefix}*|.\[$]{*dir}*) #(vi
  eval $1="[$]$1"
  case ".[$]$1" in #(vi
  .NONE/*)
    $1=`echo [$]$1 | sed -e s%NONE%$cf_path_syntax%`
    ;;
  esac
  ;; #(vi
.no|.NONE/*)
  $1=`echo [$]$1 | sed -e s%NONE%$cf_path_syntax%`
  ;;
*)
  ifelse([$2],,[AC_MSG_ERROR([expected a pathname, not \"[$]$1\"])],$2)
  ;;
esac
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PKG_CONFIG version: 8 updated: 2014/12/13 18:48:46
dnl -------------
dnl Check for the package-config program, unless disabled by command-line.
AC_DEFUN([CF_PKG_CONFIG],
[
AC_MSG_CHECKING(if you want to use pkg-config)
AC_ARG_WITH(pkg-config,
	[  --with-pkg-config{=path} enable/disable use of pkg-config],
	[cf_pkg_config=$withval],
	[cf_pkg_config=yes])
AC_MSG_RESULT($cf_pkg_config)

case $cf_pkg_config in #(vi
no) #(vi
	PKG_CONFIG=none
	;;
yes) #(vi
	CF_ACVERSION_CHECK(2.52,
		[AC_PATH_TOOL(PKG_CONFIG, pkg-config, none)],
		[AC_PATH_PROG(PKG_CONFIG, pkg-config, none)])
	;;
*)
	PKG_CONFIG=$withval
	;;
esac

test -z "$PKG_CONFIG" && PKG_CONFIG=none
if test "$PKG_CONFIG" != none ; then
	CF_PATH_SYNTAX(PKG_CONFIG)
else
	AC_MSG_WARN(pkg-config is not installed)
fi

AC_SUBST(PKG_CONFIG)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_POSIX_C_SOURCE version: 8 updated: 2010/05/26 05:38:42
dnl -----------------
dnl Define _POSIX_C_SOURCE to the given level, and _POSIX_SOURCE if needed.
dnl
dnl	POSIX.1-1990				_POSIX_SOURCE
dnl	POSIX.1-1990 and			_POSIX_SOURCE and
dnl		POSIX.2-1992 C-Language			_POSIX_C_SOURCE=2
dnl		Bindings Option
dnl	POSIX.1b-1993				_POSIX_C_SOURCE=199309L
dnl	POSIX.1c-1996				_POSIX_C_SOURCE=199506L
dnl	X/Open 2000				_POSIX_C_SOURCE=200112L
dnl
dnl Parameters:
dnl	$1 is the nominal value for _POSIX_C_SOURCE
AC_DEFUN([CF_POSIX_C_SOURCE],
[
cf_POSIX_C_SOURCE=ifelse([$1],,199506L,[$1])

cf_save_CFLAGS="$CFLAGS"
cf_save_CPPFLAGS="$CPPFLAGS"

CF_REMOVE_DEFINE(cf_trim_CFLAGS,$cf_save_CFLAGS,_POSIX_C_SOURCE)
CF_REMOVE_DEFINE(cf_trim_CPPFLAGS,$cf_save_CPPFLAGS,_POSIX_C_SOURCE)

AC_CACHE_CHECK(if we should define _POSIX_C_SOURCE,cf_cv_posix_c_source,[
	CF_MSG_LOG(if the symbol is already defined go no further)
	AC_TRY_COMPILE([#include <sys/types.h>],[
#ifndef _POSIX_C_SOURCE
make an error
#endif],
	[cf_cv_posix_c_source=no],
	[cf_want_posix_source=no
	 case .$cf_POSIX_C_SOURCE in #(vi
	 .[[12]]??*) #(vi
		cf_cv_posix_c_source="-D_POSIX_C_SOURCE=$cf_POSIX_C_SOURCE"
		;;
	 .2) #(vi
		cf_cv_posix_c_source="-D_POSIX_C_SOURCE=$cf_POSIX_C_SOURCE"
		cf_want_posix_source=yes
		;;
	 .*)
		cf_want_posix_source=yes
		;;
	 esac
	 if test "$cf_want_posix_source" = yes ; then
		AC_TRY_COMPILE([#include <sys/types.h>],[
#ifdef _POSIX_SOURCE
make an error
#endif],[],
		cf_cv_posix_c_source="$cf_cv_posix_c_source -D_POSIX_SOURCE")
	 fi
	 CF_MSG_LOG(ifdef from value $cf_POSIX_C_SOURCE)
	 CFLAGS="$cf_trim_CFLAGS"
	 CPPFLAGS="$cf_trim_CPPFLAGS $cf_cv_posix_c_source"
	 CF_MSG_LOG(if the second compile does not leave our definition intact error)
	 AC_TRY_COMPILE([#include <sys/types.h>],[
#ifndef _POSIX_C_SOURCE
make an error
#endif],,
	 [cf_cv_posix_c_source=no])
	 CFLAGS="$cf_save_CFLAGS"
	 CPPFLAGS="$cf_save_CPPFLAGS"
	])
])

if test "$cf_cv_posix_c_source" != no ; then
	CFLAGS="$cf_trim_CFLAGS"
	CPPFLAGS="$cf_trim_CPPFLAGS"
	CF_ADD_CFLAGS($cf_cv_posix_c_source)
fi

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PROG_CC version: 4 updated: 2014/07/12 18:57:58
dnl ----------
dnl standard check for CC, plus followup sanity checks
dnl $1 = optional parameter to pass to AC_PROG_CC to specify compiler name
AC_DEFUN([CF_PROG_CC],[
ifelse($1,,[AC_PROG_CC],[AC_PROG_CC($1)])
CF_GCC_VERSION
CF_ACVERSION_CHECK(2.52,
	[AC_PROG_CC_STDC],
	[CF_ANSI_CC_REQD])
CF_CC_ENV_FLAGS
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_PROG_LINT version: 2 updated: 2009/08/12 04:43:14
dnl ------------
AC_DEFUN([CF_PROG_LINT],
[
AC_CHECK_PROGS(LINT, tdlint lint alint splint lclint)
AC_SUBST(LINT_OPTS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_REMOVE_DEFINE version: 3 updated: 2010/01/09 11:05:50
dnl ----------------
dnl Remove all -U and -D options that refer to the given symbol from a list
dnl of C compiler options.  This works around the problem that not all
dnl compilers process -U and -D options from left-to-right, so a -U option
dnl cannot be used to cancel the effect of a preceding -D option.
dnl
dnl $1 = target (which could be the same as the source variable)
dnl $2 = source (including '$')
dnl $3 = symbol to remove
define([CF_REMOVE_DEFINE],
[
$1=`echo "$2" | \
	sed	-e 's/-[[UD]]'"$3"'\(=[[^ 	]]*\)\?[[ 	]]/ /g' \
		-e 's/-[[UD]]'"$3"'\(=[[^ 	]]*\)\?[$]//g'`
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_RPATH_HACK version: 11 updated: 2013/09/01 13:02:00
dnl -------------
AC_DEFUN([CF_RPATH_HACK],
[
AC_REQUIRE([CF_LD_RPATH_OPT])
AC_MSG_CHECKING(for updated LDFLAGS)
if test -n "$LD_RPATH_OPT" ; then
	AC_MSG_RESULT(maybe)

	AC_CHECK_PROGS(cf_ldd_prog,ldd,no)
	cf_rpath_list="/usr/lib /lib"
	if test "$cf_ldd_prog" != no
	then
		cf_rpath_oops=

AC_TRY_LINK([#include <stdio.h>],
		[printf("Hello");],
		[cf_rpath_oops=`$cf_ldd_prog conftest$ac_exeext | fgrep ' not found' | sed -e 's% =>.*$%%' |sort | uniq`
		 cf_rpath_list=`$cf_ldd_prog conftest$ac_exeext | fgrep / | sed -e 's%^.*[[ 	]]/%/%' -e 's%/[[^/]][[^/]]*$%%' |sort | uniq`])

		# If we passed the link-test, but get a "not found" on a given library,
		# this could be due to inept reconfiguration of gcc to make it only
		# partly honor /usr/local/lib (or whatever).  Sometimes this behavior
		# is intentional, e.g., installing gcc in /usr/bin and suppressing the
		# /usr/local libraries.
		if test -n "$cf_rpath_oops"
		then
			for cf_rpath_src in $cf_rpath_oops
			do
				for cf_rpath_dir in \
					/usr/local \
					/usr/pkg \
					/opt/sfw
				do
					if test -f $cf_rpath_dir/lib/$cf_rpath_src
					then
						CF_VERBOSE(...adding -L$cf_rpath_dir/lib to LDFLAGS for $cf_rpath_src)
						LDFLAGS="$LDFLAGS -L$cf_rpath_dir/lib"
						break
					fi
				done
			done
		fi
	fi

	CF_VERBOSE(...checking EXTRA_LDFLAGS $EXTRA_LDFLAGS)

	CF_RPATH_HACK_2(LDFLAGS)
	CF_RPATH_HACK_2(LIBS)

	CF_VERBOSE(...checked EXTRA_LDFLAGS $EXTRA_LDFLAGS)
else
	AC_MSG_RESULT(no)
fi
AC_SUBST(EXTRA_LDFLAGS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_RPATH_HACK_2 version: 6 updated: 2010/04/17 16:31:24
dnl ---------------
dnl Do one set of substitutions for CF_RPATH_HACK, adding an rpath option to
dnl EXTRA_LDFLAGS for each -L option found.
dnl
dnl $cf_rpath_list contains a list of directories to ignore.
dnl
dnl $1 = variable name to update.  The LDFLAGS variable should be the only one,
dnl      but LIBS often has misplaced -L options.
AC_DEFUN([CF_RPATH_HACK_2],
[
CF_VERBOSE(...checking $1 [$]$1)

cf_rpath_dst=
for cf_rpath_src in [$]$1
do
	case $cf_rpath_src in #(vi
	-L*) #(vi

		# check if this refers to a directory which we will ignore
		cf_rpath_skip=no
		if test -n "$cf_rpath_list"
		then
			for cf_rpath_item in $cf_rpath_list
			do
				if test "x$cf_rpath_src" = "x-L$cf_rpath_item"
				then
					cf_rpath_skip=yes
					break
				fi
			done
		fi

		if test "$cf_rpath_skip" = no
		then
			# transform the option
			if test "$LD_RPATH_OPT" = "-R " ; then
				cf_rpath_tmp=`echo "$cf_rpath_src" |sed -e "s%-L%-R %"`
			else
				cf_rpath_tmp=`echo "$cf_rpath_src" |sed -e "s%-L%$LD_RPATH_OPT%"`
			fi

			# if we have not already added this, add it now
			cf_rpath_tst=`echo "$EXTRA_LDFLAGS" | sed -e "s%$cf_rpath_tmp %%"`
			if test "x$cf_rpath_tst" = "x$EXTRA_LDFLAGS"
			then
				CF_VERBOSE(...Filter $cf_rpath_src ->$cf_rpath_tmp)
				EXTRA_LDFLAGS="$cf_rpath_tmp $EXTRA_LDFLAGS"
			fi
		fi
		;;
	esac
	cf_rpath_dst="$cf_rpath_dst $cf_rpath_src"
done
$1=$cf_rpath_dst

CF_VERBOSE(...checked $1 [$]$1)
AC_SUBST(EXTRA_LDFLAGS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_SHARED_OPTS version: 84 updated: 2013/11/03 06:26:10
dnl --------------
dnl --------------
dnl Attempt to determine the appropriate CC/LD options for creating a shared
dnl library.
dnl
dnl Notes:
dnl a) ${LOCAL_LDFLAGS} is used to link executables that will run within
dnl the build-tree, i.e., by making use of the libraries that are compiled in
dnl $rel_builddir/lib We avoid compiling-in a $rel_builddir/lib path for the
dnl shared library since that can lead to unexpected results at runtime.
dnl b) ${LOCAL_LDFLAGS2} has the same intention but assumes that the shared
dnl libraries are compiled in ../../lib
dnl
dnl The variable 'cf_cv_do_symlinks' is used to control whether we configure
dnl to install symbolic links to the rel/abi versions of shared libraries.
dnl
dnl The variable 'cf_cv_shlib_version' controls whether we use the rel or abi
dnl version when making symbolic links.
dnl
dnl The variable 'cf_cv_shlib_version_infix' controls whether shared library
dnl version numbers are infix (ex: libncurses.<ver>.dylib) or postfix
dnl (ex: libncurses.so.<ver>).
dnl
dnl Some loaders leave 'so_locations' lying around.  It's nice to clean up.
AC_DEFUN([CF_SHARED_OPTS],
[
	AC_REQUIRE([CF_LD_RPATH_OPT])
	RM_SHARED_OPTS=
	LOCAL_LDFLAGS=
	LOCAL_LDFLAGS2=
	LD_SHARED_OPTS=
	INSTALL_LIB="-m 644"
	: ${rel_builddir:=.}

	shlibdir=$libdir
	AC_SUBST(shlibdir)

	MAKE_DLLS="#"
	AC_SUBST(MAKE_DLLS)

	cf_cv_do_symlinks=no
	cf_ld_rpath_opt=
	test "$cf_cv_enable_rpath" = yes && cf_ld_rpath_opt="$LD_RPATH_OPT"

	AC_MSG_CHECKING(if release/abi version should be used for shared libs)
	AC_ARG_WITH(shlib-version,
	[  --with-shlib-version=X  Specify rel or abi version for shared libs],
	[test -z "$withval" && withval=auto
	case $withval in #(vi
	yes) #(vi
		cf_cv_shlib_version=auto
		;;
	rel|abi|auto|no) #(vi
		cf_cv_shlib_version=$withval
		;;
	*)
		AC_MSG_ERROR([option value must be one of: rel, abi, auto or no])
		;;
	esac
	],[cf_cv_shlib_version=auto])
	AC_MSG_RESULT($cf_cv_shlib_version)

	cf_cv_rm_so_locs=no
	cf_try_cflags=

	# Some less-capable ports of gcc support only -fpic
	CC_SHARED_OPTS=
	if test "$GCC" = yes
	then
		AC_MSG_CHECKING(which $CC option to use)
		cf_save_CFLAGS="$CFLAGS"
		for CC_SHARED_OPTS in -fPIC -fpic ''
		do
			CFLAGS="$cf_save_CFLAGS $CC_SHARED_OPTS"
			AC_TRY_COMPILE([#include <stdio.h>],[int x = 1],[break],[])
		done
		AC_MSG_RESULT($CC_SHARED_OPTS)
		CFLAGS="$cf_save_CFLAGS"
	fi

	cf_cv_shlib_version_infix=no

	case $cf_cv_system_name in #(vi
	aix4.[3-9]*|aix[[5-7]]*) #(vi
		if test "$GCC" = yes; then
			CC_SHARED_OPTS=
			MK_SHARED_LIB='${CC} -shared -Wl,-brtl -Wl,-blibpath:${RPATH_LIST}:/usr/lib -o [$]@'
		else
			# CC_SHARED_OPTS='-qpic=large -G'
			# perhaps "-bM:SRE -bnoentry -bexpall"
			MK_SHARED_LIB='${CC} -G -Wl,-brtl -Wl,-blibpath:${RPATH_LIST}:/usr/lib -o [$]@'
		fi
		;;
	beos*) #(vi
		MK_SHARED_LIB='${CC} ${CFLAGS} -o $[@] -Xlinker -soname=`basename $[@]` -nostart -e 0'
		;;
	cygwin*) #(vi
		CC_SHARED_OPTS=
		MK_SHARED_LIB='sh '$rel_builddir'/mk_shared_lib.sh [$]@ [$]{CC} [$]{CFLAGS}'
		RM_SHARED_OPTS="$RM_SHARED_OPTS $rel_builddir/mk_shared_lib.sh *.dll.a"
		cf_cv_shlib_version=cygdll
		cf_cv_shlib_version_infix=cygdll
		shlibdir=$bindir
		MAKE_DLLS=
		cat >mk_shared_lib.sh <<-CF_EOF
		#!/bin/sh
		SHARED_LIB=\[$]1
		IMPORT_LIB=\`echo "\[$]1" | sed -e 's/cyg/lib/' -e 's/[[0-9]]*\.dll[$]/.dll.a/'\`
		shift
		cat <<-EOF
		Linking shared library
		** SHARED_LIB \[$]SHARED_LIB
		** IMPORT_LIB \[$]IMPORT_LIB
EOF
		exec \[$]* -shared -Wl,--out-implib=\[$]{IMPORT_LIB} -Wl,--export-all-symbols -o \[$]{SHARED_LIB}
CF_EOF
		chmod +x mk_shared_lib.sh
		;;
	msys*) #(vi
		CC_SHARED_OPTS=
		MK_SHARED_LIB='sh '$rel_builddir'/mk_shared_lib.sh [$]@ [$]{CC} [$]{CFLAGS}'
		RM_SHARED_OPTS="$RM_SHARED_OPTS $rel_builddir/mk_shared_lib.sh *.dll.a"
		cf_cv_shlib_version=msysdll
		cf_cv_shlib_version_infix=msysdll
		shlibdir=$bindir
		MAKE_DLLS=
		cat >mk_shared_lib.sh <<-CF_EOF
		#!/bin/sh
		SHARED_LIB=\[$]1
		IMPORT_LIB=\`echo "\[$]1" | sed -e 's/msys-/lib/' -e 's/[[0-9]]*\.dll[$]/.dll.a/'\`
		shift
		cat <<-EOF
		Linking shared library
		** SHARED_LIB \[$]SHARED_LIB
		** IMPORT_LIB \[$]IMPORT_LIB
EOF
		exec \[$]* -shared -Wl,--out-implib=\[$]{IMPORT_LIB} -Wl,--export-all-symbols -o \[$]{SHARED_LIB}
CF_EOF
		chmod +x mk_shared_lib.sh
		;;
	darwin*) #(vi
		cf_try_cflags="no-cpp-precomp"
		CC_SHARED_OPTS="-dynamic"
		MK_SHARED_LIB='${CC} ${CFLAGS} -dynamiclib -install_name ${libdir}/`basename $[@]` -compatibility_version ${ABI_VERSION} -current_version ${ABI_VERSION} -o $[@]'
		test "$cf_cv_shlib_version" = auto && cf_cv_shlib_version=abi
		cf_cv_shlib_version_infix=yes
		AC_CACHE_CHECK([if ld -search_paths_first works], cf_cv_ldflags_search_paths_first, [
			cf_save_LDFLAGS=$LDFLAGS
			LDFLAGS="$LDFLAGS -Wl,-search_paths_first"
			AC_TRY_LINK(, [int i;], cf_cv_ldflags_search_paths_first=yes, cf_cv_ldflags_search_paths_first=no)
				LDFLAGS=$cf_save_LDFLAGS])
		if test $cf_cv_ldflags_search_paths_first = yes; then
			LDFLAGS="$LDFLAGS -Wl,-search_paths_first"
		fi
		;;
	hpux[[7-8]]*) #(vi
		# HP-UX 8.07 ld lacks "+b" option used for libdir search-list 
		if test "$GCC" != yes; then
			CC_SHARED_OPTS='+Z'
		fi
		MK_SHARED_LIB='${LD} -b -o $[@]'
		INSTALL_LIB="-m 555"
		;;
	hpux*) #(vi
		# (tested with gcc 2.7.2 -- I don't have c89)
		if test "$GCC" = yes; then
			LD_SHARED_OPTS='-Xlinker +b -Xlinker ${libdir}'
		else
			CC_SHARED_OPTS='+Z'
			LD_SHARED_OPTS='-Wl,+b,${libdir}'
		fi
		MK_SHARED_LIB='${LD} +b ${libdir} -b -o $[@]'
		# HP-UX shared libraries must be executable, and should be
		# readonly to exploit a quirk in the memory manager.
		INSTALL_LIB="-m 555"
		;;
	interix*)
		test "$cf_cv_shlib_version" = auto && cf_cv_shlib_version=rel
		if test "$cf_cv_shlib_version" = rel; then
			cf_shared_soname='`basename $@ .${REL_VERSION}`.${ABI_VERSION}'
		else
			cf_shared_soname='`basename $@`'
		fi
		CC_SHARED_OPTS=
		MK_SHARED_LIB='${CC} -shared -Wl,-rpath,${RPATH_LIST} -Wl,-h,'$cf_shared_soname' -o $@'
		;;
	irix*) #(vi
		if test "$cf_cv_enable_rpath" = yes ; then
			EXTRA_LDFLAGS="${cf_ld_rpath_opt}\${RPATH_LIST} $EXTRA_LDFLAGS"
		fi
		# tested with IRIX 5.2 and 'cc'.
		if test "$GCC" != yes; then
			CC_SHARED_OPTS='-KPIC'
			MK_SHARED_LIB='${CC} -shared -rdata_shared -soname `basename $[@]` -o $[@]'
		else
			MK_SHARED_LIB='${CC} -shared -Wl,-soname,`basename $[@]` -o $[@]'
		fi
		cf_cv_rm_so_locs=yes
		;;
	linux*|gnu*|k*bsd*-gnu) #(vi
		if test "$DFT_LWR_MODEL" = "shared" ; then
			LOCAL_LDFLAGS="${LD_RPATH_OPT}\$(LOCAL_LIBDIR)"
			LOCAL_LDFLAGS2="$LOCAL_LDFLAGS"
		fi
		if test "$cf_cv_enable_rpath" = yes ; then
			EXTRA_LDFLAGS="${cf_ld_rpath_opt}\${RPATH_LIST} $EXTRA_LDFLAGS"
		fi
		CF_SHARED_SONAME
		MK_SHARED_LIB='${CC} ${CFLAGS} -shared -Wl,-soname,'$cf_cv_shared_soname',-stats,-lc -o $[@]'
		;;
	mingw*) #(vi
		cf_cv_shlib_version=mingw
		cf_cv_shlib_version_infix=mingw
		shlibdir=$bindir
		MAKE_DLLS=
		if test "$DFT_LWR_MODEL" = "shared" ; then
			LOCAL_LDFLAGS="-Wl,--enable-auto-import"
			LOCAL_LDFLAGS2="$LOCAL_LDFLAGS"
			EXTRA_LDFLAGS="-Wl,--enable-auto-import $EXTRA_LDFLAGS"
		fi
		CC_SHARED_OPTS=
		MK_SHARED_LIB='sh '$rel_builddir'/mk_shared_lib.sh [$]@ [$]{CC} [$]{CFLAGS}'
		RM_SHARED_OPTS="$RM_SHARED_OPTS $rel_builddir/mk_shared_lib.sh *.dll.a"
		cat >mk_shared_lib.sh <<-CF_EOF
		#!/bin/sh
		SHARED_LIB=\[$]1
		IMPORT_LIB=\`echo "\[$]1" | sed -e 's/[[0-9]]*\.dll[$]/.dll.a/'\`
		shift
		cat <<-EOF
		Linking shared library
		** SHARED_LIB \[$]SHARED_LIB
		** IMPORT_LIB \[$]IMPORT_LIB
EOF
		exec \[$]* -shared -Wl,--enable-auto-import,--out-implib=\[$]{IMPORT_LIB} -Wl,--export-all-symbols -o \[$]{SHARED_LIB}
CF_EOF
		chmod +x mk_shared_lib.sh
		;;
	openbsd[[2-9]].*|mirbsd*) #(vi
		if test "$DFT_LWR_MODEL" = "shared" ; then
			LOCAL_LDFLAGS="${LD_RPATH_OPT}\$(LOCAL_LIBDIR)"
			LOCAL_LDFLAGS2="$LOCAL_LDFLAGS"
		fi
		if test "$cf_cv_enable_rpath" = yes ; then
			EXTRA_LDFLAGS="${cf_ld_rpath_opt}\${RPATH_LIST} $EXTRA_LDFLAGS"
		fi
		CC_SHARED_OPTS="$CC_SHARED_OPTS -DPIC"
		CF_SHARED_SONAME
		MK_SHARED_LIB='${CC} ${CFLAGS} -shared -Wl,-Bshareable,-soname,'$cf_cv_shared_soname',-stats,-lc -o $[@]'
		;;
	nto-qnx*|openbsd*|freebsd[[12]].*) #(vi
		CC_SHARED_OPTS="$CC_SHARED_OPTS -DPIC"
		MK_SHARED_LIB='${LD} -Bshareable -o $[@]'
		test "$cf_cv_shlib_version" = auto && cf_cv_shlib_version=rel
		;;
	dragonfly*|freebsd*) #(vi
		CC_SHARED_OPTS="$CC_SHARED_OPTS -DPIC"
		if test "$DFT_LWR_MODEL" = "shared" && test "$cf_cv_enable_rpath" = yes ; then
			LOCAL_LDFLAGS="${cf_ld_rpath_opt}\$(LOCAL_LIBDIR)"
			LOCAL_LDFLAGS2="${cf_ld_rpath_opt}\${RPATH_LIST} $LOCAL_LDFLAGS"
			EXTRA_LDFLAGS="${cf_ld_rpath_opt}\${RPATH_LIST} $EXTRA_LDFLAGS"
		fi
		CF_SHARED_SONAME
		MK_SHARED_LIB='${LD} -shared -Bshareable -soname=`basename $[@]` -o $[@]'
		;;
	netbsd*) #(vi
		CC_SHARED_OPTS="$CC_SHARED_OPTS -DPIC"
		if test "$DFT_LWR_MODEL" = "shared" && test "$cf_cv_enable_rpath" = yes ; then
			LOCAL_LDFLAGS="${cf_ld_rpath_opt}\$(LOCAL_LIBDIR)"
			LOCAL_LDFLAGS2="$LOCAL_LDFLAGS"
			EXTRA_LDFLAGS="${cf_ld_rpath_opt}\${RPATH_LIST} $EXTRA_LDFLAGS"
			if test "$cf_cv_shlib_version" = auto; then
			if test -f /usr/libexec/ld.elf_so; then
				cf_cv_shlib_version=abi
			else
				cf_cv_shlib_version=rel
			fi
			fi
			CF_SHARED_SONAME
			MK_SHARED_LIB='${CC} ${CFLAGS} -shared -Wl,-soname,'$cf_cv_shared_soname' -o $[@]'
		else
			MK_SHARED_LIB='${CC} -Wl,-shared -Wl,-Bshareable -o $[@]'
		fi
		;;
	osf*|mls+*) #(vi
		# tested with OSF/1 V3.2 and 'cc'
		# tested with OSF/1 V3.2 and gcc 2.6.3 (but the c++ demo didn't
		# link with shared libs).
		MK_SHARED_LIB='${LD} -set_version ${REL_VERSION}:${ABI_VERSION} -expect_unresolved "*" -shared -soname `basename $[@]`'
		case $host_os in #(vi
		osf4*)
			MK_SHARED_LIB="${MK_SHARED_LIB} -msym"
			;;
		esac
		MK_SHARED_LIB="${MK_SHARED_LIB}"' -o $[@]'
		if test "$DFT_LWR_MODEL" = "shared" ; then
			LOCAL_LDFLAGS="${LD_RPATH_OPT}\$(LOCAL_LIBDIR)"
			LOCAL_LDFLAGS2="$LOCAL_LDFLAGS"
		fi
		cf_cv_rm_so_locs=yes
		;;
	sco3.2v5*)  # (also uw2* and UW7: hops 13-Apr-98
		# tested with osr5.0.5
		if test "$GCC" != yes; then
			CC_SHARED_OPTS='-belf -KPIC'
		fi
		MK_SHARED_LIB='${LD} -dy -G -h `basename $[@] .${REL_VERSION}`.${ABI_VERSION} -o [$]@'
		if test "$cf_cv_enable_rpath" = yes ; then
			# only way is to set LD_RUN_PATH but no switch for it
			RUN_PATH=$libdir
		fi
		test "$cf_cv_shlib_version" = auto && cf_cv_shlib_version=rel
		LINK_PROGS='LD_RUN_PATH=${libdir}'
		LINK_TESTS='Pwd=`pwd`;LD_RUN_PATH=`dirname $${Pwd}`/lib'
		;;
	sunos4*) #(vi
		# tested with SunOS 4.1.1 and gcc 2.7.0
		if test "$GCC" != yes; then
			CC_SHARED_OPTS='-KPIC'
		fi
		MK_SHARED_LIB='${LD} -assert pure-text -o $[@]'
		test "$cf_cv_shlib_version" = auto && cf_cv_shlib_version=rel
		;;
	solaris2*) #(vi
		# tested with SunOS 5.5.1 (solaris 2.5.1) and gcc 2.7.2
		# tested with SunOS 5.10 (solaris 10) and gcc 3.4.3
		if test "$DFT_LWR_MODEL" = "shared" ; then
			LOCAL_LDFLAGS="-R \$(LOCAL_LIBDIR):\${libdir}"
			LOCAL_LDFLAGS2="$LOCAL_LDFLAGS"
		fi
		if test "$cf_cv_enable_rpath" = yes ; then
			EXTRA_LDFLAGS="-R \${libdir} $EXTRA_LDFLAGS"
		fi
		CF_SHARED_SONAME
		if test "$GCC" != yes; then
			cf_save_CFLAGS="$CFLAGS"
			for cf_shared_opts in -xcode=pic32 -xcode=pic13 -KPIC -Kpic -O
			do
				CFLAGS="$cf_shared_opts $cf_save_CFLAGS"
				AC_TRY_COMPILE([#include <stdio.h>],[printf("Hello\n");],[break])
			done
			CFLAGS="$cf_save_CFLAGS"
			CC_SHARED_OPTS=$cf_shared_opts
			MK_SHARED_LIB='${CC} -dy -G -h '$cf_cv_shared_soname' -o $[@]'
		else
			MK_SHARED_LIB='${CC} -shared -dy -G -h '$cf_cv_shared_soname' -o $[@]'
		fi
		;;
	sysv5uw7*|unix_sv*) #(vi
		# tested with UnixWare 7.1.0 (gcc 2.95.2 and cc)
		if test "$GCC" != yes; then
			CC_SHARED_OPTS='-KPIC'
		fi
		MK_SHARED_LIB='${LD} -d y -G -o [$]@'
		;;
	*)
		CC_SHARED_OPTS='unknown'
		MK_SHARED_LIB='echo unknown'
		;;
	esac

	# This works if the last tokens in $MK_SHARED_LIB are the -o target.
	case "$cf_cv_shlib_version" in #(vi
	rel|abi)
		case "$MK_SHARED_LIB" in #(vi
		*'-o $[@]') #(vi
			test "$cf_cv_do_symlinks" = no && cf_cv_do_symlinks=yes
			;;
		*)
			AC_MSG_WARN(ignored --with-shlib-version)
			;;
		esac
		;;
	esac

	if test -n "$cf_try_cflags"
	then
cat > conftest.$ac_ext <<EOF
#line __oline__ "${as_me:-configure}"
#include <stdio.h>
int main(int argc, char *argv[[]])
{
	printf("hello\n");
	return (argv[[argc-1]] == 0) ;
}
EOF
		cf_save_CFLAGS="$CFLAGS"
		for cf_opt in $cf_try_cflags
		do
			CFLAGS="$cf_save_CFLAGS -$cf_opt"
			AC_MSG_CHECKING(if CFLAGS option -$cf_opt works)
			if AC_TRY_EVAL(ac_compile); then
				AC_MSG_RESULT(yes)
				cf_save_CFLAGS="$CFLAGS"
			else
				AC_MSG_RESULT(no)
			fi
		done
		CFLAGS="$cf_save_CFLAGS"
	fi


	# RPATH_LIST is a colon-separated list of directories
	test -n "$cf_ld_rpath_opt" && MK_SHARED_LIB="$MK_SHARED_LIB $cf_ld_rpath_opt\${RPATH_LIST}"
	test -z "$RPATH_LIST" && RPATH_LIST="\${libdir}"

	test $cf_cv_rm_so_locs = yes && RM_SHARED_OPTS="$RM_SHARED_OPTS so_locations"

	CF_VERBOSE(CC_SHARED_OPTS: $CC_SHARED_OPTS)
	CF_VERBOSE(MK_SHARED_LIB:  $MK_SHARED_LIB)

	AC_SUBST(CC_SHARED_OPTS)
	AC_SUBST(LD_RPATH_OPT)
	AC_SUBST(LD_SHARED_OPTS)
	AC_SUBST(MK_SHARED_LIB)
	AC_SUBST(RM_SHARED_OPTS)

	AC_SUBST(LINK_PROGS)
	AC_SUBST(LINK_TESTS)

	AC_SUBST(EXTRA_LDFLAGS)
	AC_SUBST(LOCAL_LDFLAGS)
	AC_SUBST(LOCAL_LDFLAGS2)

	AC_SUBST(INSTALL_LIB)
	AC_SUBST(RPATH_LIST)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_SHARED_SONAME version: 3 updated: 2008/09/08 18:34:43
dnl ----------------
dnl utility macro for CF_SHARED_OPTS, constructs "$cf_cv_shared_soname" for
dnl substitution into MK_SHARED_LIB string for the "-soname" (or similar)
dnl option.
dnl
dnl $1 is the default that should be used for "$cf_cv_shlib_version".
dnl If missing, use "rel".
define([CF_SHARED_SONAME],
[
	test "$cf_cv_shlib_version" = auto && cf_cv_shlib_version=ifelse($1,,rel,$1)
	if test "$cf_cv_shlib_version" = rel; then
		cf_cv_shared_soname='`basename $[@] .${REL_VERSION}`.${ABI_VERSION}'
	else
		cf_cv_shared_soname='`basename $[@]`'
	fi
])
dnl ---------------------------------------------------------------------------
dnl CF_TRY_PKG_CONFIG version: 5 updated: 2013/07/06 21:27:06
dnl -----------------
dnl This is a simple wrapper to use for pkg-config, for libraries which may be
dnl available in that form.
dnl
dnl $1 = package name
dnl $2 = extra logic to use, if any, after updating CFLAGS and LIBS
dnl $3 = logic to use if pkg-config does not have the package
AC_DEFUN([CF_TRY_PKG_CONFIG],[
AC_REQUIRE([CF_PKG_CONFIG])

if test "$PKG_CONFIG" != none && "$PKG_CONFIG" --exists $1; then
	CF_VERBOSE(found package $1)
	cf_pkgconfig_incs="`$PKG_CONFIG --cflags $1 2>/dev/null`"
	cf_pkgconfig_libs="`$PKG_CONFIG --libs   $1 2>/dev/null`"
	CF_VERBOSE(package $1 CFLAGS: $cf_pkgconfig_incs)
	CF_VERBOSE(package $1 LIBS: $cf_pkgconfig_libs)
	CF_ADD_CFLAGS($cf_pkgconfig_incs)
	CF_ADD_LIBS($cf_pkgconfig_libs)
	ifelse([$2],,:,[$2])
else
	cf_pkgconfig_incs=
	cf_pkgconfig_libs=
	ifelse([$3],,:,[$3])
fi
])
dnl ---------------------------------------------------------------------------
dnl CF_TRY_XOPEN_SOURCE version: 1 updated: 2011/10/30 17:09:50
dnl -------------------
dnl If _XOPEN_SOURCE is not defined in the compile environment, check if we
dnl can define it successfully.
AC_DEFUN([CF_TRY_XOPEN_SOURCE],[
AC_CACHE_CHECK(if we should define _XOPEN_SOURCE,cf_cv_xopen_source,[
	AC_TRY_COMPILE([
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
],[
#ifndef _XOPEN_SOURCE
make an error
#endif],
	[cf_cv_xopen_source=no],
	[cf_save="$CPPFLAGS"
	 CPPFLAGS="$CPPFLAGS -D_XOPEN_SOURCE=$cf_XOPEN_SOURCE"
	 AC_TRY_COMPILE([
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
],[
#ifdef _XOPEN_SOURCE
make an error
#endif],
	[cf_cv_xopen_source=no],
	[cf_cv_xopen_source=$cf_XOPEN_SOURCE])
	CPPFLAGS="$cf_save"
	])
])

if test "$cf_cv_xopen_source" != no ; then
	CF_REMOVE_DEFINE(CFLAGS,$CFLAGS,_XOPEN_SOURCE)
	CF_REMOVE_DEFINE(CPPFLAGS,$CPPFLAGS,_XOPEN_SOURCE)
	cf_temp_xopen_source="-D_XOPEN_SOURCE=$cf_cv_xopen_source"
	CF_ADD_CFLAGS($cf_temp_xopen_source)
fi
])
dnl ---------------------------------------------------------------------------
dnl CF_UPPER version: 5 updated: 2001/01/29 23:40:59
dnl --------
dnl Make an uppercase version of a variable
dnl $1=uppercase($2)
AC_DEFUN([CF_UPPER],
[
$1=`echo "$2" | sed y%abcdefghijklmnopqrstuvwxyz./-%ABCDEFGHIJKLMNOPQRSTUVWXYZ___%`
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_VERBOSE version: 3 updated: 2007/07/29 09:55:12
dnl ----------
dnl Use AC_VERBOSE w/o the warnings
AC_DEFUN([CF_VERBOSE],
[test -n "$verbose" && echo "	$1" 1>&AC_FD_MSG
CF_MSG_LOG([$1])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_VERSION_INFO version: 6 updated: 2013/06/16 10:25:53
dnl ---------------
dnl Define several useful symbols derived from the VERSION file.  A separate
dnl file is preferred to embedding the version numbers in various scripts.
dnl (automake is a textbook-example of why the latter is a bad idea, but there
dnl are others).
dnl
dnl The file contents are:
dnl	libtool-version	release-version	patch-version
dnl or
dnl	release-version
dnl where
dnl	libtool-version (see ?) consists of 3 integers separated by '.'
dnl	release-version consists of a major version and minor version
dnl		separated by '.', optionally followed by a patch-version
dnl		separated by '-'.  The minor version need not be an
dnl		integer (but it is preferred).
dnl	patch-version is an integer in the form yyyymmdd, so ifdef's and
dnl		scripts can easily compare versions.
dnl
dnl If libtool is used, the first form is required, since CF_WITH_LIBTOOL
dnl simply extracts the first field using 'cut -f1'.
dnl
dnl Optional parameters:
dnl $1 = internal name for package
dnl $2 = external name for package
AC_DEFUN([CF_VERSION_INFO],
[
if test -f $srcdir/VERSION ; then
	AC_MSG_CHECKING(for package version)

	# if there are not enough fields, cut returns the last one...
	cf_field1=`sed -e '2,$d' $srcdir/VERSION|cut -f1`
	cf_field2=`sed -e '2,$d' $srcdir/VERSION|cut -f2`
	cf_field3=`sed -e '2,$d' $srcdir/VERSION|cut -f3`

	# this is how CF_BUNDLED_INTL uses $VERSION:
	VERSION="$cf_field1"

	VERSION_MAJOR=`echo "$cf_field2" | sed -e 's/\..*//'`
	test -z "$VERSION_MAJOR" && AC_MSG_ERROR(missing major-version)

	VERSION_MINOR=`echo "$cf_field2" | sed -e 's/^[[^.]]*\.//' -e 's/-.*//'`
	test -z "$VERSION_MINOR" && AC_MSG_ERROR(missing minor-version)

	AC_MSG_RESULT(${VERSION_MAJOR}.${VERSION_MINOR})

	AC_MSG_CHECKING(for package patch date)
	VERSION_PATCH=`echo "$cf_field3" | sed -e 's/^[[^-]]*-//'`
	case .$VERSION_PATCH in
	.)
		AC_MSG_ERROR(missing patch-date $VERSION_PATCH)
		;;
	.[[0-9]][[0-9]][[0-9]][[0-9]][[0-9]][[0-9]][[0-9]][[0-9]])
		;;
	*)
		AC_MSG_ERROR(illegal patch-date $VERSION_PATCH)
		;;
	esac
	AC_MSG_RESULT($VERSION_PATCH)
else
	AC_MSG_ERROR(did not find $srcdir/VERSION)
fi

# show the actual data that we have for versions:
CF_VERBOSE(ABI VERSION $VERSION)
CF_VERBOSE(VERSION_MAJOR $VERSION_MAJOR)
CF_VERBOSE(VERSION_MINOR $VERSION_MINOR)
CF_VERBOSE(VERSION_PATCH $VERSION_PATCH)

AC_SUBST(VERSION)
AC_SUBST(VERSION_MAJOR)
AC_SUBST(VERSION_MINOR)
AC_SUBST(VERSION_PATCH)

dnl if a package name is given, define its corresponding version info.  We
dnl need the package name to ensure that the defined symbols are unique.
ifelse($1,,,[
	cf_PACKAGE=$1
	PACKAGE=ifelse($2,,$1,$2)
	AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE",[Define to the package-name])
	AC_SUBST(PACKAGE)
	CF_UPPER(cf_PACKAGE,$cf_PACKAGE)
	AC_DEFINE_UNQUOTED(${cf_PACKAGE}_VERSION,"${VERSION_MAJOR}.${VERSION_MINOR}")
	AC_DEFINE_UNQUOTED(${cf_PACKAGE}_PATCHDATE,${VERSION_PATCH})
])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_ABI_VERSION version: 1 updated: 2003/09/20 18:12:49
dnl -------------------
dnl Allow library's ABI to be overridden.  Generally this happens when a
dnl packager has incremented the ABI past that used in the original package,
dnl and wishes to keep doing this.
dnl
dnl $1 is the package name, if any, to derive a corresponding {package}_ABI
dnl symbol.
AC_DEFUN([CF_WITH_ABI_VERSION],[
test -z "$cf_cv_abi_version" && cf_cv_abi_version=0
AC_ARG_WITH(abi-version,
[  --with-abi-version=XXX  override derived ABI version],
[AC_MSG_WARN(overriding ABI version $cf_cv_abi_version to $withval)
 cf_cv_abi_version=$withval])
 CF_NUMBER_SYNTAX($cf_cv_abi_version,ABI version)
ifelse($1,,,[
$1_ABI=$cf_cv_abi_version
])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_DBMALLOC version: 7 updated: 2010/06/21 17:26:47
dnl ----------------
dnl Configure-option for dbmalloc.  The optional parameter is used to override
dnl the updating of $LIBS, e.g., to avoid conflict with subsequent tests.
AC_DEFUN([CF_WITH_DBMALLOC],[
CF_NO_LEAKS_OPTION(dbmalloc,
	[  --with-dbmalloc         test: use Conor Cahill's dbmalloc library],
	[USE_DBMALLOC])

if test "$with_dbmalloc" = yes ; then
	AC_CHECK_HEADER(dbmalloc.h,
		[AC_CHECK_LIB(dbmalloc,[debug_malloc]ifelse([$1],,[],[,$1]))])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_DMALLOC version: 7 updated: 2010/06/21 17:26:47
dnl ---------------
dnl Configure-option for dmalloc.  The optional parameter is used to override
dnl the updating of $LIBS, e.g., to avoid conflict with subsequent tests.
AC_DEFUN([CF_WITH_DMALLOC],[
CF_NO_LEAKS_OPTION(dmalloc,
	[  --with-dmalloc          test: use Gray Watson's dmalloc library],
	[USE_DMALLOC])

if test "$with_dmalloc" = yes ; then
	AC_CHECK_HEADER(dmalloc.h,
		[AC_CHECK_LIB(dmalloc,[dmalloc_debug]ifelse([$1],,[],[,$1]))])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_LIBTOOL version: 31 updated: 2014/11/15 19:05:29
dnl ---------------
dnl Provide a configure option to incorporate libtool.  Define several useful
dnl symbols for the makefile rules.
dnl
dnl The reference to AC_PROG_LIBTOOL does not normally work, since it uses
dnl macros from libtool.m4 which is in the aclocal directory of automake.
dnl Following is a simple script which turns on the AC_PROG_LIBTOOL macro.
dnl But that still does not work properly since the macro is expanded outside
dnl the CF_WITH_LIBTOOL macro:
dnl
dnl	#!/bin/sh
dnl	ACLOCAL=`aclocal --print-ac-dir`
dnl	if test -z "$ACLOCAL" ; then
dnl		echo cannot find aclocal directory
dnl		exit 1
dnl	elif test ! -f $ACLOCAL/libtool.m4 ; then
dnl		echo cannot find libtool.m4 file
dnl		exit 1
dnl	fi
dnl
dnl	LOCAL=aclocal.m4
dnl	ORIG=aclocal.m4.orig
dnl
dnl	trap "mv $ORIG $LOCAL" 0 1 2 5 15
dnl	rm -f $ORIG
dnl	mv $LOCAL $ORIG
dnl
dnl	# sed the LIBTOOL= assignment to omit the current directory?
dnl	sed -e 's/^LIBTOOL=.*/LIBTOOL=${LIBTOOL:-libtool}/' $ACLOCAL/libtool.m4 >>$LOCAL
dnl	cat $ORIG >>$LOCAL
dnl
dnl	autoconf-257 $*
dnl
AC_DEFUN([CF_WITH_LIBTOOL],
[
AC_REQUIRE([CF_DISABLE_LIBTOOL_VERSION])
ifdef([AC_PROG_LIBTOOL],,[
LIBTOOL=
])
# common library maintenance symbols that are convenient for libtool scripts:
LIB_CREATE='${AR} -cr'
LIB_OBJECT='${OBJECTS}'
LIB_SUFFIX=.a
LIB_PREP="$RANLIB"

# symbols used to prop libtool up to enable it to determine what it should be
# doing:
LIB_CLEAN=
LIB_COMPILE=
LIB_LINK='${CC}'
LIB_INSTALL=
LIB_UNINSTALL=

AC_MSG_CHECKING(if you want to build libraries with libtool)
AC_ARG_WITH(libtool,
	[  --with-libtool          generate libraries with libtool],
	[with_libtool=$withval],
	[with_libtool=no])
AC_MSG_RESULT($with_libtool)
if test "$with_libtool" != "no"; then
ifdef([AC_PROG_LIBTOOL],[
	# missing_content_AC_PROG_LIBTOOL{{
	AC_PROG_LIBTOOL
	# missing_content_AC_PROG_LIBTOOL}}
],[
	if test "$with_libtool" != "yes" ; then
		CF_PATH_SYNTAX(with_libtool)
		LIBTOOL=$with_libtool
	else
		AC_CHECK_TOOLS(LIBTOOL,[libtool glibtool],none)
		CF_LIBTOOL_VERSION
		if test -z "$cf_cv_libtool_version" && test "$LIBTOOL" = libtool
		then
			CF_FORGET_TOOL(LIBTOOL)
			AC_CHECK_TOOLS(LIBTOOL,[glibtool],none)
			CF_LIBTOOL_VERSION
		fi
	fi
	if test -z "$LIBTOOL" ; then
		AC_MSG_ERROR(Cannot find libtool)
	fi
])dnl
	LIB_CREATE='${LIBTOOL} --mode=link ${CC} -rpath ${DESTDIR}${libdir} ${LIBTOOL_VERSION} `cut -f1 ${top_srcdir}/VERSION` ${LIBTOOL_OPTS} ${LT_UNDEF} $(LIBS) -o'
	LIB_OBJECT='${OBJECTS:.o=.lo}'
	LIB_SUFFIX=.la
	LIB_CLEAN='${LIBTOOL} --mode=clean'
	LIB_COMPILE='${LIBTOOL} --mode=compile'
	LIB_LINK='${LIBTOOL} --mode=link ${CC} ${LIBTOOL_OPTS}'
	LIB_INSTALL='${LIBTOOL} --mode=install'
	LIB_UNINSTALL='${LIBTOOL} --mode=uninstall'
	LIB_PREP=:

	CF_CHECK_LIBTOOL_VERSION

	# special hack to add -no-undefined (which libtool should do for itself)
	LT_UNDEF=
	case "$cf_cv_system_name" in #(vi
	cygwin*|msys*|mingw32*|uwin*|aix[[4-7]]) #(vi
		LT_UNDEF=-no-undefined
		;;
	esac
	AC_SUBST([LT_UNDEF])

	# special hack to add --tag option for C++ compiler
	case $cf_cv_libtool_version in #(vi
	1.[[5-9]]*|[[2-9]].[[0-9.a-z]]*) #(vi
		LIBTOOL_CXX="$LIBTOOL --tag=CXX"
		LIBTOOL="$LIBTOOL --tag=CC"
		;;
	*)
		LIBTOOL_CXX="$LIBTOOL"
		;;
	esac
else
	LIBTOOL=""
	LIBTOOL_CXX=""
fi

test -z "$LIBTOOL" && ECHO_LT=

AC_SUBST(LIBTOOL)
AC_SUBST(LIBTOOL_CXX)
AC_SUBST(LIBTOOL_OPTS)

AC_SUBST(LIB_CREATE)
AC_SUBST(LIB_OBJECT)
AC_SUBST(LIB_SUFFIX)
AC_SUBST(LIB_PREP)

AC_SUBST(LIB_CLEAN)
AC_SUBST(LIB_COMPILE)
AC_SUBST(LIB_LINK)
AC_SUBST(LIB_INSTALL)
AC_SUBST(LIB_UNINSTALL)

])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_LIBTOOL_OPTS version: 3 updated: 2014/11/02 16:11:49
dnl --------------------
dnl Allow user to pass additional libtool options into the library creation
dnl and link steps.  The main use for this is to do something like
dnl	./configure --with-libtool-opts=-static
dnl to get the same behavior as automake-flavored
dnl	./configure --enable-static
AC_DEFUN([CF_WITH_LIBTOOL_OPTS],[
AC_MSG_CHECKING(for additional libtool options)
AC_ARG_WITH(libtool-opts,
	[  --with-libtool-opts=XXX specify additional libtool options],
	[with_libtool_opts=$withval],
	[with_libtool_opts=no])
AC_MSG_RESULT($with_libtool_opts)

case .$with_libtool_opts in
.yes|.no|.)
	;;
*)
	LIBTOOL_OPTS="$LIBTOOL_OPTS $with_libtool_opts"
	;;
esac

AC_SUBST(LIBTOOL_OPTS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_REL_VERSION version: 1 updated: 2003/09/20 18:12:49
dnl -------------------
dnl Allow library's release-version to be overridden.  Generally this happens when a
dnl packager has incremented the release-version past that used in the original package,
dnl and wishes to keep doing this.
dnl
dnl $1 is the package name, if any, to derive corresponding {package}_MAJOR
dnl and {package}_MINOR symbols
dnl symbol.
AC_DEFUN([CF_WITH_REL_VERSION],[
test -z "$cf_cv_rel_version" && cf_cv_rel_version=0.0
AC_ARG_WITH(rel-version,
[  --with-rel-version=XXX  override derived release version],
[AC_MSG_WARN(overriding release version $cf_cv_rel_version to $withval)
 cf_cv_rel_version=$withval])
ifelse($1,,[
 CF_NUMBER_SYNTAX($cf_cv_rel_version,Release version)
],[
 $1_MAJOR=`echo "$cf_cv_rel_version" | sed -e 's/\..*//'`
 $1_MINOR=`echo "$cf_cv_rel_version" | sed -e 's/^[[^.]]*//' -e 's/^\.//' -e 's/\..*//'`
 CF_NUMBER_SYNTAX([$]$1_MAJOR,Release major-version)
 CF_NUMBER_SYNTAX([$]$1_MINOR,Release minor-version)
])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_SHARED_OR_LIBTOOL version: 7 updated: 2014/11/02 16:11:49
dnl -------------------------
dnl Provide shared libraries using either autoconf macros (--with-shared) or
dnl using the external libtool script (--with-libtool).
dnl
dnl $1 = program name (all caps preferred)
dnl $1 = release version
dnl $2 = ABI version
define([CF_WITH_SHARED_OR_LIBTOOL],[

REL_VERSION=$2
ABI_VERSION=$3
cf_cv_rel_version=$REL_VERSION
AC_SUBST(ABI_VERSION)
AC_SUBST(REL_VERSION)

CF_WITH_REL_VERSION($1)
CF_WITH_ABI_VERSION

LIB_MODEL=static
DFT_LWR_MODEL=$LIB_MODEL
LIBTOOL_MAKE="#"

# use to comment-out makefile lines
MAKE_NORMAL=
MAKE_STATIC=
MAKE_SHARED="#"
MAKE_DLLS="#"

shlibdir=$libdir
AC_SUBST(shlibdir)

CF_WITH_LIBTOOL

LIB_CREATE="$LIB_CREATE \[$]@"

if test "$with_libtool" = "yes" ; then
	OBJEXT="lo"
	LIB_MODEL=libtool
	DFT_LWR_MODEL=$LIB_MODEL
	LIBTOOL_MAKE=
	CF_WITH_LIBTOOL_OPTS
	CF_WITH_EXPORT_SYMS
	MAKE_NORMAL="#"
	MAKE_STATIC="#"
	MAKE_SHARED=
else
	AC_MSG_CHECKING(if you want to build shared libraries)
	AC_ARG_WITH(shared,
		[  --with-shared           generate shared-libraries],
		[with_shared=$withval],
		[with_shared=no])
	AC_MSG_RESULT($with_shared)
	if test "$with_shared" = "yes" ; then
		LIB_MODEL=shared
		DFT_LWR_MODEL=$LIB_MODEL
		CF_SHARED_OPTS
		CF_WITH_VERSIONED_SYMS
		LIB_PREP=:
		LIB_CREATE="[$]MK_SHARED_LIB"
		CFLAGS="$CFLAGS $CC_SHARED_OPTS"
		MAKE_NORMAL="#"
		MAKE_STATIC="#"
		MAKE_SHARED=
	fi
fi

LIB_SUFFIX=
CF_LIB_SUFFIX($LIB_MODEL, DFT_LIB_SUFFIX, DFT_DEP_SUFFIX)
LIB_SUFFIX=$DFT_LIB_SUFFIX

AC_SUBST(DFT_LWR_MODEL)
AC_SUBST(DFT_LIB_SUFFIX)
AC_SUBST(DFT_DEP_SUFFIX)
AC_SUBST(LIB_MODEL)

AC_SUBST(LIBTOOL_MAKE)

AC_SUBST(MAKE_DLLS)
AC_SUBST(MAKE_NORMAL)
AC_SUBST(MAKE_SHARED)
AC_SUBST(MAKE_STATIC)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_VALGRIND version: 1 updated: 2006/12/14 18:00:21
dnl ----------------
AC_DEFUN([CF_WITH_VALGRIND],[
CF_NO_LEAKS_OPTION(valgrind,
	[  --with-valgrind         test: use valgrind],
	[USE_VALGRIND])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_XPM version: 3 updated: 2012/10/04 06:57:36
dnl -----------
dnl Test for Xpm library, update compiler/loader flags if it is wanted and
dnl found.
dnl
dnl Also sets ICON_SUFFIX
AC_DEFUN([CF_WITH_XPM],
[
ICON_SUFFIX=.xbm

cf_save_cppflags="${CPPFLAGS}"
cf_save_ldflags="${LDFLAGS}"

AC_MSG_CHECKING(if you want to use the Xpm library for colored icon)
AC_ARG_WITH(xpm,
[  --with-xpm=DIR          use Xpm library for colored icon, may specify path],
	[cf_Xpm_library="$withval"],
	[cf_Xpm_library=yes])
AC_MSG_RESULT($cf_Xpm_library)

if test "$cf_Xpm_library" != no ; then
    if test "$cf_Xpm_library" != yes ; then
	CPPFLAGS="$CPPFLAGS -I$withval/include"
	LDFLAGS="$LDFLAGS -L$withval/lib"
    fi
    AC_CHECK_HEADER(X11/xpm.h,[
	AC_CHECK_LIB(Xpm, XpmCreatePixmapFromData,[
	    AC_DEFINE(HAVE_LIBXPM,1,[Define to 1 if we should use Xpm library])
	    ICON_SUFFIX=.xpm
	    LIBS="-lXpm $LIBS"],
	    [CPPFLAGS="${cf_save_cppflags}" LDFLAGS="${cf_save_ldflags}"],
	    [-lX11 $X_LIBS])],
	[CPPFLAGS="${cf_save_cppflags}" LDFLAGS="${cf_save_ldflags}"])
fi

AC_SUBST(ICON_SUFFIX)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_XOPEN_SOURCE version: 48 updated: 2014/09/01 12:29:14
dnl ---------------
dnl Try to get _XOPEN_SOURCE defined properly that we can use POSIX functions,
dnl or adapt to the vendor's definitions to get equivalent functionality,
dnl without losing the common non-POSIX features.
dnl
dnl Parameters:
dnl	$1 is the nominal value for _XOPEN_SOURCE
dnl	$2 is the nominal value for _POSIX_C_SOURCE
AC_DEFUN([CF_XOPEN_SOURCE],[
AC_REQUIRE([AC_CANONICAL_HOST])

cf_XOPEN_SOURCE=ifelse([$1],,500,[$1])
cf_POSIX_C_SOURCE=ifelse([$2],,199506L,[$2])
cf_xopen_source=

case $host_os in #(vi
aix[[4-7]]*) #(vi
	cf_xopen_source="-D_ALL_SOURCE"
	;;
cygwin|msys) #(vi
	cf_XOPEN_SOURCE=600
	;;
darwin[[0-8]].*) #(vi
	cf_xopen_source="-D_APPLE_C_SOURCE"
	;;
darwin*) #(vi
	cf_xopen_source="-D_DARWIN_C_SOURCE"
	cf_XOPEN_SOURCE=
	;;
freebsd*|dragonfly*) #(vi
	# 5.x headers associate
	#	_XOPEN_SOURCE=600 with _POSIX_C_SOURCE=200112L
	#	_XOPEN_SOURCE=500 with _POSIX_C_SOURCE=199506L
	cf_POSIX_C_SOURCE=200112L
	cf_XOPEN_SOURCE=600
	cf_xopen_source="-D_BSD_TYPES -D__BSD_VISIBLE -D_POSIX_C_SOURCE=$cf_POSIX_C_SOURCE -D_XOPEN_SOURCE=$cf_XOPEN_SOURCE"
	;;
hpux11*) #(vi
	cf_xopen_source="-D_HPUX_SOURCE -D_XOPEN_SOURCE=500"
	;;
hpux*) #(vi
	cf_xopen_source="-D_HPUX_SOURCE"
	;;
irix[[56]].*) #(vi
	cf_xopen_source="-D_SGI_SOURCE"
	cf_XOPEN_SOURCE=
	;;
linux*|gnu*|mint*|k*bsd*-gnu) #(vi
	CF_GNU_SOURCE
	;;
minix*) #(vi
	cf_xopen_source="-D_NETBSD_SOURCE" # POSIX.1-2001 features are ifdef'd with this...
	;;
mirbsd*) #(vi
	# setting _XOPEN_SOURCE or _POSIX_SOURCE breaks <sys/select.h> and other headers which use u_int / u_short types
	cf_XOPEN_SOURCE=
	CF_POSIX_C_SOURCE($cf_POSIX_C_SOURCE)
	;;
netbsd*) #(vi
	cf_xopen_source="-D_NETBSD_SOURCE" # setting _XOPEN_SOURCE breaks IPv6 for lynx on NetBSD 1.6, breaks xterm, is not needed for ncursesw
	;;
openbsd[[4-9]]*) #(vi
	# setting _XOPEN_SOURCE lower than 500 breaks g++ compile with wchar.h, needed for ncursesw
	cf_xopen_source="-D_BSD_SOURCE"
	cf_XOPEN_SOURCE=600
	;;
openbsd*) #(vi
	# setting _XOPEN_SOURCE breaks xterm on OpenBSD 2.8, is not needed for ncursesw
	;;
osf[[45]]*) #(vi
	cf_xopen_source="-D_OSF_SOURCE"
	;;
nto-qnx*) #(vi
	cf_xopen_source="-D_QNX_SOURCE"
	;;
sco*) #(vi
	# setting _XOPEN_SOURCE breaks Lynx on SCO Unix / OpenServer
	;;
solaris2.*) #(vi
	cf_xopen_source="-D__EXTENSIONS__"
	cf_cv_xopen_source=broken
	;;
sysv4.2uw2.*) # Novell/SCO UnixWare 2.x (tested on 2.1.2)
	cf_XOPEN_SOURCE=
	cf_POSIX_C_SOURCE=
	;;
*)
	CF_TRY_XOPEN_SOURCE
	CF_POSIX_C_SOURCE($cf_POSIX_C_SOURCE)
	;;
esac

if test -n "$cf_xopen_source" ; then
	CF_ADD_CFLAGS($cf_xopen_source,true)
fi

dnl In anything but the default case, we may have system-specific setting
dnl which is still not guaranteed to provide all of the entrypoints that
dnl _XOPEN_SOURCE would yield.
if test -n "$cf_XOPEN_SOURCE" && test -z "$cf_cv_xopen_source" ; then
	AC_MSG_CHECKING(if _XOPEN_SOURCE really is set)
	AC_TRY_COMPILE([#include <stdlib.h>],[
#ifndef _XOPEN_SOURCE
make an error
#endif],
	[cf_XOPEN_SOURCE_set=yes],
	[cf_XOPEN_SOURCE_set=no])
	AC_MSG_RESULT($cf_XOPEN_SOURCE_set)
	if test $cf_XOPEN_SOURCE_set = yes
	then
		AC_TRY_COMPILE([#include <stdlib.h>],[
#if (_XOPEN_SOURCE - 0) < $cf_XOPEN_SOURCE
make an error
#endif],
		[cf_XOPEN_SOURCE_set_ok=yes],
		[cf_XOPEN_SOURCE_set_ok=no])
		if test $cf_XOPEN_SOURCE_set_ok = no
		then
			AC_MSG_WARN(_XOPEN_SOURCE is lower than requested)
		fi
	else
		CF_TRY_XOPEN_SOURCE
	fi
fi
])
dnl ---------------------------------------------------------------------------
dnl CF_X_EXT version: 3 updated: 2010/06/02 05:03:05
dnl --------
AC_DEFUN([CF_X_EXT],[
CF_TRY_PKG_CONFIG(Xext,,[
	AC_CHECK_LIB(Xext,XextCreateExtension,
		[CF_ADD_LIB(Xext)])])
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_X_TOOLKIT version: 22 updated: 2014/07/13 14:33:27
dnl ------------
dnl Check for X Toolkit libraries
AC_DEFUN([CF_X_TOOLKIT],
[
AC_REQUIRE([AC_PATH_XTRA])
AC_REQUIRE([CF_CHECK_CACHE])

# OSX is schizoid about who owns /usr/X11 (old) versus /opt/X11 (new), and (and
# in some cases has installed dummy files in the former, other cases replaced
# it with a link to the new location).  This complicates the configure script. 
# Check for that pitfall, and recover using pkg-config
#
# If none of these are set, the configuration is almost certainly broken.
if test -z "${X_CFLAGS}${X_PRE_LIBS}${X_LIBS}${X_EXTRA_LIBS}"
then
	CF_TRY_PKG_CONFIG(x11,,[AC_MSG_WARN(unable to find X11 library)])
	CF_TRY_PKG_CONFIG(ice,,[AC_MSG_WARN(unable to find ICE library)])
	CF_TRY_PKG_CONFIG(sm,,[AC_MSG_WARN(unable to find SM library)])
	CF_TRY_PKG_CONFIG(xt,,[AC_MSG_WARN(unable to find Xt library)])
fi

cf_have_X_LIBS=no

CF_TRY_PKG_CONFIG(xt,[

	case "x$LIBS" in #(vi
	*-lX11*) #(vi
		;;
	*)
# we have an "xt" package, but it may omit Xt's dependency on X11
AC_CACHE_CHECK(for usable X dependency,cf_cv_xt_x11_compat,[
AC_TRY_LINK([
#include <X11/Xlib.h>
],[
	int rc1 = XDrawLine((Display*) 0, (Drawable) 0, (GC) 0, 0, 0, 0, 0);
	int rc2 = XClearWindow((Display*) 0, (Window) 0);
	int rc3 = XMoveWindow((Display*) 0, (Window) 0, 0, 0);
	int rc4 = XMoveResizeWindow((Display*)0, (Window)0, 0, 0, 0, 0);
],[cf_cv_xt_x11_compat=yes],[cf_cv_xt_x11_compat=no])])
		if test "$cf_cv_xt_x11_compat" = no
		then
			CF_VERBOSE(work around broken X11 dependency)
			# 2010/11/19 - good enough until a working Xt on Xcb is delivered.
			CF_TRY_PKG_CONFIG(x11,,[CF_ADD_LIB_AFTER(-lXt,-lX11)])
		fi
		;;
	esac

AC_CACHE_CHECK(for usable X Toolkit package,cf_cv_xt_ice_compat,[
AC_TRY_LINK([
#include <X11/Shell.h>
],[int num = IceConnectionNumber(0)
],[cf_cv_xt_ice_compat=yes],[cf_cv_xt_ice_compat=no])])

	if test "$cf_cv_xt_ice_compat" = no
	then
		# workaround for broken ".pc" files used for X Toolkit.
		case "x$X_PRE_LIBS" in #(vi
		*-lICE*)
			case "x$LIBS" in #(vi
			*-lICE*) #(vi
				;;
			*)
				CF_VERBOSE(work around broken ICE dependency)
				CF_TRY_PKG_CONFIG(ice,
					[CF_TRY_PKG_CONFIG(sm)],
					[CF_ADD_LIB_AFTER(-lXt,$X_PRE_LIBS)])
				;;
			esac
			;;
		esac
	fi

	cf_have_X_LIBS=yes
],[

	LDFLAGS="$X_LIBS $LDFLAGS"
	CF_CHECK_CFLAGS($X_CFLAGS)

	AC_CHECK_FUNC(XOpenDisplay,,[
	AC_CHECK_LIB(X11,XOpenDisplay,
		[CF_ADD_LIB(X11)],,
		[$X_PRE_LIBS $LIBS $X_EXTRA_LIBS])])

	AC_CHECK_FUNC(XtAppInitialize,,[
	AC_CHECK_LIB(Xt, XtAppInitialize,
		[AC_DEFINE(HAVE_LIBXT,1,[Define to 1 if we can compile with the Xt library])
		 cf_have_X_LIBS=Xt
		 LIBS="-lXt $X_PRE_LIBS $LIBS $X_EXTRA_LIBS"],,
		[$X_PRE_LIBS $LIBS $X_EXTRA_LIBS])])
])

if test $cf_have_X_LIBS = no ; then
	AC_MSG_WARN(
[Unable to successfully link X Toolkit library (-lXt) with
test program.  You will have to check and add the proper libraries by hand
to makefile.])
fi
])dnl
dnl ---------------------------------------------------------------------------
dnl CF__ADD_SHLIB_RULES version: 4 updated: 2013/07/22 18:49:24
dnl -------------------
dnl Append rules for creating, installing, uninstalling and cleaning library.
dnl In particular, this is needed for shared libraries since there are symbolic
dnl links which depend on configuration choices.
dnl
dnl The logic is controlled by these cache variables:
dnl $cf_cv_do_symlinks
dnl $cf_cv_shlib_version
dnl
dnl The macro uses variables set by CF__DEFINE_SHLIB_VARS
dnl
dnl $1 = makefile to append to
dnl $2 = model (static, shared, libtool)
dnl $3 = objects (dependencies)
dnl $4 = additional libraries needed to link the shared library
define([CF__ADD_SHLIB_RULES],[

CF__DEFINE_LIB_TARGET

case x$2 in
xlibtool|xshared) #(vi
	cf_libdeps="ifelse($4,,,[$4])"
	;;
x*)
	cf_libdeps=
	;;
esac

cat >>$1 <<CF_EOF

# generated by CF__ADD_SHLIB_RULES
# libmodel: $2
# symlinks: $cf_cv_do_symlinks
# shlibver: $cf_cv_shlib_version

CF_EOF

cat >>$1 <<CF_EOF
$cf_libname :: \\
CF_EOF

cat >>$1 <<CF_EOF
		$3
	@echo linking \[$]@
	\$(LIBTOOL_CREATE) $3 $cf_libdeps
CF_EOF

if test "x$cf_cv_do_symlinks" = xyes
then
cat >>$1 <<CF_EOF
	\$(LN_S) $cf_libname $cf_liblink
	\$(LN_S) $cf_liblink $cf_libroot
CF_EOF
fi

cat >>$1 <<CF_EOF

install \\
install.libs :: \$(DESTDIR)\$(libdir)/$cf_libname

\$(DESTDIR)\$(libdir)/$cf_libname :: \\
		\$(DESTDIR)\$(libdir) \\
		$3
	@echo linking \[$]@
	\$(LIBTOOL_CREATE) $3 $cf_libdeps
CF_EOF

if test "x$cf_cv_do_symlinks" = xyes
then
cat >>$1 <<CF_EOF
	cd \$(DESTDIR)\$(libdir) && (\$(LN_S) $cf_libname $cf_liblink; \$(LN_S) $cf_liblink $cf_libroot; )
CF_EOF
fi

if test x$2 = xshared
then
cat >>$1 <<CF_EOF
	- test -z "\$(DESTDIR)" && /sbin/ldconfig
CF_EOF
fi

cat >>$1 <<CF_EOF

uninstall \\
uninstall.libs ::
	@echo uninstalling \$(DESTDIR)\$(libdir)/$cf_libname
CF_EOF

if test "x$cf_cv_do_symlinks" = xyes
then
cat >>$1 <<CF_EOF
	-rm -f \$(DESTDIR)\$(libdir)/$cf_libroot
	-rm -f \$(DESTDIR)\$(libdir)/$cf_liblink
CF_EOF
fi

cat >>$1 <<CF_EOF
	-rm -f \$(DESTDIR)\$(libdir)/$cf_libname

clean \\
clean.libs ::
CF_EOF

if test "x$cf_cv_do_symlinks" = xyes
then
cat >>$1 <<CF_EOF
	-rm -f $cf_libroot
	-rm -f $cf_liblink
CF_EOF
fi

cat >>$1 <<CF_EOF
	-rm -f $cf_libname

mostlyclean::
	-rm -f $3
# end generated by CF__ADD_SHLIB_RULES
CF_EOF
])dnl
dnl ---------------------------------------------------------------------------
dnl CF__DEFINE_LIB_TARGET version: 1 updated: 2013/07/22 18:27:50
dnl ---------------------
define([CF__DEFINE_LIB_TARGET],[
cf_libname=\${LIB_BASENAME}
cf_liblink=$cf_libname
cf_libroot=$cf_libname

if test "x$cf_cv_do_symlinks" = xyes
then
	case "x$cf_cv_shlib_version" in #(vi
	xrel) #(vi
		cf_liblink="\${LIB_ABI_NAME}"
		cf_libname="\${LIB_REL_NAME}"
		;;
	xabi)
		cf_liblink="\${LIB_REL_NAME}"
		cf_libname="\${LIB_ABI_NAME}"
		;;
	esac
fi
LIB_TARGET=$cf_libname
])dnl
dnl ---------------------------------------------------------------------------
dnl CF__DEFINE_SHLIB_VARS version: 3 updated: 2015/01/02 17:09:39
dnl ---------------------
dnl Substitute makefile variables useful for CF__ADD_SHLIB_RULES.
dnl
dnl The substitution requires these variables:
dnl		LIB_PREFIX - "lib"
dnl		LIB_ROOTNAME - "foo"
dnl		LIB_SUFFIX - ".so"
dnl		REL_VERSION - "5.0"
dnl		ABI_VERSION - "4.2.4"
define([CF__DEFINE_SHLIB_VARS],[
CF__DEFINE_LIB_TARGET
SET_SHLIB_VARS="# begin CF__DEFINE_SHLIB_VARS\\
LIB_BASENAME	= ${LIB_PREFIX}\${LIB_ROOTNAME}\${LIB_SUFFIX}\\
LIB_REL_NAME	= \${LIB_BASENAME}.\${REL_VERSION}\\
LIB_ABI_NAME	= \${LIB_BASENAME}.\${ABI_VERSION}\\
LIB_TARGET	= $LIB_TARGET\\
RM_SHARED_OPTS	= $RM_SHARED_OPTS\\
# end CF__DEFINE_SHLIB_VARS"
AC_SUBST(SET_SHLIB_VARS)
AC_SUBST(LIB_TARGET)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF__INIT_SHLIB_RULES version: 2 updated: 2013/07/27 17:38:32
dnl --------------------
dnl The third parameter to AC_OUTPUT, used to pass variables needed for
dnl CF__ADD_SHLIB_RULES.
define([CF__INIT_SHLIB_RULES],[
ABI_VERSION="$ABI_VERSION"
REL_VERSION="$REL_VERSION"
LIB_MODEL="$LIB_MODEL"
LIB_PREFIX="$LIB_PREFIX"
LIB_ROOTNAME="$LIB_ROOTNAME"
DFT_DEP_SUFFIX="$DFT_DEP_SUFFIX"
RM_SHARED_OPTS="$RM_SHARED_OPTS"
cf_cv_do_symlinks="$cf_cv_do_symlinks"
cf_cv_shlib_version="$cf_cv_shlib_version"
])
dnl ---------------------------------------------------------------------------
dnl CF_WITH_VERSIONED_SYMS version: 3 updated: 2014/12/20 19:16:08
dnl ----------------------
dnl Use this when building shared library with ELF, to markup symbols with the
dnl version identifier from the given input file.  Generally that identifier is
dnl the same as the SONAME at which the symbol was first introduced.
dnl
dnl $1 = basename of the ".map" file (default $PACKAGE)
AC_DEFUN([CF_WITH_VERSIONED_SYMS],
[
AC_MSG_CHECKING(if versioned-symbols file should be used)
AC_ARG_WITH(versioned-syms,
	[  --with-versioned-syms=X markup versioned symbols using ld],
	[with_versioned_syms=$withval],
	[with_versioned_syms=no])
if test "x$with_versioned_syms" = xyes
then
	with_versioned_syms='${top_srcdir}/package/ifelse($1,,${PACKAGE},[$1]).map'
	AC_SUBST(PACKAGE)
fi
AC_MSG_RESULT($with_versioned_syms)

RESULTING_SYMS=
VERSIONED_SYMS=

if test "x$with_versioned_syms" != xno
then
	RESULTING_SYMS=$with_versioned_syms
	case "x$MK_SHARED_LIB" in
	*-Wl,*) #(vi
		VERSIONED_SYMS="-Wl,--version-script,\$(RESULTING_SYMS)"
		MK_SHARED_LIB=`echo "$MK_SHARED_LIB" | sed -e "s%-Wl,%\\[$]{VERSIONED_SYMS} -Wl,%"`
		CF_VERBOSE(MK_SHARED_LIB:  $MK_SHARED_LIB)
		;;
	*-dy*) #(vi
		VERSIONED_SYMS="-Wl,-M,\$(RESULTING_SYMS)"
		MK_SHARED_LIB=`echo "$MK_SHARED_LIB" | sed -e "s%-dy%\\[$]{VERSIONED_SYMS} -dy%"`
		CF_VERBOSE(MK_SHARED_LIB:  $MK_SHARED_LIB)
		;;
	*)
		AC_MSG_WARN(this system does not support versioned-symbols)
		;;
	esac
fi
AC_SUBST(RESULTING_SYMS)
AC_SUBST(VERSIONED_SYMS)
])dnl
dnl ---------------------------------------------------------------------------
dnl CF_WITH_EXPORT_SYMS version: 3 updated: 2014/12/20 19:16:08
dnl -------------------
dnl Use this with libtool to specify the list of symbols that may be exported.
dnl The input file contains one symbol per line; comments work with "#".
dnl
dnl $1 = basename of the ".sym" file (default $PACKAGE)
AC_DEFUN([CF_WITH_EXPORT_SYMS],
[
AC_MSG_CHECKING(if exported-symbols file should be used)
AC_ARG_WITH(export-syms,
	[  --with-export-syms=XXX  limit exported symbols using libtool],
	[with_export_syms=$withval],
	[with_export_syms=no])
if test "x$with_export_syms" = xyes
then
	with_export_syms='${top_srcdir}/package/ifelse($1,,${PACKAGE},[$1]).sym'
	AC_SUBST(PACKAGE)
fi
AC_MSG_RESULT($with_export_syms)
if test "x$with_export_syms" != xno
then
	EXPORT_SYMS="-export-symbols $with_export_syms"
	AC_SUBST(EXPORT_SYMS)
fi
])dnl
