#pragma hdrstop

#include "StdAfx.h"
#include <windows.h>
#include <cstdio>

#include "../GlobExport/XercesNode.hpp"
#include "../GlobExport/XercesDriver.hpp"
#include "../GlobExport/ArchiveUtil.hpp"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/framework/XMLFormatter.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>

using namespace xercesc;

namespace Archiving
{
	namespace Xerces
	{
		Driver::Driver()
			: m_pCurrentNode(NULL)
			, m_pDocument(NULL)
			, m_pRootElement(NULL)
			, m_pParser(NULL)
			, m_DOMErrorCount(0)
			, m_bIsLoad(false)
		{
			//this->init();
		}

		Driver::~Driver()
		{
			if(m_pParser)
				delete m_pParser; // document owned by parser
			else if(m_pDocument)
				m_pDocument->release();
			
			XMLPlatformUtils::Terminate();
		}

		void Driver::init()
		{
			try 
			{
				XMLPlatformUtils::Initialize();
			}
			catch (...)
			{
				throw(std::runtime_error("Error initializing Xerces API!"));
			}

			m_pParser = new XercesDOMParser();
			assert(m_pParser != NULL);
			m_pParser->setValidationScheme(XercesDOMParser::Val_Never);
			m_pParser->setDoNamespaces(false);
			m_pParser->setDoSchema(false);
			m_pParser->setLoadExternalDTD(false);
		}

		INode* Driver::getRootNode()
		{
			if (!m_pCurrentNode) { /* A new archiver was created without loading a file */
				XMLCh *xml_core_impl = XMLString::transcode("Core");
				DOMImplementation* impl =  DOMImplementationRegistry::getDOMImplementation(xml_core_impl);
				XMLString::release(&xml_core_impl);
				try
				{
					XMLCh* xml_document_name = XMLString::transcode("archive");
					m_pDocument = impl->createDocument(0, xml_document_name, 0);
					XMLString::release(&xml_document_name);
					
					m_pRootElement = m_pDocument->getDocumentElement();
					this->m_pCurrentNode = new Node(NULL, this->m_pDocument, this->m_pRootElement, false);
					m_pCurrentNode->setDriver(this);
				}
				catch (...)
				{
					throw(std::runtime_error("error creating document!"));
				}
			}
			return m_pCurrentNode;
		}

		bool Driver::save(std::string sPath)
		{
			if (sPath.length() == 0)
				throw(std::runtime_error("invalid path!"));

			FILE *pFile = fopen(const_cast<const char*>(sPath.c_str()), "wb");
			if (pFile)
			{
				std::string sTempXML = getString();
				size_t iSize = sTempXML.size();
                
				fwrite(sTempXML.c_str(), sizeof(char), iSize, pFile);
				fclose(pFile);

				return true;
			}
	
			return false;
		}

		std::string Driver::getString()
		{
			DOMImplementation *implementation = DOMImplementationRegistry::getDOMImplementation(L"LS");
			
			DOMWriter *pWriter = ((DOMImplementationLS*)implementation)->createDOMWriter();
			
			if (pWriter->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
				pWriter->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
			
			MemBufFormatTarget *aFormatTarget = new MemBufFormatTarget();
			XMLFormatter aFormatter(L"UTF-8", aFormatTarget);
			
			pWriter->writeNode(aFormatTarget, *m_pDocument);
			
			return std::string((char *)(aFormatTarget->getRawBuffer()));
		}

		bool Driver::loadFromFile(const std::string& sFile)
		{
			try
			{
				reset();
					
				m_pParser->reset();
				m_pParser->parse(sFile.c_str());
			}
			catch (XMLException* e)
			{
				delete e;
				return (m_bIsLoad = false);
			}
			catch (DOMException* e)
			{
				delete e;
				return (m_bIsLoad = false);
			}
			
			m_DOMErrorCount = (unsigned long)m_pParser->getErrorCount();
			//ASSERT(m_DOMErrorCount == 0 && "Archive XML parser error.");
			
			if (m_pDocument = m_pParser->getDocument())
			{
				if (m_pRootElement = m_pDocument->getDocumentElement())
				{
					this->m_pCurrentNode = new Node(NULL, this->m_pDocument, this->m_pRootElement, false);
					m_pCurrentNode->setDriver(this);

					return (m_bIsLoad = true);
				}
			}

			return (m_bIsLoad = false);
		}
		
		bool Driver::loadFromString(const std::string& sData)
		{
			try
			{
				reset();
					
				m_pParser->reset();
				xercesc::MemBufInputSource archiveSource((const XMLByte*)sData.c_str(), sData.length(), "archive_dummy", false);
				m_pParser->parse(archiveSource);
			}
			catch (XMLException* e)
			{
				//...
				delete e;
				return (m_bIsLoad = false);
			}
			catch (DOMException* e)
			{
				//...
				delete e;
				return (m_bIsLoad = false);
			}

			m_DOMErrorCount = (unsigned long)m_pParser->getErrorCount();
			//if (xml_error_count != 0)
			//	return false;

			if (m_pDocument = m_pParser->getDocument())
			{
				if (m_pRootElement = m_pDocument->getDocumentElement())
				{
					this->m_pCurrentNode = new Node(NULL, this->m_pDocument, this->m_pRootElement, false);
					m_pCurrentNode->setDriver(this);

					return (m_bIsLoad = true);
				}
			}

			return (m_bIsLoad = false);
		}

		/*
		 *
		 */
		unsigned long Driver::getErrorCount()
		{
			return m_DOMErrorCount;
		}

		bool Driver::getIsLoad()
		{
			return m_bIsLoad;
		}

		void Driver::reset()
		{
			if (m_pDocument)
				m_pDocument->release();
				
			m_pRootElement = NULL;
			getRootNode();
		}
	}
}