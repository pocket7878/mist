#include <iostream>
#include "LObj.h"
#include "print.h"

using namespace std;

void print(lptr obj, ostream &os) {
  if(obj == nullptr) {
    os << "" << endl;;
  } else {
    obj->printObj(os);
    os << endl;
  }
}
