#include <iostream>
#include "../lib/pub/lib.h"

int main(int argc, char* argv[])
{
	std::cout << "Hello World qwe" << std::endl;
  
  foo();
  
	std::cin.get();
	return 0;
}

/*
	g++ -c -Ilib/bub lib/lib.cpp -o build/objects/lib.o
	g++ -c -Ilib/bub runtime/main.cpp -o build/objects/main.o
	g++ build/objects/lib.o build/objects/main.o -o proj.exe
	
	creat prog whith link libraries
	#g++ main.o -Ijohnpaul/pub johnpaul/objects/libjhonpaul.a -o prog
*/

