#include <map>
#include "Env.h"
#include "eval.h"
#include "print.h"
#include "builtin.h"

map<string, LFUNC> builtins;

lptr setq(lptr sexp, eptr env) {
  if(!sexp->getCdr()->isAtom()) {
    auto next = sexp->getCdr()->getCar();
    auto varname = next->getName();
    auto value = eval(sexp->getCdr()->getCdr()->getCar(), env);
    env->bind(varname, value);
    return value;
  }
  return  make_shared<Nil>();
}

lptr add(lptr sexp, eptr env) {
  int acc = 0;
  for(lptr cell = sexp->getCdr(); typeid(*cell) != typeid(Nil); cell = cell->getCdr()) {
    lptr res = eval(cell->getCar(), env);
    if(typeid(*res) == typeid(Number)) {
      acc += res->getNumber();
    }
  }
  return make_shared<Number>(acc);
}

lptr min(lptr sexp, eptr env) {
  int first = sexp->getCdr()->getCar()->getNumber();
  for(lptr cell = sexp->getCdr()->getCdr(); typeid(*cell) != typeid(Nil); cell = cell->getCdr()) {
    lptr res = eval(cell->getCar(), env);
    if(typeid(*res) == typeid(Number)) {
      first -= res->getNumber();
    }
  }
  return make_shared<Number>(first);
}

lptr mul(lptr sexp, eptr env) {
  int acc = 1;
  for(lptr cell = sexp->getCdr(); typeid(*cell) != typeid(Nil); cell = cell->getCdr()) {
    lptr res = eval(cell->getCar(), env);
    if(typeid(*res) == typeid(Number)) {
      acc *= res->getNumber();
    }
  }
  return make_shared<Number>(acc);
}

lptr div(lptr sexp, eptr env) {
  int first = sexp->getCdr()->getCar()->getNumber();
  for(lptr cell = sexp->getCdr()->getCdr(); typeid(*cell) != typeid(Nil); cell = cell->getCdr()) {
    lptr res = eval(cell->getCar(), env);
    if(typeid(*res) == typeid(Number)) {
      first /= res->getNumber();
    }
  }
  return make_shared<Number>(first);
}

lptr quit(lptr sexp, eptr env) {
  exit(0);
}

lptr lambda(lptr sexp, eptr env) {
  lptr args = sexp->getCdr()->getCar();
  shared_ptr< vector<string> > argnames = make_shared<vector<string> >();
  for(lptr cell = args; typeid(*cell) != typeid(Nil); cell = cell->getCdr()) {
    string name = cell->getCar()->getName();
    argnames->push_back(name);
  }
  lptr body = sexp->getCdr()->getCdr()->getCar();
  return make_shared<Closure>(argnames, body, env);
}

void initBuiltins() {
  builtins["setq"] = setq;
  builtins["+"] = add;
  builtins["-"] = min;
  builtins["*"] = mul;
  builtins["/"] = div;
  builtins["quit"] = quit;
  builtins["lambda"] = lambda;
}

LFUNC builtin(string name) {
  map<string, LFUNC>::iterator iter = builtins.find(name);
  if(iter != builtins.end()) {
    return iter->second;
  } else {
    return nullptr;
  }
}
