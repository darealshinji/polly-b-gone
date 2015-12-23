// -*- C++ -*-

#include <fstream>
#include <ios>
#include <iostream>

#ifdef STORAGE_PATH
  #include <sys/types.h>
  #include <sys/stat.h>
#endif

#include "resource.h"

struct stat info;

using namespace mbostock;

const char* Resources::path() {
#ifdef __APPLE__
  return "Contents/Resources/";
#else
  #ifdef STORAGE_PATH
    // check if STORAGE_PATH exists
    // (i.e. "/usr/local/share/polly-b-gone/"),
    // otherwise fall back to "resources/
    if( stat( STORAGE_PATH, &info ) != 0 )
      return "resources/";
    else
      return STORAGE_PATH;
  #else
    return "resources/";
  #endif
#endif
}

const char* Resources::readFile(const char* p) {
  std::string fullPath(path());
  fullPath.append(p);
  std::ifstream file(fullPath.c_str());
  file.seekg(0, std::ios::end);
  std::ifstream::pos_type size = file.tellg();
  file.seekg(0, std::ios::beg);
  char* buffer = new char[1 + size];
  file.read(buffer, size);
  buffer[size] = '\0';
  file.close();
  return buffer;
}
