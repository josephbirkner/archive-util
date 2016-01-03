#ifndef _KEYVALUEARCHIVE_HPP_
#define _KEYVALUEARCHIVE_HPP_

#include "../include/StdAfx.h"

#ifdef ARCHIVEUTIL_EXPORTS
#define ARCHIVEUTIL_API __declspec(dllexport)
#else
#define ARCHIVEUTIL_API __declspec(dllimport)
#endif

#include "../ArchiveUtil/GlobExport/IArchivingDriver.hpp"

#include "ArchiveUtil.hpp"
#include "INode.hpp"
#include "IArchivingDriver.hpp"
#include "IArchivableObject.hpp"
#include "ISerializer.hpp"
#include "IDeserializer.hpp"
#include "IArchiveDelegate.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

/**
 * Implementation of an archive that stores values (represented by nodes) in a tree,
 * whereas each node is represented by a specific INode Subclass instance for each specific Archiving driver.
 * In the nested key value archive, each node represents a serialized variable.
 *
 * The public interface of the KeyValueArchive consists largely of interface methods inherited from ISerializer and IDeserializer.
 *
 * To set the value of a variable in the archive, use the setBool/setShort/setInt/setLong/setFloat/setDouble/setString methods,
 * which are inherited from the ISerializer interface.
 * A node that has subnodes, therefore subvariables, must represent a variable of type IArchivableObject.
 * To set a node of type IArchivableObject, use the ISerializer's setObject() method. As that method is
 * called, the issued IArchiableObject instance's serialize() method will be called with an ISerializer instance.
 * Within the serialize() method of the object, any call to ISerializer::setObject/setBool/setShort/setInt/setLong/setFloat/setDouble/setString
 * will now create new nodes within the scope of the object node instead of the root node.
 * Calling setObject within the scope of a parent object is also allowed. That way,
 * nested object hirarchies can be saved.
 *
 * The following could be a valid archive scheme (MyClass and MyOtherClass extend IArchivableObject):
 * |- MyClass object with key "object1"
 * |  |- MyOtherClass object with key "subobject"
 * |  |  |- subnodes of subobject
 * |  |
 * |  |- bool with key "myBool" and value "false"
 * |
 * |- MyClass object with key "object2"
 * |  |- bool with key "myBool" and value "true"
 * |
 * |- integer with key "myInt" and value "10"
 *
 * Another particulary interesting serialization command is the ISerializer::setArray command. In that command,
 * you can issue an std::vector collection that must contain IArchivableObject instances.
 * Those instance's serialize methods will be called within the scope of a node representing that collection.
 * Note, that it is not necessary that all objects in the vector are of the same final type. They just have to extend IArchivableObject.
 * The Archive will automatically remember the correct type to deserialize the object with.
 * E.g., the following could be a representation of an array in an archive:
 *
 * root
 * |- array with key "myArray" and length 2
 * |  |- MyClass object with key "item1"
 * |  |  |- MyOtherClass object with key "subobject"
 * |  |  |  |- subnodes of subobject
 * |  |  |
 * |  |  |- bool with key "myBool" and value "false"
 * |  |
 * |  |- MyOtherClass object with key "item2"
 * |     |- bool with key "myBool" and value "true"
 *
 * In that example, "item1" and "item2" would be members of that array collection.
 *
 * The IDeserializer::getObject/getBool/getShort/getInt/getLong/getFloat/getDouble/getString/getArray
 * methods operate likewise: A new instance of one or multiple objects is generated when calling the getObject or getArray method.
 * On that object, the deserialize() method is called with an IDeserializer interface.
 * While inside the object's deserialize() method, all calls to archive accessors will operate on the scope of the object.
 *
 * E.g., referring to the example, a call to ExampleArchive.getArray<MyClass>("myArray") would result in a collection
 * of one MyClass object, since only one MyClass instance has been put in.
 * The call of MyClass::deserialize() would then result in a call to getObject<MyOtherClass>("subobject") at the scope
 * of that instance. That way, the object hierarchy would be fully restored.
 *
 * An Archive can be created/loaded using one of its constructors. When using the alternate contructor, where
 * a path can be specified, the archive's load() method will be called automatically with that path.
 * Otherwise, you can call the load() method manually.
 *
 * You may also want to use the setDelegate/getDelegate methods, which allow you to add custom
 * behaviour to the archive, in terms of error handling and pre/post-deserialization/pre-serialization instance handling.
 */
