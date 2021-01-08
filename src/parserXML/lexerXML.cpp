
#include <cctype>
#include "pub/lexerXML.h"

namespace parserXML {

	const char* TokenXML::token_name[] = {
		"UNDEFINE_TOKEN",
		"OPEN_TAG",
		"OPEN_CLOSE_TAG",
		"OPEN_PROLOG_TAG",
		"FINAL_CLOSE_TAG",
		"CLOSE_TAG",
		"EQUAL",
		"CLOSE_PROLOG_TAG",
		"TEXT",
		"NAME_ID",
		"ATRIBUTE_VALUE",
		"END_OF_FILE",
		"OPEN_COMENT_TAG",
		"CLOSE_COMENT_TAG",
		"CDATA_BEGIN",
		"CDATA_END"
	};

	LexerXML::LexerXML(const std::shared_ptr<SmartBuffer>& ptrBuffer)
		: m_Buffer(ptrBuffer),
		  m_LexemBegin(m_Buffer->begin()), m_Forward(m_LexemBegin) {
		m_LastToken = token_t::UNDEFINE_TOKEN;
	}

	void LexerXML::setToken(TokenXML& token, token_t tok_type) {
		m_LastToken = token.mTypeToken = tok_type;
		token.mLexemValue = std::string(m_LexemBegin, m_Forward);
	}

	void LexerXML::skipSymbolUntil(char stopSymbol) {
		while((*m_Forward != stopSymbol) && (*m_Forward != std::char_traits<char>::eof())) {
			m_LexemBegin = ++m_Forward;
		}
	}

	bool LexerXML::defTextToken(TokenXML& token) {
		switch(m_LastToken) { // TEXT_TOKEN
			case (token_t::FINAL_CLOSE_TAG):
			case (token_t::CLOSE_TAG):
			case (token_t::CLOSE_PROLOG_TAG):
			case (token_t::OPEN_COMENT_TAG):
			case (token_t::CDATA_BEGIN):
				while(*m_Forward != std::char_traits<char>::eof()) {
					if(*m_Forward == '<') { // "<"
						break;
					}

					if(*m_Forward == '-') { // "-->"
						SmartBuffer::IteratorSmartB substring(m_Forward);
						if(*++substring == '-')
							if(*++substring == '>') {
								break;
							}
						m_Forward = substring;
					}

					if(*m_Forward == ']') { // "]]>"
						SmartBuffer::IteratorSmartB substring(m_Forward);
						if(*++substring == ']')
							if(*++substring == '>') {
								break;
							}
						m_Forward = substring;
					}
					++m_Forward;
				}
				setToken(token, token_t::TEXT);
				return true;
				break;

			default:
				return false;
		}
	}

	TokenXML LexerXML::getNextToken() {
		TokenXML token{token_t::UNDEFINE_TOKEN};

		while((*m_Forward == '\t') || (*m_Forward == '\n') || (*m_Forward == ' '))
			m_LexemBegin = ++m_Forward;

		switch(*m_Forward) {
			case '<':
				switch(*++m_Forward) {
					case '/': // "</"
						++m_Forward;
						setToken(token, token_t::OPEN_CLOSE_TAG);
						break;

					case '?': // "<?"
						++m_Forward;
						setToken(token, token_t::OPEN_PROLOG_TAG);
						break;

					case '!':
						if(*++m_Forward == '-') {
							if(*++m_Forward == '-') { // "<!--"
								++m_Forward;
								setToken(token, token_t::OPEN_COMENT_TAG);
							} else {
								++m_Forward;
								setToken(token, token_t::UNDEFINE_TOKEN);
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
														setToken(token, token_t::CDATA_BEGIN);
														break;
													}
							++m_Forward;
							setToken(token, token_t::UNDEFINE_TOKEN);
							skipSymbolUntil(' ');
						}
						break;

					default: // "<"
						setToken(token, token_t::OPEN_TAG);
						break;
				}
				break;

			case '/': // "/>"
				if(*++m_Forward == '>') {
					++m_Forward;
					setToken(token, token_t::FINAL_CLOSE_TAG);
				} else {
					setToken(token, token_t::UNDEFINE_TOKEN);
					skipSymbolUntil(' ');
				}
				break;

			case '?': // "?>"
				if(*++m_Forward == '>') {
					++m_Forward;
					setToken(token, token_t::CLOSE_PROLOG_TAG);
				} else {
					setToken(token, token_t::UNDEFINE_TOKEN);
					skipSymbolUntil(' ');
				}
				break;

			case '>': // ">"
				++m_Forward;
				setToken(token, token_t::CLOSE_TAG);
				break;

			case ']': // "]]>"
				if(*++m_Forward == ']')
					if(*++m_Forward == '>') {
						++m_Forward;
						setToken(token, token_t::CDATA_END);
						break;
					}
				++m_Forward;
				setToken(token, token_t::UNDEFINE_TOKEN);
				skipSymbolUntil(' ');
				break;

			case '=': // "="
				++m_Forward;
				setToken(token, token_t::EQUAL);
				break;

			case '-': // "-->"
				if(*++m_Forward == '-') {
					if(*++m_Forward == '>') {
						++m_Forward;
						setToken(token, token_t::CLOSE_COMENT_TAG);
					} else {
						++m_Forward;
						setToken(token, token_t::UNDEFINE_TOKEN);
						skipSymbolUntil(' ');
					}
				} else {
					++m_Forward;
					setToken(token, token_t::UNDEFINE_TOKEN);
					skipSymbolUntil(' ');
				}
				break;

			case '_':
			case ':':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
				++m_Forward;
				if(!defTextToken(token)) {
					while (std::isalpha(*m_Forward) || std::isdigit(*m_Forward) || (*m_Forward == '_') || (*m_Forward == '-') || (*m_Forward == '.') || (*m_Forward == ':'))
						++m_Forward;
					setToken(token, token_t::NAME_ID);
				}
				break;

			case '"': // "text"
				++m_Forward;
				while((*m_Forward != '"') && (*m_Forward != std::char_traits<char>::eof())) {
					++m_Forward;
				}
				++m_LexemBegin;
				setToken(token, token_t::ATRIBUTE_VALUE);
				++m_Forward;
				break;

			case 0x27: // "'"
				++m_Forward;
				while((*m_Forward != char(0x27))  && (*m_Forward != std::char_traits<char>::eof())) {
					++m_Forward;
				}
				++m_LexemBegin;
				setToken(token, token_t::ATRIBUTE_VALUE);
				++m_Forward;
				break;

			case std::char_traits<char>::eof(): // "EOF"
				m_LastToken = token.mTypeToken = token_t::END_OF_FILE;
				token.mLexemValue = std::string("EOF");
				break;

			// add special entiti (< = &lt) (> = &gt) (& = &amp) (' = &apos) (" = &quot)
			// <![CDATA[ some_text ]]>

			default:
				if(!defTextToken(token)) {
					// ERROR => skip all simbols befor meet delim
					skipSymbolUntil('<');
				}
				break;
		}
		m_LexemBegin = m_Forward;
		return token;
	}

}