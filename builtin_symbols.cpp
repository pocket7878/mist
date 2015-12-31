//
// Created by 十亀眞怜 on 2015/12/31.
//

#include "builtin_symbols.h"
#include "LObj.h"
#include <map>

map<string, lptr> builtin_symbols;

void initBuiltinSymbols() {
    builtin_symbols["NIL"] = make_shared<Nil>();
}

lptr builtin_symbol(string name) {
    map<string, lptr>::iterator iter = builtin_symbols.find(name);
    if(iter != builtin_symbols.end()) {
        return iter->second;
    } else {
        return nullptr;
    }
}