#include <vector>
#include "Env.h"
#include "eval.h"
#include "print.h"
#include "builtin.h"

lptr eval(lptr sexp, shared_ptr<Env> env) {
  if(sexp->isSymbol()) {
    string name = sexp->getName();
    return env->lookup(name);
  } else if(sexp->isAtom()) {
    return sexp;
  } else {
    if(sexp->getCar()->isSymbol()) {
      string name = sexp->getCar()->getName();
      LFUNC fn = builtin(name);
      if(fn != nullptr) {
        return fn(sexp, env);
      } else {
        lptr head = eval(sexp->getCar(), env);
        if(typeid(*head) == typeid(Closure)) {
          //Lambda!!
          //Eval every arguments
          vector<lptr> actural_args;
          lptr args = sexp->getCdr();
          for(lptr arg = args; typeid(*arg) != typeid(Nil); arg = arg->getCdr()) {
            lptr val = eval(arg->getCar(), env);
            actural_args.push_back(val);
          }
          shared_ptr<vector<string> > formal_args = head->getArgNames();
          shared_ptr<Env> newEnv = shared_ptr<Env>(new Env(head->getClosureEnv()));
          for(int i = 0; i < formal_args->size(); i++) {
            newEnv->bind(formal_args->at(i), actural_args[i]);
          }
          return eval(head->getBody(), newEnv);
        }
      }
    }
  }
  return nullptr;
}
