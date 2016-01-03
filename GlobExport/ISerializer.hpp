#ifndef _ISERIALIZER_H_
#define _ISERIALIZER_H_

/** includes */
#include <string>
#include <vector>

#include "IArchivableObject.hpp"


namespace Archiving
{
	class ISerializer
	{
	public:
		/** Standardtype Serialization */
		virtual void setBool(bool bBool, const std::string& sKey) = 0;
		virtual void setChar(char cChar, const std::string& sKey) = 0;
		virtual void setShort(short sShort, const std::string& sKey) = 0;
		virtual void setInt(int iInt, const std::string& sKey) = 0;
		virtual void setLong(long lLong, const std::string& sKey) = 0;
		virtual void setFloat(float fFloat, const std::string& sKey) = 0;
		virtual void setDouble(double dDouble, const std::string& sKey) = 0;
		
		/** Object Serialization */
		virtual void setObject(IArchivableObject*, const std::string& sKey) = 0;
		virtual void setString(const std::string& sString, const std::string& sKey) = 0;
		virtual void setArray(std::list<IArchivableObject*>& lList, const std::string& sKey) = 0;
	};
}

#endif