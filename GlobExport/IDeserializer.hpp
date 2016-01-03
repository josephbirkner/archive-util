#ifndef _IDESERIALIZER_H_
#define _IDESERIALIZER_H_

#ifdef ARCHIVEUTIL_EXPORTS
#define ARCHIVEUTIL_API __declspec(dllexport)
#else
#define ARCHIVEUTIL_API __declspec(dllimport)
#endif

/** includes */
#include <string>
#include "../include/ArchiveUtil.h"
#include "ArchiveUtil.hpp"

/** declarations */
class Archiving::IArchivableObject;

namespace Archiving
{
	class ARCHIVEUTIL_API IDeserializer
	{
	public:
		/** Standardtype Deserialization */
		virtual bool	getBool(const std::string& sKey, ArchivingResult *bStatus) = 0;
		virtual char	getChar(const std::string& sKey, ArchivingResult *bStatus) = 0;
		virtual short getShort(const std::string& sKey, ArchivingResult *bStatus) = 0;
		virtual int		getInt(const std::string& sKey, ArchivingResult *bStatus) = 0;
		virtual long	getLong(const std::string& sKey, ArchivingResult *bStatus) = 0;
		virtual float getFloat(const std::string& sKey, ArchivingResult *bStatus) = 0;
		virtual double getDouble(const std::string& sKey, ArchivingResult *bStatus) = 0;
		virtual std::string getString(const std::string& sKey, ArchivingResult *bStatus = 0) = 0;
		
		/** Object Deserialization */
		template<class T_ObjectClass> T_ObjectClass* getObject(const std::string& sKey, ArchivingResult *bStatus)
		{
			T_ObjectClass* object = new T_ObjectClass();
			T_ObjectClass* orig = object;
			IArchivableObject* refvar = object;

			if (fillObject(sKey, refvar, bStatus))
			{
				/* Through the handleInstance() method of the IArchiveDelegate,
				 * the object pointer might change. Thats why we must ensure, that
				 * object remains of a class related to T_ObjectClass,
				 * for the returned pointer to be valid.
				 */
				if (refvar != orig)
				{
					delete orig;
					object = dynamic_cast<T_ObjectClass*>(refvar);
					assert( object && "IArchiveDelegate::handleInstance() returned an unrelated object!");
				}

				return object;
			}
			else
			{
				delete object;
				return NULL;
			}
		}

		template<class T_ListClass> unsigned long getArrayCount(const std::string& sKey, ArchivingResult *bStatus)
		{
			INode *pTempNode = getScope()->getChild(sKey, "array");
			if (verifyNode("array", pTempNode, bStatus)) 
				return static_cast<unsigned long>(boost::lexical_cast<long>(pTempNode->getAttribute("count").c_str()));

			return 0;
		}

		template<class T_ListClass> std::list<T_ListClass*>* getArray(const std::string& sKey, ArchivingResult *bStatus)
		{
			INode *pTempNode = getScope()->getChild(sKey, "array");
			if (verifyNode("array", pTempNode, bStatus)) 
			{
				std::list<T_ListClass*>* pNodeList = new std::list<T_ListClass*>;
				unsigned long arrayCount = getArrayCount<T_ListClass>(sKey, bStatus);
				
				pushScope(pTempNode);
				
				if (arrayCount && *bStatus == Archiving::Found)
				{
					for (unsigned long i = 0; i < arrayCount; ++i)
					{
						char tempKey[64];
						sprintf(tempKey, "item%lu", i);
						ArchivingResult nObjectStatus;
						T_ListClass* pObject = this->getObject<T_ListClass>(std::string(tempKey), &nObjectStatus);

						if (nObjectStatus == Found || nObjectStatus == Undefined /*ist nie undefined*/)
							pNodeList->push_back(pObject);
						else if(bStatus && *bStatus < nObjectStatus && nObjectStatus!=NotFound)
							*bStatus = nObjectStatus;
					}
				}
				popScope();
				return pNodeList;
			}

			return NULL;
		}
		
	protected:
		/** Scope */
		virtual INode *pushScope(INode *pNode) = 0;
		virtual INode *popScope() = 0;
		virtual INode *getScope() = 0;
		virtual bool fillObject(const std::string& sKey, IArchivableObject*& pObject, ArchivingResult *bStatus = NULL) = 0;
	};
}

#endif