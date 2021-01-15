#ifndef SYMBOLTABLEXML_H_
#define SYMBOLTABLEXML_H_

#include <vector>
#include <string>

#include "smartBuffer.h"

namespace parserXML {
	
	struct TokenXML {
		enum TokenType {
			UNDEFINE_TOKEN = 0, 
			OPEN_TAG,          // <
			OPEN_CLOSE_TAG,    // </
			OPEN_PROLOG_TAG,   // <?
			FINAL_CLOSE_TAG,   // />
			CLOSE_TAG,         //	>
			EQUAL,             //	=
			CLOSE_PROLOG_TAG,  // ?>
			TEXT,              // text
			NAME_ID,           //	nameID
			ATRIBUTE_VALUE,    // attribValue
			END_OF_FILE,       // EOF
			OPEN_COMENT_TAG,   // <!--
			CLOSE_COMENT_TAG,  // -->
			CDATA_BEGIN,       // <![CDATA[
			CDATA_END,         //	]]>
			START_FILE
		};
		static const char* smTokenEnumName[];
		static const char* smTokenLexem[];
		// Members
		TokenType mTokenType = TokenType::UNDEFINE_TOKEN;
		size_t mLexemPos = 0;
	};
	
	std::string token_enum_name(TokenXML token);
	
	class SymbolTableXML {
	private:
		using token_t = TokenXML::TokenType;
		using iterToken = SmartBuffer::IteratorSmartB;
	public:
		std::vector<std::string> m_StoreNameID;
		std::vector<std::string> m_StoreAttribValue;
		std::vector<std::string> m_StoreText;
	public:
		size_t  addNewLexemVal(TokenXML token, const iterToken& beginLexem, const iterToken& endLexem);
		size_t  addNewLexemVal(TokenXML token, const std::string &lexemString);
		std::string getTokenLexemVal(TokenXML token) const;
		//std::string	getNameID(size_t pos) const;
		//std::string	getAttribVal(size_t pos) const;
		//std::string	getTextVal(size_t pos) const;
	};
	
}

#endif // SYMBOLTABLEXML_H_