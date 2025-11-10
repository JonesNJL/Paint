#include "Inflate.hpp"

void Infate(LeastFirstBitReader* bitReader, std::vector<unsigned char>& data)
{
	while (true)
	{
		int finalBlock = bitReader->ReadBit();
		int blockCompressionType = bitReader->ReadBits(2);

		if (blockCompressionType == 0)
		{
			InflateBlockNoCompression(bitReader, data);
		}
		else if (blockCompressionType == 1)
		{
			InflateBlockFixed(bitReader, data);
		}
		else if (blockCompressionType == 2)
		{
			InflateBlockDynamic(bitReader, data);
		}
		else
		{
			throw std::invalid_argument("Block compression type invalid!");
		}

		if (finalBlock) { break; }
	}
}

void InflateBlockNoCompression(LeastFirstBitReader* bitReader, std::vector<unsigned char>& data)
{
	std::cout << "Inflating with no compression!" << std::endl;

	int numberOfBytes = bitReader->ReadBytes(2);
	int nLen = bitReader->ReadBytes(2);

	for (int i = 0; i < numberOfBytes; i++)
	{
		data.push_back(bitReader->ReadByte());
	}
}

void InflateBlockFixed(LeastFirstBitReader* bitReader, std::vector<unsigned char>& data)
{
	std::cout << "Inflating with fixed compression!" << std::endl;

	HuffmanTree literalLengthTree = GetFixedLiteralLengthTree();
	HuffmanTree distanceTree = GetFixedDistanceTree();

	InflateBlock(bitReader, &literalLengthTree, &distanceTree, data);
}

void InflateBlockDynamic(LeastFirstBitReader* bitReader, std::vector<unsigned char>& data)
{
	std::cout << "Inflating with dynamic compression!" << std::endl;

	HuffmanTree literalLengthTree;
	HuffmanTree distanceTree;
	DecodeTrees(bitReader, literalLengthTree, distanceTree);
	InflateBlock(bitReader, &literalLengthTree, &distanceTree, data);
}

void InflateBlock(LeastFirstBitReader* bitReader, HuffmanTree* literalLengthTree, HuffmanTree* distanceTree, std::vector<unsigned char>& data)
{
	int LengthExtraBits[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
	int LengthBase[] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
	int DistanceExtraBits[] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
	int DistanceBase[] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };

	while (true)
	{
		unsigned int symbol = DecodeSymbol(bitReader, literalLengthTree);

		//std::cout << "Symbol: " << symbol << std::endl;

		if (symbol < 256) //Literal value
		{
			data.push_back((unsigned char)symbol);
		}
		else if (symbol == 256) //End of block
		{
			return;
		}
		else //Length
		{
			int lengthIndex = symbol - 257;
			int lengthExtraBitsValue = bitReader->ReadBits(LengthExtraBits[lengthIndex]);
			int length = LengthBase[lengthIndex] + lengthExtraBitsValue;

			int distanceIndex = DecodeSymbol(bitReader, distanceTree);
			int distanceExtraBitsValue = bitReader->ReadBits(DistanceExtraBits[distanceIndex]);
			int distance = DistanceBase[distanceIndex] + distanceExtraBitsValue;

			int dataStartIndex = data.size() - distance;

			for (int i = 0; i < length; i++)
			{
				int index = dataStartIndex + (i % distance);
				data.push_back(data[index]);
			}
		}
	}
}

