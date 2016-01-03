#ifndef _IARCHIVINGNODE_HPP_
#define _IARCHIVINGNODE_HPP_

#ifdef ARCHIVEUTIL_EXPORTS
#define ARCHIVEUTIL_API __declspec(dllexport)
#else
#define ARCHIVEUTIL_API __declspec(dllimport)
#endif

#include "IInstanceCounter.hpp"
#include "IArchivingDriver.hpp"

namespace Archiving
{
	class ARCHIVEUTIL_API INode : public IInstanceCounter<INode>
	{
		friend class IArchivingDriver;
		friend class INode;

	private:
		INode* m_pParent;
		IArchivingDriver* m_pDriver;
		std::map<std::string, INode*> m_mapChildNodeNames;
		
		void addChild(INode* pNode) {m_mapChildNodeNames[pNode->getTagName()] = pNode; pNode->setDriver(m_pDriver);}
		
	public:
		INode() : m_pParent(NULL), m_pDriver(NULL) {;}
	
		virtual ~INode()
		{
			//for each(const std::pair<std::string, INode*>& pairNodeName in m_mapChildNodeNames)
			//	if(pairNodeName.second)
			//		delete pairNodeName.second;
		}
		
		virtual std::string getTagName() = 0;

		virtual std::string getAttribute(const std::string& sKey) = 0;
		virtual void setAttribute(const std::string& sKey, const std::string& sValue) = 0;
		
		virtual INode* getChild(const std::string& sKey, const std::string& sType="*") {return m_mapChildNodeNames[sKey];};
		virtual INode* addChild(const std::string& sKey) = 0;

		virtual std::string getValue() = 0;
		virtual void setValue(std::string sValue) = 0;

		INode* getParent() {return m_pParent;}
		void setParent(INode *pParent) {m_pParent = pParent; if(pParent) pParent->addChild(this);}
		
		bool hasChildren() {return !m_mapChildNodeNames.empty();}
		void setDriver(IArchivingDriver* pDriver) {if(m_pDriver != pDriver) {m_pDriver = pDriver; m_pDriver->addNode(this);} }
	};
}

#endif