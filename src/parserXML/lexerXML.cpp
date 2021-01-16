
#include <cctype>
#include "pub/lexerXML.h"

namespace parserXML {
	
	LexerXML::LexerXML()
		: m_PreviousToken(token_t::END_OF_FILE)
	{
		m_CountNewLine = 1;
	}

	bool LexerXML::init(const std::string &fileName, SymbolTableXML *symbolTable){
		m_SymbolTable = symbolTable;
		m_SymbolTable->reset();
		m_CountNewLine = 1;
		if(!m_Buffer.init(fileName)){
			m_Buffer.reset();
			m_Forward = m_LexemBegin = m_Buffer.begin();
			m_PreviousToken = TokenXML(token_t::END_OF_FILE);
			return false;
		}
		m_PreviousToken = TokenXML(token_t::START_FILE);
		m_Forward = m_LexemBegin = m_Buffer.begin();
		return true;
	}
	
	// theprocedure reset() make LexerXML undefine. If you try to get symbol after call
	// this procedure you get unexpected behavior. You must call init(...) function
	// for using LexerXML 
	void LexerXML::reset(){
		m_SymbolTable->reset();
		m_Buffer.reset();
		m_Forward = m_LexemBegin = m_Buffer.begin();
		m_CountNewLine = 1;
		m_PreviousToken = TokenXML(token_t::END_OF_FILE);
	}
	
	bool LexerXML::isInit() const {
		if(m_Buffer.isInit() && (m_PreviousToken.getTokenType() != token_t::END_OF_FILE))
			return true;
		else 
			return false;
	}
	
	LexerXML::LexerXML(const std::string &fileName, SymbolTableXML *symbolTable)
		: m_Buffer(fileName), m_SymbolTable(symbolTable), m_PreviousToken(token_t::START_FILE)
	{
		m_Forward = m_LexemBegin = m_Buffer.begin(); 
		m_CountNewLine = 1;
	}

	TokenXML LexerXML::getCurToken() {
		if(m_PreviousToken.getTokenType() == token_t::START_FILE){
			return getNextToken();
		} else {
			return m_PreviousToken;
		}
	}

	void LexerXML::setToken(TokenXML& token, token_t tok_type, const std::string &lexemVal) {
		token = TokenXML(tok_type, lexemVal, m_SymbolTable);
		m_PreviousToken = token;
	}

	void LexerXML::skipSymbolUntil(char stopSymbol) {
		while((*m_Forward != stopSymbol) && (*m_Forward != std::char_traits<char>::eof())) {
			m_LexemBegin = ++m_Forward;
		}
	}
	
	std::string LexerXML::replacePredefXMLEntity(const SmartBuffer::IteratorSmartB &begin, const SmartBuffer::IteratorSmartB &end)
	{
		// &lt;				<			less than
		// &gt;				>			greater than
		// &amp;			&			ampersand 
		// &apos;			'			apostrophe
		// &quot;			"			quotation mark
		std::string str(begin, end);
		std::string::iterator forwardIter(str.begin());
		std::string::iterator prevfIter(forwardIter);
		std::string::iterator replaceIter(forwardIter);
		while(forwardIter != str.end())
		{
			if(*forwardIter != '&'){
				*replaceIter = *forwardIter;
				++replaceIter;
				++forwardIter;
				continue;
			}

			prevfIter = forwardIter;
			switch(*++forwardIter)
			{
				case 'g': // &quot
					if(*++forwardIter == 't')
					if(*++forwardIter == ';'){
						*replaceIter = '>';
						++replaceIter; ++forwardIter;
						break;					
					}
					++forwardIter;
					for(;prevfIter != forwardIter; ++prevfIter){
						*replaceIter = *prevfIter;
						++replaceIter;
					}
					break;
				
				case 'l': // &quot
					if(*++forwardIter == 't')
					if(*++forwardIter == ';'){
						*replaceIter = '<';
						++replaceIter; ++forwardIter;
						break;					
					}
					++forwardIter;
					for(;prevfIter != forwardIter; ++prevfIter){
						*replaceIter = *prevfIter;
						++replaceIter;
					}
					break;
				
				case 'a': // &amp and &apos
					if(*++forwardIter == 'm'){
						if(*++forwardIter == 'p')
						if(*++forwardIter == ';'){
							*replaceIter = '&';
							++replaceIter; ++forwardIter;
							break;
						}
					}	else {
						if(*forwardIter == 'p')
						if(*++forwardIter == 'o')
						if(*++forwardIter == 's')
						if(*++forwardIter == ';'){
							*replaceIter = '\'';
							++replaceIter; ++forwardIter;
							break;
						}
					}
					++forwardIter;
					for(;prevfIter != forwardIter; ++prevfIter){
						*replaceIter = *prevfIter;
						++replaceIter;
					}
					break;

				case 'q': // &quot
					if(*++forwardIter == 'u')
					if(*++forwardIter == 'o')
					if(*++forwardIter == 't')
					if(*++forwardIter == ';'){
						*replaceIter = '"';
						++replaceIter; ++forwardIter;
						break;					
					}
					++forwardIter;
					for(;prevfIter != forwardIter; ++prevfIter){
						*replaceIter = *prevfIter;
						++replaceIter;
					}
					break;

				default:
					*replaceIter = *prevfIter;
					++replaceIter;
					break;
			}
		}
		if(replaceIter != str.end())
			str.erase(replaceIter, str.end());
			
		return str;
	}

