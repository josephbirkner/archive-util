#ifndef _XERCESNODE_HPP_
#define _XERCESNODE_HPP_

#include <string>
#include "INode.hpp"
#include <xercesc\util\XercesDefs.hpp>

#ifdef ARCHIVEUTIL_EXPORTS
#define ARCHIVEUTIL_API __declspec(dllexport)
#else
#define ARCHIVEUTIL_API __declspec(dllimport)
#endif

XERCES_CPP_NAMESPACE_BEGIN
	class DOMElement;
	class DOMDocument;
XERCES_CPP_NAMESPACE_END

#include "IInstanceCounter.hpp"

namespace Archiving
{
	namespace Xerces
	{
		class ARCHIVEUTIL_API Node : public INode, public IInstanceCounter<Node>
		{
			friend class Driver;
			friend class IArchivingDriver;

			static const std::string kType;
		
		protected:
			/** Con/Destructor */
			Node(Node *pParentNode, xercesc::DOMDocument *pDOMDocument, std::string sName);
			Node(Node *pParentNode, xercesc::DOMDocument *pDOMDocument, xercesc::DOMElement *pDOMElement, bool bNew = true);
			virtual ~Node();

		public:
			/** Interface methods */
			virtual std::string getTagName();
			virtual std::string getAttribute(const std::string& sKey);
			virtual void setAttribute(const std::string& sKey, const std::string& sValue);
			virtual std::string getValue();
			virtual void setValue(std::string sValue);
			virtual INode* getChild(const std::string& sKey, const std::string& sType="*");
			virtual INode* addChild(const std::string& sKey);

			/** Xerces specific methods */
			xercesc::DOMElement *getDOMElement();
			void setDOMElement(xercesc::DOMElement *pDOMElement);

		protected:
			xercesc::DOMElement *m_pElement;
			xercesc::DOMDocument *m_pDocument;
		};
	}
}

#endif