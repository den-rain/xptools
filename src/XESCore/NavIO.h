#ifndef NAVIO_H
#define NAVIO_H

struct XAtomContainer;

#include "NavDefs.h"
#include <set>

#define LATEST_NAV_VERSION 1100

string ReadNavFile(const char * inFileName, NavData_t & outNavData);
string ReadNavFileMem(const char * inBegin, const char * inEnd, NavData_t & outNavData);

#endif //NAVIO_H
