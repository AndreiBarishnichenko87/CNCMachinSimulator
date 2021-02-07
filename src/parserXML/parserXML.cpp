#include <iostream>
#include <memory>
#include <sstream>

#include "pub/parserXML.h"

namespace parserXML {
	
	// ELEMENTXML METHODS

	ElementXML ElementXML::childElementNum(size_t num) {
		ElementXML element(*this);
		element.m_NodeID = m_TreeOfElements->getChildNode(m_NodeID, num);
		return element;
	}
	
	// TREEELEMENTXML METHODS
	
	size_t ElementXML::TreeElementXML::getChildNode(size_t nodeID, size_t numChildNode) {
		if(numChildNode < countChildNode(nodeID)){
			return std::get<1>(m_TreeElements[nodeID])[numChildNode];
		}
		else
			throw TreeElementXMLException(std::string("out of range getChildNode(...)"));
	}
	
	size_t ElementXML::TreeElementXML::countAttributeNode(size_t nodeID) const {
		return std::get<0>(m_TreeElements[nodeID]).mListAttribID.size();
	}
	
	const std::string& ElementXML::TreeElementXML::getAttribNameNum(size_t nodeID, size_t numAttrib) {
		if(numAttrib < countAttributeNode(nodeID)){
			return m_ListName[std::get<0>(std::get<0>(m_TreeElements[nodeID]).mListAttribID[numAttrib])];
		}
		else 
			throw TreeElementXMLException(std::string("out of range getAttribNameNum(...)"));
	}
	
	const std::string& ElementXML::TreeElementXML::getAttribValNum(size_t nodeID, size_t numAttrib) {
		if(numAttrib < countAttributeNode(nodeID)){
			return m_ListAttribVal[std::get<1>(std::get<0>(m_TreeElements[nodeID]).mListAttribID[numAttrib])];
		}
		else 
			throw TreeElementXMLException(std::string("out of range getAttribValNum(...)"));
	}
	
	void ElementXML::TreeElementXML::resetTreeElements() {
		std::vector<std::pair<DataNodeID, std::vector<size_t> > >().swap(m_TreeElements);
		std::vector<std::string>().swap(m_ListName);
		std::vector<std::string>().swap(m_ListAttribVal);
		std::vector<std::string>().swap(m_ListVal);
	}

	size_t ElementXML::TreeElementXML::addNewNode(size_t nodeID) {
		if(m_TreeElements.empty()) {
			m_TreeElements.push_back(std::make_pair(DataNodeID(), std::vector<size_t>()));
			return 0;
		} else {
			if(nodeID < m_TreeElements.size()) {
				m_TreeElements.push_back(std::make_pair(DataNodeID(), std::vector<size_t>()));
				std::get<1>(m_TreeElements[nodeID]).push_back(m_TreeElements.size() - 1);
				return m_TreeElements.size() - 1;
			} else {
				return std::numeric_limits<size_t>::max();
			}
		}
	}
	
	size_t ElementXML::TreeElementXML::addName(const std::string &name) {
		if(m_ListName.empty()) {
			m_ListName.push_back(name);
			return m_ListName.size() - 1;
		} else {
			for(size_t i = 0; i < m_ListName.size(); ++i){
				if(m_ListName[i] == name) {
					return i;
				}
			}
			m_ListName.push_back(name);
			return m_ListName.size() - 1;
		}
	}
	
	size_t ElementXML::TreeElementXML::addValue(const std::string &value) {
		if(value.empty()) {
			return std::numeric_limits<size_t>::max();
		} else {
			m_ListVal.push_back(value);
			return m_ListVal.size() - 1;			
		}
	}
	
	size_t ElementXML::TreeElementXML::addAttribValue(const std::string &attribValue) {
		m_ListAttribVal.push_back(attribValue);
		return m_ListAttribVal.size() - 1;
	}
	
	void ElementXML::TreeElementXML::fillNodeData(size_t nodeID, DataNode &DataNode) {
		if(nodeID >= m_TreeElements.size()) {
			throw TreeElementXMLException(std::string("out of range nodeID"));
		}
		else {
			std::get<0>(m_TreeElements[nodeID]).mNameNodeID = addName(DataNode.getNodeNameRef());
			std::get<0>(m_TreeElements[nodeID]).mValueNodeID = addValue(DataNode.getNodeValueRef());
			
			std::vector<ElementXML::TreeElementXML::AttributeNode> &listAttrib = DataNode.getListAttrib();
			if(listAttrib.empty()) return;
			for(auto iter = listAttrib.begin(); iter != listAttrib.end(); ++iter) {
				std::pair<size_t, size_t> pairAttrib = std::make_pair(addName(iter->mNameAttrib), addAttribValue(iter->mValueAttrib));
				std::get<0>(m_TreeElements[nodeID]).mListAttribID.push_back(pairAttrib);
			}
		}
	}