namespace Archiving
{
	/**
	 */
	template <class T_IArchivingDriver>
	class KeyValueArchive
		: public ISerializer
		, public IDeserializer
	{
	private:
		/** Private: Copy-constructor to prevent copying the archive. */
		KeyValueArchive(KeyValueArchive&){;}

	protected:
		IArchivingDriver* m_pArchivingDriver;  /** A pointer to the archive-driver instance. */
		INode* m_pScope;                       /** A pointer that points to the current scope node. See pushScope(INode *pScope) and popScope() */
		IArchiveDelegate* m_pDelegate;         /** A pointer to the delegate-object. See setDelegate() and getDelegate() */
		std::string m_sSource;                 /** A string identifying the source this driver is accessing (e.g., a file path). */

		/**
		 * Protected: Searches the current scope for a node with the given key,
		 * and creates it if it doesn't exist.
		 * @return A new instace of the node_type template associated with the instace of the archive.
		 */
		INode* getSubNode(const std::string& sKey, const std::string& sType);

		/**
		 * Protected: Get the scope-path recursive of the given node.
		 * @return The scope path for the given node.
		 */
		std::string getPath(INode *pNode);

	public:
		/**
		 * Standard constructor.
		 * @return A new KeyValueArchive object with no file loaded.
		 */
		KeyValueArchive();

		/**
		 * Loading constructor.
		 * @param The file path to load.
		 * @return A new KeyValueArchive object with a file loaded, ready for deserializing.
		 */
		KeyValueArchive(const std::string& sPath);

		/**
		 * Standard destructor.
		 */
		~KeyValueArchive();

		/**
		 * setDelegate.
		 * Set's the given instance as delegate to the archive.
		 * @param The delegate instance to set.
		 * @see getDelegate() IArchiveDelegate
		 */
		void setDelegate(IArchiveDelegate *pDelegate);

		/**
		 * getDelegate.
		 * @return The delegate if set. Otherwise NULL.
		 * @see setDelegate() and IArchiveDelegate
		 */
		IArchiveDelegate *getDelegate();

		/**
		 * Load the archives content from an XML file.
		 * @param The file path to load.
		 * @return True if loading was succesfull. Otherwise false.
		 * @see save()
		 */
		bool loadFromFile(const std::string& sPath);
		bool loadFromString(const std::string& sData);

		/**
		 * Get if the archive ever was load.
		 * @return True if a file was loaded succesfull. Otherwise false.
		 * @see load()
		 */
		bool getIsLoad();

		/**
		 * Saves the archive as XML file.
		 * @param The file path to save.
		 * @return True if saving was succesfull. Otherwise false.
		 * @see getArchiveString()
		 */
		bool save(std::string sPath="");

		/**
		 * Returns the archives XML as string.
		 * @return The archives XML string.
		 */
		std::wstring getArchiveString();

		/* Serializer-Interface Methods */
		virtual void setBool(bool bBool, const std::string& sKey);
		virtual void setChar(char cChar, const std::string& sKey);
		virtual void setShort(short sShort, const std::string& sKey);
		virtual void setInt(int iInt, const std::string& sKey);
		virtual void setLong(long lLong, const std::string& sKey);
		virtual void setFloat(float fFloat, const std::string& sKey);
		virtual void setDouble(double dDouble, const std::string& sKey);
		virtual void setString(const std::string& sString, const std::string& sKey);
		virtual void setObject(IArchivableObject*, const std::string& sKey);
		virtual void setArray(std::list<IArchivableObject*>& lList, const std::string& sKey);

		/* Deserializer Methods */
		virtual bool	     getBool(const std::string& sKey, ArchivingResult *bStatus = NULL);
		virtual char	     getChar(const std::string& sKey, ArchivingResult *bStatus = NULL);
		virtual short       getShort(const std::string& sKey, ArchivingResult *bStatus = NULL);
		virtual int		     getInt(const std::string& sKey, ArchivingResult *bStatus = NULL);
		virtual long	     getLong(const std::string& sKey, ArchivingResult *bStatus = NULL);
		virtual float       getFloat(const std::string& sKey, ArchivingResult *bStatus = NULL);
		virtual double      getDouble(const std::string& sKey, ArchivingResult *bStatus = NULL);
		virtual std::string getString(const std::string& sKey, ArchivingResult *bStatus = NULL);

		/**
		 * Get the archives parsing error count
		 * @return The number of parsing errors occured while loading the file.
		 */
		unsigned long getErrorCount();

		/**
		 * Reset the archive.
		 * Sets the archives state like it was newly loaded or created.
		 */
		void reset();

		/** Obtain a string identifying the source this driver is accessing (e.g., a file path).
		 */
		std::string getSource() {return m_sSource;}

	protected:
		/**
		 * Protected: Push the scope.
		 * @param The node to set as curretn scope.
		 * @return The new pushed scope node.
		 */
		virtual INode *pushScope(INode *pNode);

		/**
		 * Protected: Pop the scope.
		 * @return The current scope node.
		 */
		virtual INode *popScope();

		/**
		 * Protected: Get the current scope node.
		 * @return The current scope node.
		 */
		virtual INode *getScope(){return m_pScope;}

		/**
		 * Protected: Deserialize an object with the given key.
		 * This method exists because of the restriction that virtual functions can not be virtual. This method is only used internally by getObject<T>(key, status)
		 * @param The key of the object to deserialize.
		 * @param A pointer to the object.
		 * @param A pointer to an ArchivingResult variable.
		 * @return True if the object was found. Otherwise false.
		 * @see ArchivingResult, IArchivableObject and getObject<class T>(const std::string& sKey, ArchivingResult *bStatus = NULL)
		 */
		virtual bool fillObject(const std::string& sKey, IArchivableObject*& pObject, ArchivingResult *bStatus = NULL );
	};

}

