ifneq ($(CROSS_PREFIX),)
CXX        := $(CROSS_PREFIX)-c++
PKG_CONFIG := $(CROSS_PREFIX)-pkg-config
STRIP      := $(CROSS_PREFIX)-strip
else
PKG_CONFIG := pkg-config
STRIP      := strip
endif

ifneq ($(shell $(CXX) -dumpmachine | grep apple),)
TARGET := apple
else
ifneq ($(shell $(CXX) -dumpmachine | grep mingw),)
TARGET := windows
else
TARGET := other
endif
endif


# new fully GPL-compatible game resources
GAME_RES := new

# original game resources
#GAME_RES := original


CXXFLAGS ?= -Wall -O3


ifeq ($(TARGET), apple)
HOMEBREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /opt/homebrew)

CXXFLAGS += \
	-std=c++11 \
	-Wno-deprecated-declarations \
	-I$(HOMEBREW_PREFIX)/include \
	-I$(HOMEBREW_PREFIX)/include/SDL2

LDFLAGS += \
	-L$(HOMEBREW_PREFIX)/lib

LIBS := \
	-framework Cocoa \
	-framework CoreFoundation \
	-framework OpenGL \
	-framework GLUT \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_mixer \
	-ltinyxml2

else

SDL2_CFLAGS       ?= $(shell $(PKG_CONFIG) --cflags sdl2)
SDL2_LIBS         ?= $(shell $(PKG_CONFIG) --libs sdl2)
SDL2_IMAGE_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags SDL2_image)
SDL2_IMAGE_LIBS   ?= $(shell $(PKG_CONFIG) --libs SDL2_image)
SDL2_MIXER_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags SDL2_mixer)
SDL2_MIXER_LIBS   ?= $(shell $(PKG_CONFIG) --libs SDL2_mixer)
TINYXML2_CFLAGS   ?= $(shell $(PKG_CONFIG) --cflags tinyxml2)
TINYXML2_LIBS     ?= $(shell $(PKG_CONFIG) --libs tinyxml2)
GLUT_CFLAGS       ?= $(shell $(PKG_CONFIG) --cflags glut)
GLUT_LIBS         ?= $(shell $(PKG_CONFIG) --libs glut)

ifeq ($(TARGET), windows)
GLEW_CFLAGS       ?= $(shell $(PKG_CONFIG) --cflags glew)
GLEW_LIBS         ?= $(shell $(PKG_CONFIG) --libs glew)
OPENGL_LIBS       := -lopengl32 -lglu32
EXEEXT             = .exe
else
GLEW_CFLAGS        =
GLEW_LIBS          =
OPENGL_LIBS       := -lGL -lGLU
EXEEXT             =
endif

CXXFLAGS += \
	$(OPENGL_CFLAGS) \
	$(GLEW_CFLAGS) \
	$(GLUT_CFLAGS) \
	$(SDL2_CFLAGS) \
	$(SDL2_IMAGE_CFLAGS) \
	$(SDL2_MIXER_CFLAGS) \
	$(TINYXML2_CFLAGS)

LIBS := \
	$(OPENGL_LIBS) \
	$(GLEW_LIBS) \
	$(GLUT_LIBS) \
	$(SDL2_LIBS) \
	$(SDL2_IMAGE_LIBS) \
	$(SDL2_MIXER_LIBS) \
	$(TINYXML2_LIBS)
endif


RESOURCES = \
	resources/$(GAME_RES)/ceramic.jpg \
	resources/$(GAME_RES)/clovers.jpg \
	resources/$(GAME_RES)/concrete.jpg \
	resources/$(GAME_RES)/drain.jpg \
	resources/$(GAME_RES)/ivy.jpg \
	resources/$(GAME_RES)/metal.jpg \
	resources/$(GAME_RES)/intro.ogg \
	resources/$(GAME_RES)/loop.ogg \
	resources/$(GAME_RES)/outro.ogg \
	resources/normal.frag \
	resources/normal.vert \
	resources/title.png \
	resources/world.xml
ifeq ($(TARGET), apple)
RESOURCES += \
	resources/Polly.icns
