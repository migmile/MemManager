#include <deque>
using std::deque;

class MemManagerD
{
	struct MemChain
	{
		unsigned shift;		// смещение от начала 
		unsigned size;
		char free;      // занят - 0, свободен - 1
	};
	unsigned HeadSize;
	deque< MemChain *> List;
	char * Mem=NULL;
	unsigned totalSize=0;
public:
	MemManagerD(unsigned _size) {
		HeadSize = sizeof MemChain;
		Mem = (char*) malloc(_size); 
		if (Mem == NULL) throw "error allocating mem"; 
		totalSize = _size;
		MemChain * ch=(MemChain *)Mem;
		(*ch).shift = 0;
		ch->free = 1;
		ch->size = totalSize - HeadSize;
		List.push_front(ch);
	}

	~MemManagerD() {
		if (Mem!=NULL) free(Mem); 
	}

	void * alloc(unsigned size) {
		unsigned req_size = size + HeadSize;
//		if (есть память)
		{
			auto ch=List.begin();
			for (; ch != List.end();ch++) if ((*ch)->free == 1 && (*ch)->size >= req_size ) break;
			if (ch != List.end())
			{	// выделим из данного блока нужный кусок и вставим в список ссылку на оставшуюся свободную часть 
				MemChain * ch1;		// describe остаток
				unsigned shift = (*ch)->shift + req_size; // сдвиг нового блока 
				ch1 = (MemChain * )(Mem + shift);
				ch1->shift = shift;
				ch1->free = 1;
				ch1->size = (*ch)->size - req_size;
				(*ch)->size = size;
				(*ch)->free = 0;
				void * m_ptr = Mem + (*ch)->shift + HeadSize;

				auto ptr = ch;
				ptr++;

				List.insert(ptr,ch1);

				return m_ptr;
			}
		}
		return NULL;
	}
	void  free(void * ptr) {
		if (ptr<Mem && ptr>Mem + totalSize) throw "wrong adr";
		auto ch = List.begin();
		for (; ch != List.end(); ch++) if ((char *)(*ch) + HeadSize == ptr) {
			(*ch)->free = 1;
			break;
		}
	}
	unsigned maxSize() { return 0; };

	void GarbCol() 
	{
		// сборщик мусора
		auto ch = List.begin(),pr=ch;
		for (int c = 0; ch != List.end(); ch++)
		{
			if ((*ch)->free == 1)
			{
				c++;
				if (c == 2)	// предыдущий был тоже свободный
				{
					(*pr)->size += (*ch)->size;
					List.erase(ch);
					c = 1;
					ch = pr;
					continue;
				}
			}
			else c = 0;
			pr = ch;
		}
	};

	void Print(const char * str) {
		cout << str << "\n";
		for (auto ch = List.begin(); ch != List.end(); ch++) 
			cout << (*ch)->shift << "    " 
			     << (*ch)->size << "    " 
			     << (*ch)->free << "\n";
	}

};
