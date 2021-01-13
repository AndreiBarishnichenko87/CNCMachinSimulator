#ifndef PARSERXML_H_
#define PARSERXML_H_

#include <string>
#include <utility>
#include <list>
#include <memory>

#include "lexerXML.h"
#include "exceptionParserXML.h"

namespace parserXML{
	
	class ElementXML {
	public:
		using pair_attrib = std::pair<std::string, std::string>;
	public:
		std::string m_Name;
		std::string m_Text;
		std::list<pair_attrib> m_ListAttrib;
		std::list<std::shared_ptr<ElementXML> > m_ListElement;
	public:
		inline void assignName(const std::string &elementName) { m_Name = elementName; }
		inline void assignText(const std::string &text) { m_Text = text; }
		inline void addAttribute(const pair_attrib &attrib) { m_ListAttrib.push_back(attrib); }
		inline void addNewElement(const std::shared_ptr<ElementXML> &element) { m_ListElement.push_back(element); }
		inline bool nameIsMatch(const std::string &strMatches) const { return strMatches == m_Name; }
		bool nameIsEmpty() const { return m_Name.empty(); }
	};
	
	class ParserXML
	{
		using type_token = TokenXML::token_XML;
	public:
		std::shared_ptr<ElementXML> m_TreeElements;
	private:
		LexerXML m_Lexer;
	private:
		// syntax rule functions
		std::shared_ptr<ElementXML> element();
		void attribute(std::shared_ptr<ElementXML> &elementXML);
		void closeElement(std::shared_ptr<ElementXML> &elementXML);
		void endElement(std::shared_ptr<ElementXML> &elementXML);
	
	private:
		void errorHandleParser(const std::string &subMsgExcept);
	
	public:
		ParserXML(const std::string& fileRead);
		void findElement();
	};
	
}

#endif //PARSERXML_H_