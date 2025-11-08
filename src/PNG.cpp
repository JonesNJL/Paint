#include "PNG.h"

Image LoadPNG(std::string path)
{
	Image image = Image(Int2(16, 16));
	image.GenerateCheckerboard(Color(255, 128, 0, 255), Color(128, 64, 0, 255));

	std::ifstream input(path, std::ios::binary);

	if (!input.good())
	{
		std::cout << "File not found!" << std::endl;
		return image;
	}

	std::vector<char>* sdata = new std::vector<char>((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
	input.close();

	std::vector<unsigned char> data;
	for (int i = 0; i < sdata->size(); i++)
	{
		data.push_back((*sdata)[i]);
		if (data[i] < 0) { data[i] += 128; }
	}
	delete sdata;

	std::map<std::string, std::vector<Chunk>> chunks;
	int currentByte = 8;

	bool endOfImage = false;
	int chunkCount = 0;
	while (!endOfImage)
	{
		Chunk chunk = ReadChunk(currentByte, data);
		chunks[chunk.type].push_back(chunk);

		if (chunk.type == "IEND")
		{
			endOfImage = true;
		}

		chunkCount += 1;
		std::cout << chunkCount << " Balls (length): " << chunk.length << std::endl;
		std::cout << chunkCount << " Balls (type): " << chunk.type << std::endl;
		std::cout << chunkCount << " Balls (crc): " << chunk.CRC << std::endl;
	}

	std::cout << "Count: " << chunkCount << std::endl;

	HeaderChunkData headerChunkData = ReadHeaderChunk(chunks["IHDR"][0]);

	std::cout << "Header 1: " << headerChunkData.width << std::endl;
	std::cout << "Header 2: " << headerChunkData.height << std::endl;
	std::cout << "Header 3: " << headerChunkData.bitDepth << std::endl;
	std::cout << "Header 4: " << headerChunkData.colorSpace << std::endl;
	std::cout << "Header 5: " << headerChunkData.compressionMethod << std::endl;
	std::cout << "Header 6: " << headerChunkData.filterMethod << std::endl;
	std::cout << "Header 7: " << headerChunkData.interlacing << std::endl;

	std::vector<unsigned char> uncompressedData = DecompressDataChunks(chunks["IDAT"]);
	std::vector<unsigned char> unfilteredData = UnfilterData(uncompressedData, headerChunkData);

	Image newImage = Image(Int2(headerChunkData.width, headerChunkData.height), Color(0, 0, 0, 255));
	for (int i = 0; i < unfilteredData.size() / 4; i++)
	{
		Int2 pixelPos = Int2(i % headerChunkData.width, (headerChunkData.height - 1) - (i / headerChunkData.width));
		int pixelStartIndex = i * 4;

		Color pixelColor = Color
		(
			unfilteredData[pixelStartIndex],
			unfilteredData[pixelStartIndex + 1],
			unfilteredData[pixelStartIndex + 2],
			unfilteredData[pixelStartIndex + 3]
		);

		newImage.SetPixel(pixelPos, pixelColor);
	}

	std::cout << "hehehehe: " << unfilteredData.size() << std::endl;

	return newImage;
}

Chunk ReadChunk(int& currentByte, std::vector<unsigned char>& data)
{
	Chunk chunk;

	chunk.length = ReadInt(currentByte, data, 4);
	chunk.type = ReadString(currentByte, data, 4);
	chunk.data = &(data[currentByte]);
	currentByte += chunk.length;
	chunk.CRC = ReadInt(currentByte, data, 4);

	return chunk;
}

int ReadInt(int& currentByte, std::vector<unsigned char>& data, int length)
{
	int integer = 0;
	for (int i = 0; i < length; i++)
	{
		integer *= 256;
		integer += data[currentByte];
		currentByte++;
	}

	return integer;
}

std::string ReadString(int& currentByte, std::vector<unsigned char>& data, int length)
{
	std::string string = "";
	for (int i = 0; i < length; i++)
	{
		string += data[currentByte];
		currentByte++;
	}

	return string;
}

HeaderChunkData ReadHeaderChunk(Chunk& headerChunk)
{
	HeaderChunkData headerChunkData;
	MostFirstBitReader bitReader(headerChunk.data);

	headerChunkData.width = bitReader.ReadBytes(4);
	headerChunkData.height = bitReader.ReadBytes(4);
	headerChunkData.bitDepth = bitReader.ReadByte();
	headerChunkData.colorSpace = bitReader.ReadByte();
	headerChunkData.compressionMethod = bitReader.ReadByte();
	headerChunkData.filterMethod = bitReader.ReadByte();
	headerChunkData.interlacing = bitReader.ReadByte();

	return headerChunkData;
}

std::vector<unsigned char> DecompressDataChunks(std::vector<Chunk>& dataChunks)
{
	std::cout << "Starting all chunk decompression!" << std::endl;

	std::vector<unsigned char> uncompressedData;

	std::vector<unsigned char> compressedData;
	for (int i = 0; i < dataChunks.size(); i++)
	{
		for (int j = 0; j < dataChunks[i].length; j++)
		{
			compressedData.push_back(dataChunks[i].data[j]);
		}
	}

	std::cout << "Data Chunks Length: " << dataChunks.size() << std::endl;

	LeastFirstBitReader bitReader(compressedData.data());

	unsigned char cmf = bitReader.ReadByte(); //Reading the CMF byte

	int cm = cmf & 15;
	if (cm != 8)
	{
		//8 specifies DEFLATE, the only one supported by zlib
		throw std::invalid_argument("Invalid CM value!");
	}

	int cInfo = (cmf >> 4) & 15;
	if (cInfo > 7)
	{
		//Anything higher excedes the LZ77 window size
		throw std::invalid_argument("Invalid CINFO value!");
	}

	unsigned char flg = bitReader.ReadByte(); //Reading the FLG byte

	int fCheck = flg & 32; //Not needed

	int fDict = (flg >> 5) & 1;
	if (fDict == 1)
	{
		//PNGs (and ZIPs) don't use preset dictionaries
		throw std::invalid_argument("Invalid FDICT value!");
	}

	int fLevel = (flg >> 6) & 2; //Not needed

	if (((cmf * 256) + flg) % 31 != 0)
	{
		throw std::invalid_argument("CMF and FLG checksum must be a multiple of 31!");
	}

	Infate(&bitReader, uncompressedData);

	return uncompressedData;
}

std::vector<unsigned char> UnfilterData(std::vector<unsigned char>& uncompressedData, HeaderChunkData& headerChunkData)
{
	std::cout << "Uncompressed Length: " << uncompressedData.size() << std::endl;

	std::vector<unsigned char> unfilteredData;

	int i = 0;
	for (int y = 0; y < headerChunkData.height; y++)
	{
		unsigned char filter = uncompressedData[i];
		i += 1;

		for (int x = 0; x < headerChunkData.width; x++)
		{
			int pixelIndex = ((y * headerChunkData.width) + x) * 4;
			for (int b = 0; b < 4; b++)
			{
				unsigned char uncompressedByte = uncompressedData[i];
				unsigned char unfilteredByte = 255;

				if (filter == 0)
				{
					unfilteredByte = uncompressedByte;
				}
				else if (filter == 1)
				{
					unsigned char leftByte = 0;
					if (x != 0) { leftByte = unfilteredData[pixelIndex + b - 4]; }

					unfilteredByte = uncompressedByte + leftByte;
					//std::cout << "Filter 1!" << std::endl;
				}
				else if (filter == 2)
				{
					unsigned char upByte = 0;
					if (y != 0) { upByte = unfilteredData[pixelIndex + b - (4 * headerChunkData.width)]; }

					unfilteredByte = uncompressedByte + upByte;
					//std::cout << "unc: " << (int)uncompressedByte << std::endl;
					//std::cout << "upb: " << (int)upByte << std::endl;
					//std::cout << "unf: " << (int)unfilteredByte << std::endl;
					//std::cout << "Filter 2!" << std::endl;
				}
				else if (filter == 3)
				{
					unsigned char leftByte = 0;
					if (x != 0) { leftByte = unfilteredData[pixelIndex + b - 4]; }

					unsigned char upByte = 0;
					if (y != 0) { upByte = unfilteredData[pixelIndex + b - (4 * headerChunkData.width)]; }

					unfilteredByte = uncompressedByte + floor(((unsigned int)leftByte + (unsigned int)upByte) / 2);
					//std::cout << "Filter 3!" << std::endl;
				}
				else if (filter == 4)
				{
					unsigned char leftByte = 0;
					if (x != 0) { leftByte = unfilteredData[pixelIndex + b - 4]; }

					unsigned char upByte = 0;
					if (y != 0) { upByte = unfilteredData[pixelIndex + b - (4 * headerChunkData.width)]; }

					unsigned char leftUpByte = 0;
					if (x != 0 && y != 0) { leftUpByte = unfilteredData[pixelIndex + b - (4 * headerChunkData.width) - 4]; }

					int paethPredictor;

					int a = leftByte;
					int b = upByte;
					int c = leftUpByte;

					int p = a + b - c;
					int pa = abs(p - a);
					int pb = abs(p - b);
					int pc = abs(p - c);

					if (pa <= pb && pa <= pc) { paethPredictor = a; }
					else if (pb <= pc) { paethPredictor = b; }
					else { paethPredictor = c; }

					unfilteredByte = uncompressedByte + paethPredictor;

					//std::cout << "Filter 4!" << std::endl;
				}
				else
				{
					std::cout << "filter: " << (int)filter << std::endl;
					throw std::invalid_argument("Invalid filter type!");
				}

				unfilteredData.push_back(unfilteredByte);
				i += 1;
			}
		}
	}

	return unfilteredData;
}