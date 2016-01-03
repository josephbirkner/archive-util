#ifndef _XERCESDRIVER_HPP_
#define _XERCESDRIVER_HPP_

#include "IArchivingDriver.hpp"
#include <xercesc\util\XercesDefs.hpp>

#ifdef ARCHIVEUTIL_EXPORTS
#define ARCHIVEUTIL_API __declspec(dllexport)
#else
#define ARCHIVEUTIL_API __declspec(dllimport)
#endif

XERCES_CPP_NAMESPACE_BEGIN
class DOMElement;
class DOMDocument;
class XercesDOMParser;
XERCES_CPP_NAMESPACE_END

class KeyValueArchive;
class INode;

namespace Archiving
{
	namespace Xerces
	{
		class ARCHIVEUTIL_API Driver : public IArchivingDriver
		{
		public:
			Driver();
			virtual ~Driver();

		protected:
			xercesc::XercesDOMParser *m_pParser;
			xercesc::DOMDocument *m_pDocument;
			xercesc::DOMElement *m_pRootElement;
			INode *m_pCurrentNode; //root INode (contains rootElement)
			unsigned long m_DOMErrorCount;
			bool m_bIsLoad;

		public:
			/** Init */
			virtual void init();

			/** Load/Write */
			virtual bool save(std::string sFile = "");
			virtual bool loadFromFile(const std::string& sFile);
			virtual bool loadFromString(const std::string& sData);
			virtual void reset();
			
			virtual std::string getString();
			
			/** Accessors */
			virtual INode* getRootNode();
			virtual unsigned long getErrorCount();
			virtual bool getIsLoad();
		};
	}
}

#endif