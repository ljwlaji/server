#include "Zipper.h"
#include <zlib.h>


bool Zipper::ZipFile(const char* Buffer, uint32& Lenth)
{
	uLong OriginSize = Lenth;
	uLong AfterSize = 0;

	AfterSize = compressBound(Lenth);
	unsigned char* Compressed = new unsigned char[AfterSize]();

	if (compress((Bytef *)Compressed, &AfterSize, (Bytef *)Buffer, OriginSize) != Z_OK)
	{
		delete[] Compressed;
		return false;
	}

	char* After = new char();
	if (uncompress((Bytef*)After, &OriginSize, (Bytef*)Compressed, AfterSize) != Z_OK)
	{
		return false;
	}

	Lenth = AfterSize;
	return true;
}

bool Zipper::UnZip()
{
	return false;
}
