#ifndef _IARCHIVABLEOBJECT_H_
#define _IARCHIVABLEOBJECT_H_

#ifdef ARCHIVEUTIL_EXPORTS
#define ARCHIVEUTIL_API __declspec(dllexport)
#else
#define ARCHIVEUTIL_API __declspec(dllimport)
#endif

#include "IInstanceCounter.hpp"

namespace Archiving
{
	class ISerializer;
	class IDeserializer;

	/* @brief   IArchivableObject interface class.
	 * @author  jbi, jwl
	 * @version 1.1
	 * @see     ISerializer, IDeserializer
	 */
	class ARCHIVEUTIL_API IArchivableObject : public IInstanceCounter<IArchivableObject>
	{
	public:
		/* @brief   Overwrite this method with the serialization
		 *          of your class.
		 * @see     deserialize(IDeserializer *)
		 */
		virtual void serialize(ISerializer *encoder) = 0;

		/* @brief   Overwrite this method with the deserialization
		 *          of your class.
		 * @see     deserialize(IDeserializer *)
		 */
		virtual void deserialize(IDeserializer *decoder) = 0;

		/* @brief   Get the class name under which this class instaces should be serialized.
		 *          The default implementation returns the original class name (example: "class MyObject").
		 *          Note that this classname is also used for deserializing. If youn change it your old archive files might not work correctly.
		 * @return  The class that should be used for serializing instance of this class.
		 * @see     setObject(), serialize() and deserialize()
		 */
		virtual std::string getClassName() const;
	};
}

#endif
