#ifndef SMARTBUFFER_H_
#define SMARTBUFFER_H_

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <memory>
#include <utility>
#include <stdexcept>
#include <iterator>

#include "exceptionParserXML.h"

namespace parserXML {

	class SmartBuffer {
		public:
			class IteratorSmartB;
			enum {CHUNK_SIZE = 4096};
		private:
			struct Chunk
			{
				char mBuffer[CHUNK_SIZE];
				unsigned int mCountUsersChank;
			};
			std::ifstream m_Fin;
			std::list<Chunk*> m_ListOfChunks;
			std::list<Chunk*>::iterator m_ChunkBuf;
			char *m_BeginChunkBuf, *m_EndChunkBuf;
		private:
			void updateCurChunkPtr(const std::list<Chunk*>::iterator &iter, unsigned int countRead);
			bool addNewChunk();
			unsigned int fillChunkData(const std::list<Chunk*>::iterator &iter);
			void removeUnusedChunk();
			unsigned int addChunksUser(std::list<Chunk*>::iterator &iter) { return ++((*iter)->mCountUsersChank); }
			unsigned int subChunksUser(std::list<Chunk*>::iterator &iter) { return --((*iter)->mCountUsersChank); }
			unsigned int getChunksUser(std::list<Chunk*>::iterator &iter) { return (*iter)->mCountUsersChank; }
		
			SmartBuffer(const SmartBuffer& smartBuffer) = delete;
			SmartBuffer& operator=(const SmartBuffer& smartBuffer) = delete;
		public:
			explicit SmartBuffer(const std::string fileName);
			SmartBuffer();
			~SmartBuffer();
		public:
			bool init(const std::string &fileName);
			void reset();
			bool isInit() const;
			IteratorSmartB begin();
			
		public:
		
			class IteratorSmartB : public std::iterator<std::input_iterator_tag, char>
			{
				friend class SmartBuffer;
			private:
				SmartBuffer *m_BindBuffer;
				std::list<Chunk*>::iterator m_ChunkIter;
				char *m_BeginChunkIter, *m_EndChunkIter;
			private:
				IteratorSmartB	(SmartBuffer *bindBuffer, const std::list<Chunk*>::iterator &iter, 
				char* beginPtr, char* endPtr);
				const Chunk* operator->() { return *m_ChunkIter; }
			public:
				IteratorSmartB();
				IteratorSmartB(const IteratorSmartB &iter);
				const IteratorSmartB& operator=(const IteratorSmartB &iter);
				~IteratorSmartB	();
				char operator*() const { return *m_BeginChunkIter; }
				IteratorSmartB& operator++();
				IteratorSmartB operator++(int);
				bool operator==(const IteratorSmartB &rightVal);
				bool operator!=(const IteratorSmartB &rightVal);
				explicit operator bool() const noexcept { return m_BindBuffer == nullptr ? false : true;}
				char* c_ptr() { return m_BeginChunkIter; }
			};
	};
}

#endif //SMARTBUFFER_H_