/***************************************************************************\
 *
 * PROJEKTNAME            : Verlagsloesung
 * -------------------------------------------------------------------------
 *
 * TEILPROJEKTNAME        : Archiver
 *
 * MODULBESCHREIBUNG      : 
 *
 * ROUTINEN               :
 *
 * COMPILER               : 
 *
 * NOTWENDIGE DATEIEN     : 
 *
 * NOTWENDIGE BIBLIOTHEKEN: 
 *
 * AUTOR                  : $Author: JBI, JWL $
 *
 * VERSION                : $Revision: 1.18 $
 *
 * RELEASE                : $Name: $
 *
 * CHANGE LOG (Letzte Aenderung $Date: 2011/30/12 16:36:30Z $):
 *
 *  Copyright (C) 2011 HUP AG
 *
\******************************************************************************/

/** includes */
#include "stdafx.h"
#include "../GlobExport/ArchiveUtil.hpp"
#include <bitset>


namespace Archiving
{
	char *newline = "\n";

	/** Verify Node */
	bool verifyNode(string sType, INode *pNode, ArchivingResult *pResult)
	{
		if (pNode == NULL) {
			if (pResult != NULL) {*pResult = NotFound;}
			return false;
		}
		if (pNode && (pNode->getAttribute("type")!=sType)) {
			if (pResult != NULL) {*pResult = BadType;}
			return false;
		}
		if (pResult != NULL) {*pResult = Found;}
		return true;
	}
}