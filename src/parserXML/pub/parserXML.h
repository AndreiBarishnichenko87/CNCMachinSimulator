#ifndef PARSERXML_H_
#define PARSERXML_H_

#include <string>
#include "lexerXML.h"

namespace parserXML{
	
	class ParserXML
	{
		using type_token = TokenXML::token_XML;
	private:
		LexerXML m_Lexer;
	
	private:
		void element();
		void openElement();
		void attribute();
		void closeElement();
		void endElement();
		void error() { std::cout << "error: " << m_Lexer.getCurToken().mLexemValue << std::endl; }
	public:
		ParserXML(const std::string& fileRead);
		void findElement();
	};
	
}

#endif //PARSERXML_H_