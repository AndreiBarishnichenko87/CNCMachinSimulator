
#include <iostream>
#include <string>
#include <vector>
#include <list>
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
		std::ofstream fout("d:\\project\\PROJECT\\resourses\\test.xml");
		
		parserXML::SmartBuffer::IteratorSmartB begLexem = buf.begin();
		parserXML::SmartBuffer::IteratorSmartB endLexem(begLexem);
		for(int i = 0; i < 45; i++)
			++endLexem;
		
		//std::string str(begLexem, endLexem);
		//PRINT(str) << endl;
		while(begLexem != endLexem)
		{
			fout << *begLexem;
			++begLexem;
		}
			
		cout << endl;
	} 
	catch (parserXML::SmartBufferException &exc){
		std::cout << exc.what() << std::endl;
	}

}

int main(int argc, char* argv[])
{
	
	test_smartBuffer();

	return 0;
}

/* notes
	g++ -c -Ilib/bub lib/lib.cpp -o build/objects/lib.o
	g++ -c -Ilib/bub runtime/main.cpp -o build/objects/main.o
	g++ build/objects/lib.o build/objects/main.o -o proj.exe

	creat prog whith link libraries
	#g++ main.o -Ijohnpaul/pub johnpaul/objects/libjhonpaul.a -o prog
*/

