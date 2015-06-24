#ifndef __LOBJ_H__
#define __LOBJ_H__

#include <iostream>
#include <vector>
#include "TypeDef.h"

using namespace std;

class LObj {
  public:
    //Common function
    virtual void printObj(ostream &os) { throw 1; };
    virtual bool isAtom() { throw 1; }
    virtual bool isSymbol() { throw 1; }

    //as cons
    virtual void setCar(shared_ptr<LObj> newCar) { throw 1; }
    virtual void setCdr(shared_ptr<LObj> newCdr) { throw 1; }
    virtual shared_ptr<LObj> getCar() { throw 1; }
    virtual shared_ptr<LObj> getCdr() { throw 1; }

    //as symbol
    virtual string getName() { throw 1; }

    //as number
    virtual int getNumber() { throw 1; }

    //as closure
    virtual shared_ptr< vector<string> > getArgNames() { throw 1; }
    virtual lptr getBody() { throw 1; }
    virtual eptr getClosureEnv() { throw 1; }
};

class Nil: public LObj {
  public:
    Nil();
    void printObj(ostream &os);
    bool isAtom();
    bool isSymbol();
};

class Cons: public LObj {
  private:
    lptr car;
    lptr cdr;
  public:
    Cons(lptr car, lptr cdr);
    Cons();

    void printObj(ostream &os);
    bool isAtom();
    bool isSymbol();
    lptr getCar();
    lptr getCdr();
    void setCar(lptr newCar);
    void setCdr(lptr newCdr);
};


class Number: public LObj {
  private:
    int value;
  public:
    Number(int value);
    int getNumber();
    void printObj(ostream &os);
    bool isAtom();
    bool isSymbol();
};

class String: public LObj {
  private:
    string value;
  public:
    String(string s);
    bool isAtom();
    bool isSymbol();
};

class Symbol: public LObj {
  private:
    string name;
  public:
    Symbol(string name);
    string getName();
    bool isAtom();
    bool isSymbol();
    void printObj(ostream &os);
};

class Closure: public LObj {
  private:
    eptr env;
    shared_ptr< vector<string> > args;
    lptr body;
  public:
    Closure(shared_ptr< vector<string> >, lptr, eptr);
    bool isAtom();
    bool isSymbol();
    void printObj(ostream &os);
    shared_ptr<vector<string> > getArgNames();
    lptr getBody();
    eptr getClosureEnv();
};

#endif
