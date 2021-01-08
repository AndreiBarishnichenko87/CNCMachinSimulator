
#include "pub/smartBuffer.h"

#define EOF_BUF std::char_traits<char>::eof()

namespace parserXML
{
	SmartBuffer::SmartBuffer(const std::string FileName)
	{
		m_Fin.open(FileName, std::ios_base::in);
		if(!m_Fin)
			throw SmartBufferException(std::string("file is invalid: " + FileName));

		// add new chunk and fill the data
		if(!addNewChunk())
			throw SmartBufferException("fail to add new chunk in SmartBuffer");
		unsigned int read = fillChunkData(m_ListOfChunks.begin());
		if (read == 0)
			throw SmartBufferException(std::string("file is empty: " + FileName));

		updateCurChunkPtr(m_ListOfChunks.begin(), read);
	}

	unsigned int SmartBuffer::fillChunkData(const std::list<Chunk*>::iterator& iter)
	{
		// the buffer that doesn't fill intier sets the last element EOF
		char* bufIter = (*iter)->mBuffer;
		m_Fin.read(bufIter, CHUNK_SIZE);

		return m_Fin.gcount();
	}

	bool SmartBuffer::addNewChunk()
	{
		SmartBuffer::Chunk* newChank = new Chunk;
		if(newChank == nullptr)
		{
			return false;
		}
		newChank->mCountUsersChank = 0;
		m_ListOfChunks.push_back(newChank);
		return true;
	}

	void SmartBuffer::updateCurChunkPtr(const std::list<Chunk*>::iterator& iter, unsigned int countRead)
	{
		m_ChunkBuf = iter;
		m_BeginChunkBuf = (*m_ChunkBuf)->mBuffer;
	
		if(countRead < CHUNK_SIZE) {
			if(countRead != 0)
				m_EndChunkBuf = m_BeginChunkBuf + countRead;
			else
				m_EndChunkBuf = m_BeginChunkBuf;
			*m_EndChunkBuf = EOF_BUF;
		} else {
			m_EndChunkBuf = m_BeginChunkBuf + (countRead - 1);
		}
	}
	
	SmartBuffer::IteratorSmartB SmartBuffer::begin()
	{
		return SmartBuffer::IteratorSmartB(this, m_ChunkBuf, m_BeginChunkBuf, m_EndChunkBuf);
	}

	void SmartBuffer::removeUnusedChunk()
	{
		std::list<Chunk*>::iterator removeIter = m_ListOfChunks.begin();
		if(getChunksUser(removeIter) == 0)
		{
			unsigned int maxRemoveElement = m_ListOfChunks.size();
			for(; maxRemoveElement > 2; --maxRemoveElement)
			{
				if(getChunksUser(removeIter) == 0)
				{
					delete *removeIter;
					removeIter = m_ListOfChunks.erase(removeIter);
				}
				else
					break;
			}
		}
	}


	SmartBuffer::~SmartBuffer()
	{
		if(!m_ListOfChunks.empty())
		{
			for(SmartBuffer::Chunk* ptr : m_ListOfChunks)
			{
				delete ptr;
			}
		}
	}

	// ITERATOR SMARTBUFFER MEMBERS FUNCTIONS

	SmartBuffer::IteratorSmartB::IteratorSmartB(SmartBuffer* bindBuffer, 
	const std::list<SmartBuffer::Chunk*>::iterator& iter, 
	char* beginPtr, char* endPtr)
		: m_BindBuffer(bindBuffer)
	{
		m_ChunkIter = iter;
		m_BeginChunkIter = beginPtr;
		m_EndChunkIter  = endPtr;
		m_BindBuffer->addChunksUser(m_ChunkIter);
	}

	SmartBuffer::IteratorSmartB::IteratorSmartB(const SmartBuffer::IteratorSmartB& iter)
		: m_BindBuffer(iter.m_BindBuffer)
	{
		m_ChunkIter = iter.m_ChunkIter;
		m_BeginChunkIter = iter.m_BeginChunkIter;
		m_EndChunkIter  = iter.m_EndChunkIter;
		m_BindBuffer->addChunksUser(m_ChunkIter);
	}