endif


DOCS = \
	doc/xml-format.html \
	resources/$(GAME_RES)/COPYING.* \
	resources/$(GAME_RES)/README \
	LICENSE


# APPDIR
ifeq ($(TARGET), apple)
APPDIR = out/Polly-B-Gone.app
else
ifeq ($(TARGET), windows)
APPDIR = out/Polly-B-Gone
else
APPDIR = out/Polly-B-Gone.appdir
endif
endif


MAIN_OBJS = \
	src/ball.o \
	src/block.o \
	src/escalator.o \
	src/fan.o \
	src/lighting.o \
	src/main.o \
	src/material.o \
	src/model.o \
	src/physics/constraint.o \
	src/physics/force.o \
	src/physics/particle.o \
	src/physics/rotation.o \
	src/physics/shape.o \
	src/physics/transform.o \
	src/physics/translation.o \
	src/physics/vector.o \
	src/player.o \
	src/portal.o \
	src/ramp.o \
	src/resource.o \
	src/room.o \
	src/room_force.o \
	src/room_object.o \
	src/rotating.o \
	src/seesaw.o \
	src/shader.o \
	src/simulation.o \
	src/sound.o \
	src/switch.o \
	src/texture.o \
	src/trail.o \
	src/transforming.o \
	src/translating.o \
	src/tube.o \
	src/wall.o \
	src/world.o \
	src/worlds.o

BIN = src/polly-b-gone$(EXEEXT)

TEST_BINS = \
	src/physics/particle_test \
	src/physics/shape_test \
	src/physics/vector_test



all: $(APPDIR)

clean:
	rm -rf out
	rm -f $(BIN) $(TEST_BINS)
	rm -f src/*.o src/physics/*.o
	rm -f src/*.exe src/physics/*.exe

$(APPDIR): $(BIN)

src/physics/particle_test$(EXEEXT): \
	src/physics/force.o \
	src/physics/particle.o \
	src/physics/vector.o \
	src/simulation.o

src/physics/shape_test$(EXEEXT): \
	src/physics/shape.o \
	src/physics/vector.o

src/physics/vector_test$(EXEEXT): \
	src/physics/vector.o


ifeq ($(TARGET), apple)

out/Polly-B-Gone.app : $(BIN) $(RESOURCES) resources/Info.plist
	rm -rf $@
	mkdir -p $@/Contents/MacOS $@/Contents/Resources
	cp $< $@/Contents/MacOS/Polly-B-Gone
	cp resources/Info.plist $@/Contents
	cp $(RESOURCES) $@/Contents/Resources

else

ifeq ($(TARGET), windows)

out/Polly-B-Gone: $(BIN) $(RESOURCES)
	rm -rf $@
	mkdir -p $@/resources/
	cp $< $@/polly-b-gone.exe
	$(STRIP) $@/polly-b-gone.exe
	cp $(RESOURCES) $@/resources/

else

out/Polly-B-Gone.appdir: $(BIN) $(RESOURCES)
	rm -rf $@
	## binary
	mkdir -p $@/usr/bin && cp $< $@/usr/bin/polly-b-gone
	$(STRIP) $@/usr/bin/polly-b-gone
	ln -s usr/bin/polly-b-gone $@/polly-b-gone
	## game files
	mkdir -p $@/usr/share/polly-b-gone && cp $(RESOURCES) $@/usr/share/polly-b-gone
	## desktop files
	mkdir -p $@/usr/share/applications && cp resources/polly-b-gone.desktop $@/usr/share/applications
	for size in 16 22 24 32 48 64 96 128 ; do \
	  mkdir -p $@/usr/share/icons/hicolor/$${size}x$${size}/apps && \
	  cp resources/polly-b-gone_$${size}x$${size}.png \
	    $@/usr/share/icons/hicolor/$${size}x$${size}/apps/polly-b-gone.png ; \
	done
	## documents
	mkdir -p $@/usr/share/doc/polly-b-gone && cp $(DOCS) $@/usr/share/doc/polly-b-gone

endif
endif


$(BIN): $(MAIN_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)


.PHONY: all clean $(APPDIR)

