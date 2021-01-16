
#include <iostream>
#include <string>
#include <memory>

#include "../parserXML/pub/smartBuffer.h"
#include "../parserXML/pub/lexerXML.h"
#include "../parserXML/pub/parserXML.h"
#include "../parserXML/pub/exceptionParserXML.h"
#include "../parserXML/pub/symbolTableXML.h"

using std::cout;
using std::cin;
using std::endl;

// MACROS FUNCTIONS
#define PRINT(p) std::cout << #p << " => " << p

void test_smartBuffer() {
	try {
		parserXML::SmartBuffer buf("d:\\project\\PROJECT\\resourses\\NTX1000_original.xml");
		std::ofstream fout("d:\\project\\PROJECT\\resourses\\test.xml");

		parserXML::SmartBuffer::IteratorSmartB begLexem = buf.begin();
		parserXML::SmartBuffer::IteratorSmartB endLexem(begLexem);
		//for(int i = 0; i < 22; i++)
		while(*endLexem != std::char_traits<char>::eof()) {
			++endLexem;
		}
		std::string str(begLexem, endLexem);
		fout << str;

		cout << endl;
	} catch (parserXML::SmartBufferException& exc) {
		std::cout << exc.what() << std::endl;
	}

}

void test_lexer() {
	std::ofstream foutLexer("d:\\project\\PROJECT\\resourses\\testLexer.xml");

	parserXML::SmartBuffer buf;
	parserXML::SmartBuffer::IteratorSmartB iter(buf.begin());
	parserXML::SmartBuffer::IteratorSmartB iter1;
	iter1 = iter;
	
	parserXML::SymbolTableXML *table = new parserXML::SymbolTableXML;
	parserXML::LexerXML lexer;
	lexer.init("d:\\project\\PROJECT\\resourses\\NTX1000_original.xml", table);
	while(lexer.getNextToken().getTokenType() != parserXML::TokenXML::TokenType::END_OF_FILE)
	{
		foutLexer << table->getTokenLexemVal(lexer.getCurToken()) << endl;
	}
	foutLexer << "============ NEXT FILE ==============\n";
	cout << "SIZE: " << table->m_StoreNameID.size() << endl;
	lexer.reset();
	lexer.init("d:\\project\\PROJECT\\resourses\\NTX1000_original.xml", table);
	while(lexer.getNextToken().getTokenType() != parserXML::TokenXML::TokenType::END_OF_FILE)
	{
		foutLexer << table->getTokenLexemVal(lexer.getCurToken()) << endl;
	}
	//std::cin.get();
}

void test_parserXML() {

	std::ofstream fout("d:\\project\\PROJECT\\resourses\\testParser.xml");

	parserXML::ParserXML parser("d:\\project\\PROJECT\\resourses\\NTX1000_original.xml");
	parser.parse() ? cout << "parse is valid\n" : cout << "parse is invalid\n";
	if(parser.m_TreeElements){
		PRINT(parser.m_TreeElements->m_NameID) << std::endl;
		PRINT(parser.m_TreeElements->m_TextID) << std::endl;
		PRINT(parser.m_TreeElements->m_ListAttrib.size()) << std::endl;
		PRINT(parser.m_TreeElements->m_ListElement.size()) << std::endl;	
	}
}

int main(int argc, char* argv[]) {

	//test_smartBuffer();
	//test_lexer();
	try{
		test_parserXML();
	} catch(parserXML::ExceptionParserXML &except) {
		cout << except.what() << endl;
	}

	//std::cin.get();
	return 0;
}



/* notes
	g++ -c -Ilib/bub lib/lib.cpp -o build/objects/lib.o
	g++ -c -Ilib/bub runtime/main.cpp -o build/objects/main.o
	g++ build/objects/lib.o build/objects/main.o -o proj.exe

	creat prog whith link libraries
	#g++ main.o -Ijohnpaul/pub johnpaul/objects/libjhonpaul.a -o prog
*/

