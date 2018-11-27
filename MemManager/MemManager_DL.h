#pragma once

// ������ ��������� ������ in place.
// ���������� �������, � ������ ������� - ��������� MemChain {�����, �����}
// ��������� 2 ������ - ��� ��������� � ��� ���������� ������.
// 

#include <iostream>
#include <stdlib.h>
#include <deque>
#include <set>
using std::deque;
using std::set;

class MemManager_DL
{
	struct MemChain
	{					// structure for memory block information
		void * addr;			// adress of data block
		int size;				// don't include size of header !!! MUST be SIGNED!
		//char free;					// ����� - 0, �������� - 1
	};
	unsigned HeadSize;
	
	deque<MemChain*> AllocatedList;		// List of allocated blockes
	set<MemChain*> FreeList;			// list of free blockes
	char * Mem ;						// "heap" - placement of all blockes
	unsigned totalSize = 0;				// ?
public:
	MemManager_DL(unsigned _size);
	~MemManager_DL();
	void * alloc(unsigned size);
	void free(void * ptr);
	void Print(const char * str);
	void GarbCol();
};

// 
MemManager_DL::MemManager_DL(unsigned _size)
{
	HeadSize = sizeof MemChain;
	Mem = (char*)malloc(_size);
	if (Mem == NULL) throw "error allocating mem";
	totalSize = _size;
	MemChain *ch = (MemChain *)Mem;
	ch->addr = Mem ;
	ch->size = _size - HeadSize;
	FreeList.insert(ch);
}

MemManager_DL::~MemManager_DL()
{
	if (Mem != NULL) free(Mem);
	FreeList.clear();
	AllocatedList.clear();
}


void * MemManager_DL::alloc(unsigned size)
{
	unsigned req_size = size + HeadSize;
	auto ch = FreeList.begin();

	for (; ch !=FreeList.end(); ch++)
	{
		if ((*ch)->size >= size) break;
	}
	if (ch != FreeList.end())
	{	// ������� �� ������� ����� ������ ����� � ������� � ������ ������ �� ���������� ��������� ����� 
		MemChain * ch1;					// new block - describe �������
		unsigned shift = req_size;		// ����� ������ ����� relative to prev
		ch1 = (MemChain *)((char*)(*ch) + req_size);
		ch1->addr = (char*)(*ch)->addr + req_size;
		ch1->size = (*ch)->size - req_size;
		(*ch)->size = size;
		AllocatedList.push_back(*ch);
		auto address = (char*)(*ch)->addr + HeadSize;
		if (ch1->size > 0) {
			FreeList.erase(ch);
			FreeList.insert(ch1);
		}
		else FreeList.erase(ch);
		return address;
	}
	return NULL;
}

void  MemManager_DL::free(void * ptr)
{
	if (ptr<Mem && ptr>Mem + totalSize) throw "wrong adr";
	auto ch = AllocatedList.begin();
	for (; ch != AllocatedList.end(); ch ++) if ((char*)(*ch)->addr + HeadSize == ptr) {
		FreeList.insert(*ch);
		AllocatedList.erase(ch);
		break;
	}
}
unsigned maxSize() { return 1; };

void MemManager_DL::GarbCol()
{
	// ������� ������
	auto ch = FreeList.begin(), pr = ch;
	ch++;
	for (; ch != FreeList.end(); ch++)
	{
		if ((char*)(*pr)->addr+(*pr)->size + HeadSize == (*ch)->addr)
		{
				(*pr)->size += ((*ch)->size + HeadSize);
				FreeList.erase(ch);
				ch = pr;
				continue;
		}
		else 
		pr = ch;
	}
}

void MemManager_DL::Print(const char * str)
{
	std::cout << str << "\n";

	for (auto ch = FreeList.begin(); ch != FreeList.end(); ch++)
		std::cout	<< *ch << "    "
					<< (*ch)->addr << "    "
					<< (*ch)->size << "\n";

}