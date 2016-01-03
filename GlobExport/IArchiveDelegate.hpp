#ifndef _IARCHIVEDELEGATE_HPP_
#define _IARCHIVEDELEGATE_HPP_

#ifdef ARCHIVEUTIL_EXPORTS
#define ARCHIVEUTIL_API __declspec(dllexport)
#else
#define ARCHIVEUTIL_API __declspec(dllimport)
#endif

#pragma warning(disable: 4100)

namespace Archiving
{
	class IArchivableObject;

	/* Delegate interface for the KeyValueArchive class.
	 * @version 1.3
	 * @see     KeyValueArchive
	 */
	class ARCHIVEUTIL_API IArchiveDelegate
	{
	public:
		/* Handles an error occured while serializing or deserializing with the archive.
		 * Note taht this method does not handle errors occured while loading or saving the archive;
		 * The default implementation returns always true.
		 * @param  The error object.
		 * @return If the returned value is false, the archive will stop serializing or deserializing the object that raises the error. Otherwise not.
		 */
		virtual bool handleError( const int iError ){return true;} //Nocht nicht verwenden.. noch in arbeit :)

		/* After serializing an object to the archive this method will be called.
		 * @param The new already serialized object.
		 * @see   afterDeserializeObject()
		 */
		virtual void afterSerializeObject(IArchivableObject *pObject){;}

		/* Before serializing an object from the archive this method will be called.
		 * It allows you to modify it or block it from being inserted into the archive.
		 * @param  pObject The object to be serialized.
		 * @return A flag telling, whether the new instance is to be returned to the client.
		 *         True by default. If set to false, then pObject will not be inserted
		 *         into the archive.
		 * @see    afterSerializeObject, IDeserializer::getObject, handleInstance
		 */
		virtual bool preSerializeObject(IArchivableObject *pObject){return true;}

		/* After deserializing an object from the archive this method will be called.
		 * It allows you to modify it or block it from being returned to the client.
		 * @param  pObject The new deserialized object.
		 * @return A flag telling, whether the new instance is to be returned to the client.
		 *         True by default. If set to false, then the corresponding call to getObject
		 *         will return NULL
		 * @see    afterSerializeObject, IDeserializer::getObject, handleInstance
		 */
		virtual bool afterDeserializeObject(IArchivableObject *pObject){return true;}

		/* Handles the instance of the next deserialized object.
		 * The return value of this method is used for deserializing the given object.
		 * This Method allows you to do change the class or the object itself before it wil be deserialized.
		 * The default implementation returns the given object unmodified.
		 * Note that this is the wrong place doing some slow.
		 * @param  The object that should be deserialized.
		 * @return The object that will be deserialized.
		 * @see    afterSerializeObject(), IArchivalbeObject and KeyValueArchive
		 */
		virtual IArchivableObject* handleInstance( const IArchivableObject* pObject ){return const_cast<IArchivableObject*>(pObject);}
	};
}

#endif

#pragma warning(default: 4100)
