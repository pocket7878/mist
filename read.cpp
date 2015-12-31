#include <iostream>
#include <algorithm>
#include <string>
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

int peekNextChar(istream &in) {
  int c;
  in >> ws;
  c = in.peek();
  return c;
}

lptr readList(istream &in) {
  char c;
  //Skip first paren
  cout << "Skip first paren" << endl;
  in.get();
  c = peekNextChar(in);
  cout << "Next char is: " << c << endl;
  if(c == ')') {
    cout << "return nil" << endl;
    in.get();
    return make_shared<Nil>();
  } else {
    cout << "Start normal list" << endl;
    lptr top = make_shared<Cons>();
    lptr list = top;
    cout << "start read car" << endl;
    top->setCar(read(in));
    cout << "end read car(start read rest)" << endl;
    while(true) {
      c = peekNextChar(in);
      cout << "Next peeked char is: " << c << endl;
      if(c == ')')  {
        cout << "List end foudn." << endl;
        list->setCdr(make_shared<Nil>());
        in.get();
        break;
      } else if(c == '.') {
        cout << "Found dot end" << endl;
        in.get();
        cout << "Start read end" << endl;
        list->setCdr(read(in));
        cout << "Read End end" << endl;
        in >> skipws >> c;
        if(c != ')') {
          throw string("More than one object follows . in list.");
        }
        break;
      } else {
        cout << "Create new cell" << endl;
        lptr newCell = make_shared<Cons>();
        cout << "Read new cell car" << endl;
        newCell->setCar(read(in));
        cout << "Read new cell car end" << endl;
        list->setCdr(newCell);
        list = newCell;
      }
    }
    return top;
  }
}

lptr readQuote(istream &in) {
  //Skip '
  in.get();
  lptr top = make_shared<Cons>();
  lptr quote = top;
  quote->setCar(make_shared<Symbol>("QUOTE"));
  quote->setCdr(make_shared<Cons>());
  quote = quote->getCdr();
  quote->setCar(read(in));
  quote->setCdr(make_shared<Nil>());
  return top;
}

lptr readQuasiQuote(istream &in) {
  //Skip `
  in.get();
  lptr top = make_shared<Cons>();
  lptr quote = top;
  quote->setCar(make_shared<Symbol>("QUASIQUOTE"));
  quote->setCdr(make_shared<Cons>());
  quote = quote->getCdr();
  quote->setCar(read(in));
  quote->setCdr(make_shared<Nil>());
  return top;
}

lptr readUnQuote(istream &in) {
  //Skip `
  in.get();
  lptr top = make_shared<Cons>();
  lptr quote = top;
  quote->setCar(make_shared<Symbol>("UNQUOTE"));
  quote->setCdr(make_shared<Cons>());
  quote = quote->getCdr();
  quote->setCar(read(in));
  quote->setCdr(make_shared<Nil>());
  return top;
}

lptr readSpliceUnQuote(istream &in) {
  //Skip `
  in.get();
  lptr top = make_shared<Cons>();
  lptr quote = top;
  quote->setCar(make_shared<Symbol>("SPLICE-UNQUOTE"));
  quote->setCdr(make_shared<Cons>());
  quote = quote->getCdr();
  quote->setCar(read(in));
  quote->setCdr(make_shared<Nil>());
  return top;
}

lptr readNegNum(istream &in) {
  in.get();
  string acc = "";
  char c;
  while(true) {
    c = in.peek();
    if(isdigit(c)) {
      in.get();
      acc += c;
    } else 
      break;
  }
  int value = stoi(acc) * -1;
  return make_shared<Number>(value);
}

lptr readPosNum(istream &in) {
  char c;
  string acc = "";
  while(true) {
    c = in.peek();
    cout << "Next char is : " << c << endl;
    if(isdigit(c)) {
      in.get();
      acc += c;
      cout << "Current acc: " << acc << endl;
    } else 
      break;
  }
  int value = stoi(acc);
  cout << "Read pos num:: " << value << endl;
  return make_shared<Number>(value);
}

lptr readSymbol(istream &in) {
  char c;
  string acc = "";
  while(true) {
    c = in.peek();
    if(isalpha(c) || c == '+' || c == '-' || c == '*' || c == '%') {
      in.get();
      acc += c;
    }
    else
      break;
  }
  cout << "Symbol read: " << acc << endl;
  return make_shared<Symbol>(acc);
}

lptr read(istream &in) {
  /*
  string line;
  getline(in, line);
  list<string> tokens = tokenizer(line);
  return read_from(tokens);
  */
  char c;
  c = peekNextChar(in);
  if (c == '(') {
    cout << "Read list start" << endl;
    return readList(in);
  } else if(c == '\'') {
    return readQuote(in);
  } else if(c == '`') {
    return readQuasiQuote(in);
  } else if(c == ',') {
    in.get();
    if(in.peek() == '@') {
      return readSpliceUnQuote(in);
    } else {
      in.unget();
      return readUnQuote(in);
    }
  } else if(c == '-') {
    return readNegNum(in);
  } else if(isdigit(c)) {
    return readPosNum(in);
  } else if(c == '+') {
    in.get();
    char moreNext;
    moreNext = in.peek();
    if(isdigit(moreNext)) {
      return readPosNum(in);
    } else {
      in.unget();
      return readSymbol(in);
    }
  } else if(isalpha(c) || c == '+' || c == '-' || c == '*' || c == '%') {
    return readSymbol(in);
  } else {
    in.get();
    string msg = "Unexpected character: ";
    msg += c;
    throw msg;
  }
}
