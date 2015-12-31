//
// Created by 十亀眞怜 on 2015/12/31.
//

#ifndef MYLISP_BUILTIN_SYMBOLS_H
#define MYLISP_BUILTIN_SYMBOLS_H

#include <string>
#include "TypeDef.h"

using namespace std;

void initBuiltinSymbols();
lptr builtin_symbol(string name);

#endif //MYLISP_BUILTIN_SYMBOLS_H
