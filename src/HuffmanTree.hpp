#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

struct HuffmanNode
{
	unsigned int symbol = 0;
	HuffmanNode* left = nullptr;
	HuffmanNode* right = nullptr;
};

class HuffmanTree
{
	private:
		HuffmanNode* root;
	public:
		HuffmanTree()
		{
			root = new HuffmanNode();
		}

		~HuffmanTree()
		{
			DeleteTree(root);
		}

		void DeleteTree(HuffmanNode* tree)
		{
			if (tree == nullptr) { return; }

			DeleteTree(tree->left);
			DeleteTree(tree->right);
			delete tree;
		}
	
		void Insert(unsigned int code, int numberOfBits, unsigned int symbol)
		{
			HuffmanNode* currentNode = root;
	
			for (int i = numberOfBits - 1; i >= 0; i--)
			{
				int direction = (code >> i) & 1;
				if (direction == 0) // Left
				{
					if (currentNode->left == nullptr)
					{
						currentNode->left = new HuffmanNode();
					}
	
					currentNode = currentNode->left;
				}
				else // Right
				{
					if (currentNode->right == nullptr)
					{
						currentNode->right = new HuffmanNode();
					}
	
					currentNode = currentNode->right;
				}
			}
	
			currentNode->symbol = symbol;
		}
	
		HuffmanNode* GetRoot()
		{
			return root;
		}
};

#endif