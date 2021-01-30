#include <iostream>
#include <memory>
#include <sstream>

#include "pub/parserXML.h"

#define DEBUG PRINT_TOKEN();

namespace parserXML {
	
	ElementXML::ElementXML() : m_NameID(-1), m_TextID(-1), m_SymbTable(nullptr) { }
	
	long ElementXML::getExistNumAttrib(const std::string &name) const {
		if(m_ListAttrib.size() > 0){
			for (unsigned int i = 0; i < m_ListAttrib.size(); ++i) {
				if(m_SymbTable->nameIsMatch(name, m_ListAttrib[i].first))
					return i;
			}
			return -1;
		} else {
			return -1;
		}
	}
	
	std::string ElementXML::attribVal(const std::string &attribName) const {
		long numAttribPos = getExistNumAttrib(attribName);
		return numAttribPos != -1 ? m_SymbTable->getAttribValByID(m_ListAttrib[numAttribPos].second) : std::string(""); 
	}
	
	void ParserXML::findElements(std::shared_ptr<ElementXML> element, long tagNameID, std::vector<ElementXML> &vecElement) {
		if(element->m_NameID == tagNameID)
			vecElement.push_back(*element);
		for(auto iter = element->m_ListElement.begin(); iter != element->m_ListElement.end(); ++iter) {
			findElements(*iter, tagNameID, vecElement);
		}
	}
	
	void ParserXML::findElements(std::shared_ptr<ElementXML> element, std::vector<ElementXML> &vecElement) {
		vecElement.push_back(*element);
		for(auto iter = element->m_ListElement.begin(); iter != element->m_ListElement.end(); ++iter) {
			findElements(*iter, vecElement);
		}
	}
	
	std::vector<ElementXML> ParserXML::getAllElement() {
		std::vector<ElementXML> listElement;
		findElements(m_RootTreeElements, listElement);
		return listElement;
	}
	
	std::vector<ElementXML> ParserXML::getElementsByTagName(const std::string &tagName) {
		std::vector<ElementXML> listElement;
		long tagNameID = m_SymbolTable.getNameID(tagName);
		findElements(m_RootTreeElements, tagNameID, listElement);
		return listElement;
	}
	
	ParserXML::ParserXML() {	
			testFile.open("d:\\project\\PROJECT\\resourses\\parser_testFile.xml");
	}
	
	ParserXML::ParserXML(const std::string& fileName)
		: m_Lexer(fileName, &m_SymbolTable)
	{
		testFile.open("d:\\project\\PROJECT\\resourses\\parser_testFile.xml");
	}
	
	bool ParserXML::parse(){
		if(!isInit())
			return false;
		m_RootTreeElements = findElement(); // skip prolog of xmlFile
		if(!m_RootTreeElements)
			return false;
		return true;
	}

	bool ParserXML::bindFile(const std::string &fileName){
		if(m_Lexer.isInit())
			unbind();
		if(m_Lexer.init(fileName, &m_SymbolTable)){
			return true;
		}
		unbind();
		return false;
	}
	
	void ParserXML::unbind(){
		m_Lexer.reset();
		m_SymbolTable.reset();
		m_RootTreeElements.reset();
	}
	
	bool ParserXML::isInit() const {
		if(!m_Lexer.isInit())
			return false;
		return true;
	}
	
	void ParserXML::errorHandleParser(const std::string &subMsgExcept){
		std::ostringstream msgExcept;
		msgExcept << "\nexpext token in proc " << subMsgExcept << std::endl;
		msgExcept << "syntaxError in line number# " << m_Lexer.getLinePositionToken() 
							<< "\ncurent token: " 		<< token_enum_name(m_Lexer.getCurToken()) << "[" << m_SymbolTable.getTokenLexemVal(m_Lexer.getCurToken()) <<"]" <<  std::endl
							<< "next token: 		" 		<< token_enum_name(m_Lexer.getNextToken()) << "[" << m_SymbolTable.getTokenLexemVal(m_Lexer.getCurToken()) << "]" << std::endl;
		throw ParserXMLException(msgExcept.str());
	}
	
