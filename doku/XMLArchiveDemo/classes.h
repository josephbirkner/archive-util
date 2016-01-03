#include <memory>
#include "Base/ArchiveUtil/GlobExport/ArchiveUtil.hpp"

using namespace std::tr1;
using namespace Archiving;

class Size : public IArchivableObject
{
public:
	float width;
	float height;

	Size() : width(0), height(0) {}

	void serialize(ISerializer *encoder)
	{
		encoder->setFloat(width, "width");
		encoder->setFloat(height, "height");
	}

	void deserialize(IDeserializer *decoder)
	{
		width = decoder->getFloat("width", NULL);
		height = decoder->getFloat("height", NULL);
	}
};

class Point : public IArchivableObject
{
public:
	float x;
	float y;

	Point() : x(0), y(0) {}

	void serialize(ISerializer *encoder)
	{
		encoder->setFloat(x, "x");
		encoder->setFloat(y, "y");
	}

	void deserialize(IDeserializer *decoder)
	{
		x = decoder->getFloat("x", NULL);
		y = decoder->getFloat("y", NULL);
	}

};

class Rect : public IArchivableObject
{
public:
	shared_ptr<Size> size;
	shared_ptr<Point> origin;

	Rect(float x, float y, float w, float h)
	{
		size = shared_ptr<Size>(new Size());
		origin = shared_ptr<Point>(new Point());
		
		origin->x = x;
		origin->y = y;
		size->width = w;
		size->height = h;
	}

	void serialize(ISerializer *encoder)
	{
		encoder->setObject(&*size, "size");
		encoder->setObject(&*origin, "origin");
	}

	void deserialize(IDeserializer *decoder)
	{
		size = shared_ptr<Size>(decoder->getObject<Size>("size", NULL));
		origin = shared_ptr<Point>(decoder->getObject<Point>("origin", NULL));
	}
};
