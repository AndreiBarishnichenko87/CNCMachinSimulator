
#include "pub/symbolTableXML.h"

namespace parserXML {
	
	std::string token_enum_name(TokenXML token) {
		return std::string(TokenXML::smTokenEnumName[token.getTokenType()]);
	}
	std::string token_lexem_name(TokenXML token) {
		return std::string(TokenXML::smTokenLexem[token.getTokenType()]);
	}
	
	const char* TokenXML::smTokenEnumName[] = {
		"UNDEFINE_TOKEN",
		"OPEN_TAG",         // <
		"OPEN_CLOSE_TAG",   // </
		"OPEN_PROLOG_TAG",  // <?
		"FINAL_CLOSE_TAG",  // />
		"CLOSE_TAG",        //	>
		"EQUAL",            //	=
		"CLOSE_PROLOG_TAG", // ?>
		"TEXT",             // text
		"NAME_ID",          //	nameID
		"ATRIBUTE_VALUE",   // attribValue
		"END_OF_FILE",      // EOF
		"OPEN_COMENT_TAG",  // <!--
		"CLOSE_COMENT_TAG", // -->
		"CDATA_BEGIN",      // <![CDATA[
		"CDATA_END",        //	]]>
		"START_FILE"
	};
	const char* TokenXML::smTokenLexem[] = {
		"UNDEFINE_TOKEN",
		R"(<)",
		R"(</)",
		R"(<?)",
		R"(/>)",
		R"(>)",
		R"(=)",
		R"(?>)",
		"TEXT",
		"NAME_ID",
		"ATRIBUTE_VALUE",
		"EOF",
		R"(<!--)",
		R"(-->)",
		R"(<![CDATA[)",
		R"(]]>)",
		"START_FILE"
	};

	TokenXML::TokenXML(TokenType tokType, const std::string &lexemString, SymbolTableXML *symbTable){
		mTokenType = tokType;
		mLexemPos = symbTable->addNewLexemVal(*this, lexemString);
	}

	void SymbolTableXML::reset(){
		std::vector<std::string>().swap(m_StoreNameID);
		std::vector<std::string>().swap(m_StoreAttribValue);
		std::vector<std::string>().swap(m_StoreText);
	}
	
	std::string SymbolTableXML::getTokenLexemVal(TokenXML token) const {
		switch(token.getTokenType()){
			case token_t::NAME_ID:
				return m_StoreNameID[token.getTokenLexemVal()];
				break;
				
			case token_t::TEXT:
				return m_StoreText[token.getTokenLexemVal()];
				break;
				
			case token_t::ATRIBUTE_VALUE:
				return m_StoreAttribValue[token.getTokenLexemVal()];
				break;
				
			default:
				return TokenXML::smTokenLexem[token.getTokenLexemVal()];
				break;
		}
	}

	size_t  SymbolTableXML::addNewLexemVal(TokenXML token, const std::string &lexemString){
		switch(token.getTokenType()){
			case token_t::NAME_ID:
				for(size_t i = 0; i < m_StoreNameID.size(); ++i){
					if(lexemString == m_StoreNameID[i])
					return i;
				}
				m_StoreNameID.push_back(lexemString);
				return m_StoreNameID.size() - 1;
				break;
			
			case token_t::TEXT:
				m_StoreText.push_back(lexemString);
				return m_StoreText.size() - 1;
				break;
			
			case token_t::ATRIBUTE_VALUE:
				m_StoreAttribValue.push_back(lexemString);
				return m_StoreAttribValue.size() - 1;
				break;
			
			case token_t::UNDEFINE_TOKEN  :
			case token_t::OPEN_TAG        :
			case token_t::OPEN_CLOSE_TAG  :
			case token_t::OPEN_PROLOG_TAG :
			case token_t::FINAL_CLOSE_TAG :
			case token_t::CLOSE_TAG       :
			case token_t::EQUAL           :
			case token_t::CLOSE_PROLOG_TAG:
			case token_t::END_OF_FILE     :
			case token_t::OPEN_COMENT_TAG :
			case token_t::CLOSE_COMENT_TAG:
			case token_t::CDATA_BEGIN     :
			case token_t::CDATA_END       :
			case token_t::START_FILE      :
				return token.getTokenType();
				break;
			
			default:
				return -1;
		}
	}

}