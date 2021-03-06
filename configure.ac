# usage: AX_CHECK_PKG_LIB(prefix, pkg-module, library, headers)
m4_define([AX_CHECK_PKG_LIB], [{
    AC_LANG_PUSH([C++])
    PKG_CHECK_MODULES([$1], [$2], [], [
        # library check
        LIBS_backup="$LIBS"
        LIBS="-l$3"
        AC_MSG_CHECKING([for -l$3])
        AC_LINK_IFELSE([
            AC_LANG_SOURCE(
                [[int main() { return 0; }]]
            )
        ], [AC_MSG_RESULT([found])],
           [AC_MSG_ERROR([$3 not found])]
        )
        LIBS="$LIBS_backup -l$3"
        # header checks
        AS_IF([test "x$4" != "x"], [
            AC_CHECK_HEADERS([$4], [], [exit 1])
        ])
    ])
    AC_LANG_POP([C++])
}])

# usage: AX_CHECK_CXXFLAGS(flags)
m4_define([AX_CHECK_CXXFLAGS], [{
    CXXFLAGS_backup="$CXXFLAGS"
    CXXFLAGS="-Werror $1"
    AC_MSG_CHECKING([if $CXX understands $1])
    AC_LANG_PUSH([C++])
    AC_COMPILE_IFELSE([
        AC_LANG_SOURCE(
            [[int main() { return 0; }]]
        )
    ], [AC_MSG_RESULT([yes])
        addflag="$1"],
       [AC_MSG_RESULT([no])]
    )
    CXXFLAGS="$addflag $CXXFLAGS_backup"
    AC_LANG_POP([C++])
}])


AC_INIT([Polly-B-Gone], [1.0],
        [https://github.com/mbostock/polly-b-gone/issues], [],
        [https://github.com/mbostock/polly-b-gone])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror dist-bzip2 dist-xz foreign subdir-objects])
AM_SILENT_RULES([yes])

: ${CFLAGS="-Wall -g -O2"}
: ${CXXFLAGS="-Wall -g -O2"}

AC_PROG_CC
AC_PROG_CXX


# math library
AC_SEARCH_LIBS([cosf], [m], [], [exit 1])

# (Open)GL, GLU, glut
AS_CASE([$host],
    [*mingw32*], [
     libgl="opengl32"
     libglu="glu32"
     libglut="glut32"
     glew="yes"
    ], [
     libgl="GL"
     libglu="GLU"
     libglut="glut"
])
AX_CHECK_PKG_LIB([GL], [gl], [$libgl], [GL/gl.h])
AX_CHECK_PKG_LIB([GLU], [glu], [$libglu], [GL/glu.h])
AC_ARG_WITH([glut],
  [AS_HELP_STRING([--with-glut], [use the GLUT library])]
)
AS_IF([test "x$with_glut" = "xyes"], [
  AX_CHECK_PKG_LIB([GLUT], [glut], [$libglut], [GL/glut.h])
  CXXFLAGS="$CXXFLAGS -DWITH_GLUT"
])

## GLEW library
# on OSX is doesn't seem to be required, mingw32 needs it
# and on Linux it's optional (tested on Ubuntu)
AC_ARG_WITH([glew],
  [AS_HELP_STRING([--with-glew], [use the GLEW library (option has no effect for Win32 and Darwin)])]
)
AS_CASE([$host],
    [*darwin*], [with_glew="no"],
    [*mingw32*], [
     AX_CHECK_PKG_LIB([GLEW], [glew], [glew32], [GL/glew.h])
     with_glew="yes"
    ], [
     AS_IF([test "x$with_glew" = "xyes"], [
         AX_CHECK_PKG_LIB([GLEW], [glew], [GLEW], [GL/glew.h]) ], [
         AC_CHECK_HEADERS([GLES2/gl2.h GLES2/gl2platform.h], [], [exit 1])
     ])
])
AS_IF([test "x$with_glew" = "xyes"], [
    CXXFLAGS="$CXXFLAGS -DWITH_GLEW"
])

# SDL libraries
AX_CHECK_PKG_LIB([SDL], [sdl < 2], [SDL], [SDL/SDL.h SDL/SDL_error.h])
AX_CHECK_PKG_LIB([SDL_IMAGE], [SDL_image < 2], [SDL_image], [SDL/SDL_image.h])
AX_CHECK_PKG_LIB([SDL_MIXER], [SDL_mixer < 2], [SDL_mixer], [SDL/SDL_mixer.h])

AX_CHECK_PKG_LIB([TINYXML], [tinyxml], [tinyxml], [tinyxml.h])


AC_ARG_ENABLE([non-free],
    AS_HELP_STRING([--enable-non-free], [install original non-free game files]))
AM_CONDITIONAL([NONFREE], [test "x$enable_non_free" = "xyes"])


AC_CONFIG_FILES([Makefile
                 resources/Makefile
                 src/Makefile])
AC_OUTPUT
