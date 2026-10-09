// -*- C++ -*-

#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <string.h>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <mach-o/dyld.h>
#include <libgen.h>
#elif defined(__linux__)
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

static std::string resourcePath;

#ifdef __linux__
/* check if path exists and is a directory */
static inline bool check_resourcePath() {
  struct stat sb;
  return (stat(resourcePath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode));
}
#endif

static void initResourcePath() {
  if (!resourcePath.empty()) return;

#ifdef __APPLE__
  // Try to get the bundle resources path
  CFBundleRef mainBundle = CFBundleGetMainBundle();
  if (mainBundle) {
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    if (resourcesURL) {
      char path[PATH_MAX];
      if (CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8*)path, PATH_MAX)) {
        resourcePath = path;
        resourcePath += "/";
        CFRelease(resourcesURL);
        return;
      }
      CFRelease(resourcesURL);
    }
  }

  // Fallback: get executable path and look for resources relative to it
  char execPath[PATH_MAX];
  uint32_t size = sizeof(execPath);
  if (_NSGetExecutablePath(execPath, &size) == 0) {
    char* dir = dirname(execPath);
    resourcePath = dir;
    resourcePath += "/../Resources/";
    return;
  }

  // Last fallback
  resourcePath = "Contents/Resources/";
#elif defined(__linux__)
  /* get full executable path */
  char* execPath = realpath("/proc/self/exe", NULL);
  if (execPath) {
    char* dir = dirname(execPath);
    std::string path = dir;
    free(execPath);

    /* check for LSB directory structure and resources inside <prefix>/share/ */
    char* copy = strdup(path.c_str());
    char* parent = basename(copy);
    if (strcmp(parent, "bin") == 0 || path == "/usr/local/games" || path == "/usr/games") {
      resourcePath = path;
      resourcePath += "/../share/polly-b-gone/";
      free(copy);
      if (check_resourcePath()) return;
    }
    free(copy);

    /* check for resources next to executable */
    resourcePath = path;
    resourcePath += "/resources/";
    if (check_resourcePath()) return;
  }

  /* Fallback */
  resourcePath = "resources/";
#elif defined(_WIN32)
  char execPath[MY_MAX_PATH];
  char* ptr;

  /* get full executable path */
  DWORD dwRet = GetModuleFileNameA(NULL, execPath, sizeof(execPath));
  if (dwRet > 0 && dwRet < sizeof(execPath) && (ptr = strrchr(execPath, '\\')) != NULL) {
    /* executable directory */
    *ptr = 0;
    resourcePath = execPath;
    resourcePath += "\\resources\\";
    return;
  }

  /* Fallback */
  resourcePath = ".\\resources\\";
#else /* other platforms */
  resourcePath = "resources/";
#endif
}

const char* Resources::path() {
  initResourcePath();
  return resourcePath.c_str();
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
