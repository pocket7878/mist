#ifndef __ENV_H__
#define __ENV_H__

#include <map>
#include "Typedef.h"

using namespace std;

class Env {
  private:
    shared_ptr<Env> parent;
    map<string, lptr> binding;
  public:
    Env(shared_ptr<Env> parent);
    void bind(string name, lptr value);
    void unbind(string name, lptr value);
    lptr lookup(string name);
};

#endif
