PKG_CONFIG ?= pkg-config
STRIP      ?= strip

GL_CFLAGS        ?= $(shell $(PKG_CONFIG) --cflags gl)
GL_LIBS          ?= $(shell $(PKG_CONFIG) --libs gl 2>/dev/null || echo '-lGL')
GLU_CFLAGS       ?= $(shell $(PKG_CONFIG) --cflags glu)
GLU_LIBS         ?= $(shell $(PKG_CONFIG) --libs glu 2>/dev/null || echo '-lGLU')
GLUT_CFLAGS      ?= $(shell $(PKG_CONFIG) --cflags glut)
GLUT_LIBS        ?= $(shell $(PKG_CONFIG) --libs glut 2>/dev/null || echo '-lglut')

# MinGW32
ifneq ($(shell $(CXX) -dumpmachine | grep mingw32),)
GLEW_CFLAGS      ?= $(shell $(PKG_CONFIG) --cflags glew)
GLEW_LIBS        ?= $(shell $(PKG_CONFIG) --libs glew 2>/dev/null || echo '-lglew32')
endif

SDL_CFLAGS       ?= $(shell $(PKG_CONFIG) --cflags sdl)
SDL_LIBS         ?= $(shell $(PKG_CONFIG) --libs sdl)
SDL_IMAGE_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags SDL_image)
SDL_IMAGE_LIBS   ?= $(shell $(PKG_CONFIG) --libs SDL_image)
SDL_MIXER_CFLAGS ?= $(shell $(PKG_CONFIG) --cflags SDL_mixer)
SDL_MIXER_LIBS   ?= $(shell $(PKG_CONFIG) --libs SDL_mixer)

TINYXML2_CFLAGS  ?= $(shell $(PKG_CONFIG) --cflags tinyxml2)
TINYXML2_LIBS    ?= $(shell $(PKG_CONFIG) --libs tinyxml2)


CXXFLAGS ?= -Wall -O3
CXXFLAGS += \
	$(GL_CFLAGS) \
	$(GLU_CFLAGS) \
	$(GLUT_CFLAGS) \
	$(GLEW_CFLAGS) \
	$(SDL_CFLAGS) \
	$(SDL_IMAGE_CFLAGS) \
	$(SDL_MIXER_CFLAGS) \
	$(TINYXML2_CFLAGS)

LDFLAGS ?= -Wl,--as-needed

LIBS = \
	$(GL_LIBS) \
	$(GLU_LIBS) \
	$(GLUT_LIBS) \
	$(GLEW_LIBS) \
	$(SDL_LIBS) \
	$(SDL_IMAGE_LIBS) \
	$(SDL_MIXER_LIBS) \
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
	obj/worlds.o

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
