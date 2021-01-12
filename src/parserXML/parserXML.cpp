
#include <iostream>
#include <memory>

#include "pub/parserXML.h"

#define PRINT_CUR_TOK_STR() std::cout << m_Lexer.getCurToken().mLexemValue << " "
#define ERROR_PARSER //error()

namespace parserXML {
	
	ParserXML::ParserXML(const std::string& fileRead)
		: m_Lexer(std::shared_ptr<SmartBuffer>(new SmartBuffer(fileRead)))
	{
		
	}
	
	void ParserXML::findElement(){
		element();
	}
	
	void ParserXML::element(){
			openElement();
			closeElement();			
	}
	
	void ParserXML::openElement(){
		if(m_Lexer.getCurToken().mTypeToken == type_token::OPEN_PROLOG_TAG){
			PRINT_CUR_TOK_STR();
			if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
				PRINT_CUR_TOK_STR();
				m_Lexer.getNextToken();
			} else {
				// error procedure
				ERROR_PARSER;
				return;
			}
			attribute();
			if(m_Lexer.getCurToken().mTypeToken == type_token::CLOSE_PROLOG_TAG){
				PRINT_CUR_TOK_STR();
				m_Lexer.getNextToken();
			} else {
				// error procedure
				ERROR_PARSER;
				return;
			}
		} else {
			if(m_Lexer.getCurToken().mTypeToken == type_token::OPEN_TAG){
				PRINT_CUR_TOK_STR();
				if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
					PRINT_CUR_TOK_STR();
					m_Lexer.getNextToken();
					attribute();
				} else {
					// error procedure
					ERROR_PARSER;
					return;
				}
			} else {
				// error procedure
				ERROR_PARSER;
				return;
			}
		}
	}
	
	void ParserXML::attribute(){
		if(m_Lexer.getCurToken().mTypeToken == type_token::NAME_ID){
			PRINT_CUR_TOK_STR();
			if(m_Lexer.getNextToken().mTypeToken == type_token::EQUAL){
				PRINT_CUR_TOK_STR();
				if(m_Lexer.getNextToken().mTypeToken == type_token::ATRIBUTE_VALUE){
						PRINT_CUR_TOK_STR();
						m_Lexer.getNextToken();
						attribute();
				} else {
					// error procedure
					ERROR_PARSER;
					return;
				}
			} else {
				// error procedure
				ERROR_PARSER;
				return;
			}
		} 
	}
	
	void ParserXML::closeElement(){
		if(m_Lexer.getCurToken().mTypeToken == type_token::FINAL_CLOSE_TAG){
			PRINT_CUR_TOK_STR();
			m_Lexer.getNextToken();
		} else {
			if(m_Lexer.getCurToken().mTypeToken == type_token::CLOSE_TAG){
				PRINT_CUR_TOK_STR();
				m_Lexer.getNextToken();
				endElement();
			} else {
				// error procedure
				ERROR_PARSER;
				return;
			}
		}
	}
	
	void ParserXML::endElement(){
		
		switch(m_Lexer.getCurToken().mTypeToken){
			
			case type_token::TEXT:
				PRINT_CUR_TOK_STR();
				if(m_Lexer.getNextToken().mTypeToken == type_token::OPEN_CLOSE_TAG){
					PRINT_CUR_TOK_STR();
					if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
						PRINT_CUR_TOK_STR();
						if(m_Lexer.getNextToken().mTypeToken == type_token::CLOSE_TAG){
							PRINT_CUR_TOK_STR();
							m_Lexer.getNextToken();
						} else {
							// error procedure
							ERROR_PARSER;
							return;
						}
					} else {
						// error procedure
						ERROR_PARSER;
						return;
					}
				} else {
					// error procedure
					ERROR_PARSER;
					return;
				}
				break;
			
			case type_token::OPEN_CLOSE_TAG:
				PRINT_CUR_TOK_STR();
				if(m_Lexer.getNextToken().mTypeToken == type_token::NAME_ID){
					PRINT_CUR_TOK_STR();
					if(m_Lexer.getNextToken().mTypeToken == type_token::CLOSE_TAG){
						PRINT_CUR_TOK_STR();
						m_Lexer.getNextToken();
					} else {
						// error procedure
						ERROR_PARSER;
						return;
					}
				} else {
					// error procedure
					ERROR_PARSER;
					return;
				}
				break;

			default:
				element();
				endElement();
				break;
		}
	}
}