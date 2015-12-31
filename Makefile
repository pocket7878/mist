TARGET=mylisp

all: $(TARGET)

$(TARGET): main.cpp LObj.cpp Env.cpp read.cpp print.cpp eval.cpp builtin.cpp
	clang++ -std=c++1y -g -o $@ $^

run: $(TARGET)
	./$(TARGET)
