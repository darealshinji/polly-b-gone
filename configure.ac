AC_INIT([Polly-B-Gone], [1.0],
        [https://github.com/mbostock/polly-b-gone/issues], [],
        [https://github.com/mbostock/polly-b-gone])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror dist-bzip2 dist-xz foreign subdir-objects])
AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_CXX

CXXFLAGS="$CXXFLAGS -Wall -Wno-switch"

# usage: AX_CHECK_PKG_LIB(prefix, pkg-module, library, function)
m4_define([AX_CHECK_PKG_LIB], [{
    PKG_CHECK_MODULES([$1], [$2], [], [
        AC_CHECK_LIB([$3], [$4], [LIBS="$LIBS -l$3"], [exit 1])
    ])
}])
AX_CHECK_PKG_LIB([GL], [gl], [GL], [glFogf])
AX_CHECK_PKG_LIB([GLU], [glu], [GLU], [gluDisk])
AX_CHECK_PKG_LIB([GLUT], [glut], [glut], [glutInit])
AX_CHECK_PKG_LIB([SDL], [sdl < 2], [SDL], [SDL_GetError])
AX_CHECK_PKG_LIB([SDL_IMAGE], [SDL_image < 2], [SDL_image], [IMG_Load])
AX_CHECK_PKG_LIB([SDL_MIXER], [SDL_mixer < 2], [SDL_mixer], [Mix_OpenAudio])
AX_CHECK_PKG_LIB([TINYXML], [tinyxml], [tinyxml], [TiXmlNode])

AC_CHECK_HEADERS([GL/gl.h GL/glu.h GL/glut.h GLES2/gl2.h], [], [exit 1])

AC_CONFIG_FILES([Makefile
                 resources/Makefile
                 src/Makefile])
AC_OUTPUT
