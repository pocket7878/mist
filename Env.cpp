#include <map>
#include <string>
#include "TypeDef.h"
#include "Env.h"

using namespace std;

Env::Env(shared_ptr<Env> parent): parent(parent) {}

void Env::bind(string name, lptr value) {
  this->binding[name] = value;
}

void Env::unbind(string name, lptr value) {
  this->binding.erase(name);
}

lptr Env::lookup(string name) {
  map<string, lptr>::iterator iter;
  iter = this->binding.find(name);
  if(iter != this->binding.end()) {
    return iter->second;
  }
  return nullptr;
}

