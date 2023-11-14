#pragma once
#include "zconf.h"
#include "zlib.h"
#include "cstdint"

namespace Utils
{
	class Zlw
	{
	public:
		static bool Decompress(uint32_t tCompressSize, uint8_t* tCompress, uLongf& tOriginalSize, uint8_t* tOriginal)
		{
			if ((uncompress(tOriginal, &tOriginalSize, tCompress, tCompressSize)) != Z_OK)
			{
				return false;
			}
			return true;
		}

		static bool Compress(uint8_t* tOriginal, uLongf tOriginalSize, uint8_t* tCompress, uLongf& tCompressSize)
		{
			if ((compress(tCompress, &tCompressSize, tOriginal, tOriginalSize)) != Z_OK)
			{
				return false;
			}
			return true;
		}

		static uLongf CompressBound(uLong size)
		{
			return compressBound(size);
		}
	};
}
