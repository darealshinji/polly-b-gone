// -*- C++ -*-

#ifndef MBOSTOCK_SOUND_H
#define MBOSTOCK_SOUND_H

namespace mbostock {

  class Sound {
  public:
    virtual ~Sound() {}
    virtual void play(int loops = 0) const = 0;
    virtual void stop() const = 0;
  };

  class Sounds {
  public:
    static void initialize();
    static void pause();
    static void resume();
    static void dispose();
    static void volumeHigher();
    static void volumeLower();
    static Sound& fromFile(const char* path);

  private:
    static int volume_;
    static void setVolume();
    Sounds();
  };

}

#endif
