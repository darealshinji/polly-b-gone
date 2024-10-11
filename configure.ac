# usage: AX_CHECK_PKG_LIB(prefix, pkg-module, library)
m4_define([AX_CHECK_PKG_LIB], [{
    AC_LANG_PUSH([C++])
    AC_CHECK_HEADERS([$4], [], [exit 1])
    PKG_CHECK_MODULES([$1], [$2], [], [
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
    ])
    AC_LANG_POP([C++])
}])


AC_INIT([Polly-B-Gone], [1.0],
        [https://github.com/mbostock/polly-b-gone/issues], [],
        [https://github.com/mbostock/polly-b-gone])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror dist-xz foreign subdir-objects])
AM_SILENT_RULES([yes])

: ${CFLAGS="-Wall -O3"}
: ${CXXFLAGS="-Wall -O3"}
: ${LDFLAGS="-s"}

AC_PROG_CC
AC_PROG_CXX
PKG_PROG_PKG_CONFIG

AC_SEARCH_LIBS([cosf], [m], [], [])

AS_CASE([$host],
    [*mingw32*], [
     AX_CHECK_PKG_LIB([GL], [gl], [opengl32], [GL/gl.h])
     AX_CHECK_PKG_LIB([GLU], [glu], [glu32], [GL/glu.h])
     AX_CHECK_PKG_LIB([GLUT], [glut], [glut32], [GL/glut.h])
     AX_CHECK_PKG_LIB([GLEW], [glew], [glew32], [GL/glew.h])
    ], [
     AX_CHECK_PKG_LIB([GL], [gl], [GL], [GL/gl.h GLES2/gl2.h])
     AX_CHECK_PKG_LIB([GLU], [glu], [GLU], [GL/glu.h])
     AX_CHECK_PKG_LIB([GLUT], [glut], [glut], [GL/glut.h])
])

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