#endif


namespace Archiving
{
	template <class T_IArchivingDriver>
	KeyValueArchive<T_IArchivingDriver>::KeyValueArchive()
			: m_pDelegate(NULL)
			, m_pArchivingDriver(IArchivingDriver::CreateArchive<T_IArchivingDriver>())
			, m_pScope(NULL)
	{
		assert(m_pArchivingDriver != NULL );
		pushScope(m_pArchivingDriver->getRootNode());
	}

	template <class T_IArchivingDriver>
	KeyValueArchive<T_IArchivingDriver>::KeyValueArchive(const std::string& sPath)
			: m_pDelegate(NULL)
			, m_pArchivingDriver(IArchivingDriver::LoadArchive<T_IArchivingDriver>(sPath))
			, m_pScope(NULL)
	{
		assert(m_pArchivingDriver != NULL );
		pushScope(m_pArchivingDriver->getRootNode());
	}

	template <class T_IArchivingDriver>
	KeyValueArchive<T_IArchivingDriver>::~KeyValueArchive()
	{
		delete m_pArchivingDriver;
	}

	/** Delegate */
	template <class T_IArchivingDriver>
	void KeyValueArchive<T_IArchivingDriver>::setDelegate(IArchiveDelegate *pDelegate)
	{
		m_pDelegate = pDelegate;
		assert(pDelegate != NULL);
	}