	std::shared_ptr<ParserXML::BuildElementXML> ParserXML::findElement(){
		std::cout << "findElement \n";
		return element();
	}
	
	void ParserXML::skipelement(){
		std::shared_ptr<ParserXML::BuildElementXML> newElement(new ParserXML::BuildElementXML(&m_SymbolTable));
		switch(m_Lexer.getCurToken().getTokenType()){
			case type_token::OPEN_PROLOG_TAG:
				DEBUG
				if(m_Lexer.getNextToken().getTokenType() == type_token::NAME_ID){
					DEBUG
					m_Lexer.getNextToken();
				} else {
					// error procedure
					errorHandleParser("skipelement(): NAME_ID");
				}
				attribute(newElement);
				if(m_Lexer.getCurToken().getTokenType() == type_token::CLOSE_PROLOG_TAG){
					DEBUG
					m_Lexer.getNextToken();
					skipelement();
				} else {
					// error procedure
					errorHandleParser("skipelement(): CLOSE_PROLOG_TAG");
				}
				break;
			
			case type_token::CDATA_BEGIN:
				DEBUG
				if(m_Lexer.getNextToken().getTokenType() == type_token::TEXT){
					DEBUG
					if(m_Lexer.getNextToken().getTokenType() == type_token::CDATA_END){
						DEBUG
						m_Lexer.getNextToken();
						skipelement();
					} else {
						// error procedure
						errorHandleParser("skipelement(): CDATA_END");
					}
				} else {
					// error procedure
					errorHandleParser("skipelement(): TEXT");
				}
				break;
			
			case type_token::OPEN_COMENT_TAG:
				DEBUG
				if(m_Lexer.getNextToken().getTokenType() == type_token::TEXT){
					DEBUG
					if(m_Lexer.getNextToken().getTokenType() == type_token::CLOSE_COMENT_TAG){
						DEBUG
						m_Lexer.getNextToken();
						skipelement();
					} else {
						// error procedure
						errorHandleParser("skipelement(): CLOSE_COMENT_TAG");
					}
				} else {
					// error procedure
					errorHandleParser("skipelement(): TEXT");
				}
				break;
			
			case type_token::DOCTYPE:
				DEBUG
				if(m_Lexer.getNextToken().getTokenType() == type_token::TEXT){
					DEBUG
					if(m_Lexer.getNextToken().getTokenType() == type_token::CLOSE_TAG){
						DEBUG
						m_Lexer.getNextToken();
						skipelement();
					} else {
						// error procedure
						errorHandleParser("skipelement(): DOCTYPE => CLOSE_TAG");
					}
				} else {
					// error procedure
					errorHandleParser("skipelement(): DOCTYPE => TEXT");
				}
				break;
				
			default:
				return;
		}
	}
	
	std::shared_ptr<ParserXML::BuildElementXML> ParserXML::element(){
		
		std::shared_ptr<ParserXML::BuildElementXML> newElement(new ParserXML::BuildElementXML(&m_SymbolTable));

		skipelement();
		
		switch(m_Lexer.getCurToken().getTokenType()){
			case type_token::OPEN_TAG:
				DEBUG
				if(m_Lexer.getNextToken().getTokenType() == type_token::NAME_ID){
					DEBUG
					newElement->assignNameID(m_Lexer.getCurToken());
					m_Lexer.getNextToken();
					attribute(newElement);
					closeElement(newElement);
				} else {
					// error procedure
					errorHandleParser("element(): NAME_ID");
				}
				break;
				
			case type_token::END_OF_FILE:
				return newElement;
			
			default:
				// error procedure
				errorHandleParser("element(): default switch");
				break;
		}
		return newElement;
	}
	
