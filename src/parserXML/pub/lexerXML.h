#ifndef LEXERXML_H_
#define LEXERXML_H_

#include <memory>
#include <string>
#include "smartBuffer.h"

namespace parserXML {

	struct TokenXML {
		enum token_XML {
			UNDEFINE_TOKEN = 0,
			OPEN_TAG,
			OPEN_CLOSE_TAG,
			OPEN_PROLOG_TAG,
			FINAL_CLOSE_TAG,
			CLOSE_TAG,
			EQUAL,
			CLOSE_PROLOG_TAG,
			TEXT,
			NAME_ID,
			ATRIBUTE_VALUE,
			END_OF_FILE,
			OPEN_COMENT_TAG,
			CLOSE_COMENT_TAG,
			CDATA_BEGIN,
			CDATA_END
		};
		static const char* token_name[];

		token_XML mTypeToken = token_XML::UNDEFINE_TOKEN;
		std::string mLexemValue = "";

		std::string tokenName() {
			return std::string(token_name[mTypeToken]);
		}
		std::string lexemValue() {
			return mLexemValue;
		}
	};

	class LexerXML {
		private:
			TokenXML::token_XML m_LastToken;
			std::shared_ptr<SmartBuffer> m_Buffer;
			SmartBuffer::IteratorSmartB m_LexemBegin;
			SmartBuffer::IteratorSmartB m_Forward;

		public:
			using token_t = TokenXML::token_XML;

		private:
			void setToken(TokenXML& token, token_t tok_type);
			bool defTextToken(TokenXML& token);
			void skipSymbolUntil(char stopSymbol);
			void replacePredefXMLEntity(std::string& str);
		public:
			explicit LexerXML(const std::shared_ptr<SmartBuffer>& ptrBuffer);
			TokenXML getNextToken();
	};
}

#endif // LEXERXML_H_