	bool LexerXML::defTextToken(TokenXML& token) {
		switch(m_PreviousToken.getTokenType()) { // "text"
			case (token_t::CLOSE_TAG):
			case (token_t::OPEN_COMENT_TAG):
			case (token_t::CDATA_BEGIN):
				break;
			default:
				return false;
		}
		SmartBuffer::IteratorSmartB substring(m_Forward);
		while(*m_Forward != std::char_traits<char>::eof()) {
			switch(*m_Forward){
				case '<': // "<"
					if(m_PreviousToken.getTokenType() == token_t::CLOSE_TAG) {
						if(m_Forward != m_LexemBegin){
							setToken(token, token_t::TEXT, replacePredefXMLEntity(m_LexemBegin, m_Forward));
							return true;								
						} else {
							return false;
						}
					} else {
						++m_Forward;
					}
					break;

				case '-': // "-->"
					if(m_PreviousToken.getTokenType() == token_t::OPEN_COMENT_TAG){
						substring = m_Forward;
						if(*++substring == '-')
						if(*++substring == '>') {
							setToken(token, token_t::TEXT, std::string(m_LexemBegin, m_Forward));
							return true;
						}
						m_Forward = substring;						
					} else {
						++m_Forward;
					}
					break;
				
				case ']': // "]]>"
					if(m_PreviousToken.getTokenType() == token_t::CDATA_BEGIN){
						substring = m_Forward;
						if(*++substring == ']')
						if(*++substring == '>') {
							setToken(token, token_t::TEXT, std::string(m_LexemBegin, m_Forward));
							return true;
						}
						m_Forward = substring;						
					} else {
						++m_Forward;
					}
					break;
				
				default:
					++m_Forward;
			}
			if(*m_Forward == '\n')
				++m_CountNewLine;
		}
		setToken(token, token_t::TEXT, std::string(m_LexemBegin, m_Forward));
		return true;
	}