void DecodeTrees(LeastFirstBitReader* bitReader, HuffmanTree& literalLengthTree, HuffmanTree& distanceTree)
{
	int literalLengthAlphabetLength = bitReader->ReadBits(5) + 257;
	int distanceAlphabetLength = bitReader->ReadBits(5) + 1;
	int huffmanLengthAlphabetLength = bitReader->ReadBits(4) + 4;

	unsigned int huffmanLengthAlphabetOrder[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
	std::vector<unsigned int> huffmanLengthAlphabet = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };

	std::vector<int> huffmanLengthBitLengths(19, 0);
	for (int i = 0; i < huffmanLengthAlphabetLength; i++)
	{
		huffmanLengthBitLengths[huffmanLengthAlphabetOrder[i]] = bitReader->ReadBits(3);
	}

	HuffmanTree huffmanLengthTree = BitLengthsToHuffmanTree(huffmanLengthBitLengths, huffmanLengthAlphabet);

	std::vector<int> bitLengths;
	while (bitLengths.size() < literalLengthAlphabetLength + distanceAlphabetLength)
	{
		unsigned int symbol = DecodeSymbol(bitReader, &huffmanLengthTree);

		//std::cout << "lit: " << literalLengthAlphabetLength << std::endl;
		//std::cout << "dis: " << distanceAlphabetLength << std::endl;
		//std::cout << "bls: " << bitLengths.size() << std::endl;

		if (symbol >= 0 && symbol <= 15)
		{
			bitLengths.push_back((int)symbol);
		}
		else if (symbol == 16)
		{
			int lastBitLength = bitLengths[bitLengths.size() - 1];
			int repeatAmount = bitReader->ReadBits(2) + 3;

			for (int i = 0; i < repeatAmount; i++)
			{
				bitLengths.push_back(lastBitLength);
			}
		}
		else if (symbol == 17)
		{
			int repeatAmount = bitReader->ReadBits(3) + 3;

			for (int i = 0; i < repeatAmount; i++)
			{
				bitLengths.push_back(0);
			}
		}
		else if (symbol == 18)
		{
			int repeatAmount = bitReader->ReadBits(7) + 11;

			for (int i = 0; i < repeatAmount; i++)
			{
				bitLengths.push_back(0);
			}
		}
		else
		{
			std::cout << "lit: " << literalLengthAlphabetLength << std::endl;
			std::cout << "dis: " << distanceAlphabetLength << std::endl;
			std::cout << "bls: " << bitLengths.size() << std::endl;
			std::cout << "bruh" << symbol << std::endl;
			throw std::invalid_argument("Invalid symbol! Must be between 0 and 18!");
		}
	}

	//std::cout << "BRUHHH: " << bitLengths[255] << std::endl;
	//std::cout << "l: " << huffmanLengthAlphabetLength << std::endl;
	//throw std::invalid_argument("eeee");

	std::vector<int> literalLengthBitLengths(bitLengths.begin(), bitLengths.begin() + literalLengthAlphabetLength);
	std::vector<int> distanceBitLengths(bitLengths.begin() + literalLengthAlphabetLength, bitLengths.end());

	std::vector<unsigned int> literalLengthAlphabet; for (unsigned int i = 0; i < 286; i++) { literalLengthAlphabet.push_back(i); };
	std::vector<unsigned int> distanceAlphabet; for (unsigned int i = 0; i < 30; i++) { distanceAlphabet.push_back(i); };

	literalLengthTree = BitLengthsToHuffmanTree(literalLengthBitLengths, literalLengthAlphabet);
	distanceTree = BitLengthsToHuffmanTree(distanceBitLengths, distanceAlphabet);
}

HuffmanTree GetFixedLiteralLengthTree()
{
	std::vector<int> bitLengths(288, 0);
	for (int i = 0; i <= 143; i++) { bitLengths[i] = 8; }
	for (int i = 144; i <= 255; i++) { bitLengths[i] = 9; }
	for (int i = 256; i <= 279; i++) { bitLengths[i] = 7; }
	for (int i = 280; i <= 287; i++) { bitLengths[i] = 8; }

	std::vector<unsigned int> alphabet(288, 0);
	for (int i = 0; i < 288; i++) { alphabet[i] = i; }

	return BitLengthsToHuffmanTree(bitLengths, alphabet);
}

HuffmanTree GetFixedDistanceTree()
{
	std::vector<int> bitLengths(30, 0);
	for (int i = 0; i <= 29; i++) { bitLengths[i] = 5; }

	std::vector<unsigned int> alphabet(30, 0);
	for (int i = 0; i < 30; i++) { alphabet[i] = i; }

	return BitLengthsToHuffmanTree(bitLengths, alphabet);
}

HuffmanTree BitLengthsToHuffmanTree(std::vector<int> bitLengths, std::vector<unsigned int> alphabet)
{
	int maxBitLength = 0;
	for (int i = 0; i < bitLengths.size(); i++)
	{
		if (bitLengths[i] > maxBitLength) { maxBitLength = bitLengths[i]; }
	}

	std::vector<int> bitLengthCounts(maxBitLength + 1, 0);
	for (int i = 0; i < bitLengths.size(); i++)
	{
		if (bitLengths[i] == 0) { continue; }

		bitLengthCounts[bitLengths[i]] += 1;
	}

	std::vector<unsigned int> nextCodes(2, 0);
	for (int i = 2; i < maxBitLength + 1; i++)
	{
		nextCodes.push_back((nextCodes[i - 1] + bitLengthCounts[i - 1] << 1));
	}

	HuffmanTree huffmanTree = HuffmanTree();
	for (int i = 0; i < bitLengths.size(); i++)
	{
		unsigned int symbol = alphabet[i];
		int bitLength = bitLengths[i];

		if (bitLength == 0) { continue; }

		huffmanTree.Insert(nextCodes[bitLength], bitLength, symbol);
		nextCodes[bitLength] += 1;
	}

	return huffmanTree;
}

unsigned int DecodeSymbol(LeastFirstBitReader* bitReader, HuffmanTree* huffmanTree)
{
	HuffmanNode* currentNode = huffmanTree->GetRoot();

	while (currentNode->left != nullptr && currentNode->right != nullptr)
	{
		int direction = bitReader->ReadBit();
		if (direction == 0) { currentNode = currentNode->left; }
		else { currentNode = currentNode->right; }
	}

	return currentNode->symbol;
}