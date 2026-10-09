// -*- C++ -*-

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

#include "resource.h"
#include "room.h"
#include "shader.h"
#include "sound.h"
#include "texture.h"
#include "world.h"
#include "worlds.h"

using namespace mbostock;

static const int defaultWidth = 640;
static const int defaultHeight = 480;
static int screenWidth = 0;
static int screenHeight = 0;
static int windowWidth = defaultWidth;
static int windowHeight = defaultHeight;
static bool run = true;
static bool fullScreen = false;
static World* world = NULL;
static int shaderi = 0;

static SDL_Window* window = NULL;
static SDL_GLContext glContext = NULL;
static SDL_GameController* controller = NULL;

static Shader* shader() {
  static Shader* shaders[] = {
    Shaders::defaultShader(),
    Shaders::wireframeShader(),
    Shaders::normalShader()
  };
  return shaders[shaderi];
}

static void resizeSurface(int width, int height) {
  if (width == 0 || height == 0) {
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    width = screenWidth = mode.w;
    height = screenHeight = mode.h;
  }

  windowWidth = width;
  windowHeight = height;

  // Get actual drawable size (may differ from window size on HiDPI displays)
  int drawableWidth, drawableHeight;
  SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);

  glViewport(0, 0, drawableWidth, drawableHeight);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.f, (float)drawableWidth / (float)drawableHeight, 1.0f, 100.f);
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0.f, 0.f, 0.f, 0.f);

  shader()->initialize();
  Textures::initialize();
  world->model().initialize();
}

static void handleDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  world->simulate();

  const Vector& p = world->player().origin();
  const Vector& min = world->room().cameraBounds().min();
  const Vector& max = world->room().cameraBounds().max();

  const float kd = .060f; /* frame-rate dependent */

  /* Interpolate the eye location. */
  Vector ee(p.x, p.y + 4.f, p.z + 6.f);
  ee = Vector::min(Vector::max(min, ee), max);
  static Vector e = ee;
  e = e * (1.f - kd) + ee * kd;

  /* Interpolate the camera direction towards the player. */
  static Vector c = p;
  c = c * (1.f - kd) + p * kd;

  gluLookAt(e.x, e.y, e.z,
            c.x, c.y, c.z,
            0.f, 1.f, 0.f);

  shader()->display(world->model());
  SDL_GL_SwapWindow(window);
}

static void toggleShader() {
  const int shadern = 3;
  shaderi = (shaderi + 1) % shadern;
  shader()->initialize();
}

static void toggleFullScreen() {
  fullScreen = !fullScreen;
  if (fullScreen) {
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_ShowCursor(SDL_DISABLE);
    SDL_GL_GetDrawableSize(window, &windowWidth, &windowHeight);
    resizeSurface(windowWidth, windowHeight);
  } else {
    SDL_SetWindowFullscreen(window, 0);
    SDL_SetWindowSize(window, defaultWidth, defaultHeight);
    SDL_ShowCursor(SDL_ENABLE);
    resizeSurface(defaultWidth, defaultHeight);
  }
}

static void handleKeyDown(SDL_Event* event) {
  switch (event->key.keysym.sym) {
    /* move forward */
    case SDLK_w:
    case SDLK_UP:
      world->player().move(Player::FORWARD);
      break;

    /* turn left */
    case SDLK_a:
    case SDLK_LEFT:
      world->player().move(Player::LEFT);
      break;

    /* turn right */
    case SDLK_d:
    case SDLK_RIGHT:
      world->player().move(Player::RIGHT);
      break;

    /* move backward */
    case SDLK_s:
    case SDLK_DOWN:
      world->player().move(Player::BACKWARD);
      break;

    case SDLK_PAGEUP:
      world->nextRoom();
      break;
    case SDLK_PAGEDOWN:
      world->previousRoom();
      break;
    case SDLK_r:
    case SDLK_BACKSPACE:
      world->resetPlayer();
      break;

    /* music volume */
    case SDLK_KP_MINUS:
    case SDLK_MINUS:
      Sounds::volumeLower();
      break;
    case SDLK_KP_PLUS:
    case SDLK_PLUS:
      Sounds::volumeHigher();
      break;

    default:
      break;
  }
}

