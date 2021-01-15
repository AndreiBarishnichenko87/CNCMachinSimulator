
#include <iostream>
#include <memory>

#include "pub/parserXML.h"
#include <sstream>

#define DEBUG PRINT_TOKEN();

namespace parserXML {

	ParserXML::ParserXML(const std::string& fileRead)
		: m_Lexer(std::shared_ptr<SmartBuffer>(new SmartBuffer(fileRead)), &m_SymbolTable)
	{
		testFile.open("d:\\project\\PROJECT\\resourses\\parser_testFile.xml");
		m_TreeElements = findElement(); // skip prolog of xmlFile
	}
	
	void ParserXML::errorHandleParser(const std::string &subMsgExcept){
		std::ostringstream msgExcept;
		msgExcept << "\nexpext token in proc " << subMsgExcept << std::endl;
		msgExcept << "syntaxError in line number# " << m_Lexer.getLinePositionToken() 
							<< "\ncurent token: " 		<< token_enum_name(m_Lexer.getCurToken()) << "[" << m_SymbolTable.getTokenLexemVal(m_Lexer.getCurToken()) <<"]" <<  std::endl
							<< "next token: 		" 		<< token_enum_name(m_Lexer.getNextToken()) << "[" << m_SymbolTable.getTokenLexemVal(m_Lexer.getCurToken()) << "]" << std::endl;
		throw ParserXMLException(msgExcept.str());
	}
	
	std::shared_ptr<ElementXML> ParserXML::findElement(){
		std::cout << "findElement \n";
		return element();
	}
	
	void ParserXML::skipelement(){
		std::shared_ptr<ElementXML> newElement(new ElementXML);
		newElement->m_NameID = -1;
		newElement->m_TextID = -1;
		switch(m_Lexer.getCurToken().mTokenType){
			case type_token::OPEN_PROLOG_TAG:
				DEBUG
				if(m_Lexer.getNextToken().mTokenType == type_token::NAME_ID){
					DEBUG
					m_Lexer.getNextToken();
				} else {
					// error procedure
					errorHandleParser("element(): NAME_ID");
				}
				attribute(newElement);
				if(m_Lexer.getCurToken().mTokenType == type_token::CLOSE_PROLOG_TAG){
					DEBUG
					m_Lexer.getNextToken();
					skipelement();
				} else {
					// error procedure
					errorHandleParser("element(): CLOSE_PROLOG_TAG");
				}
				break;
			
			case type_token::CDATA_BEGIN:
				DEBUG
				if(m_Lexer.getNextToken().mTokenType == type_token::TEXT){
					DEBUG
					if(m_Lexer.getNextToken().mTokenType == type_token::CDATA_END){
						DEBUG
						m_Lexer.getNextToken();
						skipelement();
					} else {
						// error procedure
						errorHandleParser("element(): CDATA_END");
					}
				} else {
					// error procedure
					errorHandleParser("element(): TEXT");
				}
				break;
			
			case type_token::OPEN_COMENT_TAG:
				DEBUG
				if(m_Lexer.getNextToken().mTokenType == type_token::TEXT){
					DEBUG
					if(m_Lexer.getNextToken().mTokenType == type_token::CLOSE_COMENT_TAG){
						DEBUG
						m_Lexer.getNextToken();
						skipelement();
					} else {
						// error procedure
						errorHandleParser("element(): CLOSE_COMENT_TAG");
					}
				} else {
					// error procedure
					errorHandleParser("element(): TEXT");
				}
				break;
				
			default:
				return;
		}
	}
	
	std::shared_ptr<ElementXML> ParserXML::element(){
		
		std::shared_ptr<ElementXML> newElement(new ElementXML);
		newElement->m_NameID = -1;
		newElement->m_TextID = -1;
		
		skipelement();
		
		switch(m_Lexer.getCurToken().mTokenType){
			case type_token::OPEN_TAG:
				DEBUG
				if(m_Lexer.getNextToken().mTokenType == type_token::NAME_ID){
					DEBUG
					newElement->assignNameID(m_Lexer.getCurToken().mLexemPos);
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
	
	void ParserXML::attribute(std::shared_ptr<ElementXML> &elementXML){
		if(m_Lexer.getCurToken().mTokenType == type_token::NAME_ID){
			DEBUG
			ElementXML::pair_attrib attrib;
			attrib.first = m_Lexer.getCurToken().mLexemPos;
			if(m_Lexer.getNextToken().mTokenType == type_token::EQUAL){
				DEBUG
				if(m_Lexer.getNextToken().mTokenType == type_token::ATRIBUTE_VALUE){
					DEBUG
					attrib.second = m_Lexer.getCurToken().mLexemPos;
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
	
	void ParserXML::closeElement(std::shared_ptr<ElementXML> &elementXML){
		if(m_Lexer.getCurToken().mTokenType == type_token::FINAL_CLOSE_TAG){
			DEBUG
			m_Lexer.getNextToken();
		} else {
			if(m_Lexer.getCurToken().mTokenType == type_token::CLOSE_TAG){
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
	
	void ParserXML::endElement(std::shared_ptr<ElementXML> &elementXML){
		std::shared_ptr<ElementXML> childElement;
		
		skipelement();
		
		switch(m_Lexer.getCurToken().mTokenType){
			
			case type_token::TEXT:
				DEBUG
				if(m_Lexer.getNextToken().mTokenType == type_token::OPEN_CLOSE_TAG){
					DEBUG
					if(m_Lexer.getNextToken().mTokenType == type_token::NAME_ID){
						DEBUG
						if(!elementXML->nameIsMatch(m_Lexer.getCurToken().mLexemPos)){
							// error procedure
							errorHandleParser("endElement(): case TEXT: EndName Element is not the same as StartNameElement");
							return;
						}
						if(m_Lexer.getNextToken().mTokenType == type_token::CLOSE_TAG){
							DEBUG
							elementXML->assignTextID(m_Lexer.getCurToken().mLexemPos);
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
				if(m_Lexer.getNextToken().mTokenType == type_token::NAME_ID){
					DEBUG
					if(!elementXML->nameIsMatch(m_Lexer.getCurToken().mLexemPos)){
						// error procedure
						errorHandleParser("endElement(): case OPEN_CLOSE_TAG: EndName Element is not the same as StartNameElement");
						return;
					}
					if(m_Lexer.getNextToken().mTokenType == type_token::CLOSE_TAG){
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