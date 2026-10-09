ifneq ($(CROSS_PREFIX),)
CXX        ?= $(CROSS_PREFIX)-c++
PKG_CONFIG ?= $(CROSS_PREFIX)-pkg-config
STRIP      ?= $(CROSS_PREFIX)-strip
else
PKG_CONFIG ?= pkg-config
STRIP      ?= strip
endif

ifeq ($(shell $(CXX) -dumpmachine | grep mingw32),)
OPENGL_LIBS ?= -lGL -lGLU -lglut
else
# MinGW32
OPENGL_LIBS ?= -lopengl32 -lglu32 -lglut32 -lglew32
endif

SDL2_CFLAGS       ?= $(shell $(PKG_CONFIG) --cflags sdl2)
SDL2_LIBS         ?= $(shell $(PKG_CONFIG) --libs sdl2)
SDL2_IMAGE_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags SDL2_image)
SDL2_IMAGE_LIBS   ?= $(shell $(PKG_CONFIG) --libs SDL2_image)
SDL2_MIXER_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags SDL2_mixer)
SDL2_MIXER_LIBS   ?= $(shell $(PKG_CONFIG) --libs SDL2_mixer)
TINYXML2_CFLAGS   ?= $(shell $(PKG_CONFIG) --cflags tinyxml2)
TINYXML2_LIBS     ?= $(shell $(PKG_CONFIG) --libs tinyxml2)


CXXFLAGS ?= -Wall -O3 #-std=c++11
CXXFLAGS += \
	$(SDL2_CFLAGS) \
	$(SDL2_IMAGE_CFLAGS) \
	$(SDL2_MIXER_CFLAGS) \
	$(TINYXML2_CFLAGS)

LDFLAGS ?= -Wl,--as-needed

LIBS = \
	$(OPENGL_LIBS) \
	$(SDL2_LIBS) \
	$(SDL2_IMAGE_LIBS) \
	$(SDL2_MIXER_LIBS) \
	$(TINYXML2_LIBS)

NEW_OR_ORIG = new
#NEW_OR_ORIG = original

RESOURCES = \
	resources/*.frag \
	resources/$(NEW_OR_ORIG)/*.jpg \
	resources/$(NEW_OR_ORIG)/*.ogg \
	resources/*.png \
	resources/*.vert \
	resources/world.xml

all: obj/Polly-B-Gone

obj/main.out: \
	obj/ball.o \
	obj/block.o \
	obj/escalator.o \
	obj/fan.o \
	obj/lighting.o \
	obj/material.o \
	obj/model.o \
	obj/physics/constraint.o \
	obj/physics/force.o \
	obj/physics/particle.o \
	obj/physics/rotation.o \
	obj/physics/shape.o \
	obj/physics/transform.o \
	obj/physics/translation.o \
	obj/physics/vector.o \
	obj/player.o \
	obj/portal.o \
	obj/ramp.o \
	obj/resource.o \
	obj/room.o \
	obj/room_force.o \
	obj/room_object.o \
	obj/rotating.o \
	obj/seesaw.o \
	obj/shader.o \
	obj/simulation.o \
	obj/sound.o \
	obj/switch.o \
	obj/texture.o \
	obj/trail.o \
	obj/transforming.o \
	obj/translating.o \
	obj/tube.o \
	obj/wall.o \
	obj/world.o \
	obj/worlds.o \
	obj/main.o

obj/physics/particle_test.out: \
	obj/physics/force.o \
	obj/physics/particle.o \
	obj/physics/vector.o \
	obj/simulation.o

obj/physics/shape_test.out: \
	obj/physics/shape.o \
	obj/physics/vector.o

obj/physics/vector_test.out: \
	obj/physics/vector.o

obj/Polly-B-Gone: obj/main.out $(RESOURCES) Makefile
	rm -rf $@
	mkdir -p $@
	cp obj/main.out $@/polly-b-gone
	$(STRIP) $@/polly-b-gone
	mkdir -p $@/resources/
	cp -f $(RESOURCES) $@/resources/

obj/%.out: obj/%.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

obj/%.o: src/%.cpp obj/physics
	$(CXX) -c $(CXXFLAGS) -o $@ $<

obj/physics:
	mkdir -p $@

.PRECIOUS: obj/%.o obj/physics/%.o

clean:
	rm -rf obj

.PHONY: all clean

