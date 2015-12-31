#include <map>
#include "Env.h"
#include "eval.h"
#include "print.h"
#include "builtin_func.h"

map<string, LFUNC> builtin_funcs;

lptr land(lptr args, eptr env) {
  lptr last;
  for(lptr cell = args; typeid(*cell) != typeid(Nil); cell = cell->getCdr()) {
    last = eval(cell->getCar(), env);
    if(typeid(*last) == typeid(Nil)) {
      return last;
    }
  }
  return last;
}

lptr lor(lptr args, eptr env) {
  lptr last;
  for(lptr cell = args; typeid(*cell) != typeid(Nil); cell = cell->getCdr()) {
    last = eval(cell->getCar(), env);
    if(typeid(*last) != typeid(Nil)) {
      return last;
    } else {
      return last;
    }
  }
  return last;
}

lptr lnot(lptr args, eptr env) {
  lptr last = eval(args->getCar(), env);
  if(typeid(*last) == typeid(Nil)) {
    return make_shared<T>();
  } else {
    return make_shared<Nil>();
  }
}

lptr lif(lptr args, eptr env) {
  lptr test = args->getCar();
  lptr then = args->getCdr()->getCar();
  lptr testRes = eval(test, env);
  if(typeid(*testRes) != typeid(Nil)) {
    return eval(then, env);
  } else {
    if(typeid(*args->getCdr()->getCdr()->getCar()) != typeid(Nil)) {
      return eval(args->getCdr()->getCdr()->getCar(), env);
    } else {
      return make_shared<Nil>();
    }
  }
}

lptr cons_impl(lptr car, lptr cdr, eptr env) {
  lptr cell = make_shared<Cons>();
  cell->setCar(car);
  cell->setCdr(cdr);
  return cell;
}

lptr cons(lptr args, eptr env) {
  lptr left = eval(args->getCar(), env);
  lptr right = eval(args->getCdr()->getCar(), env);
  return cons_impl(left, right, env);
}

lptr append_impl(lptr left, lptr right, eptr env) {
  if(typeid(*left) == typeid(Nil)) {
    return right;
  } else {
    lptr result;
    result = cons_impl(left->getCar(), append_impl(left->getCdr(), right, env), env);
    return result;
  }
}

lptr append(lptr args, eptr env) {
  lptr left = eval(args->getCar(), env);
  lptr right = eval(args->getCdr()->getCar(), env);
  return append_impl(left, right, env);
}

lptr setq(lptr arg, eptr env) {
  if(!arg->isAtom()) {
    auto varname = arg->getCar()->getName();
    auto value = eval(arg->getCdr()->getCar(), env);
    env->bind(varname, value);
    return value;
  }
  return  make_shared<Nil>();
}

lptr add(lptr args, eptr env) {
  int acc = 0;
  for(lptr cell = args; typeid(*cell) != typeid(Nil); cell = cell->getCdr()) {
    lptr res = eval(cell->getCar(), env);
    if(typeid(*res) == typeid(Number)) {
      acc += res->getNumber();
    }
  }
  return make_shared<Number>(acc);
}

lptr min(lptr args, eptr env) {
  int first = args->getCar()->getNumber();
  for(lptr cell = args->getCdr(); typeid(*cell) != typeid(Nil); cell = cell->getCdr()) {
    lptr res = eval(cell->getCar(), env);
    if(typeid(*res) == typeid(Number)) {
      first -= res->getNumber();
    }
  }
  return make_shared<Number>(first);
}

lptr mul(lptr args, eptr env) {
  int acc = 1;
  for(lptr cell = args; typeid(*cell) != typeid(Nil); cell = cell->getCdr()) {
    lptr res = eval(cell->getCar(), env);
    if(typeid(*res) == typeid(Number)) {
      acc *= res->getNumber();
    }
  }
  return make_shared<Number>(acc);
}

