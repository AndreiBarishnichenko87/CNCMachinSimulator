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
		inline void assignNameID(size_t elementName) { m_NameID = elementName; }
		inline void assignTextID(size_t text) { m_TextID = text; }
		inline void addAttribute(const pair_attrib attrib) { m_ListAttrib.push_back(attrib); }
		inline void addNewElement(const std::shared_ptr<ElementXML> &element) { m_ListElement.push_back(element); }
		inline bool nameIsMatch(size_t strMatches) const { return strMatches == m_NameID; }
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
		
	private:
		std::ofstream testFile;
		inline void PRINT_TOKEN() { testFile << token_enum_name(m_Lexer.getCurToken()) << "  [" << m_SymbolTable.getTokenLexemVal(m_Lexer.getCurToken()) << "]" << std::endl; }
	
	public:
		ParserXML(const std::string& fileRead);
	};

}

#endif //PARSERXML_H_