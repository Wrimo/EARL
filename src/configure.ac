# Must init the autoconf setup
# The first parameter is project name
# second is version number
# third is bug report address
AC_INIT([main], [1.0])

# Safety checks in case user overwritten --srcdir
AC_CONFIG_SRCDIR([main.c])

# Store the auxiliary build tools (e.g., install-sh, config.sub, config.guess)
# in this dir (build-aux)
AC_CONFIG_AUX_DIR([build-aux])

# Init automake, and specify this program use relaxed structures.
# i.e. this program doesn't follow the gnu coding standards, and doesn't have
# ChangeLog, COPYING, AUTHORS, INSTALL, README etc. files.
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Check for C compiler
AC_PROG_CC

# Tells automake to create Makefiles in specified directories
AC_CONFIG_FILES([Makefile
                test/Makefile])

# Generate the output
AC_OUTPUT