lptr div(lptr args, eptr env) {
  int first = args->getCar()->getNumber();
  for(lptr cell = args->getCdr(); typeid(*cell) != typeid(Nil); cell = cell->getCdr()) {
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

lptr lambda(lptr args, eptr env) {
  lptr arg = args->getCar();
  shared_ptr< vector<string> > argnames = make_shared<vector<string> >();
  shared_ptr< string > rest_arg = nullptr;
  if(arg->isSymbol()) {
    rest_arg = shared_ptr<string>(new string(arg->getName()));
  } else {
    lptr cell;
    for(cell = arg; typeid(*cell) != typeid(Nil) && !cell->isSymbol(); cell = cell->getCdr()) {
      string name = cell->getCar()->getName();
      cout << "Lambda normal argname is : " << name << endl;
      argnames->push_back(name);
    }
    if(cell->isSymbol()) {
      cout << "Lambda rest argname is : " << cell->getName() << endl;
      rest_arg = shared_ptr<string>(new string(cell->getName()));
    }
  }
  lptr body = args->getCdr();
  cout << "BOey is" << endl;
  print(body, cout);
  lptr progn = make_shared<Cons>();
  progn->setCar(make_shared<Symbol>("progn"));
  lptr prognBody = make_shared<Cons>();
  prognBody->setCar(body->getCar());
  prognBody->setCdr(body->getCdr());
  progn->setCdr(prognBody);
  cout << "Progned body is" << endl;
  print(progn, cout);
  return shared_ptr<Closure>(new Closure(argnames, rest_arg, progn, env));
}

lptr quote(lptr args, eptr env) {
  return args->getCar();
};

lptr progn(lptr args, eptr env) {
  cout << "Eval progn" << endl;
  print(args, cout);
  lptr res = make_shared<Nil>();
  for(lptr cell = args; typeid(*cell) != typeid(Nil); cell = cell->getCdr()) {
    res = eval(cell->getCar(), env);
  }
  cout << "Result is" << endl;
  print(res, cout);
  return res;
}

lptr defmacro(lptr args, eptr env) {
  string name = args->getCar()->getName();
  lptr arg = args->getCdr()->getCar();
  shared_ptr< vector<string> > argnames = make_shared<vector<string> >();
  shared_ptr< string > rest_arg = nullptr;
  if(arg->isSymbol()) {
    rest_arg = shared_ptr<string>(new string(arg->getName()));
  } else {
    lptr cell;
    for(cell = arg; typeid(*cell) != typeid(Nil) && !cell->isSymbol(); cell = cell->getCdr()) {
      string name = cell->getCar()->getName();
      argnames->push_back(name);
    }
    if(cell->isSymbol()) {
      rest_arg = shared_ptr<string>(new string(cell->getName()));
    } 
  }
  lptr body = args->getCdr()->getCdr();
  lptr progn = make_shared<Cons>();
  progn->setCar(make_shared<Symbol>("progn"));
  lptr prognBody = make_shared<Cons>();
  prognBody->setCar(body->getCar());
  prognBody->setCdr(body->getCdr());
  progn->setCdr(prognBody);
  lptr macro = make_shared<Macro>(argnames, rest_arg, progn);
  env->bind(name, macro);
  return make_shared<Nil>();
}

void initBuiltinFunc() {
  builtin_funcs["AND"] = land;
  builtin_funcs["OR"] = lor;
  builtin_funcs["NOT"] = lnot;
  builtin_funcs["IF"] = lif;
  builtin_funcs["SETQ"] = setq;
  builtin_funcs["+"] = add;
  builtin_funcs["-"] = min;
  builtin_funcs["*"] = mul;
  builtin_funcs["/"] = div;
  builtin_funcs["QUIT"] = quit;
  builtin_funcs["QUOTE"] = quote;
  builtin_funcs["LAMBDA"] = lambda;
  builtin_funcs["PROGN"] = progn;
  builtin_funcs["DEFMACRO"] = defmacro;
  builtin_funcs["CONS"] = cons;
  builtin_funcs["APPEND"] = append;
}

LFUNC builtin_func(string name) {
  map<string, LFUNC>::iterator iter = builtin_funcs.find(name);
  if(iter != builtin_funcs.end()) {
    return iter->second;
  } else {
    return nullptr;
  }
}
