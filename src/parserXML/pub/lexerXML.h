#ifndef LEXERXML_H_
#define LEXERXML_H_

#include <memory>
#include <string>
#include "smartBuffer.h"
#include "symbolTableXML.h"

namespace parserXML {

	class LexerXML {
		public:
			using token_t = TokenXML::TokenType;
		private:
			SymbolTableXML *m_SymbolTable;
			TokenXML m_PreviousToken;
			SmartBuffer m_Buffer;
			SmartBuffer::IteratorSmartB m_LexemBegin;
			SmartBuffer::IteratorSmartB m_Forward;
			size_t m_CountNewLine;
		
		private:
			void setToken(TokenXML& token, token_t tok_type);
			void setToken(TokenXML& token, token_t tok_type, const std::string &lexemVal);
			bool defTextToken(TokenXML& token);
			void skipSymbolUntil(char stopSymbol);
			std::string replacePredefXMLEntity(const SmartBuffer::IteratorSmartB &begin, const SmartBuffer::IteratorSmartB &end);
		
			LexerXML(const LexerXML& lexerXML) = delete;
			LexerXML& operator=(const LexerXML& lexerXML) = delete;
		public:
			LexerXML();
			explicit LexerXML(const std::string &fileName, SymbolTableXML *symbolTable);
			bool init(const std::string &fileName, SymbolTableXML *symbolTable);
			void reset();
			bool isInit() const;
			TokenXML getNextToken();
			TokenXML getCurToken();
			size_t getLinePositionToken() { return m_CountNewLine; }
	};
}

#endif // LEXERXML_H_