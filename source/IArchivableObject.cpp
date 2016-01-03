#include "StdAfx.h"
#pragma hdrstop

#include "../GlobExport/IArchivableObject.hpp"

std::string Archiving::IArchivableObject::getClassName() const
{
	return std::string((typeid(*this)).name());
}