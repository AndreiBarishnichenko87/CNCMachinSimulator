
#include <iostream>
#include <memory>

#include "pub/parserXML.h"

namespace parserXML {

	ParserXML::ParserXML(const std::string& fileRead)
		: m_Lexer(std::shared_ptr<SmartBuffer>(new SmartBuffer(fileRead)))
	{
		
	}
	
	void ParserXML::findElement(){
		m_TreeElements = element();
	}
	
	ElementXML ParserXML::element(){
		
		ElementXML newElement;
		
		switch(m_Lexer.getCurToken().mTypeToken){
			case type_token::OPEN_TAG:
				if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
					newElement.assignName(m_Lexer.getCurToken().mLexemValue);
					m_Lexer.getNextToken();
					attribute(newElement);
					closeElement(newElement);
				} else {
					// error procedure
				}
				break;
				
			case type_token::OPEN_PROLOG_TAG:
				if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
					m_Lexer.getNextToken();
				} else {
					// error procedure
				}
				attribute(newElement);
				if(m_Lexer.getCurToken().mTypeToken == type_token::CLOSE_PROLOG_TAG){
					m_Lexer.getNextToken();
				} else {
					// error procedure
				}
				break;

			case type_token::CDATA_BEGIN:
				if(m_Lexer.getNextToken().mTypeToken == type_token::TEXT){
					if(m_Lexer.getNextToken().mTypeToken == type_token::CDATA_END){
						m_Lexer.getNextToken();
					} else {
						// error procedure
					}
				} else {
					// error procedure
				}
				break;

			case type_token::OPEN_COMENT_TAG:
				if(m_Lexer.getNextToken().mTypeToken == type_token::TEXT){
					if(m_Lexer.getNextToken().mTypeToken == type_token::CLOSE_COMENT_TAG){
						m_Lexer.getNextToken();
					} else {
						// error procedure
					}
				} else {
					// error procedure
				}
				break;
			
			case type_token::UNDEFINE_TOKEN:
				break;
			
			case type_token::END_OF_FILE:
				break;
			
			default:
				// error procedure
				break;
		}
		return newElement;
	}
	
	void ParserXML::attribute(ElementXML &elementXML){
		if(m_Lexer.getCurToken().mTypeToken == type_token::NAME_ID){
			ElementXML::pair_attrib attrib;
			attrib.first = m_Lexer.getCurToken().mLexemValue;
			if(m_Lexer.getNextToken().mTypeToken == type_token::EQUAL){
				if(m_Lexer.getNextToken().mTypeToken == type_token::ATRIBUTE_VALUE){
						attrib.second = m_Lexer.getCurToken().mLexemValue;
						elementXML.addAttribute(attrib);
						m_Lexer.getNextToken();
						attribute(elementXML);
				} else {
					// error procedure
					return;
				}
			} else {
				// error procedure
				return;
			}
		} 
	}
	
	void ParserXML::closeElement(ElementXML &elementXML){
		if(m_Lexer.getCurToken().mTypeToken == type_token::FINAL_CLOSE_TAG){
			m_Lexer.getNextToken();
		} else {
			if(m_Lexer.getCurToken().mTypeToken == type_token::CLOSE_TAG){
				m_Lexer.getNextToken();
				endElement(elementXML);
			} else {
				// error procedure
				return;
			}
		}
	}
	
	void ParserXML::endElement(ElementXML &elementXML){
		ElementXML childElement;
		switch(m_Lexer.getCurToken().mTypeToken){
			case type_token::TEXT:
				if(m_Lexer.getNextToken().mTypeToken == type_token::OPEN_CLOSE_TAG){
					if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
						if(!elementXML.nameIsMatch(m_Lexer.getCurToken().mLexemValue)){
							std::cout << "EndName Element is not the same as StartNameElement" << std::endl;
							return;
						}
						if(m_Lexer.getNextToken().mTypeToken == type_token::CLOSE_TAG){
							elementXML.assignText(m_Lexer.getCurToken().mLexemValue);
							m_Lexer.getNextToken();
						} else {
							// error procedure
							return;
						}
					} else {
						// error procedure
						return;
					}
				} else {
					// error procedure
					return;
				}
				break;
			
			case type_token::OPEN_CLOSE_TAG:
				if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
					if(!elementXML.nameIsMatch(m_Lexer.getCurToken().mLexemValue)){
						std::cout << "EndName Element is not the same as StartNameElement" << std::endl;
						return;
					}
					if(m_Lexer.getNextToken().mTypeToken == type_token::CLOSE_TAG){
						m_Lexer.getNextToken();
					} else {
						// error procedure
						return;
					}
				} else {
					// error procedure
					return;
				}
				break;
				
			case type_token::UNDEFINE_TOKEN :
			case type_token::END_OF_FILE    :
				std::cout << "UNDEFINE_TOKEN or END_OF_FILE" << std::endl;
				return;
				break;
			
			 case type_token::OPEN_PROLOG_TAG:
			 case type_token::OPEN_TAG       :
			 case type_token::CDATA_BEGIN    :
			 case type_token::OPEN_COMENT_TAG:
				childElement = element();
				if(!childElement.nameIsEmpty()){
					elementXML.addNewElement(childElement);
				}
				else {
					std::cout << "Child Element is undefine" << std::endl;
					return;
				}
				endElement(elementXML);
				break;
		}
	}
}