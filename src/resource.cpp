// -*- C++ -*-

#include <fstream>  /* std::ifstream */
#include <ios>  /* std::ios */
#include <iostream>  /* std::ios, std::cerr, std::endl */
#include <string>  /* std::string */

#if defined(STORAGE_PATH) && !defined(__WIN32__)
  #include <cstdlib>  /* std::exit, EXIT_FAILURE */
  #include <sys/types.h>
  #include <sys/stat.h>
  struct stat info_resource;
#endif

#include "resource.h"

using namespace mbostock;

const char* Resources::path() {
#ifdef __APPLE__
  return "Contents/Resources/";
#else
  #if defined(STORAGE_PATH) && !defined(__WIN32__)
    if( stat( "resources/", &info_resource ) == 0 )
      return "resources/";
    else if( stat( STORAGE_PATH, &info_resource ) == 0 )
      return STORAGE_PATH;
    else if( stat( "/usr/local/share/polly-b-gone/", &info_resource ) == 0 )
      return "/usr/local/share/polly-b-gone/";
    else
      std::cerr << "error: cannot find directory containing \"worlds.xml\"" << std::endl;
      std::exit (EXIT_FAILURE);
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