	template <class T_IArchivingDriver>
	IArchiveDelegate *KeyValueArchive<T_IArchivingDriver>::getDelegate()
	{
		return m_pDelegate;
	}

	/* Path */
	template <class T_IArchivingDriver>
	std::string KeyValueArchive<T_IArchivingDriver>::getPath(INode *pNode)
	{
		INode *pTempNode = pNode;
		std::string sPath = "/" + pTempNode->getTagName();

		while (pTempNode = pTempNode->getParent())
		{
			sPath = "/" + pTempNode->getTagName() + sPath;
		}

		return sPath;
	}

	/** Read/Write File */
	template <class T_IArchivingDriver>
	bool KeyValueArchive<T_IArchivingDriver>::loadFromFile(const std::string& sPath)
	{
		if (m_pArchivingDriver && m_pArchivingDriver->loadFromFile(sPath))
		{
			m_pScope = NULL;
			pushScope(m_pArchivingDriver->getRootNode());
			return true;
		}
		m_sSource = sPath;
		return false;
	}

	template <class T_IArchivingDriver>
	bool KeyValueArchive<T_IArchivingDriver>::loadFromString(const std::string& sData)
	{
		if (m_pArchivingDriver && m_pArchivingDriver->loadFromString(sData))
		{
			m_pScope = NULL;
			pushScope(m_pArchivingDriver->getRootNode());
			return true;
		}
		m_sSource = sData;
		return false;
	}

	template <class T_IArchivingDriver>
	bool KeyValueArchive<T_IArchivingDriver>::save(std::string sPath="")
	{
		assert(m_pArchivingDriver != NULL && "Attempt to save unloaded archive!");
		return m_pArchivingDriver->save(sPath);
	}

	template <class T_IArchivingDriver>
	bool KeyValueArchive<T_IArchivingDriver>::getIsLoad()
	{
		return m_pArchivingDriver->getIsLoad();
	}

	/** Serializer-Interface Methods */
	template <class T_IArchivingDriver>
	void KeyValueArchive<T_IArchivingDriver>::setBool(bool bBool, const std::string& sKey)
	{
		getSubNode(sKey, "bool")->setValue(boost::lexical_cast<std::string>(bBool));
	}

	template <class T_IArchivingDriver>
	void KeyValueArchive<T_IArchivingDriver>::setChar(char cChar, const std::string& sKey)
	{
		getSubNode(sKey, "char")->setValue(boost::lexical_cast<std::string>(cChar));
	}

	template <class T_IArchivingDriver>
	void KeyValueArchive<T_IArchivingDriver>::setShort(short sShort, const std::string& sKey)
	{
		getSubNode(sKey, "short")->setValue(boost::lexical_cast<std::string>(sShort));
	}

	template <class T_IArchivingDriver>
	void KeyValueArchive<T_IArchivingDriver>::setInt(int iInt, const std::string& sKey)
	{
		getSubNode(sKey, "int")->setValue(boost::lexical_cast<std::string>(iInt));
	}

	template <class T_IArchivingDriver>
	void KeyValueArchive<T_IArchivingDriver>::setLong(long lLong, const std::string& sKey)
	{
		getSubNode(sKey, "long")->setValue(boost::lexical_cast<std::string>(lLong));
	}

	template <class T_IArchivingDriver>
	void KeyValueArchive<T_IArchivingDriver>::setFloat(float fFloat, const std::string& sKey)
	{
		getSubNode(sKey, "float")->setValue(boost::lexical_cast<std::string>(fFloat));
	}

	template <class T_IArchivingDriver>
	void KeyValueArchive<T_IArchivingDriver>::setDouble(double dDouble, const std::string& sKey)
	{
		getSubNode(sKey, "double")->setValue(boost::lexical_cast<std::string>(dDouble));
	}

