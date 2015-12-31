#ifndef __LOBJ_H__
#define __LOBJ_H__

#include <iostream>
#include <vector>
#include <set>
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

    //as vector
    virtual lptr getItemAt(int i) { throw 1; }
    virtual void appendItem(lptr obj) { throw 1; }

    //as closure
    virtual shared_ptr< vector<string> > getArgNames() { throw 1; }
    virtual shared_ptr< string > getRestArgName() { throw 1; }
    virtual lptr getBody() { throw 1; }
    virtual eptr getClosureEnv() { throw 1; }

    //as macro
    virtual shared_ptr< vector<string> > getMacroArgNames() { throw 1; }
    virtual shared_ptr< string > getMacroRestArgName() { throw 1; }
    virtual lptr getMacroBody() { throw 1; }
};

class T: public LObj {
  public: 
    T();
    void printObj(ostream &os);
    bool isAtom();
    bool isSymbol();
};

class Nil: public LObj {
  public:
    Nil();
    void printObj(ostream &os);
    bool isAtom();
    bool isSymbol();
    lptr getCar();
    lptr getCdr();
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

class Vector: public LObj {
  private: 
    vector<lptr> values;
  public:
    Vector(vector<lptr> values);
    bool isAtom();
    bool isSymbol();
    void printObj(ostream &os);
    lptr getItemAt(int i);
    void appendItem(lptr obj);
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
    shared_ptr< vector<string> > normal_args;
    shared_ptr< string > rest_arg;
    lptr body;
  public:
    Closure(shared_ptr< vector<string> >, shared_ptr< string >, lptr, eptr);
    bool isAtom();
    bool isSymbol();
    void printObj(ostream &os);
    shared_ptr<vector<string> > getArgNames();
    shared_ptr<string> getRestArgName();
    lptr getBody();
    eptr getClosureEnv();
};

class Macro: public LObj {
  private:
    shared_ptr< vector<string> > normal_args;
    shared_ptr< string > rest_arg;
    lptr body;
  public:
    Macro(shared_ptr< vector<string> >, shared_ptr<string>, lptr);
    bool isAtom();
    bool isSymbol();
    void printObj(ostream &os);
    shared_ptr<vector<string> > getMacroArgNames();
    shared_ptr<string> getMacroRestArgName();
    lptr getMacroBody();
};

#endif
