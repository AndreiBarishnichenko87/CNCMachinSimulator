
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

void test_lexerXML() {
	std::ofstream fout("e:\\project\\PROJECT\\resourses\\testLexer.xml");
	parserXML::LexerXML lexer("e:\\project\\PROJECT\\resourses\\NTX1000_original.xml");
	while(lexer.getNextToken().m_Type != parserXML::TokenXML::TokenType::END_OF_FILE)
		fout << lexer.getCurToken().m_Lexem << endl;
}

void test_parserXML() {

	std::ofstream fout("e:\\project\\PROJECT\\resourses\\testParser.xml");
	parserXML::ParserXML parser("e:\\project\\PROJECT\\resourses\\NTX1000_original.xml");
	parser.parse();
	parserXML::ElementXML tree = parser.getRootElement();
	PRINT(tree.elementName()) << endl;
	PRINT(tree.getCountChildElements()) << endl;
	PRINT(tree.getChildElement(29).getCountChildElements()) << endl;
	PRINT(tree.getChildElement(29).getChildElement(22).getCountChildElements()) << endl;

}

int main(int argc, char* argv[]) {

	try {
		//test_lexerXML();
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