	template <class T_IArchivingDriver>
	void KeyValueArchive<T_IArchivingDriver>::setObject(IArchivableObject* pObject, const std::string& sKey)
	{
		if (m_pDelegate != NULL)
			if(!m_pDelegate->preSerializeObject(pObject))
				return;

		INode *temp_node= getSubNode(sKey, pObject->getClassName());
		assert(temp_node != NULL);
		pushScope(temp_node);
		pObject->serialize((ISerializer *)this);
		popScope();
		if (m_pDelegate != NULL)
			m_pDelegate->afterSerializeObject(pObject);
	}

	template <class T_IArchivingDriver>
	void KeyValueArchive<T_IArchivingDriver>::setString(const std::string& sString, const std::string& sKey)
	{
		getSubNode(sKey, "string")->setValue(sString);
	}

	template <class T_IArchivingDriver>
	void KeyValueArchive<T_IArchivingDriver>::setArray(std::list<IArchivableObject*>& lList, const std::string& sKey)
	{
		if(!lList.size())
			return;

		INode *array_node = NULL;
		pushScope(array_node = getSubNode(sKey, "array"));
		unsigned long int_count = 0;
		for(std::list<IArchivableObject*>::iterator list_iter = lList.begin(); list_iter != lList.end(); list_iter++)
		{
			char temp_str[64];
			sprintf(&temp_str[0], "item%lu", int_count++);
			this->setObject((IArchivableObject*)(*list_iter), std::string(temp_str));
		}
		array_node->setAttribute("count", boost::lexical_cast<std::string>((long)int_count));
		popScope();
	}

	/** Deserializer Methods */
	template <class T_IArchivingDriver>
	bool KeyValueArchive<T_IArchivingDriver>::getBool(const std::string& sKey, ArchivingResult *bStatus)
	{
		INode *pTempNode = m_pScope->getChild(sKey, "bool");
		if(verifyNode("bool", pTempNode, bStatus))
			return pTempNode->getValue() == "1" || pTempNode->getValue() == "J" || boost::iequals(pTempNode->getValue(), "true" );
		else
			return false;
	}

	template <class T_IArchivingDriver>
	char KeyValueArchive<T_IArchivingDriver>::getChar(const std::string& sKey, ArchivingResult *bStatus)
	{
		INode *pTempNode = m_pScope->getChild(sKey, "char");
		if (verifyNode("char", pTempNode, bStatus))
			return (char)pTempNode->getValue().c_str()[0];
		else
			return (char)0;
	}

	template <class T_IArchivingDriver>
	short KeyValueArchive<T_IArchivingDriver>::getShort(const std::string& sKey, ArchivingResult *bStatus)
	{
		INode *pTempNode = m_pScope->getChild(sKey, "short");
		if (verifyNode("short", pTempNode, bStatus))
			return boost::lexical_cast<short>(pTempNode->getValue());
		else
			return (short)0;
	}

	template <class T_IArchivingDriver>
	int KeyValueArchive<T_IArchivingDriver>::getInt(const std::string& sKey, ArchivingResult *bStatus)
	{
		INode *pTempNode = m_pScope->getChild(sKey, "int");
		if (verifyNode("int", pTempNode, bStatus))
			return boost::lexical_cast<int>(pTempNode->getValue());
		else
			return 0;
	}

	template <class T_IArchivingDriver>
	long KeyValueArchive<T_IArchivingDriver>::getLong(const std::string& sKey, ArchivingResult *bStatus)
	{
		INode *pTempNode = m_pScope->getChild(sKey, "long");
		if (verifyNode("long", pTempNode, bStatus))
			return boost::lexical_cast<long>(pTempNode->getValue());
		else
			return 0l;
	}

	template <class T_IArchivingDriver>
	float KeyValueArchive<T_IArchivingDriver>::getFloat(const std::string& sKey, ArchivingResult *bStatus)
	{
		INode *pTempNode = m_pScope->getChild(sKey, "float");
		if (verifyNode("float", pTempNode, bStatus))
			return boost::lexical_cast<float>(pTempNode->getValue());
		else
			return 0.0f;
	}

