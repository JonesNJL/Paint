#ifndef BIT_READER_H
#define BIT_READER_H

#include <iostream>

class LeastFirstBitReader
{
	private:
		unsigned char* data;
		int currentBytePos;
		unsigned char unreadBits;
		int unreadBitCount;

		unsigned char Reverse(unsigned char b)
		{
			b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
			b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
			b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
			return b;
		}

	public:
		LeastFirstBitReader(unsigned char* data)
		{
			this->data = data;
			this->currentBytePos = 0;
			this->unreadBits = 0;
			this->unreadBitCount = 0;
		}
	
		unsigned char ReadByte()
		{
			unreadBitCount = 0; //Discard unread bits
			unreadBits = 0; //Discard unread bits
			unsigned char byte = data[currentBytePos];
			currentBytePos += 1;
			return byte;
		}
	
		unsigned char ReadBit()
		{
			if (unreadBitCount <= 0)
			{
				unreadBits = ReadByte();
				unreadBitCount = 8;
			}

			unsigned char bit = unreadBits & 1;
			unreadBits >>= 1;
			unreadBitCount -= 1;

			return bit;
		}
	
		unsigned int ReadBits(int numberOfBits)
		{
			unsigned int bits = 0;
	
			for (int i = 0; i < numberOfBits; i++)
			{
				bits += ((unsigned int)ReadBit() << i);
			}
	
			return bits;
		}
	
		unsigned int ReadBytes(int numberOfBytes)
		{
			unsigned int bytes = 0;
	
			for (int i = 0; i < numberOfBytes; i++)
			{
				bytes += ((unsigned int)ReadByte() << (i * 8));
			}
	
			return bytes;
		}
};



class MostFirstBitReader
{
	private:
		unsigned char* data;
		int currentBytePos;
		unsigned char unreadBits;
		int unreadBitCount;
	public:
		MostFirstBitReader(unsigned char* data)
		{
			this->data = data;
			this->currentBytePos = 0;
			this->unreadBits = 0;
			this->unreadBitCount = 0;
		}
	
		unsigned char ReadByte()
		{
			unreadBitCount = 0; //Discard unread bits
			unreadBits = 0; //Discard unread bits
			unsigned char byte = data[currentBytePos];
			currentBytePos += 1;
			return byte;
		}
	
		unsigned char ReadBit()
		{
			if (unreadBitCount <= 0)
			{
				unreadBits = ReadByte();
				unreadBitCount = 8;
			}
	
			unsigned char bit = unreadBits & 1;
			unreadBits >>= 1;
			unreadBitCount -= 1;
	
			return bit;
		}
	
		unsigned int ReadBits(int numberOfBits)
		{
			throw std::invalid_argument("Don't call this function, idk how this would even work...");

			unsigned int bits = 0;
	
			for (int i = 0; i < numberOfBits; i++)
			{
				bits <<= i;
				bits += (unsigned int)ReadBit();
			}
	
			return bits;
		}
	
		unsigned int ReadBytes(int numberOfBytes)
		{
			unsigned int bytes = 0;
	
			for (int i = 0; i < numberOfBytes; i++)
			{
				unsigned int byte = (unsigned int)ReadByte();
	
				std::cout << "Byte: " << byte << std::endl;
	
				bytes <<= 8;
				bytes += (byte);
			}
	
			return bytes;
		}
};



#endif