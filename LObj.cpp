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
 * Symbol
 */

Symbol::Symbol(string name) {
  this->name = name;
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
Closure::Closure(shared_ptr< vector<string> > args, lptr body, eptr env): args(args), env(env), body(body) {}

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
  return this->args;
}

lptr Closure::getBody() {
  return this->body;
}

eptr Closure::getClosureEnv() {
  return this->env;
}


