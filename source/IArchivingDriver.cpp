#include "StdAfx.h"

#pragma hdrstop

#include "../GlobExport/IArchivingDriver.hpp"
#include "../GlobExport/INode.hpp"

Archiving::IArchivingDriver::~IArchivingDriver()
{
	for each(INode* pNode in m_lsNodes)
		delete pNode;
}

