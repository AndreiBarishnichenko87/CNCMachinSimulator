
#include <iostream>
#include <memory>

#include "pub/parserXML.h"
#include <sstream>

namespace parserXML {

	ParserXML::ParserXML(const std::string& fileRead)
		: m_Lexer(std::shared_ptr<SmartBuffer>(new SmartBuffer(fileRead)))
	{
		
	}
	
	void ParserXML::errorHandleParser(const std::string &subMsgExcept){
		std::ostringstream msgExcept;
		msgExcept << "expext token in proc " << subMsgExcept << std::endl;
		msgExcept << "syntaxError in line number# " << m_Lexer.getLinePositionToken() 
						<< " lexem: " << m_Lexer.getCurToken().mLexemValue;
		throw ParserXMLException(msgExcept.str());
	}
	
	void ParserXML::findElement(){
		m_TreeElements = element();
	}
	
	std::shared_ptr<ElementXML> ParserXML::element(){
		
		std::shared_ptr<ElementXML> newElement(new ElementXML);
		
		switch(m_Lexer.getCurToken().mTypeToken){
			case type_token::OPEN_TAG:
				if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
					newElement->assignName(m_Lexer.getCurToken().mLexemValue);
					m_Lexer.getNextToken();
					attribute(newElement);
					closeElement(newElement);
				} else {
					// error procedure
					errorHandleParser("element(): NAME_ID");
				}
				break;
				
			case type_token::OPEN_PROLOG_TAG:
				if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
					m_Lexer.getNextToken();
				} else {
					// error procedure
					errorHandleParser("element(): NAME_ID");
				}
				attribute(newElement);
				if(m_Lexer.getCurToken().mTypeToken == type_token::CLOSE_PROLOG_TAG){
					m_Lexer.getNextToken();
				} else {
					// error procedure
					errorHandleParser("element(): CLOSE_PROLOG_TAG");
				}
				break;

			case type_token::CDATA_BEGIN:
				if(m_Lexer.getNextToken().mTypeToken == type_token::TEXT){
					if(m_Lexer.getNextToken().mTypeToken == type_token::CDATA_END){
						m_Lexer.getNextToken();
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
				if(m_Lexer.getNextToken().mTypeToken == type_token::TEXT){
					if(m_Lexer.getNextToken().mTypeToken == type_token::CLOSE_COMENT_TAG){
						m_Lexer.getNextToken();
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
				// error procedure
				errorHandleParser("element(): default switch");
				break;
		}
		return newElement;
	}
	
	void ParserXML::attribute(std::shared_ptr<ElementXML> &elementXML){
		if(m_Lexer.getCurToken().mTypeToken == type_token::NAME_ID){
			ElementXML::pair_attrib attrib;
			attrib.first = m_Lexer.getCurToken().mLexemValue;
			if(m_Lexer.getNextToken().mTypeToken == type_token::EQUAL){
				if(m_Lexer.getNextToken().mTypeToken == type_token::ATRIBUTE_VALUE){
						attrib.second = m_Lexer.getCurToken().mLexemValue;
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
		if(m_Lexer.getCurToken().mTypeToken == type_token::FINAL_CLOSE_TAG){
			m_Lexer.getNextToken();
		} else {
			if(m_Lexer.getCurToken().mTypeToken == type_token::CLOSE_TAG){
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
		switch(m_Lexer.getCurToken().mTypeToken){
			
			case type_token::TEXT:
				if(m_Lexer.getNextToken().mTypeToken == type_token::OPEN_CLOSE_TAG){
					if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
						if(!elementXML->nameIsMatch(m_Lexer.getCurToken().mLexemValue)){
							// error procedure
							errorHandleParser("endElement(): case TEXT: EndName Element is not the same as StartNameElement");
							return;
						}
						if(m_Lexer.getNextToken().mTypeToken == type_token::CLOSE_TAG){
							elementXML->assignText(m_Lexer.getCurToken().mLexemValue);
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
				if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
					if(!elementXML->nameIsMatch(m_Lexer.getCurToken().mLexemValue)){
						// error procedure
						errorHandleParser("endElement(): case OPEN_CLOSE_TAG: EndName Element is not the same as StartNameElement");
						return;
					}
					if(m_Lexer.getNextToken().mTypeToken == type_token::CLOSE_TAG){
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
				else {
					std::cout << "Child Element is undefine" << std::endl;
					break;
				}
				endElement(elementXML);
				break;
				
				default:
					// error procedure
					errorHandleParser("endElement(): default");
					return;
		}
	}
}