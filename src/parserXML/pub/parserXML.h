#ifndef PARSERXML_H_
#define PARSERXML_H_

#include <string>
#include <utility>
#include <list>
#include <memory>

#include "lexerXML.h"
#include "exceptionParserXML.h"
#include "symbolTableXML.h"

namespace parserXML{

	class ElementXML {
	public:
		using pair_attrib = std::pair<size_t, size_t>;
	public:
		long m_NameID;
		long m_TextID;
		std::list<pair_attrib> m_ListAttrib;
		std::list<std::shared_ptr<ElementXML> > m_ListElement;
	public:
		inline void assignNameID(const TokenXML &token) { m_NameID = token.getTokenLexemVal(); }
		inline void assignTextID(const TokenXML &token) { m_TextID = token.getTokenLexemVal(); }
		inline void addAttribute(const std::pair<TokenXML, TokenXML> attrib) { m_ListAttrib.push_back(std::make_pair(attrib.first.getTokenLexemVal(), attrib.second.getTokenLexemVal())); }
		inline void addNewElement(const std::shared_ptr<ElementXML> &element) { m_ListElement.push_back(element); }
		inline bool nameIsMatch(const TokenXML &token) const { return token.getTokenLexemVal() == m_NameID; }
		bool nameIsEmpty() const { return m_NameID == -1; }
	};
	
	class ParserXML
	{
		using type_token = TokenXML::TokenType;
	private:
		SymbolTableXML m_SymbolTable;
		LexerXML m_Lexer;
	public:
		std::shared_ptr<ElementXML> m_TreeElements;
	private:
		// syntax rule functions
		std::shared_ptr<ElementXML> findElement();
		std::shared_ptr<ElementXML> element();
		void attribute(std::shared_ptr<ElementXML> &elementXML);
		void closeElement(std::shared_ptr<ElementXML> &elementXML);
		void endElement(std::shared_ptr<ElementXML> &elementXML);
		void skipelement();
	
	private:
		void errorHandleParser(const std::string &subMsgExcept);
		
	// test functions 
	private:
		std::ofstream testFile;
		inline void PRINT_TOKEN() { testFile << token_enum_name(m_Lexer.getCurToken()) << "  [" << m_SymbolTable.getTokenLexemVal(m_Lexer.getCurToken()) << "]" << std::endl; }
	
	public:
		ParserXML();
		ParserXML(const std::string& fileName);
		bool bindFile(const std::string &fileName);
		void unbind();
		bool isInit() const;
		bool parse();
	};

}

#endif //PARSERXML_H_