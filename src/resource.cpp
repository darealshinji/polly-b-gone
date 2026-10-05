// -*- C++ -*-

#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <string.h>

#ifdef __linux__
#include <libgen.h>
#include <stdlib.h>
#include <sys/stat.h>
#elif defined(_WIN32)
#include <windows.h>
/* https://learn.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation */
#define MY_MAX_PATH (32*1024)
//#define MY_MAX_PATH MAX_PATH
#endif

#include "resource.h"

using namespace mbostock;


#if defined(__linux__) || defined(_WIN32)
static std::string res; /* cached resources path */
#endif


#ifdef __linux__
static inline std::string get_basename(const std::string &path) {
  std::string copy = path;
  return basename(std::data(copy));
}

static inline std::string get_dirname(const std::string &path) {
  std::string copy = path;
  return dirname(std::data(copy));
}

static inline std::string get_exe_dir() {
  char* exe = realpath("/proc/self/exe", NULL);
  if (!exe) return "";
  std::string dir = get_dirname(exe);
  free(exe);
  return dir;
}

/* check if path exists and is a directory */
static inline bool check_path(const std::string &path) {
  struct stat sb;
  return (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode));
}
#endif


const char* Resources::path() {
#ifdef __linux__

  if (!res.empty()) {
    return res.c_str();
  }

  /* get full executable path */
  std::string dir = get_exe_dir();

  if (!dir.empty()) {
    /* LSB directory structure */
    if (get_basename(dir) == "bin" || dir == "/usr/local/games" || dir == "/usr/games") {
      res = get_dirname(dir); /* parent directory */
      res += "/share/polly-b-gone/";

      if (check_path(res)) {
        return res.c_str();
      }
    }

    /* executable directory */
    res = dir + "/resources/";

    if (check_path(res)) {
      return res.c_str();
    }
  }

  /* current directory (no check) */
  res = "resources/";
  return res.c_str();

#elif defined(_WIN32)

  if (!res.empty()) {
    return res.c_str();
  }

  char buf[MY_MAX_PATH];
  char* p;
  DWORD dwRet = GetModuleFileNameA(NULL, buf, sizeof(buf));

  if (dwRet > 0 && dwRet < sizeof(buf) && (p = strrchr(buf, '\\')) != NULL) {
    /* executable directory */
    *(p+1) = 0;
    res = buf;
  }

  res += "resources\\";
  return res.c_str();

#elif defined(__APPLE__)
  return "Contents/Resources/";
#else
  return "resources/";
#endif
}

char* Resources::readFile(const char* p) {
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
