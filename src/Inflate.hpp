#ifndef INFLATE_H
#define INFLATE_H

#include <vector>
#include "BitReader.hpp"
#include "HuffmanTree.hpp"

void Infate(LeastFirstBitReader* bitReader, std::vector<unsigned char>& data);
void InflateBlockNoCompression(LeastFirstBitReader* bitReader, std::vector<unsigned char>& data);
void InflateBlockFixed(LeastFirstBitReader* bitReader, std::vector<unsigned char>& data);
void InflateBlockDynamic(LeastFirstBitReader* bitReader, std::vector<unsigned char>& data);
void InflateBlock(LeastFirstBitReader* bitReader, HuffmanTree* literalLengthTree, HuffmanTree* distanceTree, std::vector<unsigned char>& data);
void DecodeTrees(LeastFirstBitReader* bitReader, HuffmanTree*& literalLengthTree, HuffmanTree*& distanceTree);
HuffmanTree* GetFixedLiteralLengthTree();
HuffmanTree* GetFixedDistanceTree();
HuffmanTree* BitLengthsToHuffmanTree(std::vector<int> bitLengths, std::vector<unsigned int> alphabet);
unsigned int DecodeSymbol(LeastFirstBitReader* bitReader, HuffmanTree* huffmanTree);

#endif