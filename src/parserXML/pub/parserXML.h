#ifndef PARSERXML_H_
#define PARSERXML_H_

#include <string>
#include <utility>
#include <list>

#include "lexerXML.h"

namespace parserXML{
	
	class ElementXML {
	public:
		using pair_attrib = std::pair<std::string, std::string>;
	public:
		std::string m_Name;
		std::string m_Text;
		std::list<pair_attrib> m_ListAttrib;
		std::list<ElementXML> m_ListElement;
	public:
		inline void assignName(const std::string &elementName) { m_Name = elementName; }
		inline void assignText(const std::string &text) { m_Text = text; }
		inline void addAttribute(const pair_attrib &attrib) { m_ListAttrib.push_back(attrib); }
		inline void addNewElement(const ElementXML &element) { m_ListElement.push_back(element); }
		inline bool nameIsMatch(const std::string &strMatches) const { return strMatches == m_Name; }
		bool nameIsEmpty() const { return m_Name.empty(); }
	};
	
	class ParserXML
	{
		using type_token = TokenXML::token_XML;
	public:
		ElementXML m_TreeElements;
	private:
		LexerXML m_Lexer;
	private:
		ElementXML element();
		void attribute(ElementXML &elementXML);
		void closeElement(ElementXML &elementXML);
		void endElement(ElementXML &elementXML);
	
	// debug functions
	private:
		void printElement(const std::string& str, unsigned int width){
			for(unsigned int i = 0; i < width; i++)
				std::cout << " ";
			std::cout << str << "_" << width << std::endl;
		}
	
	public:
		ParserXML(const std::string& fileRead);
		void findElement();
	};
	
}

#endif //PARSERXML_H_