CXXFLAGS = -O2 -Wall `pkg-config --cflags SDL_image SDL_mixer`
LDFLAGS = -lGL -lGLU `pkg-config --libs SDL_image SDL_mixer` -ltinyxml

ifeq ($(DISABLE_GLUT),1)
CXXFLAGS += -DDISABLE_GLUT
else
LDFLAGS += -lglut
endif

all: polly-b-gone

OBJS = \
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

polly-b-gone: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

clean:
	rm -f polly-b-gone $(OBJS)


