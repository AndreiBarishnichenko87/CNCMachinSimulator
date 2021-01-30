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

	class ParserXML;

	class ElementXML {
	private:
		friend ParserXML;
		using pair_attrib = std::pair<size_t, size_t>;
	protected:
		long m_NameID;
		long m_TextID;
		std::vector<pair_attrib> m_ListAttrib;
		std::vector<std::shared_ptr<ElementXML> > m_ListElement;
		SymbolTableXML *m_SymbTable;
	private:
		long getExistNumAttrib(const std::string &name) const;
	public:
		ElementXML();
		virtual ~ElementXML() { /*std::cout << "del element" << std::endl;*/ }
	public:
		std::string attribVal(const std::string &attribName) const;
		std::string elementName() const { return m_NameID != -1 ? m_SymbTable->getNameByID(m_NameID) : std::string(""); }
		unsigned int childElementCount() const { return m_ListElement.size(); }
		ElementXML getChildElement(unsigned int number) const { return (number >= childElementCount()) ? throw ElementXMLException(std::string("out of range childElementCount")) : *(m_ListElement[number]); }
	};

	class ParserXML
	{
	private:
		class BuildElementXML : public ElementXML {
		public:
			BuildElementXML(SymbolTableXML *symbolTable) { m_NameID = -1; m_TextID = -1; m_SymbTable = symbolTable; }
			inline void assignNameID(const TokenXML &token) { m_NameID = token.getTokenLexemVal(); }
			inline void assignTextID(const TokenXML &token) { m_TextID = token.getTokenLexemVal(); }
			inline void addAttribute(const std::pair<TokenXML, TokenXML> attrib) { m_ListAttrib.push_back(std::make_pair(attrib.first.getTokenLexemVal(), attrib.second.getTokenLexemVal())); }
			inline void addNewElement(const std::shared_ptr<ElementXML> &element) { m_ListElement.push_back(element); }
			inline bool nameIsMatch(const TokenXML &token) const { return token.getTokenLexemVal() == m_NameID; }
			bool nameIsEmpty() const { return m_NameID == -1; }
		};

	private:
		using type_token = TokenXML::TokenType;
		SymbolTableXML m_SymbolTable;
		LexerXML m_Lexer;
		std::shared_ptr<ElementXML> m_RootTreeElements;

	// syntax rule functions
	private:
		std::shared_ptr<BuildElementXML> findElement();
		std::shared_ptr<BuildElementXML> element();
		void attribute(std::shared_ptr<BuildElementXML> &elementXML);
		void closeElement(std::shared_ptr<BuildElementXML> &elementXML);
		void endElement(std::shared_ptr<BuildElementXML> &elementXML);
		void skipelement();
		void errorHandleParser(const std::string &subMsgExcept);

	// test functions 
	private:
		std::ofstream testFile;
		inline void PRINT_TOKEN() { testFile << token_enum_name(m_Lexer.getCurToken()) << "  [" << m_SymbolTable.getTokenLexemVal(m_Lexer.getCurToken()) << "]" << std::endl; }
	
	private:
		void findElements(std::shared_ptr<ElementXML> element, long tagNameID, std::vector<ElementXML> &vecElement);
		void findElements(std::shared_ptr<ElementXML> element, std::vector<ElementXML> &vecElement);
	private:
		ParserXML(const ParserXML& parserXML) = delete;
		ParserXML& operator=(const ParserXML& parserXML) = delete;
		
	public:
		ParserXML();
		ParserXML(const std::string& fileName);
		bool bindFile(const std::string &fileName);
		void unbind();
		bool isInit() const;
		bool parse();
		
		std::vector<ElementXML> getElementsByTagName(const std::string &tagName);
		std::vector<ElementXML> getAllElement();
		ElementXML getRootElement() { return *m_RootTreeElements; }
	};

}

#endif //PARSERXML_H_