	template <class T_IArchivingDriver>
	double KeyValueArchive<T_IArchivingDriver>::getDouble(const std::string& sKey, ArchivingResult *bStatus)
	{
		INode *pTempNode = m_pScope->getChild(sKey, "double");
		if (verifyNode("double", pTempNode, bStatus))
			return boost::lexical_cast<double>(pTempNode->getValue());
		else
			return 0.0;
	}

	template <class T_IArchivingDriver>
	bool KeyValueArchive<T_IArchivingDriver>::fillObject( const std::string& sKey, IArchivableObject*& pObject, ArchivingResult *bStatus /*= NULL*/ )
	{
		INode* pTempNode = m_pScope->getChild(sKey, pObject->getClassName());

		if (pTempNode && getDelegate())
			pObject = getDelegate()->handleInstance(pObject);

		if (verifyNode(pObject->getClassName(), pTempNode, bStatus))
		{
			pushScope(pTempNode);
			pObject->deserialize((IDeserializer *)this);
			popScope();

			if (m_pDelegate != NULL)
			{
				bool bDeserialize = m_pDelegate->afterDeserializeObject(pObject);
				if(bDeserialize == false) {
					if(bStatus)
						*bStatus = Denied;
					return false;
				}
			}

			return true;
		}
		else
			return false;
	}

	template <class T_IArchivingDriver>
	std::string KeyValueArchive<T_IArchivingDriver>::getString(const std::string& sKey, ArchivingResult *bStatus)
	{
		INode *pTempNode = m_pScope->getChild(sKey, "string");
		if (verifyNode("string", pTempNode, bStatus))
			return pTempNode->getValue();
		else
			return std::string("");
	}

	template <class T_IArchivingDriver>
	INode* KeyValueArchive<T_IArchivingDriver>::getSubNode( const std::string& sKey, const std::string& sType )
	{
		INode* pNode = this->m_pScope->getChild(sKey, sType);

		if(!pNode || (pNode && pNode->getAttribute("type") != sType))
		{
			pNode = this->m_pScope->addChild(sKey);
			pNode->setAttribute("type", sType);
		}
		return pNode;
	}

	/** Other Accessors */

	/* Method: getErrorCount()
	 * Return: Returns the count of errors occured while parsing the XML.
	 */
	template <class T_IArchivingDriver>
	unsigned long KeyValueArchive<T_IArchivingDriver>::getErrorCount()
	{
		if(m_pArchivingDriver)
			return m_pArchivingDriver->getErrorCount();
		return 0ul;
	}

	/* Method: reset()
	 * Return: Resets the archive and pops the scope down to the root element.
	 */
	template <class T_IArchivingDriver>
	void KeyValueArchive<T_IArchivingDriver>::reset()
	{
		if(m_pArchivingDriver)
		{
			m_pScope = NULL;
			pushScope(m_pArchivingDriver->getRootNode());
		}
	}

	/* Method: getArchiveString()
	 * Return: Returns the archives serialized string.
	 */
	template <class T_IArchivingDriver>
	std::wstring KeyValueArchive<T_IArchivingDriver>::getArchiveString()
	{
		return m_pArchivingDriver->getString();
	}

	/** Scope */
	template <class T_IArchivingDriver>
	INode *KeyValueArchive<T_IArchivingDriver>::pushScope(INode *pNode)
	{
		if (!(m_pScope = pNode) )
			throw(std::runtime_error("error pushing NULL scope!"));
		return m_pScope;
	}

	template <class T_IArchivingDriver>
	INode *KeyValueArchive<T_IArchivingDriver>::popScope()
	{
		if (!(m_pScope = m_pScope->getParent()))
			throw(std::runtime_error("error popping NULL scope!"));
		return m_pScope;
	}
}