	const SmartBuffer::IteratorSmartB& SmartBuffer::IteratorSmartB::operator=(const SmartBuffer::IteratorSmartB& iter)
	{
		if (this != &iter)
		{
			m_BindBuffer->subChunksUser(m_ChunkIter);
			
			m_BindBuffer = iter.m_BindBuffer;
			m_ChunkIter = iter.m_ChunkIter;
			m_BeginChunkIter = iter.m_BeginChunkIter;
			m_EndChunkIter  = iter.m_EndChunkIter;
			
			m_BindBuffer->addChunksUser(m_ChunkIter);

			m_BindBuffer->removeUnusedChunk();
		}
		return *this;
	}

	SmartBuffer::IteratorSmartB::~IteratorSmartB()
	{
		m_BindBuffer->subChunksUser(m_ChunkIter);
		m_BindBuffer->removeUnusedChunk();
	}

	SmartBuffer::IteratorSmartB& SmartBuffer::IteratorSmartB::operator++()
	{

		if(m_BeginChunkIter == m_BindBuffer->m_BeginChunkBuf)
		{
			// BUFFER AND ITERATOR ARE SINHRONIZED
			if(m_BeginChunkIter != m_EndChunkIter)
			{
				++m_BeginChunkIter;
				m_BindBuffer->m_BeginChunkBuf = m_BeginChunkIter;
			}
			else
			{
				std::list<SmartBuffer::Chunk*>::iterator newIter = m_BindBuffer->m_ChunkBuf;
				if(m_BindBuffer->getCountChunk() == 2)
				{
					if(m_ChunkIter == m_BindBuffer->m_ListOfChunks.begin())
					{
						if(m_BindBuffer->getChunksUser(m_ChunkIter) > 1)
							++newIter;
					}
					else
					{
						if((*(m_BindBuffer->m_ListOfChunks.begin()))->mCountUsersChank > 0)
						{
							if(!m_BindBuffer->addNewChunk())
								throw SmartBufferException("fail to add new chunk in SmartBuffer");
							++newIter;
						}
						else
						{
							if(m_BindBuffer->getChunksUser(m_ChunkIter) > 1)
							{
								if(!m_BindBuffer->addNewChunk())
									throw SmartBufferException("fail to add new chunk in SmartBuffer");
								++newIter;
							}
							else
							{
								newIter = m_BindBuffer->m_ListOfChunks.begin();
							}
						}
					}
				}
				else
				{
					if(!m_BindBuffer->addNewChunk())
						throw SmartBufferException("fail to add new chunk in SmartBuffer");
					++newIter;
				}
				m_BindBuffer->updateCurChunkPtr(newIter, m_BindBuffer->fillChunkData(newIter));
				*this = m_BindBuffer->begin();
				m_BindBuffer->removeUnusedChunk();
			}
		}
		else
		{
			if(m_BeginChunkIter != m_EndChunkIter)
			{
				++m_BeginChunkIter;
			}
			else
			{
				m_BindBuffer->subChunksUser(m_ChunkIter);
				++m_ChunkIter;
				m_BindBuffer->addChunksUser(m_ChunkIter);
				m_BeginChunkIter = (*m_ChunkIter)->mBuffer;
				if(m_ChunkIter == m_BindBuffer->m_ChunkBuf)
				{
					m_EndChunkIter = m_BindBuffer->m_EndChunkBuf;
				}
				else
				{
					m_EndChunkIter = m_BeginChunkIter + (CHUNK_SIZE - 1);
				}
				m_BindBuffer->removeUnusedChunk();
			}
		}

		return *this;
	}

	// postfix increment
	SmartBuffer::IteratorSmartB SmartBuffer::IteratorSmartB::operator++(int)
	{
		SmartBuffer::IteratorSmartB prevIter = *this;
		++*this;
		return prevIter;
	}

	bool SmartBuffer::IteratorSmartB::operator==(const SmartBuffer::IteratorSmartB& rightVal)
	{
		return m_BeginChunkIter == rightVal.m_BeginChunkIter;
	}

	bool SmartBuffer::IteratorSmartB::operator!=(const SmartBuffer::IteratorSmartB& rightVal)
	{
		return !(*this == rightVal);
	}

}

#undef EOF_BUF
