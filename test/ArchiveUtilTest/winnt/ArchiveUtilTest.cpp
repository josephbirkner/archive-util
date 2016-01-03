using namespace NUnit::Framework;

#include <stdio.h>
#include <string>
#include "Base/ArchiveUtil/GlobExport/ArchiveUtil.hpp"
#include "Base/ArchiveUtil/GlobExport/IDeserializer.hpp"
#include "tests/TestUtil/GlobExport/TestBase.hpp"


#define XML_TEST_HEADER "<?xml version=\"1.0\" encoding=\"UTF-16\" standalone=\"no\" ?>"

[TestFixture]
ref class ArchiveUtilTest : public Tests::TestBase
{
public:
	[SetUp]
	void Setup()
	{
		
	}

	[TearDown]
	void TearDown()
	{
		
	}

	[Test]
	void Test_SingleInt()
	{
		Archiving::XMLArchive *pArchive1 = new Archiving::XMLArchive();
		pArchive1->loadFromString(XML_TEST_HEADER "<archive><test type=\"int\">12</test></archive>");
		
		Assert::IsTrue(pArchive1->getInt("test") == 12, "Archive1 getInt");
		
		delete pArchive1;
		
		/*
		Archiving::XMLArchive2 *pArchive2 = new Archiving::XMLArchive2();
		pArchive2->loadFromString(XML_TEST_HEADER "<archive><item key=\"test\" type=\"int\">13</item></archive>");
		
		Assert::IsTrue(pArchive2->getInt("test") == 13, "Archive2 getInt");
		
		delete pArchive2;
		*/
	}

};