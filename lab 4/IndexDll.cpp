// IndexDll.cpp: ���������� ���������������� ������� ��� ���������� DLL.
//

#include "stdafx.h"
const int t = 2;
typedef struct Abonent
{
	char secondName[255];
	char name[255];
	char patronymic[255];
	char number[20];
	char adress[255];
};
typedef struct TKey
{
	char key[255];
	Abonent* abonentsArray;
};

typedef struct TNode
{
	int keyNumber;
	bool isLeaf;
	TKey keyArray[2 * t - 1];
	TNode* childrenArray[2*t];
};

typedef struct TT
{
	TNode* root;
};



TT* BTree = new TT;
void CreateBTree(TT* T);
void BTreeInsert(TT *T, TKey* k);
int BTreeDelete(TNode* x, char* key);
void BTreeSearch(TNode* x, char* key, TNode* resultNode, int* resultIndex);


extern "C"__declspec(dllexport) void __stdcall CreateIndex()
{
	CreateBTree(BTree);
}

extern "C" __declspec(dllexport) void __stdcall DataBaseInsert(char secondName[255])
{
	TNode* resultNode = 0;
	int resultIndex = 1;
	BTreeSearch(BTree->root, secondName, resultNode, &resultIndex);
	if (resultNode == NULL)
	{
		Abonent* newAbonent = new Abonent;
		strcpy(newAbonent->secondName, secondName);
		TKey* k = new TKey;
		strcpy(k->key, secondName);
		BTreeInsert(BTree, k);
	}
}

extern "C" __declspec(dllexport) void __stdcall DataBaseDelete(char secondName[255])
{
	BTreeDelete(BTree->root, secondName);
}

extern "C" __declspec(dllexport) void __stdcall DataBaseSearch(char secondName[255])
{

}


void CreateBTree(TT* T)
{
	TNode* newNode = new TNode;
	newNode->keyNumber = 0;
	newNode->isLeaf = true;
	T->root = newNode;
}

void BTreeSplitChild(TNode* x, int i)
{
	TNode* z = new TNode;
	TNode* y = x->childrenArray[i];
	z->isLeaf = y->isLeaf;
	z->keyNumber = t - 1;
	for (int j = 0; j < t - 1; j++)
		z->keyArray[j] = y->keyArray[j + t];
	if (!y->isLeaf)
	{
		for (int j = 0; j < t; j++)
			z->childrenArray[j] = y->childrenArray[j + t];
	}
	y->keyNumber = t - 1;
	for (int j = x->keyNumber; j > i + 1; j--)
	{
		x->childrenArray[j + 1] = x->childrenArray[j];
	}
	x->childrenArray[i + 1] = z;
	for (int j = x->keyNumber - 1; j > i; j--)
	{
		x->keyArray[j + 1] = x->keyArray[j];
	}
	x->keyArray[i] = y->keyArray[t - 1];
	x->keyNumber++;
}

void BTreeInsertNonFull(TNode* x, TKey* k)												
{
	if (x->isLeaf)
	{
		int i =  x->keyNumber - 1;
		while (i >= 0 && strcmp(k->key, x->keyArray[i].key) < 0)									
		{
			x->keyArray[i + 1] = x->keyArray[i];
			i--;
		}
		x->keyArray[i+1] = *k;
		x->keyNumber++;
	}
	else
	{
		int i = x->keyNumber - 1;
		while (i >= 0 && strcmp(k->key, x->keyArray[i].key) < 0)									
			i--;
		i++;
		if (x->childrenArray[i]->keyNumber == 2 * t - 1)
		{
			BTreeSplitChild(x, i);
			if (strcmp(k->key, x->keyArray[i].key) > 0)
				i++;
		}
		BTreeInsertNonFull(x->childrenArray[i], k);
	}
}

void BTreeInsert(TT *T, TKey* k)
{
	TNode* root = T->root;
	if (root->keyNumber == 2 * t - 1)
	{
		TNode* s = new TNode;
		T->root = s;
		s->isLeaf = false;
		s->keyNumber = 0;
		s->childrenArray[0] = root;
		BTreeSplitChild(s, 0);
		BTreeInsertNonFull(s, k);
	}
	else
		BTreeInsertNonFull(root, k);
}

void BTreeSearch(TNode* x, char* key, TNode* resultNode, int* resultIndex)
{
	int i = 0;
	while (i <= x->keyNumber - 1 && strcmp(key, x->keyArray[i].key) > 0)
		i++;
	if (i < x->keyNumber && strcmp(key,x->keyArray[i].key)==0)
	{
		resultNode = x;
		*resultIndex = i;
	}
	else
	{
		if (x->isLeaf == true)
		{
			resultNode = 0;
			*resultIndex = NULL;
		}
		else
			BTreeSearch(x->childrenArray[i], key, resultNode, resultIndex);
	}
}

