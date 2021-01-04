#ifndef SMARTBUFFER_H_
#define SMARTBUFFER_H_

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <memory>
#include <utility>
#include <stdexcept>

namespace parserXML {
	
	class SmartBufferException : public std::runtime_error {
	public:
		SmartBufferException(const std::string msg) : std::runtime_error(msg) { }
	};
	
	class SmartBuffer {
		enum {CHUNK_SIZE = 4096};
			
		private:
			struct Chunk {
				char mBuffer[CHUNK_SIZE];
				unsigned int mCountUsersChank; };
				
			std::ifstream m_Fin;
			std::list<Chunk*> m_ListOfChunks;
			std::list<Chunk*>::iterator m_CurChunk;
			char *m_CurPosInChunk;
			
		public:
			class IteratorSmartB {
				friend class SmartBuffer;
				private:
					SmartBuffer *m_BindBuffer;
					std::list<Chunk*>::iterator m_IterChunk;
					char *m_CurrIterPos, *m_EndIterPos;
				private:
					explicit IteratorSmartB(SmartBuffer *bindBuffer);
				public:
					IteratorSmartB(const IteratorSmartB &iter);
					const IteratorSmartB& operator=(const IteratorSmartB &iter);
					~IteratorSmartB();
				// ACCESS MEMBERS FUNCTION
					char operator*() const { return *m_CurrIterPos; }
					IteratorSmartB& operator++();
					IteratorSmartB operator++(int);
					
				// RELETIONSHIP OPERATORS
					bool operator==(const IteratorSmartB &rightVal);
					bool operator!=(const IteratorSmartB &rightVal);
					
				// DEBUGER FUNCTIONS
			unsigned int getChunksUsers() const { return (*m_IterChunk)->mCountUsersChank; }
			};

		public:
			explicit SmartBuffer(const std::string FileName);
			~SmartBuffer();
			
		private:
			bool addNewChunk();
			unsigned int fillChunkData(const std::list<Chunk*>::iterator &iter);
			
		public:
			unsigned int getSizeChunk() const	{	return CHUNK_SIZE; }
			unsigned int getCountChunk() const { return m_ListOfChunks.size(); }
			unsigned int getChunksUsers() const { return (*m_CurChunk)->mCountUsersChank; }
			IteratorSmartB begin();
	};

}

#endif //SMARTBUFFER_H_