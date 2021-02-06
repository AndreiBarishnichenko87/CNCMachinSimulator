#ifndef LEXERXML_H_
#define LEXERXML_H_

#include <memory>
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
			START_FILE,
			DOCTYPE						 // <!DOCTYPE
		};
		static const char* smTokenEnumName[];
		static const char* smTokenLexem[];
		
		TokenType m_Type = TokenType::UNDEFINE_TOKEN;
		std::string m_Lexem;
	};
	
	std::string token_enum_name(TokenXML token); 
	std::string token_lexem_name(TokenXML token); 

	class LexerXML {
		public:
			using token_t = TokenXML::TokenType;
		private:
			TokenXML m_PreviousToken;
			SmartBuffer m_Buffer;
			SmartBuffer::IteratorSmartB m_LexemBegin;
			SmartBuffer::IteratorSmartB m_Forward;
			size_t m_CountNewLine;
		
		private:
			void setToken(TokenXML& token, token_t tok_type);
			void setToken(TokenXML& token, token_t tok_type, const std::string &lexemVal);
			bool defTextToken(TokenXML& token);
			void skipSymbolUntil(char stopSymbol);
			std::string replacePredefXMLEntity(const SmartBuffer::IteratorSmartB &begin, const SmartBuffer::IteratorSmartB &end);
		
			LexerXML(const LexerXML& lexerXML) = delete;
			LexerXML& operator=(const LexerXML& lexerXML) = delete;
		public:
			LexerXML();
			explicit LexerXML(const std::string &fileName);
			bool init(const std::string &fileName);
			void reset();
			bool isInit() const;
			TokenXML getNextToken();
			TokenXML getCurToken();
			size_t getLinePositionToken() { return m_CountNewLine; }
	};
}

#endif // LEXERXML_H_