#ifndef _ARCHIVEUTIL_H_
#define _ARCHIVEUTIL_H_

namespace Archiving
{
	/**
	 * Return Values for various methods of the KeyValueArchive Class and the IDeserializer Interface.
	 */
	typedef enum
	{
		Found      = 0, /**< The requested value was found. */
		Undefined  = 1, /**< The requested value is empty, the return value is undefined. */ 
		BadType    = 2, /**< The requested value was found but is an object of a different type. */
		NotFound   = 3, /**< The requested value could not be found. */
		Denied     = 4  /**< The archives delegates afterDeserializeObject method returned false, and the result of getObject is undefined. */
	} ArchivingResult;
}

#endif

/*
Auf einem Baum ein Kuckuck, -
Sim sa la bim, bam ba, sa la du, sa la dim
Auf einem Baum ein Kuckuck saß.

Da kam ein junger Jäger, -
Sim sa la bim, bam ba, sa la du, sa la dim
Da kam ein junger Jägersmann.

Der schoß den armen Kuckuck, -
Sim sa la bim, bam ba, sa la du, sa la dim
Der schoß den armen Kuckuck tot.

Und als ein Jahr vergangen, -
Sim sa la bim, bam ba, sa la du, sa la dim
Und als ein Jahr vergangen war.

Da war der Kuckuck wieder, -
Sim sa la bim, bam ba, sa la du, sa la dim
Da war der Kuckuck wieder da (auch: lebendig).[3]

Da freuten sich die Leute, -
Sim sa la bim, bam ba, sa la du, sa la dim
Da freuten sich die Leute sehr.
*/