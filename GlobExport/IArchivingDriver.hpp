#ifndef _ARCHIVINGDRIVER_HPP_
#define _ARCHIVINGDRIVER_HPP_

/** includes */
#include <string>
#include <atlstr.h>
#include <atlconv.h>

#ifdef ARCHIVEUTIL_EXPORTS
#define ARCHIVEUTIL_API __declspec(dllexport)
#else
#define ARCHIVEUTIL_API __declspec(dllimport)
#endif

class KeyValueArchive;

namespace Archiving
{
	class INode;
	
	class ARCHIVEUTIL_API IArchivingDriver
	{
	private:
		friend class INode;
		
		std::list<INode*> m_lsNodes;
		void addNode(INode* pNode) {m_lsNodes.push_back(pNode);}
	
	public:
		/** 
		 * Creates a new ArchivingDriver from the template class with a file loaded.
		 * @param The file path to load.
		 * @return A new initialized instance of the template IArchivinDriver subclass.
		 * @see CreateArchive()
		 */
		template<class T_ArchivingDriver> static IArchivingDriver* LoadArchiveFromFile(const std::string& sPath)
		{
			T_ArchivingDriver *pNewDriver = new T_ArchivingDriver;
			pNewDriver->init();
			pNewDriver->loadFromFile(sPath);
			return pNewDriver;
		}
		
		/**
		 * Creates a new ArchivingDriver from the template class with data loaded.
		 */
		template <class T_ArchivingDriver> static IArchivingDriver* LoadArchiveFromString(const std::string& sData)
		{
			T_ArchivingDriver *pNewDriver = new T_ArchivingDriver;
			pNewDriver->init();
			pNewDriver->loadFromString(sData);
			return pNewDriver;
		}

		/** 
		 * Creates a new ArchivingDriver from the template class.
		 * @return A new initialized instance of the template IArchivinDriver subclass.
		 * @see LoadArchive()
		 */
		template<class T_ArchivingDriver> static IArchivingDriver* CreateArchive()
		{
			T_ArchivingDriver *pNewDriver = new T_ArchivingDriver;
			pNewDriver->init();
			return pNewDriver;
		}
		
		/** 
		 * The virtual destructor.
		 * Put there to do everyone a favor and allow for memory-neutral archives.
		 */
		virtual ~IArchivingDriver();
		
		/** 
		 * Initializes the driver 
		 */
		virtual void init() = 0;

		/** 
		 * Saves the archive to a file.
		 * @param The file path to save. If "" the loading path will be used.
		 * @return If saving was succesfull.
		 */
		virtual bool save(std::string sFile = "") = 0;
		
		/** 
		 * Loads the archive from a file.
		 * @param The file path to load.
		 * @return If loading was succesfull.
		 */
		virtual bool loadFromFile(const std::string& sFile) = 0;
		
		/** 
		 * Loads the archive from a string.
		 * @param The string to load from.
		 * @return If loading was succesfull.
		 */
		virtual bool loadFromString(const std::string& sData) = 0;
		
		/** 
		 * Get the archives XML string.
		 * @return The archives string representation.
		 * @see save(), std::string
		 */
		virtual std::string getString() = 0;
		
		/** 
		 * Get the root node of the archive.
		 * @return The archives root node.
		 * @see INode
		 */
		virtual INode* getRootNode() = 0;

		/** 
		 * Get the archives parsing error count.
		 * @return The archives parsing error count.
		 */
		virtual unsigned long getErrorCount() = 0; //parsing errors

		/** 
		 * Get whether the driver has loaded a document.
		 * @return True if a document has been loaded, false if otherwise.
		 */
		virtual bool getIsLoad() = 0;
		
		/* @brief  Reset
		 * @params 
		 * @return 
		 * @see    
		 */
		virtual void reset() = 0;
	};
}

#endif