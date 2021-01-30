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
			START_FILE,
			DOCTYPE						 // <!DOCTYPE
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
	
		inline bool isTokenTypeEQ(TokenXML token) const { return mTokenType == token.mTokenType; }
		inline bool isTokenLexemValEQ(TokenXML token) const {return mLexemPos == token.mLexemPos; }
		bool operator==(TokenXML token) const 
		{ return ((token.mTokenType == mTokenType) && (token.mLexemPos == mLexemPos)); }
	};
	
	std::string token_enum_name(TokenXML token);
	std::string token_lexem_name(TokenXML token);
	
	class ElementXML;
	class ParserXML;
	
	class SymbolTableXML {
	private:
		using token_t = TokenXML::TokenType;
		using iterToken = SmartBuffer::IteratorSmartB;
	private:
		std::vector<std::string> m_StoreNameID;
		std::vector<std::string> m_StoreAttribValue;
		std::vector<std::string> m_StoreText;
	
	private:
		friend ElementXML;
		friend ParserXML;
		bool nameIsMatch(const std::string &name, long nameID) const { return name == m_StoreNameID[nameID] ? true : false; }
		std::string getNameByID(unsigned int ID) const { return m_StoreNameID.size() >= (ID + 1) ? m_StoreNameID[ID] : std::string(""); }
		std::string getAttribValByID(unsigned int ID) const { return m_StoreAttribValue.size() >= (ID + 1) ? m_StoreAttribValue[ID] : std::string(""); }
		std::string getTextByID(unsigned int ID) const { return m_StoreText.size() >= (ID + 1) ? m_StoreText[ID] : std::string(""); }
		long getNameID(const std::string &name) const;
	public:
		void reset();
		size_t  addNewLexemVal(TokenXML token, const std::string &lexemString);
		std::string getTokenLexemVal(TokenXML token) const;
	};

}

#endif // SYMBOLTABLEXML_H_