#include <iostream>

using namespace std;

class A {
  public:
    A() {}
};

int main() {
  shared_ptr<A> a = shared_ptr<A>(new A());
  cout << typeid(a).name() << endl;
  if (typeid(a) == typeid(A)) {
    cout << "Same" << endl;
  }
  if (typeid(*a.get()) == typeid(A)) {
    cout << "Same" << endl;
  }
  if (typeid(*a) == typeid(A)) {
    cout << "Same 2" << endl;
  }
}