static void handleKeyUp(SDL_Event* event) {
  switch (event->key.keysym.sym) {
    /* stop moving forward */
    case SDLK_w:
    case SDLK_UP:
      world->player().stop(Player::FORWARD);
      break;

    /* stop turning left */
    case SDLK_a:
    case SDLK_LEFT:
      world->player().stop(Player::LEFT);
      break;

    /* stop turning right */
    case SDLK_d:
    case SDLK_RIGHT:
      world->player().stop(Player::RIGHT);
      break;

    /* stop moving backward */
    case SDLK_s:
    case SDLK_DOWN:
      world->player().stop(Player::BACKWARD);
      break;

    case SDLK_SPACE:
    case SDLK_PAUSE:
      world->togglePaused();
      break;
    case SDLK_ESCAPE:
      run = false;
      break;

    case SDLK_F4:
      if (event->key.keysym.mod & KMOD_ALT) {
        run = false; /* Alt+F4 */
      }
      break;
    case SDLK_F9:
      toggleShader();
      break;
    case SDLK_F10:
      world->toggleDebug();
      break;
    case SDLK_F11:
      toggleFullScreen();
      break;

    default:
      break;
  }
}

static void handleButtonDown(SDL_Event* event) {
  switch (event->cbutton.button) {
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
    case SDL_CONTROLLER_BUTTON_A:
      world->player().move(Player::FORWARD);
      break;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
      world->player().move(Player::LEFT);
      break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
      world->player().move(Player::RIGHT);
      break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
    case SDL_CONTROLLER_BUTTON_B:
      world->player().move(Player::BACKWARD);
      break;
    default:
      break;
  }
}

static void handleButtonUp(SDL_Event* event) {
  switch (event->cbutton.button) {
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
    case SDL_CONTROLLER_BUTTON_A:
      world->player().stop(Player::FORWARD);
      break;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
      world->player().stop(Player::LEFT);
      break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
      world->player().stop(Player::RIGHT);
      break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
    case SDL_CONTROLLER_BUTTON_B:
      world->player().stop(Player::BACKWARD);
      break;

    case SDL_CONTROLLER_BUTTON_START:
    case SDL_CONTROLLER_BUTTON_TOUCHPAD:
    case SDL_CONTROLLER_BUTTON_MISC1:
      world->togglePaused();
      break;

    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
      world->nextRoom();
      break;
    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
      world->previousRoom();
      break;
    case SDL_CONTROLLER_BUTTON_Y:
      world->resetPlayer();
      break;
    default:
      break;
  }
}

static void handleQuit() {
  Sounds::dispose();
  if (world) delete world;
  if (glContext) SDL_GL_DeleteContext(glContext);
  if (window) SDL_DestroyWindow(window);
  if (controller) SDL_GameControllerClose(controller);
  SDL_Quit();
}

static void eventLoop() {
  SDL_Event event;
  while (run) {
    handleDisplay();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_WINDOWEVENT:
          if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            resizeSurface(event.window.data1, event.window.data2);
          }
          break;
        case SDL_KEYDOWN:
          handleKeyDown(&event);
          break;
        case SDL_KEYUP:
          handleKeyUp(&event);
          break;
        case SDL_CONTROLLERBUTTONDOWN:
          handleButtonDown(&event);
          break;
        case SDL_CONTROLLERBUTTONUP:
          handleButtonUp(&event);
          break;
        case SDL_CONTROLLERDEVICEADDED:
          if (!controller) {
            controller = SDL_GameControllerOpen(0);
          }
          break;
        case SDL_CONTROLLERDEVICEREMOVED:
          if (controller) {
            SDL_GameControllerClose(controller);
            controller = NULL;
          }
          break;
        case SDL_QUIT:
          run = false;
          break;
        default:
          break;
      }
    }
    SDL_Delay(10);
  }
  handleQuit();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

  const Uint32 init_flags =
    SDL_INIT_VIDEO |
    SDL_INIT_AUDIO |
    SDL_INIT_GAMECONTROLLER;

  if (SDL_Init(init_flags) == -1) {
    std::cerr << "Could not initialize SDL: " << SDL_GetError() << std::endl;
    return 1;
  }

  controller = SDL_GameControllerOpen(0);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

  // Request legacy OpenGL profile for compatibility
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

  window = SDL_CreateWindow(
    "POLLY-B-GONE",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    defaultWidth, defaultHeight,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
  );

  if (!window) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    return 1;
  }

  glContext = SDL_GL_CreateContext(window);
  if (!glContext) {
    fprintf(stderr, "Failed to create GL context: %s\n", SDL_GetError());
    return 1;
  }

  SDL_GL_SetSwapInterval(1);

  Sounds::initialize();

  world = Worlds::fromFile("world.xml");
  if (!world) {
    handleQuit();
    return 1;
  }

  /* pause game before initial resizing */
  world->togglePaused();
  //resizeSurface(defaultWidth, defaultHeight);
  toggleFullScreen();
  world->togglePaused();

  eventLoop();

  return 0;
}
