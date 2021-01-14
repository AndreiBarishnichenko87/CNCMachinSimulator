
#include <iostream>
#include <memory>

#include "pub/parserXML.h"
#include <sstream>

#define DEBUG PRINT_TOKEN();

namespace parserXML {

	ParserXML::ParserXML(const std::string& fileRead)
		: m_Lexer(std::shared_ptr<SmartBuffer>(new SmartBuffer(fileRead)))
	{
		testFile.open("d:\\project\\PROJECT\\resourses\\parser_testFile.xml");
		while(!m_TreeElements && (m_Lexer.getCurToken().mTypeToken != type_token::END_OF_FILE))
		{
			findElement(); // skip prolog of xmlFile
		}
	}
	
	void ParserXML::errorHandleParser(const std::string &subMsgExcept){
		std::ostringstream msgExcept;
		msgExcept << "\nexpext token in proc " << subMsgExcept << std::endl;
		msgExcept << "syntaxError in line number# " << m_Lexer.getLinePositionToken() 
							<< "\ncurent token: " 		<< m_Lexer.getCurToken().tokenName() 	<< "[" << m_Lexer.getCurToken().mLexemValue <<"]" <<  std::endl
							<< "next token: 		" 		<< m_Lexer.getNextToken().tokenName() << "[" << m_Lexer.getCurToken().mLexemValue << "]" << std::endl;
		throw ParserXMLException(msgExcept.str());
	}
	
	void ParserXML::findElement(){
		std::cout << "findElement \n";
		std::shared_ptr<ElementXML> temp(element());
		if(!temp->nameIsEmpty()){
			m_TreeElements = temp;
		}
	}
	
	std::shared_ptr<ElementXML> ParserXML::element(){
		
		std::shared_ptr<ElementXML> newElement(new ElementXML);
		
		switch(m_Lexer.getCurToken().mTypeToken){
			case type_token::OPEN_TAG:
				DEBUG
				if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
					DEBUG
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
				DEBUG
				if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
					DEBUG
					m_Lexer.getNextToken();
				} else {
					// error procedure
					errorHandleParser("element(): NAME_ID");
				}
				attribute(newElement);
				if(m_Lexer.getCurToken().mTypeToken == type_token::CLOSE_PROLOG_TAG){
					DEBUG
					m_Lexer.getNextToken();
				} else {
					// error procedure
					errorHandleParser("element(): CLOSE_PROLOG_TAG");
				}
				break;

			case type_token::CDATA_BEGIN:
				DEBUG
				if(m_Lexer.getNextToken().mTypeToken == type_token::TEXT){
					DEBUG
					if(m_Lexer.getNextToken().mTypeToken == type_token::CDATA_END){
						DEBUG
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
				DEBUG
				if(m_Lexer.getNextToken().mTypeToken == type_token::TEXT){
					DEBUG
					if(m_Lexer.getNextToken().mTypeToken == type_token::CLOSE_COMENT_TAG){
						DEBUG
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
		if(m_Lexer.getCurToken().mTypeToken == type_token::NAME_ID){
			DEBUG
			ElementXML::pair_attrib attrib;
			attrib.first = m_Lexer.getCurToken().mLexemValue;
			if(m_Lexer.getNextToken().mTypeToken == type_token::EQUAL){
				DEBUG
				if(m_Lexer.getNextToken().mTypeToken == type_token::ATRIBUTE_VALUE){
					DEBUG
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
			DEBUG
			m_Lexer.getNextToken();
		} else {
			if(m_Lexer.getCurToken().mTypeToken == type_token::CLOSE_TAG){
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
		switch(m_Lexer.getCurToken().mTypeToken){
			
			case type_token::TEXT:
				DEBUG
				if(m_Lexer.getNextToken().mTypeToken == type_token::OPEN_CLOSE_TAG){
					DEBUG
					if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
						DEBUG
						if(!elementXML->nameIsMatch(m_Lexer.getCurToken().mLexemValue)){
							// error procedure
							errorHandleParser("endElement(): case TEXT: EndName Element is not the same as StartNameElement");
							return;
						}
						if(m_Lexer.getNextToken().mTypeToken == type_token::CLOSE_TAG){
							DEBUG
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
				DEBUG
				if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
					DEBUG
					if(!elementXML->nameIsMatch(m_Lexer.getCurToken().mLexemValue)){
						// error procedure
						errorHandleParser("endElement(): case OPEN_CLOSE_TAG: EndName Element is not the same as StartNameElement");
						return;
					}
					if(m_Lexer.getNextToken().mTypeToken == type_token::CLOSE_TAG){
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
					testFile << "In [" << elementXML->m_Name << "] add element [" << childElement->m_Name << "] ";
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