	void ParserXML::attribute(std::shared_ptr<ParserXML::BuildElementXML> &elementXML){
		if(m_Lexer.getCurToken().getTokenType() == type_token::NAME_ID){
			DEBUG
			std::pair<TokenXML, TokenXML> attrib;
			std::get<0>(attrib) = m_Lexer.getCurToken();
			if(m_Lexer.getNextToken().getTokenType() == type_token::EQUAL){
				DEBUG
				if(m_Lexer.getNextToken().getTokenType() == type_token::ATRIBUTE_VALUE){
					DEBUG
					std::get<1>(attrib) = m_Lexer.getCurToken();
					elementXML->addAttribute(attrib);
					m_Lexer.getNextToken();
					attribute(elementXML);
				} else {
					// error procedure
					errorHandleParser("attribute(): ATRIBUTE_VALUE");
					return;
				}
			} else {
				// error procedure
				errorHandleParser("attribute(): EQUAL");
				return;
			}
		} 
	}
	
	void ParserXML::closeElement(std::shared_ptr<ParserXML::BuildElementXML> &elementXML){
		if(m_Lexer.getCurToken().getTokenType() == type_token::FINAL_CLOSE_TAG){
			DEBUG
			m_Lexer.getNextToken();
		} else {
			if(m_Lexer.getCurToken().getTokenType() == type_token::CLOSE_TAG){
				DEBUG
				m_Lexer.getNextToken();
				endElement(elementXML);
			} else {
				// error procedure
				errorHandleParser("closeElement(): FINAL_CLOSE_TAG");
				return;
			}
		}
	}
	
	void ParserXML::endElement(std::shared_ptr<ParserXML::BuildElementXML> &elementXML){
		std::shared_ptr<ParserXML::BuildElementXML> childElement;
		
		skipelement();
		TokenXML tokenText;
		switch(m_Lexer.getCurToken().getTokenType()){
			
			case type_token::TEXT:
				DEBUG
				tokenText = m_Lexer.getCurToken();
				if(m_Lexer.getNextToken().getTokenType() == type_token::OPEN_CLOSE_TAG){
					DEBUG
					if(m_Lexer.getNextToken().getTokenType() == type_token::NAME_ID){
						DEBUG
						if(!elementXML->nameIsMatch(m_Lexer.getCurToken())){
							// error procedure
							errorHandleParser("endElement(): case TEXT: EndName Element is not the same as StartNameElement");
							return;
						}
						if(m_Lexer.getNextToken().getTokenType() == type_token::CLOSE_TAG){
							DEBUG
							elementXML->assignTextID(tokenText);
							m_Lexer.getNextToken();
						} else {
							// error procedure
							errorHandleParser("endElement(): case TEXT: CLOSE_TAG");
							return;
						}
					} else {
						// error procedure
						errorHandleParser("endElement(): case TEXT: NAME_ID");
						return;
					}
				} else {
					// error procedure
					errorHandleParser("endElement(): case TEXT: OPEN_CLOSE_TAG");
					return;
				}
				break;
			
			case type_token::OPEN_CLOSE_TAG:
				DEBUG
				if(m_Lexer.getNextToken().getTokenType() == type_token::NAME_ID){
					DEBUG
					if(!elementXML->nameIsMatch(m_Lexer.getCurToken())){
						// error procedure
						errorHandleParser("endElement(): case OPEN_CLOSE_TAG: EndName Element is not the same as StartNameElement");
						return;
					}
					if(m_Lexer.getNextToken().getTokenType() == type_token::CLOSE_TAG){
						DEBUG
						m_Lexer.getNextToken();
					} else {
						// error procedure
						errorHandleParser("endElement(): case OPEN_CLOSE_TAG: CLOSE_TAG");
						return;
					}
				} else {
					// error procedure
					errorHandleParser("endElement(): case OPEN_CLOSE_TAG: NAME_ID");
					return;
				}
				break;

			 case type_token::OPEN_PROLOG_TAG:
			 case type_token::OPEN_TAG       :
			 case type_token::CDATA_BEGIN    :
			 case type_token::OPEN_COMENT_TAG:
				childElement = element();
				if(!childElement->nameIsEmpty()){
					elementXML->addNewElement(childElement);
				}
				endElement(elementXML);
				break;
				
				default:
					DEBUG
					// error procedure
					errorHandleParser("endElement(): default");
					return;
		}
	}
}