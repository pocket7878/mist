#ifndef __READ_H__
#define __READ_H__

#include <iostream>
#include "LObj.h"

using namespace std;

list<string> tokenizer(string &str);
lptr read_atom(string &token);
lptr read_from(list<string> &tokens);
lptr read(istream &in);

#endif