	void ElementXML::TreeElementXML::DataNode::addAttribWhithSwap(AttributeNode &attribNode) {
		mListAttrib.push_back(AttributeNode());
		mListAttrib.back().mNameAttrib.swap(attribNode.mNameAttrib);
		mListAttrib.back().mValueAttrib.swap(attribNode.mValueAttrib);
	}
	
	// PARSERXML METHODS
	
	ElementXML ParserXML::getRootElement() const {
		ElementXML rootTree;
		rootTree.m_NodeID = 0;
		rootTree.m_TreeOfElements = m_TreeXML;
		return rootTree;
	}

	void ParserXML::errorHandleParser(const std::string &subMsgExcept){
		std::ostringstream msgExcept;
		msgExcept << "\nexpext token in proc " << subMsgExcept << std::endl;
		msgExcept << "syntaxError in line number# " << m_Lexer.getLinePositionToken() 
							<< "\ncurent token: " 		<< token_enum_name(m_Lexer.getCurToken()) << std::endl
							<< "next token: 		" 		<< token_enum_name(m_Lexer.getNextToken()) << std::endl;
		throw ParserXMLException(msgExcept.str());
	}
	
	void ParserXML::parse() {
		m_TreeXML.reset(new ElementXML::TreeElementXML);
		buildTreeXML(); // skip prolog of xmlFile
	}
	
	void ParserXML::buildTreeXML(){
		element(0);
	}
	
	void ParserXML::skipelement(){
		ElementXML::TreeElementXML::DataNode node;
		switch(m_Lexer.getCurToken().m_Type){
			case type_token::OPEN_PROLOG_TAG:
				if(m_Lexer.getNextToken().m_Type == type_token::NAME_ID){
					m_Lexer.getNextToken();
				} else {
					// error procedure
					errorHandleParser("skipelement(): NAME_ID");
				}
				attribute(node);
				if(m_Lexer.getCurToken().m_Type == type_token::CLOSE_PROLOG_TAG){
					m_Lexer.getNextToken();
					skipelement();
				} else {
					// error procedure
					errorHandleParser("skipelement(): CLOSE_PROLOG_TAG");
				}
				break;
			
			case type_token::CDATA_BEGIN:
				if(m_Lexer.getNextToken().m_Type == type_token::TEXT){
					if(m_Lexer.getNextToken().m_Type == type_token::CDATA_END){
						m_Lexer.getNextToken();
						skipelement();
					} else {
						// error procedure
						errorHandleParser("skipelement(): CDATA_END");
					}
				} else {
					// error procedure
					errorHandleParser("skipelement(): TEXT");
				}
				break;
			
			case type_token::OPEN_COMENT_TAG:
				if(m_Lexer.getNextToken().m_Type == type_token::TEXT){
					if(m_Lexer.getNextToken().m_Type == type_token::CLOSE_COMENT_TAG){
						m_Lexer.getNextToken();
						skipelement();
					} else {
						// error procedure
						errorHandleParser("skipelement(): CLOSE_COMENT_TAG");
					}
				} else {
					// error procedure
					errorHandleParser("skipelement(): TEXT");
				}
				break;
			
			case type_token::DOCTYPE:
				if(m_Lexer.getNextToken().m_Type == type_token::TEXT){
					if(m_Lexer.getNextToken().m_Type == type_token::CLOSE_TAG){
						m_Lexer.getNextToken();
						skipelement();
					} else {
						// error procedure
						errorHandleParser("skipelement(): DOCTYPE => CLOSE_TAG");
					}
				} else {
					// error procedure
					errorHandleParser("skipelement(): DOCTYPE => TEXT");
				}
				break;
				
			default:
				return;
		}
	}
	
