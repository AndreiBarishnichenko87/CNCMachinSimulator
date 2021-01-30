#ifndef EXCEPTIONPARSERXML_H_
#define EXCEPTIONPARSERXML_H_

#include <stdexcept>
#include <exception>
#include <string>

namespace parserXML {
	
	class ExceptionParserXML : public std::exception {
	private:
		std::string m_Msg;
	public:
		ExceptionParserXML(const std::string &msg) : m_Msg("ExceptionParserXML::" + msg) { }
		const char* what() const noexcept { return m_Msg.c_str(); }
	};
	
	class SmartBufferException : public ExceptionParserXML {
	public:
		SmartBufferException(const std::string &msg) : ExceptionParserXML("SmartBufferException::" + msg) { }
	};
	
	class LexerXMLException : public ExceptionParserXML {
	public:
		LexerXMLException(const std::string &msg) : ExceptionParserXML("LexerXMLException::" + msg) { }
	};
	
	class ParserXMLException : public ExceptionParserXML {
	public:
		ParserXMLException(const std::string &msg) : ExceptionParserXML("ParserXMLException::" + msg) { }
	};
	
	class ElementXMLException : public ExceptionParserXML {
	public:
		ElementXMLException(const std::string &msg) : ExceptionParserXML("ElementXMLException::" + msg) { }
	};

}

#endif // EXCEPTIONPARSERXML_H_