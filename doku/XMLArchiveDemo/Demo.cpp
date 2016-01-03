#include <stdio.h>
#include <string>

#include "Base/ArchiveUtil/GlobExport/ArchiveUtil.hpp"
#include "classes.h"

void simpleSave();
void simpleLoad();
void rectSave();
void listSave();

int main(int argc, char **args)
{	
	// Speichern...
	//simpleSave();

	// Laden...
	// simpleLoad();
	 
	// Komplexes Speichern
	// rectSave();
	
	// Liste Speichern
	listSave();

	//warten...
	printf(">");
	getc(stdin);

	return 0;
}

void simpleSave()
{
	Archiving::XMLArchive archive; // typedef KeyValueArchive<Archiving::Xerces::Driver> XMLArchive

	archive.setInt(32, "UnserInteger");

	archive.save("archiv.xml");
}

void simpleLoad()
{
	Archiving::XMLArchive archive("archiv.xml"); // typedef KeyValueArchive<Archiving::Xerces::Driver> XMLArchive

	int iUnserInteger = 0;
	iUnserInteger = archive.getInt("UnserInteger");

	printf("UnserInteger = %d \n", iUnserInteger);
}

void rectSave()
{
	Archiving::XMLArchive archive; // typedef KeyValueArchive<Archiving::Xerces::Driver> XMLArchive

	Rect rect(1, 1, 32, 32);

	archive.setObject(&rect, "UnserRect");

	archive.save("rect.xml");
}

void listSave()
{
	Archiving::XMLArchive archive;

	std::list<Archiving::IArchivableObject *> rectList;

	for (int i=0; i<10; ++i)
		rectList.push_back(new Rect(i, i, i*2, i*2));

	archive.setArray(rectList, "UnserArray");

	archive.save("array.xml");
}
