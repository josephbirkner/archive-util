#ifndef _ARCHIVEUTIL_HPP_
#define _ARCHIVEUTIL_HPP_

#ifdef ARCHIVEUTIL_EXPORTS
#define ARCHIVEUTIL_API __declspec(dllexport)
#define ARCHIVEUTIL_API_FUNCTION(typ) typ ARCHIVEUTIL_API
#else
#define ARCHIVEUTIL_API __declspec(dllimport)
#define ARCHIVEUTIL_API_FUNCTION(typ) typ ARCHIVEUTIL_API
#endif

/** Includes */
#include <string>

#include "KeyValueArchive.hpp"
#include "XercesNode.hpp"
#include "XercesDriver.hpp"

namespace Archiving
{
	using namespace std;

	/** Newline characters used to write XML output */
	extern char *newline;

	/** Verifies a INode and returns the Archiving result 
	 * @param The type or classname of the node to cheack against.
	 * @param The node itself.
	 * @param The result.
	 * @see ArchivingResult
	 * @return Returns true if the node and the expected type or class are valid. Otherwise false.
	 */
	ARCHIVEUTIL_API_FUNCTION (bool) verifyNode(string sString, INode *pNode, ArchivingResult *pResult);

	/**
	 * XML file based archive. Version 1.0
	 * Using the Xerces driver and node.
	 * @version 1.0
	 */
	typedef KeyValueArchive<Archiving::Xerces::Driver> XMLArchive;

	/**
	 * XML file based archive. Version 2.0
	 * Using the Xerces driver and node.
	 * @version 2.0
	 */
	//typedef KeyValueArchive<Archiving::Xerces::Driver_2> XMLArchive2;
}

#endif
