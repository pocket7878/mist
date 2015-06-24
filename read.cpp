#include <iostream>
#include <list>
#include <cctype>
#include "read.h"
#include "LObj.h"

using namespace std;

list<string> tokenizer(string &str) {
  list<string> tks;
  const char *s = str.c_str();
  while(*s) {
    while(*s == ' ') {
      ++s;
    }
    if (*s == '(' || *s == ')') 
      tks.push_back(*s++ == '(' ? "(" : ")");
    else {
      const char *t = s;
      while(*t && *t != ' ' && *t != '(' && *t != ')')
        ++t;
      tks.push_back(string(s, t));
      s = t;
    }
  }
  return tks;
}

lptr read_atom(string &token) {
  if(isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
    return shared_ptr<Number>(new Number(stoi(token)));
  } else {
    return shared_ptr<Symbol>(new Symbol(token));
  }
}

lptr read_from(list<string> &tokens) {
  string token = tokens.front();
  tokens.pop_front();
  if (token == "(") {
    //List start
    if(tokens.front() == ")") {
      //Empty list
      tokens.pop_front();
      return make_shared<Nil>();
    } else {
      shared_ptr<Cons> lst(make_shared<Cons>());
      shared_ptr<Cons> l = lst;
      l->setCar(read_from(tokens));
      while(tokens.front() != ")") {
        shared_ptr<Cons> newCell(make_shared<Cons>());
        newCell->setCar(read_from(tokens));
        l->setCdr(newCell);
        l = newCell;
      }
      tokens.pop_front();
      l->setCdr(make_shared<Nil>());
      return lst;
    }
  } else {
    return read_atom(token);
  }
  return nullptr;
}

lptr read(istream &in) {
  string line;
  getline(in, line);
  list<string> tokens = tokenizer(line);
  return read_from(tokens);
}