	size_t ParserXML::element(size_t elementID){
		size_t NodeID = m_TreeXML->addNewNode(elementID);
		ElementXML::TreeElementXML::DataNode node;
		skipelement();
		switch(m_Lexer.getCurToken().m_Type){
			case type_token::OPEN_TAG:
				if(m_Lexer.getNextToken().m_Type == type_token::NAME_ID){
					node.setNodeName(m_Lexer.getCurToken().m_Lexem);
					m_Lexer.getNextToken();
					attribute(node);
					closeElement(NodeID, node);
					m_TreeXML->fillNodeData(NodeID, node);
				} else {
					// error procedure
					errorHandleParser("element(): NAME_ID");
				}
				break;
				
			case type_token::END_OF_FILE:
			
			default:
				// error procedure
				errorHandleParser("element(): default switch");
				break;
		}
		return NodeID;
	}

	void ParserXML::attribute(ElementXML::TreeElementXML::DataNode &node) {
		if(m_Lexer.getCurToken().m_Type == type_token::NAME_ID){
			ElementXML::TreeElementXML::AttributeNode attrib;
			m_Lexer.getCurToken().m_Lexem.swap(attrib.mNameAttrib);
			if(m_Lexer.getNextToken().m_Type == type_token::EQUAL){
				if(m_Lexer.getNextToken().m_Type == type_token::ATRIBUTE_VALUE){
					m_Lexer.getCurToken().m_Lexem.swap(attrib.mValueAttrib);
					m_Lexer.getNextToken();
					node.addAttribWhithSwap(attrib);
					attribute(node);
				} else {
					// error procedure
					errorHandleParser("attribute(): ATRIBUTE_VALUE");
					return;
				}
			} else {
				// error procedure
				errorHandleParser("attribute(): EQUAL");
				return;
			}
		} 
	}

	void ParserXML::closeElement(size_t elementID, ElementXML::TreeElementXML::DataNode &node){
		if(m_Lexer.getCurToken().m_Type == type_token::FINAL_CLOSE_TAG){
			m_Lexer.getNextToken();
		} else {
			if(m_Lexer.getCurToken().m_Type == type_token::CLOSE_TAG){
				m_Lexer.getNextToken();
				endElement(elementID, node);
			} else {
				// error procedure
				errorHandleParser("closeElement(): FINAL_CLOSE_TAG");
				return;
			}
		}
	}
	
	void ParserXML::endElement(size_t elementID, ElementXML::TreeElementXML::DataNode &node){
		skipelement();
		switch(m_Lexer.getCurToken().m_Type){
			case type_token::TEXT:
				node.setNodeVal(m_Lexer.getCurToken().m_Lexem);
				if(m_Lexer.getNextToken().m_Type == type_token::OPEN_CLOSE_TAG){
					if(m_Lexer.getNextToken().m_Type == type_token::NAME_ID){
						if(node.getNodeNameRef() != m_Lexer.getCurToken().m_Lexem)
							throw ParserXMLException(std::string("end tag name is different than begin"));
						if(m_Lexer.getNextToken().m_Type == type_token::CLOSE_TAG){
							m_Lexer.getNextToken();
						} else {
							// error procedure
							errorHandleParser("endElement(): case TEXT: CLOSE_TAG");
							return;
						}
					} else {
						// error procedure
						errorHandleParser("endElement(): case TEXT: NAME_ID");
						return;
					}
				} else {
					// error procedure
					errorHandleParser("endElement(): case TEXT: OPEN_CLOSE_TAG");
					return;
				}
				break;
			
			case type_token::OPEN_CLOSE_TAG:
				if(m_Lexer.getNextToken().m_Type == type_token::NAME_ID){
					if(node.getNodeNameRef() != m_Lexer.getCurToken().m_Lexem)
						throw ParserXMLException(std::string("end tag name is different than begin"));
					if(m_Lexer.getNextToken().m_Type == type_token::CLOSE_TAG){
						m_Lexer.getNextToken();
					} else {
						// error procedure
						errorHandleParser("endElement(): case OPEN_CLOSE_TAG: CLOSE_TAG");
						return;
					}
				} else {
					// error procedure
					errorHandleParser("endElement(): case OPEN_CLOSE_TAG: NAME_ID");
					return;
				}
				break;

			 case type_token::OPEN_PROLOG_TAG:
			 case type_token::OPEN_TAG       :
			 case type_token::CDATA_BEGIN    :
			 case type_token::OPEN_COMENT_TAG:
				element(elementID);
				endElement(elementID, node);
				break;
				
			default:
				// error procedure
				errorHandleParser("endElement(): default");
				return;
		}
	}
}