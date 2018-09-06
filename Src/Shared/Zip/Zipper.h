#pragma once
#include <ShareDefine.h>
class Zipper
{
public:
	static bool ZipFile(const char* Buffer, uint32& Lenth);
	static bool UnZip();
};