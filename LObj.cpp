#include <iostream>
#include <list>
#include <map>
#include <cctype>
#include "LObj.h"
#include "TypeDef.h"

using namespace std;

/*
 * Cons
 */

Cons::Cons(lptr car, lptr cdr): car(car), cdr(cdr) {};
Cons::Cons(): car(nullptr), cdr(nullptr) {};

void Cons::printObj(ostream &os) {
  if(typeid(*this->car) == typeid(Nil) && typeid(*this->cdr) == typeid(Nil)) {
    os << "nil";
  } else {
    Cons *list = this;
    os << "(";
    for(;;) {
      if(typeid(*list->car) == typeid(Nil)) {
        os << "nil";
      } else {
        list->car->printObj(os);
      }
      if(typeid(*list->cdr) == typeid(Nil)) {
        os << ")";
        break;
      } else if(list->cdr->isAtom()) {
        os << " . ";
        list->cdr->printObj(os);
        os << ")";
        break;
      } else {
        os << " ";
        list = (Cons*)list->cdr.get();
      }
    }
  }
}

bool Cons::isAtom() {
  return false;
}
bool Cons::isSymbol() {
  return false;
}

lptr Cons::getCar() {
  return this->car;
}
lptr Cons::getCdr() {
  return this->cdr;
}

void Cons::setCar(lptr newCar) {
  this->car = newCar;
}

void Cons::setCdr(lptr newCdr) {
  this->cdr = newCdr;
}

/*
 * T
 */
T::T() {}
void T::printObj(ostream &os) {
  os << "t" << endl;
}

bool T::isAtom() {
  return true;
}

bool T::isSymbol() {
  return false;
}

/*
 * Nil
 */

Nil::Nil() {}
void Nil::printObj(ostream &os) {
  os << "()";
}
bool Nil::isAtom() {
  return true;
}
bool Nil::isSymbol() {
  return false;
}

lptr Nil::getCar() {
  return make_shared<Nil>();
}
lptr Nil::getCdr() {
  return make_shared<Nil>();
}

/*
 * Number
 */
Number::Number(int value) {
  this->value = value;
}
void Number::printObj(ostream &os) {
  os << this->value;
}

bool Number::isAtom() {
  return true;
}

bool Number::isSymbol() {
  return false;
}

int Number::getNumber() {
  return this->value;
}

/*
 * String
 */

String::String(string s) {
  this->value = s;
}
bool String::isAtom() {
  return true;
}
bool String::isSymbol() {
  return false;
}

/*
 * vector
 */
Vector::Vector(vector<lptr> obj): values(obj) {}

bool Vector::isAtom() {
  return false;
}

bool Vector::isSymbol() {
  return false;
}

void Vector::printObj(ostream &os) {
  os << "#(";
  for (int i = 0; i < this->values.size(); i++) {
    lptr obj = this->values[i];
    obj->printObj(os);
  }
  os << ")";
}

lptr Vector::getItemAt(int i) {
  return values[i];
}

void Vector::appendItem(lptr obj) {
  this->values.push_back(obj);
}

/*
 * Symbol
 */

Symbol::Symbol(const string name) {
  this->name = name;
  std::transform(this->name.begin(), this->name.end(), this->name.begin(), ::toupper);
}

string Symbol::getName() {
  return this->name;
}
bool Symbol::isAtom() {
  return true;
}
bool Symbol::isSymbol() {
  return true;
}
void Symbol::printObj(ostream &os) {
  os << name;
}

/*
 * Closure
 */
Closure::Closure(shared_ptr< vector<string> > args, shared_ptr<string> rarg, lptr body, eptr env):
  normal_args(args), rest_arg(rarg), env(env), body(body) {}

bool Closure::isAtom() {
  return false;
}

bool Closure::isSymbol() {
  return false;
}

void Closure::printObj(ostream &os) {
  os << "<closure>";
}

shared_ptr< vector<string> > Closure::getArgNames() {
  return this->normal_args;
}

shared_ptr< string > Closure::getRestArgName() {
  return this->rest_arg;
}

lptr Closure::getBody() {
  return this->body;
}

eptr Closure::getClosureEnv() {
  return this->env;
}

/*
 * Macro
 */
Macro::Macro(shared_ptr< vector<string> > args, shared_ptr<string> rarg, lptr body): normal_args(args), rest_arg(rarg), body(body) {}

bool Macro::isAtom() {
  return false;
}

bool Macro::isSymbol() {
  return false;
}

void Macro::printObj(ostream &os) {
  os << "<macro>";
}

shared_ptr<vector<string> > Macro::getMacroArgNames() {
  return this->normal_args;
}

shared_ptr< string > Macro::getMacroRestArgName() {
  return this->rest_arg;
}


lptr Macro::getMacroBody() {
  return this->body;
}
