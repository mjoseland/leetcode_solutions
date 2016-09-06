CPPFLAGS=-Wall -Wextra -Wc++11-extensions -std=c++11 -x c++

program:
	clang++ main.cpp $(CPPFLAGS) -o main 