int BTreeDelete(TNode* x, char* key)
{
	int i = x->keyNumber - 1;
	while (i >= 0 && strcmp(key, x->keyArray[i].key) < 0)
		i--;
	if (x->isLeaf == true)
	{
		if (i >= 0 && strcmp(key, x->keyArray[i].key) == 0)
		{
			delete &x->keyArray[i];
			while (i < x->keyNumber - 1)
			{
				x->keyArray[i] = x->keyArray[i + 1];
				i++;
			}
			x->keyNumber--;
		}
		else
			return 0;
	}
	else
	{
		if (i >= 0 && strcmp(key, x->keyArray[i].key) == 0)
		{
			TNode* y = x->childrenArray[i];
			TNode* z = x->childrenArray[i + 1];
			if (x->childrenArray[i]->keyNumber > t - 1)
			{
				x->keyArray[i] = y->keyArray[y->keyNumber - 1];
				BTreeDelete(y, key);
			}
			else
			{
				if (z->keyNumber > t - 1)
				{
					x->keyArray[i+1] = z->keyArray[0];
					BTreeDelete(z, key);
				}
				else
				{
					y->keyArray[y->keyNumber] = x->keyArray[i];
					int j;
					for (j = 0; j < z->keyNumber;j++)
					{
						y->keyArray[y->keyNumber + 1 + j] = z->keyArray[j];
						y->childrenArray[y->keyNumber + 1 + j] = z->childrenArray[j];
					}
					y->childrenArray[y->keyNumber + 2 + j] = z->childrenArray[j + 1];
					y->keyNumber = 2 * t - 1;
					//delete z;
					for (int j = i; j < x->keyNumber - 1;j++)
						x->keyArray[j] = x->keyArray[j + 1];
					for (int j = i + 1; j < x->keyNumber; j++)
						x->childrenArray[j] = x->childrenArray[j + 1];
					x->keyNumber--;
					BTreeDelete(y, key);
				}
			}
		}
		else
		{
			if (x->childrenArray[i + 1]->keyNumber == t - 1)
			{

				if (i > 0 && x->childrenArray[i]->keyNumber > t - 1)
				{ 
					for (int j = x->childrenArray[i + 1]->keyNumber - 1; j <= 0; j--)
						x->childrenArray[i + 1]->keyArray[j] = x->childrenArray[i + 1]->keyArray[j + 1];
					for (int j = x->childrenArray[i + 1]->keyNumber; j <= 0; j--)
						x->childrenArray[i + 1]->childrenArray[j] = x->childrenArray[i + 1]->childrenArray[j + 1];
					x->childrenArray[i + 1]->keyArray[0] = x->keyArray[i];
					x->childrenArray[i + 1]->childrenArray[0] = x->childrenArray[i]->childrenArray[x->childrenArray[i]->keyNumber];
					x->keyArray[i] = x->childrenArray[i]->keyArray[x->childrenArray[i]->keyNumber - 1];
					//delete  x->childrenArray[i]->childrenArray[x->childrenArray[i]->keyNumber];
					x->childrenArray[i]->keyNumber--;
					x->childrenArray[i+1]->keyNumber++;
				}
				else
				{
					if (i < 2*t-1 && x->childrenArray[i + 2]->keyNumber > t - 1)
					{
						x->childrenArray[i + 1]->keyArray[x->childrenArray[i + 1]->keyNumber] = x->keyArray[i+1];
						x->childrenArray[i + 1]->childrenArray[x->childrenArray[i + 1]->keyNumber + 1] = x->childrenArray[i+2]->childrenArray[0];
						x->keyArray[i + 1] = x->childrenArray[i + 2]->keyArray[0];
						//delete x->childrenArray[i + 2]->childrenArray[0];
						for (int j = 0; j < x->childrenArray[i + 2]->keyNumber - 1; j--)
							x->childrenArray[i + 2]->keyArray[j] = x->childrenArray[i + 2]->keyArray[j + 1];
						for (int j = 0; j < x->childrenArray[i + 2]->keyNumber; j--)
							x->childrenArray[i + 2]->childrenArray[j] = x->childrenArray[i + 2]->childrenArray[j + 1];
						x->childrenArray[i + 2]->keyNumber--;
						x->childrenArray[i + 1]->keyNumber++;
					}
					else
					{
						if (i == 0)
							i++;
						TNode* y = x->childrenArray[i];
						TNode* z = x->childrenArray[i + 1];
						y->keyArray[y->keyNumber] = x->keyArray[i];
						int j;
						for (j = 0; j < z->keyNumber; j++)
						{
							y->keyArray[y->keyNumber + 1 + j] = z->keyArray[j];
							y->childrenArray[y->keyNumber + 1 + j] = z->childrenArray[j];
						}
						y->childrenArray[y->keyNumber + 2 + j] = z->childrenArray[j + 1];
						y->keyNumber = 2 * t - 1;
						//delete z;
						for (int j = i; j < x->keyNumber - 1; j++)
							x->keyArray[j] = x->keyArray[j + 1];
						for (int j = i + 1; j < x->keyNumber; j++)
							x->childrenArray[j] = x->childrenArray[j + 1];
						x->keyNumber--;
					}
				}
				BTreeDelete(x->childrenArray[i], key);
			}
			else
				BTreeDelete(x->childrenArray[i + 1], key);
		}
	}
	return 1;
}

