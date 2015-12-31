#include <vector>
#include "Env.h"
#include "eval.h"
#include "print.h"
#include "builtin_func.h"

lptr eval(lptr sexp, shared_ptr<Env> env) {
  if(sexp->isSymbol()) {
    string name = sexp->getName();
    return env->lookup(name);
  } else if(sexp->isAtom()) {
    return sexp;
  } else {
    if(sexp->getCar()->isSymbol()) {
      string name = sexp->getCar()->getName();
      LFUNC fn = builtin_func(name);
      if(fn != nullptr) {
        return fn(sexp->getCdr(), env);
      } else {
        lptr head = eval(sexp->getCar(), env);
        if(typeid(*head) == typeid(Closure)) {
          //Lambda or user defined function!!
          //Eval every arguments
          vector<lptr> actual_args;
          lptr args = sexp->getCdr();
          for(lptr arg = args; typeid(*arg) != typeid(Nil); arg = arg->getCdr()) {
            lptr val = eval(arg->getCar(), env);
            actual_args.push_back(val);
          }
          shared_ptr<vector<string> > formal_args = head->getArgNames();
          shared_ptr<Env> newEnv = shared_ptr<Env>(new Env(head->getClosureEnv()));
          int i;
          for(i = 0; i < formal_args->size(); i++) {
            newEnv->bind(formal_args->at(i), actual_args[i]);
          }
          if(head->getRestArgName() != nullptr) {
            lptr rarg = make_shared<Cons>();
            lptr cell = rarg;
            cout << "i is " << i << endl;
            cout << "Actual args size is: " << actual_args.size() << endl;
            for(; i < actual_args.size()-1; i++) {
              cout << "rest actual arg is" << endl;
              print(actual_args[i], cout);
              cell->setCar(actual_args[i]);
              lptr newCell = make_shared<Cons>();
              cell->setCdr(newCell);
              cell = newCell;
            }
            cell->setCar(actual_args[i]);
            cell->setCdr(make_shared<Nil>());
            cout << "Rest argments is" << endl;
            print(rarg, cout);
            newEnv->bind(*head->getRestArgName(), rarg);
          }
          return eval(head->getBody(), newEnv);
        } else if(typeid(*head) == typeid(Macro)) {
          //Evaluate macro
          vector<lptr> actual_args;
          lptr args = sexp->getCdr();
          for(lptr arg = args; typeid(*arg) != typeid(Nil); arg = arg->getCdr()) {
            lptr val = arg->getCar();
            actual_args.push_back(val);
          }
          shared_ptr<vector<string> > formal_args = head->getMacroArgNames();
          shared_ptr<Env> newEnv = shared_ptr<Env>(new Env(env));
          int i;
          for(i = 0; i < formal_args->size(); i++) {
            newEnv->bind(formal_args->at(i), actual_args[i]);
          }
          if(head->getMacroRestArgName() != nullptr) {
            lptr rarg = make_shared<Cons>();
            lptr cell = rarg;
            for(; i < actual_args.size()-1; i++) {
              cell->setCar(actual_args[i]);
              lptr newCell = make_shared<Cons>();
              cell->setCdr(newCell);
              cell = newCell;
            }
            cell->setCar(actual_args[i]);
            cell->setCdr(make_shared<Nil>());
            newEnv->bind(*head->getMacroRestArgName(), rarg);
          }
          lptr macroRes = eval(head->getMacroBody(), newEnv);
          return eval(macroRes, env);
        }
      }
    }
  }
  return nullptr;
}
