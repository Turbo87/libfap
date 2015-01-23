AC_INIT(APRS parser,0.5,OH2GVE,libfap)

dnl iface version[:implementation version[:iface backward compatibility count]]
FAP_LIBRARY_VERSION=0:5
AC_SUBST(FAP_LIBRARY_VERSION)

AC_LANG([C])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE

AM_PROG_LIBTOOL
AC_PROG_CC
AC_PROG_INSTALL

AC_CHECK_HEADER([stdlib.h])
AC_CHECK_HEADER([stdio.h])
AC_CHECK_HEADER([string.h])
AC_CHECK_HEADER([ctype.h])
AC_CHECK_HEADER([regex.h])
AC_CHECK_HEADER([time.h])
AC_CHECK_HEADER([math.h])

AC_CHECK_LIB(m, pow)

AC_CONFIG_FILES([Makefile doc/Makefile licenses/Makefile src/Makefile])
AC_OUTPUT
