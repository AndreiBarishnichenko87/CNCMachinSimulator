#ifndef SYMBOLTABLEXML_H_
#define SYMBOLTABLEXML_H_

#include <vector>
#include <string>

#include "smartBuffer.h"

namespace parserXML {
	
	class SymbolTableXML;
	
	class TokenXML {
	public:
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
	
	private:
		TokenType mTokenType = TokenType::UNDEFINE_TOKEN;
		size_t mLexemPos = 0;
	
	public:
		TokenXML() : mTokenType(TokenType::UNDEFINE_TOKEN), mLexemPos(TokenType::UNDEFINE_TOKEN) {}
		TokenXML(TokenType tokType, const std::string &lexemString, SymbolTableXML *symbTable);
		TokenXML(TokenType tokType) : mTokenType(tokType), mLexemPos(tokType) {}
	
	public:	
		inline TokenType getTokenType() const { return mTokenType; }
		inline size_t getTokenLexemVal() const { return mLexemPos; }
	
	public:		
		inline bool isTokenTypeEQ(TokenXML token) const { return mTokenType == token.mTokenType; }
		inline bool isTokenLexemValEQ(TokenXML token) const {return mLexemPos == token.mLexemPos; }
		bool operator==(TokenXML token) const 
		{ return ((token.mTokenType == mTokenType) && (token.mLexemPos == mLexemPos)); }
	};
	
	std::string token_enum_name(TokenXML token);
	std::string token_lexem_name(TokenXML token);
	
	class SymbolTableXML {
	private:
		using token_t = TokenXML::TokenType;
		using iterToken = SmartBuffer::IteratorSmartB;
	public:
		std::vector<std::string> m_StoreNameID;
		std::vector<std::string> m_StoreAttribValue;
		std::vector<std::string> m_StoreText;
	public:
		size_t  addNewLexemVal(TokenXML token, const std::string &lexemString);
		std::string getTokenLexemVal(TokenXML token) const;
		void reset();
	};

}

#endif // SYMBOLTABLEXML_H_