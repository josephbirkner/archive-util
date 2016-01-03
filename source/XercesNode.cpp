#pragma hdrstop

#include "stdafx.h"

#include "Base/ArchiveUtil/GlobExport/XercesNode.hpp"

#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNodeList.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLString.hpp>

using namespace xercesc;

namespace Archiving
{
	namespace Xerces
	{
		/** Con/Destructor */

		Node::Node(Node *pParentNode, xercesc::DOMDocument *pDOMDocument, std::string sName)
			: m_pDocument(pDOMDocument)
		{
			assert(pDOMDocument);
			
			XMLCh* xml_tagname = XMLString::transcode(sName.c_str());
			
			setDOMElement(m_pDocument->createElement(xml_tagname));
			setParent(pParentNode);
			
			XMLString::release(&xml_tagname);

			if (getParent())
				((DOMNode*)pParentNode->getDOMElement())->appendChild((DOMNode*)this->getDOMElement());
		}

		Node::Node(Node *pParentNode, xercesc::DOMDocument *pDOMDocument, xercesc::DOMElement *pDOMElement, bool bNew /*=true*/)
			: m_pDocument(pDOMDocument), m_pElement(pDOMElement)
		{
			assert(pDOMDocument);
			assert(pDOMElement);

			setParent(pParentNode);

			if (bNew && getParent())
				((xercesc::DOMNode*)pParentNode->getDOMElement())->appendChild((xercesc::DOMNode*)this->getDOMElement());
		}

		Node::~Node()
		{
			m_pDocument = NULL;
			setDOMElement(NULL);
			setParent(NULL);
		}

		/** Accessors */

		std::string Node::getTagName()
		{
			char *xml_tagname = XMLString::transcode(m_pElement->getTagName());
			std::string sRet = xml_tagname;

			XMLString::release(&xml_tagname);
			return sRet;
		}

		std::string Node::getAttribute(const std::string& sKey)
		{
			XMLCh *xml_key = XMLString::transcode(sKey.c_str());
			char *xml_attribute = XMLString::transcode(m_pElement->getAttribute(const_cast<const XMLCh *>(xml_key)));
			
			std::string sRet = xml_attribute;
			
			XMLString::release(&xml_attribute);
			XMLString::release(&xml_key);
			
			return sRet;
		}

		void Node::setAttribute(const std::string& sKey, const std::string& sValue)
		{
			XMLCh *xml_key   = XMLString::transcode(sKey.c_str());
			XMLCh *xml_value = XMLString::transcode(sValue.c_str());
			m_pElement->setAttribute(const_cast<const XMLCh *>(xml_key), const_cast<const XMLCh *>(xml_value));

			XMLString::release(&xml_key);
			XMLString::release(&xml_value);
		}

		/* Returns the inner content of a XML-Tag as std::string */
		std::string Node::getValue()
		{
			char *xml_value = XMLString::transcode(m_pElement->getTextContent());

			std::string sValue = xml_value ? xml_value : "";
			XMLString::release(&xml_value);
			
			return sValue;
		}
		
		/* Sets the inner content of a XML-Tag */
		void Node::setValue(std::string sValue)
		{
			XMLCh *xml_value = XMLString::transcode(sValue.c_str());
			m_pElement->setTextContent(xml_value);

			XMLString::release(&xml_value);
		}
		
		/* Returns a child (search-depth = 1) for the given tag-name or NULL if it doesnt exist */
		INode* Node::getChild(const std::string& sKey, const std::string& sType /*="*"*/)
		{
			// Create child nodes if not created yet
			if(!hasChildren())
				for(DOMNode *pChild = m_pElement->getFirstChild(); pChild != NULL; pChild = pChild->getNextSibling())
					if (pChild->getNodeType() == DOMNode::ELEMENT_NODE)
						new Node(this, m_pDocument, (DOMElement *)pChild, false);
			
			// Obtain the node with the specified name
			INode* pResult = INode::getChild(sKey);
			
			// Check the type if a result was found. The type must not be empty.
			if(pResult)
				if(sType != pResult->getAttribute(kType) && sType != "*" || sType == "")
					pResult = NULL;

			return pResult;
		}

		INode* Node::addChild(const std::string& sKey)
		{
			return new Node(this, m_pDocument, sKey);
		}

		/** XercesNode Specific Accessors */

		DOMElement *Node::getDOMElement()
		{
			return m_pElement;
		}

		void Node::setDOMElement(DOMElement *pDOMElement)
		{
			m_pElement = pDOMElement;
		}

		const std::string Node::kType = "type";
	}
}