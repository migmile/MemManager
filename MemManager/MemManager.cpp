// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// MemManager.cpp 


#include <iostream>
#include <list>
#include <deque>
#include <vector>
#include <map>
#include <malloc.h>
#include <stdlib.h>
#include <random>
#include <chrono>
#include "MManagerL.h"
#include "MManagerL1.h"
#include "MManagerD.h"
#include "MemManager_DL.h"
#include "TestMem.h"

using std::cout;
using std::list;

const unsigned Len = 16;
const unsigned Count = 3;



int TestMM_DL()
{
	MemManager_DL mm(Len * sizeof(int) + Count * 16);

	std::vector<int *> pointers;
	pointers.reserve(Count);
	mm.Print("begin");
	std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now(), p2, p3;

	for (int i = 0; i < Count; i++)
	{
		int * ptr = (int*)mm.alloc(Len / Count * sizeof(int));
		pointers.push_back(ptr);
	}
	mm.Print("allocated");
	p2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p2 - p1).count() << "\n";

	for (int i = 0; i < Count; i++)
	{
		mm.free(pointers[i]);
	}
	mm.Print("end");
	p3 = std::chrono::high_resolution_clock::now();
	mm.GarbCol();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p3 - p2).count() << "\n";
	return 1;
}

int TestMM_DL_rnd()
{
	MemManager_DL mm(1024*10);

	std::map<int *,unsigned> pointers;
//	pointers.reserve(100);
	mm.Print("begin");
	std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now(), p2, p3;
	unsigned counter = 0;
	do
	{
		unsigned length;
		std::random_device rd;   // non-deterministic generator
		std::mt19937 gen(rd());  // to seed mersenne twister.
		std::uniform_int_distribution<> dist(1, 1000); // distribute results between 1 and 6 inclusive.
		length = dist(gen);

		int * ptr = (int*)mm.alloc(length);
		if (ptr == NULL) break;
		counter++;
		WriteMem(ptr, length);
		pointers.insert(std::make_pair(ptr,length));
	} while (true);
	std::cout << "allocated " << counter << " blockes\n\n";
	mm.Print("allocated");
	p2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p2 - p1).count() << "\n";

	for (auto p: pointers)
	{
		ReadMem(p.first,p.second);
		mm.free(p.first);
	}
	mm.Print("end");
	p3 = std::chrono::high_resolution_clock::now();
	mm.GarbCol();
	mm.Print("after GarbCOl");
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p3 - p2).count() << "\n";
	return 1;
}

/*
int Test_newPlacement()
{
	std::vector<int *> pointers;
	pointers.reserve(Count);
	//	mm.Print("begin");
	std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now(), p2, p3;
	//int * Mem = new int[Len * 10];

	for (int i = 0; i < Count; i++)
	{
		int * ptr = new int[Len / Count];//(int*)malloc(Len / Count * sizeof(int));
		pointers.push_back(ptr);
	}
	p2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p2 - p1).count() << "\n";
	return 1;
	for (int i = 0; i < Count; i++)
	{
		//free(pointers[i]);
		//delete[] pointers[i];
	}
	p3 = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p3 - p2).count() << "\n";

	//delete[] Mem;
	return 1;
}


int TestMMList1()
{
	MemManagerL1 mm(Len * sizeof(int));

	//std::vector<int *> pointers;
	//pointers.reserve(Count);
//	mm.Print("begin");
	std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now(), p2, p3;

	for (int i = 0; i < Count; i++)
	{
		int * ptr = (int*)mm.alloc(Len / Count * sizeof(int));
	//	pointers.push_back(ptr);
	}
	p2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p2 - p1).count() << "\n";
	return 1;
	for (int i = 0; i < Count; i++)
	{
		//mm.free(pointers[i]);
	}
	p3 = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p3 - p2).count() << "\n";
	return 1;
}

int TestMMList()
{
	MemManagerL mm(Len * sizeof(int));

	//	std::vector<int *> pointers;
	//	pointers.reserve(Count);
//	mm.Print("begin");
	std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now(), p2, p3;

	for (int i = 0; i < Count; i++)
	{
		int * ptr = (int*)mm.alloc(Len / Count * sizeof(int));
		//pointers.push_back(ptr);
	}
	p2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p2 - p1).count() << "\n";
	return 1;
	for (int i = 0; i < Count; i++)
	{
	//	mm.free(pointers[i]);
	}
	p3 = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p3 - p2).count() << "\n";
	return 1;

	int * ptr1 = (int*)mm.alloc(5 * sizeof(int));
	int * ptr2 = (int*)mm.alloc(10 * sizeof(int));
	ptr1[0] = 1;
	ptr1[5] = 5;
	mm.Print("first");
	//	mm.free(ptr);
	mm.free(ptr1);
	mm.free(ptr2);
	mm.Print("first");
	return 1;

	//	int * ptr1 = (int*)mm.alloc(5 * sizeof(int));
	//	int * ptr2=(int*)mm.alloc(10 * sizeof(int));
	mm.Print("first");
	//	mm.free(ptr);
	mm.free(ptr1);
	mm.Print("free");
	//	ptr=(int*)mm.alloc(5 * sizeof(int));
	mm.GarbCol();
	mm.Print("second");
}

int TestMMDeque()
{
	MemManagerD mm(Len * sizeof(int));

	//std::vector<int *> pointers;
	//pointers.reserve(Count);
//	mm.Print("begin");
	std::chrono::time_point<std::chrono::high_resolution_clock> p1 = std::chrono::high_resolution_clock::now(), p2, p3;

	for (int i = 0; i < Count; i++)
	{
		int * ptr = (int*)mm.alloc(Len / Count * sizeof(int));
	//	pointers.push_back(ptr);
	}
	p2 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p2 - p1).count() << "\n";
	return 1;
	for (int i = 0; i < Count; i++)
	{
		//mm.free(pointers[i]);
	}
	p3 = std::chrono::high_resolution_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1> >>(p3 - p2).count() << "\n";
	return 1;

	int * ptr1 = (int*)mm.alloc(5 * sizeof(int));
	int * ptr2 = (int*)mm.alloc(10 * sizeof(int));
	ptr1[0] = 1;
	ptr1[5] = 5;
	mm.Print("first");
	//	mm.free(ptr);
	mm.free(ptr1);
	mm.free(ptr2);
	mm.Print("first");
	return 1;

	//	int * ptr1 = (int*)mm.alloc(5 * sizeof(int));
	//	int * ptr2=(int*)mm.alloc(10 * sizeof(int));
	mm.Print("first");
	//	mm.free(ptr);
	mm.free(ptr1);
	mm.Print("free");
	//	ptr=(int*)mm.alloc(5 * sizeof(int));
	mm.GarbCol();
	mm.Print("second");
}

void TestMem(void * adr, unsigned len)
{
	int * ptr = (int *)adr;
	int count = len / sizeof(int);
	for (int i = 0; i < count; i++) ptr[i] = i;
	
}
*/
int main()
{
	std::cout << "My List Manager\n";
//	TestMM_DL();
	TestMM_DL_rnd();
	
	/*
	std::cout << "New Manager\n";
	Test_newPlacement();

	std::cout << "LIST1\n";
	TestMMList1();

	std::cout << "LIST\n";
	TestMMList();

	std::cout << "DEQUE\n";
	TestMMDeque();
	*/
	return 0;
}