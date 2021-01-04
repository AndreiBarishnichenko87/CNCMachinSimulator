
#include <iostream>
#include <string>
#include <vector>
#include "../parserXML/pub/smartBuffer.h"

using std::cout;
using std::cin;
using std::endl;

// MACROS FUNCTIONS
#define PRINT(p) std::cout << #p << " => " << p

void test_smartBuffer()
{
	try {
		parserXML::SmartBuffer buf("d:\\project\\PROJECT\\resourses\\NTX1000_original.xml");
		parserXML::SmartBuffer::IteratorSmartB iter = buf.begin();
		
		std::ofstream fout("d:\\project\\PROJECT\\resourses\\test.xml");
		
		while(*iter != std::char_traits<char>::eof())
		{
			fout << *iter;
			++iter;
		}

		cout << endl;
		PRINT(buf.getSizeChunk()) << endl;
		PRINT(buf.getCountChunk()) << endl;
		PRINT(buf.getChunksUsers()) << endl;
		
	} 
	catch (parserXML::SmartBufferException &exc){
		std::cout << exc.what() << std::endl;
	}

}

void test_vector(){
	
	std::vector<int> intList;
	for(int i = 0; i < 10; i++)
	{
		intList.push_back(i);
	}
	std::vector<int>::iterator iter = intList.begin();
	for(int i = 0; i < 8; i++)
	{
		cout << *iter << " : " << &(*iter) << endl;
		++iter;
	}
	
}

int main(int argc, char* argv[])
{

	test_smartBuffer();
	//test_vector();

	
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

