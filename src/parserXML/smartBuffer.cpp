
#include "pub/smartBuffer.h"


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
		m_CurChunk = m_ListOfChunks.begin();

		if (fillChunkData(m_CurChunk) == 0)
			throw SmartBufferException(std::string("file is empty: " + FileName));
		m_CurPosInChunk = (char*)(*m_CurChunk);
	}

	unsigned int SmartBuffer::fillChunkData(const std::list<Chunk*>::iterator& iter)
	{

		m_Fin.read((*iter)->mBuffer, CHUNK_SIZE);

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

	SmartBuffer::~SmartBuffer()
	{

		if(!m_ListOfChunks.empty())
		{
			for(SmartBuffer::Chunk* ptr : m_ListOfChunks)
			{
				std::cout << "del chunk remove listChunk" << std::endl;
				delete ptr;
			}
		}

	}

	// ITERATOR SMARTBUFFER MEMBERS FUNCTIONS

	SmartBuffer::IteratorSmartB::IteratorSmartB(SmartBuffer* bindBuffer)
		: m_BindBuffer(bindBuffer)
	{
		m_IterChunk = m_BindBuffer->m_CurChunk;
		m_CurrIterPos = (char*)(*m_IterChunk);
		m_EndIterPos  = m_CurrIterPos + (CHUNK_SIZE - 1);
		(*m_IterChunk)->mCountUsersChank += 1;
	}

	SmartBuffer::IteratorSmartB::IteratorSmartB(const SmartBuffer::IteratorSmartB& iter)
		: m_BindBuffer(iter.m_BindBuffer)
	{
		m_IterChunk = iter.m_IterChunk;
		m_CurrIterPos = iter.m_CurrIterPos;
		m_EndIterPos  = iter.m_EndIterPos;
		(*m_IterChunk)->mCountUsersChank += 1;
	}

	const SmartBuffer::IteratorSmartB& SmartBuffer::IteratorSmartB::operator=(const SmartBuffer::IteratorSmartB& iter)
	{
		m_BindBuffer = iter.m_BindBuffer;
		m_IterChunk = iter.m_IterChunk;
		m_CurrIterPos = iter.m_CurrIterPos;
		m_EndIterPos  = iter.m_EndIterPos;
		(*m_IterChunk)->mCountUsersChank += 1;

		return *this;
	}

	SmartBuffer::IteratorSmartB::~IteratorSmartB()
	{
		--((*m_IterChunk)->mCountUsersChank);
		if(m_IterChunk == m_BindBuffer->m_ListOfChunks.begin())
		{
			unsigned int maxRemoveElement = m_BindBuffer->m_ListOfChunks.size();
			std::list<Chunk*>::iterator iter = m_IterChunk;
			for(; maxRemoveElement > 2; --maxRemoveElement)
			{
				std::cout << "del chunk from iter" << std::endl;
				if((*iter)->mCountUsersChank == 0)
				{
					delete *iter;
					iter = m_BindBuffer->m_ListOfChunks.erase(iter);
				}
				else
					break;
			}
		}
	}

	SmartBuffer::IteratorSmartB SmartBuffer::begin()
	{
		return SmartBuffer::IteratorSmartB(this);
	}

	SmartBuffer::IteratorSmartB& SmartBuffer::IteratorSmartB::operator++()
	{
		if(m_CurrIterPos == m_BindBuffer->m_CurPosInChunk)
		{
			if (m_CurrIterPos != m_EndIterPos)
				m_BindBuffer->m_CurPosInChunk = ++m_CurrIterPos;
			else
			{
				// add new chunk
				IteratorSmartB old = *this;
				if(m_BindBuffer->getCountChunk() == 2)
				{
					if(m_BindBuffer->m_CurChunk == m_BindBuffer->m_ListOfChunks.begin())
						m_BindBuffer->m_CurChunk = ++m_IterChunk;
					else if((*(m_BindBuffer->m_ListOfChunks.begin()))->mCountUsersChank == 0)
						m_BindBuffer->m_CurChunk = m_IterChunk = m_BindBuffer->m_ListOfChunks.begin();
					else
					{
						if(!m_BindBuffer->addNewChunk())
							throw SmartBufferException("fail to add new chunk in SmartBuffer");
						m_BindBuffer->m_CurChunk = ++m_IterChunk;
					}
				}
				else
				{
					if(!m_BindBuffer->addNewChunk())
						throw SmartBufferException("fail to add new chunk in SmartBuffer");
					m_BindBuffer->m_CurChunk = ++m_IterChunk;
				}

				m_CurrIterPos = m_BindBuffer->m_CurPosInChunk = (char*)*(m_BindBuffer->m_CurChunk);
				m_EndIterPos  = m_CurrIterPos + (CHUNK_SIZE - 1);
				(*m_IterChunk)->mCountUsersChank += 1;
				
				unsigned int countReadData = 0;
				if((countReadData = m_BindBuffer->fillChunkData(m_IterChunk)) < CHUNK_SIZE)
				{
					*(m_CurrIterPos + countReadData) = std::char_traits<char>::eof();
				}
				std::cout << "countReadData: " << countReadData << std::endl;
				
				(*(old.m_IterChunk))->mCountUsersChank -= 1;
			}
			
		}
		else
		{
			if (m_CurrIterPos != m_EndIterPos)
				++m_CurrIterPos;
			else
			{
				++m_IterChunk;
				m_CurrIterPos = (char*)(*m_IterChunk);
				m_EndIterPos  = m_CurrIterPos + CHUNK_SIZE + 1;
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
		return m_CurrIterPos == rightVal.m_CurrIterPos;
	}

	bool SmartBuffer::IteratorSmartB::operator!=(const SmartBuffer::IteratorSmartB& rightVal)
	{
		return m_CurrIterPos != rightVal.m_CurrIterPos;
	}

}