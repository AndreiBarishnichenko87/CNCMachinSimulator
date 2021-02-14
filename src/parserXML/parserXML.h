#ifndef PARSERXML_H_
#define PARSERXML_H_

#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <limits> 

#include "lexerXML.h"
#include "exceptionParserXML.h"

namespace parserXML{
	
	class ParserXML;
	
	class ElementXML {
		friend ParserXML;
	private:
		class TreeElementXML {
		public:
			struct AttributeNode {
				std::string mNameAttrib;
				std::string mValueAttrib;
			};
			class DataNode {
			public:
			private:
				std::string mName;
				std::string mValue;
				std::vector<AttributeNode> mListAttrib;
			public:
				void setNodeName(const std::string &nameNode) { mName = nameNode; }
				void setNodeVal(const std::string &valueNode) { mValue = valueNode; }
				void addAttribute(const AttributeNode &attribNode) { mListAttrib.push_back(attribNode); }
				void addAttribWhithSwap(AttributeNode &attribNode);
				
				std::string& getNodeNameRef() { return mName; }
				std::string& getNodeValueRef() { return mValue; }
				std::vector<AttributeNode>& getListAttrib() { return mListAttrib; }
			};
		private:
			struct DataNodeID {
				size_t mNameNodeID = std::numeric_limits<size_t>::max();
				size_t mValueNodeID = std::numeric_limits<size_t>::max();
				std::vector<std::pair<size_t, size_t> > mListAttribID;
			};
			std::vector<std::pair<DataNodeID, std::vector<size_t> > > m_TreeElements;
			std::vector<std::string> m_ListName;
			std::vector<std::string> m_ListAttribVal;
			std::vector<std::string> m_ListVal;
		
		private:
			size_t addName(const std::string &name);
			size_t addValue(const std::string &value);
			size_t addAttribValue(const std::string &attribValue);
		private:
			TreeElementXML(const TreeElementXML &treeElementXML) = delete;
			TreeElementXML& operator=(const TreeElementXML &treeElementXML) = delete;
		public:
			TreeElementXML() {}
			void resetTreeElements();
			
		// set methods
		public:
			size_t getRootNode() const { return m_TreeElements.empty() ? std::numeric_limits<size_t>::max() : 1; }
			size_t addNewNode(size_t nodeID);
			void fillNodeData(size_t nodeID, DataNode &DataNode);
		// get methods
		public:
			std::string& getName(size_t nodeID) { return m_ListName[std::get<0>(m_TreeElements[nodeID]).mNameNodeID]; }
			size_t countChildNode(size_t nodeID) { return std::get<1>(m_TreeElements[nodeID]).size(); }
			size_t getChildNode(size_t nodeID, size_t numChildNode);
			size_t countAttributeNode(size_t nodeID) const;
			const std::string& getAttribNameNum(size_t nodeID, size_t numAttrib);
			const std::string& getAttribValNum(size_t nodeID, size_t numAttrib);
			bool nodeValueExist(size_t nodeID) const { 
				return (std::get<0>(m_TreeElements[nodeID]).mValueNodeID == std::numeric_limits<size_t>::max()) ? false : true;
			}
			const std::string& getNodeValue(size_t nodeID) { return m_ListVal[std::get<0>(m_TreeElements[nodeID]).mValueNodeID]; }
		};
	private:
		size_t m_NodeID;
		std::shared_ptr<TreeElementXML> m_TreeOfElements;
	public:
		ElementXML() : m_NodeID(std::numeric_limits<size_t>::max()){ }
		ElementXML getRootElement() const { ElementXML element(*this); element.m_NodeID = 0; return element; }
		void reset() { m_NodeID = std::numeric_limits<size_t>::max(); m_TreeOfElements.reset(); }
		operator bool() const { return (m_NodeID == std::numeric_limits<size_t>::max() && !m_TreeOfElements) ? false : true; }
	public:
		const std::string& elementName() { return m_TreeOfElements->getName(m_NodeID);}
		size_t countChildElements() const { return m_TreeOfElements->countChildNode(m_NodeID); }
		ElementXML childElementNum(size_t num);
		size_t countAttribute() const { return m_TreeOfElements->countAttributeNode(m_NodeID); }
		const std::string& attribNameNum(size_t numAttribute) const { return m_TreeOfElements->getAttribNameNum(m_NodeID ,numAttribute); }
		const std::string& attribValueNum(size_t numAttribute) const { return m_TreeOfElements->getAttribValNum(m_NodeID ,numAttribute); }
		bool valueExist() const { return m_TreeOfElements->nodeValueExist(m_NodeID); }
		const std::string& elementValue() { return m_TreeOfElements->getNodeValue(m_NodeID); }
	};
	
	class ParserXML
	{
	private:
		using type_token = TokenXML::TokenType;
		LexerXML m_Lexer;
		std::shared_ptr<ElementXML::TreeElementXML> m_TreeXML;
	private:
		void buildTreeXML();
		size_t element(size_t elementID);
		void attribute(ElementXML::TreeElementXML::DataNode &node);
		void closeElement(size_t elementID, ElementXML::TreeElementXML::DataNode &node);
		void endElement(size_t elementID, ElementXML::TreeElementXML::DataNode &node);
		void skipelement();
	private:
		void errorHandleParser(const std::string &subMsgExcept);
	private:
		ParserXML(const ParserXML& parserXML) = delete;
		ParserXML& operator=(const ParserXML& parserXML) = delete;
	public:
		ParserXML() { }
		ParserXML(const std::string& fileName) : m_Lexer(fileName) { }
		bool bindFile(const std::string &fileName) { unbindFile(); return m_Lexer.init(fileName) ? true : false; }
		void unbindFile() { m_Lexer.reset(); m_TreeXML.reset(); }
		void parse();
		ElementXML getRootElement() const;
	};
}

#endif //PARSERXML_H_