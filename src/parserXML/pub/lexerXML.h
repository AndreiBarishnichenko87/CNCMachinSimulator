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
			token_t m_LastToken_t;
			TokenXML m_PreviousToken;
			std::shared_ptr<SmartBuffer> m_Buffer;
			SmartBuffer::IteratorSmartB m_LexemBegin;
			SmartBuffer::IteratorSmartB m_Forward;
			size_t m_CountNewLine;

		private:
			void setToken(TokenXML& token, token_t tok_type);
			void setToken(TokenXML& token, token_t tok_type, const std::string &lexemVal);
			bool defTextToken(TokenXML& token);
			void skipSymbolUntil(char stopSymbol);
			std::string replacePredefXMLEntity(const SmartBuffer::IteratorSmartB &begin, const SmartBuffer::IteratorSmartB &end);
		
		public:
			explicit LexerXML(const std::shared_ptr<SmartBuffer>& ptrBuffer, SymbolTableXML *symbolTable);
			TokenXML getNextToken();
			TokenXML getCurToken();
			size_t getLinePositionToken() { return m_CountNewLine; }
	};
}

#endif // LEXERXML_H_