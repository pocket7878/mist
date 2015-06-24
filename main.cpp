#include <iostream>
#include <list>
#include <map>
#include <cctype>
#include "LObj.h"
#include "Env.h"
#include "read.h"
#include "print.h"
#include "eval.h"
#include "builtin.h"

using namespace std;

int main() {
  shared_ptr<Env> env = shared_ptr<Env>(new Env(nullptr));
  initBuiltins();
  for(;;) {
    cout << "Lisp>";
    print(eval(read(cin), env), cout);
  }
  cout << endl;
}
