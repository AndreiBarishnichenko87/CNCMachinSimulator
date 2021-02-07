
#include <iostream>
#include <string>
#include <memory>

#include "../parserXML/pub/smartBuffer.h"
#include "../parserXML/pub/lexerXML.h"
#include "../parserXML/pub/parserXML.h"
#include "../parserXML/pub/exceptionParserXML.h"

using std::cout;
using std::cin;
using std::endl;

// MACROS FUNCTIONS
#define PRINT(p) std::cout << #p << " => " << p
#define PAUSE_MSG(msg) std::cout << msg << endl; std::cin.get()

void printAllDoc(parserXML::ElementXML element, std::ostream &fout, unsigned int depthTree) {
	
	for(int i = 0; i < depthTree; ++i) fout << " ";
	fout << "<" << element.elementName() << ">";
	if(element.valueExist())
		fout << " [" << element.elementValue() << "]" << endl;
	else
		fout << endl;
	if(element.countAttribute() != 0) {
		for(unsigned int j = 0; j < element.countAttribute(); ++j) {
			for(int i = 0; i < depthTree + 1; ++i) fout << " ";
			fout << element.attribNameNum(j) << " = [" << element.attribValueNum(j) << "]" << endl;
		}
	}
	
	++depthTree;
	if(element.countChildElements() != 0) {
		for(unsigned int i = 0; i < element.countChildElements(); ++i) {
			printAllDoc(element.childElementNum(i), fout, depthTree);
		}
	}
}

void test_parserXML() {

	std::ofstream fout("e:\\project\\PROJECT\\resourses\\testParser.xml");
	parserXML::ParserXML parser("e:\\project\\PROJECT\\resourses\\langs.model.xml");
	parser.parse();
	printAllDoc(parser.getRootElement(), fout, 0);
}

int main(int argc, char* argv[]) {

	try {
		test_parserXML();
	} catch(parserXML::ExceptionParserXML &except) {
		cout << except.what() << endl;
	}

	return 0;
}



/* notes
	g++ -c -Ilib/bub lib/lib.cpp -o build/objects/lib.o
	g++ -c -Ilib/bub runtime/main.cpp -o build/objects/main.o
	g++ build/objects/lib.o build/objects/main.o -o proj.exe

	creat prog whith link libraries
	#g++ main.o -Ijohnpaul/pub johnpaul/objects/libjhonpaul.a -o prog
*/

