
#include <iostream>
#include <string>
#include <memory>

#include "../parserXML/pub/smartBuffer.h"
#include "../parserXML/pub/lexerXML.h"
#include "../parserXML/pub/parserXML.h"

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
	std::ofstream fout("d:\\project\\PROJECT\\resourses\\testLexer.xml");
	std::shared_ptr<parserXML::SmartBuffer> ptrBuf(new parserXML::SmartBuffer("d:\\project\\PROJECT\\resourses\\NTX1000_original.xml"));
	if(ptrBuf.use_count() != 1) {
		cout << "pointer is invalid" << endl;
		return;
	}
	parserXML::LexerXML lexer(ptrBuf);
	parserXML::TokenXML token;
	while(token.mTypeToken != parserXML::TokenXML::token_XML::END_OF_FILE) {
		token = lexer.getNextToken();
		fout << token.tokenName() << " : " << token.lexemValue() << endl;
	}
}

void test_parserXML() {

	std::ofstream fout("d:\\project\\PROJECT\\resourses\\testParser.xml");
	std::shared_ptr<parserXML::SmartBuffer> ptrBuf(new parserXML::SmartBuffer("d:\\project\\PROJECT\\resourses\\NTX1000_original.xml"));
	if(ptrBuf.use_count() != 1) {
		cout << "pointer is invalid" << endl;
		return;
	}
	
	parserXML::ParserXML parser("d:\\project\\PROJECT\\resourses\\NTX1000_original.xml");
	for(int i = 0; i < 10; i++){
		parser.findElement();
	}

}

int main(int argc, char* argv[]) {

	//test_smartBuffer();
	//test_lexer();
	test_parserXML();
	
	
	return 0;
}



/* notes
	g++ -c -Ilib/bub lib/lib.cpp -o build/objects/lib.o
	g++ -c -Ilib/bub runtime/main.cpp -o build/objects/main.o
	g++ build/objects/lib.o build/objects/main.o -o proj.exe

	creat prog whith link libraries
	#g++ main.o -Ijohnpaul/pub johnpaul/objects/libjhonpaul.a -o prog
*/