	TokenXML LexerXML::getNextToken() {
		TokenXML token;

		while((*m_Forward == '\t') || (*m_Forward == '\n') || (*m_Forward == ' '))
		{
			if(*m_Forward == '\n')
				++m_CountNewLine;
			m_LexemBegin = ++m_Forward;
		}
		
		switch(*m_Forward) {
			case '<':
				if(defTextToken(token)) break;
				switch(*++m_Forward) {
					case '/': // "</"
						++m_Forward;
						setToken(token, token_t::OPEN_CLOSE_TAG, std::string(m_LexemBegin, m_Forward));
						break;

					case '?': // "<?"
						++m_Forward;
						setToken(token, token_t::OPEN_PROLOG_TAG, std::string(m_LexemBegin, m_Forward));
						break;

					case '!':
						if(*++m_Forward == '-') {
							if(*++m_Forward == '-') { // "<!--"
								++m_Forward;
								setToken(token, token_t::OPEN_COMENT_TAG, std::string(m_LexemBegin, m_Forward));
							} else {
								++m_Forward;
								setToken(token, token_t::UNDEFINE_TOKEN, std::string(m_LexemBegin, m_Forward));
								skipSymbolUntil(' ');
							}
						} else {
							if(*m_Forward == '[') // "<![CDATA["
							if(*++m_Forward == 'C')
							if(*++m_Forward == 'D')
							if(*++m_Forward == 'A')
							if(*++m_Forward == 'T')
							if(*++m_Forward == 'A')
							if(*++m_Forward == '[') {
								++m_Forward;
								setToken(token, token_t::CDATA_BEGIN, std::string(m_LexemBegin, m_Forward));
								break;
							}
							++m_Forward;
							setToken(token, token_t::UNDEFINE_TOKEN, std::string(m_LexemBegin, m_Forward));
							skipSymbolUntil(' ');
						}
						break;

					default: // "<"
						setToken(token, token_t::OPEN_TAG, std::string(m_LexemBegin, m_Forward));
						break;
				}
				break;

			case '/': // "/>"
				if(defTextToken(token)) break;
				if(*++m_Forward == '>') {
					++m_Forward;
					setToken(token, token_t::FINAL_CLOSE_TAG, std::string(m_LexemBegin, m_Forward));
				} else {
					setToken(token, token_t::UNDEFINE_TOKEN, std::string(m_LexemBegin, m_Forward));
					skipSymbolUntil(' ');
				}
				break;

			case '?': // "?>"
				if(defTextToken(token)) break;
				if(*++m_Forward == '>') {
					++m_Forward;
					setToken(token, token_t::CLOSE_PROLOG_TAG, std::string(m_LexemBegin, m_Forward));
				} else {
					setToken(token, token_t::UNDEFINE_TOKEN, std::string(m_LexemBegin, m_Forward));
					skipSymbolUntil(' ');
				}
				break;

			case '>': // ">"
				if(defTextToken(token)) break;
				++m_Forward;
				setToken(token, token_t::CLOSE_TAG, std::string(m_LexemBegin, m_Forward));
				break;

			case ']': // "]]>"
				if(defTextToken(token)) break;
				if(*++m_Forward == ']')
					if(*++m_Forward == '>') {
						++m_Forward;
						setToken(token, token_t::CDATA_END, std::string(m_LexemBegin, m_Forward));
						break;
					}
				++m_Forward;
				setToken(token, token_t::UNDEFINE_TOKEN, std::string(m_LexemBegin, m_Forward));
				skipSymbolUntil(' ');
				break;

			case '=': // "="
				if(defTextToken(token)) break;
				++m_Forward;
				setToken(token, token_t::EQUAL, std::string(m_LexemBegin, m_Forward));
				break;

			case '-': // "-->"
				if(defTextToken(token)) break;
				if(*++m_Forward == '-') {
					if(*++m_Forward == '>') {
						++m_Forward;
						setToken(token, token_t::CLOSE_COMENT_TAG, std::string(m_LexemBegin, m_Forward));
					} else {
						++m_Forward;
						setToken(token, token_t::UNDEFINE_TOKEN, std::string(m_LexemBegin, m_Forward));
						skipSymbolUntil(' ');
					}
				} else {
					++m_Forward;
					setToken(token, token_t::UNDEFINE_TOKEN, std::string(m_LexemBegin, m_Forward));
					skipSymbolUntil(' ');
				}
				break;

			case '_':	case ':':	case 'a':	case 'b':	case 'c':	case 'd':	case 'e':
			case 'f':	case 'g':	case 'h':	case 'i':	case 'j':	case 'k':	case 'l':
			case 'm':	case 'n':	case 'o':	case 'p':	case 'q':	case 'r':	case 's':
			case 't':	case 'u':	case 'v':	case 'w':	case 'x':	case 'y':	case 'z':
			case 'A':	case 'B':	case 'C':	case 'D':	case 'E':	case 'F':	case 'G':
			case 'H':	case 'I':	case 'J':	case 'K':	case 'L':	case 'M':	case 'N':
			case 'O':	case 'P':	case 'Q':	case 'R':	case 'S':	case 'T':	case 'U':
			case 'V':	case 'W':	case 'X':	case 'Y':	case 'Z':
				++m_Forward;
				if(defTextToken(token)) break;
				while (std::isalpha(*m_Forward) || std::isdigit(*m_Forward) || (*m_Forward == '_') || (*m_Forward == '-') || (*m_Forward == '.') || (*m_Forward == ':'))
					++m_Forward;
				setToken(token, token_t::NAME_ID, std::string(m_LexemBegin, m_Forward));
				break;

			case '"': // "attribute value"
				if(defTextToken(token)) break;
				++m_Forward;
				while((*m_Forward != '"') && (*m_Forward != std::char_traits<char>::eof())) {
					++m_Forward;
				}
				++m_LexemBegin;
				setToken(token, token_t::ATRIBUTE_VALUE, replacePredefXMLEntity(m_LexemBegin, m_Forward));
				++m_Forward;
				break;

			case '\'': // "'"
				if(defTextToken(token)) break;
				++m_Forward;
				while((*m_Forward != '\'')  && (*m_Forward != std::char_traits<char>::eof())) {
					++m_Forward;
				}
				++m_LexemBegin;
				setToken(token, token_t::ATRIBUTE_VALUE, replacePredefXMLEntity(m_LexemBegin, m_Forward));
				++m_Forward;
				break;

			case std::char_traits<char>::eof(): // "EOF"
				setToken(token, token_t::END_OF_FILE, std::string(m_LexemBegin, m_Forward));
				m_PreviousToken = token;
				break;

			default:
				// define text token if start symbol not mach section nameID and text 
				// else skip all symbol untill '<'
				if(!defTextToken(token)) {
					skipSymbolUntil('<');
				}
				break;
		}
		m_LexemBegin = m_Forward;
		return token;
	}

}