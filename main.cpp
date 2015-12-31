#include <iostream>
#include <list>
#include <map>
#include <cctype>
#include "LObj.h"
#include "Env.h"
#include "read.h"
#include "print.h"
#include "eval.h"
#include "builtin_func.h"
#include "builtin_symbols.h"

using namespace std;

int main() {
  shared_ptr<Env> env = shared_ptr<Env>(new Env(nullptr));
  initBuiltinFunc();
  initBuiltinSymbols();
  for(;;) {
    cout << "Lisp>";
    try {
      print(eval(read(cin), env), cout);
    } catch(string str) {
      cerr << str << endl;
    }
  }
  cout << endl;
}
