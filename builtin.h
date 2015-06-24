#ifndef __BUILTIN_H__
#define __BUILTIN_H__

#include <map>
#include "LObj.h"
#include "Env.h"
#include "eval.h"

using namespace std;

typedef lptr (*LFUNC)(lptr, eptr);

void initBuiltins();
LFUNC builtin(string name);

#endif
