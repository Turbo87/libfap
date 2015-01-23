AC_INIT(APRS parser,1.2,OH2GVE,libfap)

dnl iface version[:implementation version[:iface backward compatibility count]]
FAP_LIBRARY_VERSION=5:0:0
AC_SUBST(FAP_LIBRARY_VERSION)

AC_CONFIG_MACRO_DIR([m4])
AC_LANG([C])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE

AC_PROG_LIBTOOL
AC_PROG_CC
AC_PROG_INSTALL

AC_CHECK_HEADER([stdlib.h],,AC_MSG_ERROR([Required header not found.]))
AC_CHECK_HEADER([stdio.h],,AC_MSG_ERROR([Required header not found.]))
AC_CHECK_HEADER([string.h],,AC_MSG_ERROR([Required header not found.]))
AC_CHECK_HEADER([stdint.h],,AC_MSG_ERROR([Required header not found.]))
AC_CHECK_HEADER([ctype.h],,AC_MSG_ERROR([Required header not found.]))
AC_CHECK_HEADER([regex.h],,AC_MSG_ERROR([Required header not found.]))
AC_CHECK_HEADER([time.h],,AC_MSG_ERROR([Required header not found.]))
AC_CHECK_HEADER([math.h],,AC_MSG_ERROR([Required header not found.]))

AC_CHECK_LIB(m, pow)

AC_CONFIG_FILES([Makefile licenses/Makefile src/Makefile libfap.pc])
AC_OUTPUT
