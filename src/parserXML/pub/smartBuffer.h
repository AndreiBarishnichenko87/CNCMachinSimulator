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

namespace parserXML {
	
	class SmartBufferException : public std::runtime_error {
	public:
		SmartBufferException(const std::string msg) : std::runtime_error(msg) { }
	};

	class SmartBuffer {
		public:
			class IteratorSmartB;
			enum {CHUNK_SIZE = 4096};
		private:
			struct Chunk 
			{
				char 					mBuffer[CHUNK_SIZE];
				unsigned int 	mCountUsersChank; 
			};
				
			std::ifstream 								m_Fin;
			std::list<Chunk*> 						m_ListOfChunks;
			std::list<Chunk*>::iterator		m_ChunkBuf;
			char 													*m_BeginChunkBuf, *m_EndChunkBuf;
			
		public:
			explicit SmartBuffer(const std::string FileName);
			~SmartBuffer();

		private:
			void 				 		updateCurChunkPtr	(const std::list<Chunk*>::iterator &iter, unsigned int countRead);
			bool 				 		addNewChunk				();
			unsigned int 		fillChunkData			(const std::list<Chunk*>::iterator &iter);
			void 				 		removeUnusedChunk	();
			unsigned int 		addChunksUser			(std::list<Chunk*>::iterator &iter) { return ++((*iter)->mCountUsersChank); }
			unsigned int 		subChunksUser			(std::list<Chunk*>::iterator &iter) { return --((*iter)->mCountUsersChank); }
			unsigned int 		getChunksUser			(std::list<Chunk*>::iterator &iter) { return (*iter)->mCountUsersChank; }
		
		public:
			unsigned int 		getSizeChunk			() const	{	return CHUNK_SIZE; }
			unsigned int 		getCountChunk			() const 	{ return m_ListOfChunks.size(); }
			IteratorSmartB 	begin							();
			
		public:
			class IteratorSmartB : public std::iterator<std::input_iterator_tag, char>
			{
			public:
				friend class 									SmartBuffer;
				SmartBuffer 									*m_BindBuffer;
				std::list<Chunk*>::iterator 	m_ChunkIter;
				char 													*m_BeginChunkIter, *m_EndChunkIter;
			private:
																			IteratorSmartB	(SmartBuffer *bindBuffer, const std::list<Chunk*>::iterator &iter);
				const Chunk* 									operator->() 		{ return *m_ChunkIter; }
			public:			
																			IteratorSmartB	(const IteratorSmartB &iter);
				const IteratorSmartB& 				operator=				(const IteratorSmartB &iter);
																			~IteratorSmartB	();
				char 													operator*				() const { return *m_BeginChunkIter; }
				IteratorSmartB& 							operator++			();
				IteratorSmartB 								operator++			(int);
				bool 													operator==			(const IteratorSmartB &rightVal);
				bool 													operator!=			(const IteratorSmartB &rightVal);				
			};
	};

}

#endif //SMARTBUFFER_H_