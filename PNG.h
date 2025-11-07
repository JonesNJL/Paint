#ifndef PNG_H
#define PNG_H

#include "Image.h"
#include <fstream>
#include <vector>
#include <map>
#include "BitReader.h"
#include "Inflate.h"

struct Chunk
{
	int length;
	std::string type;
	unsigned char* data;
	int CRC;
};

struct HeaderChunkData
{
	int width;
	int height;
	int bitDepth;
	int colorSpace;
	int compressionMethod;
	int filterMethod;
	int interlacing;
};

Image LoadPNG(std::string path);
Chunk ReadChunk(int& currentByte, std::vector<unsigned char>& data);
int ReadInt(int& currentByte, std::vector<unsigned char>& data, int length);
std::string ReadString(int& currentByte, std::vector<unsigned char>& data, int length);
HeaderChunkData ReadHeaderChunk(Chunk& headerChunk);
std::vector<unsigned char> DecompressDataChunks(std::vector<Chunk>& dataChunks);
std::vector<unsigned char> UnfilterData(std::vector<unsigned char>& uncompressedData, HeaderChunkData& headerChunkData);

#endif