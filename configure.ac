# Process this file with autoconf to produce a configure script.
AC_INIT([spass], [2.0], [guyrutenberg@gmail.com])
AM_INIT_AUTOMAKE([-Wall foreign])
AC_CONFIG_SRCDIR([main.cpp])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

# Configure options
AC_ARG_WITH([portaudio],
	[AS_HELP_STRING([--with-portaudio], [use PortAudio as the audio backend
	instead of ALSA] @<:@default=no@:>@)],
	[],
	[with_portaudio=no])

AC_ARG_WITH([oss],
	[AS_HELP_STRING([--with-oss], [use Open Sound System (OSS) as the audio
	backend instead of ALSA] @<:@default=no@:>@)],
	[],
	[with_oss=no])

AC_ARG_WITH([alsa],
	[AS_HELP_STRING([--with-alsa], [use Advanced Linux Sound Architecture
	(ALSA) as the audio backend] @<:@default=yes@:>@)],
	[],
	[with_alsa=yes])

# Checks for libraries.

# the alsa check must be first, so OSS and PortAudio can override it
AS_IF([test "x$with_alsa" != xno],
	[
	PKG_CHECK_MODULES([ALSA], [alsa])
	AM_CONDITIONAL(WITH_ALSA, true)
	AM_CONDITIONAL(WITH_PORTAUDIO, false)
	AM_CONDITIONAL(WITH_OSS, false)
	AC_DEFINE(WITH_ALSA,[],[Use ALSA])
	], [
	AM_CONDITIONAL(WITH_ALSA, false)
	])

AS_IF([test "x$with_portaudio" != xno],
	[
	PKG_CHECK_MODULES([PORTAUDIO], [portaudio-2.0])
	AM_CONDITIONAL(WITH_PORTAUDIO, true)
	AM_CONDITIONAL(WITH_OSS, false)
	AM_CONDITIONAL(WITH_ALSA, false)
	AC_DEFINE(WITH_PORTAUDIO,[],[Use PortAudio])
	], [
	AM_CONDITIONAL(WITH_PORTAUDIO, false)
	])

AS_IF([test "x$with_oss" != xno],
	[
	AM_CONDITIONAL(WITH_OSS, true)
	AM_CONDITIONAL(WITH_PORTAUDIO, false)
	AM_CONDITIONAL(WITH_ALSA, false)
	AC_DEFINE(WITH_OSS,[],[Use OSS])
	], [
	AM_CONDITIONAL(WITH_OSS, false)
	])


# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
