#include <iostream>
#include <list>


using std::cout;
using std::list;


class MemManagerL1
{
        struct MemChain
        {
                unsigned shift;         // смещение от начала 
                unsigned size;
                char free;      // занят - 0, свободен - 1
        };

        list <MemChain> List;
        char * Mem=NULL;
        unsigned totalSize=0;
public:
        MemManagerL1(unsigned _size) {
                Mem = (char*) malloc(_size); 
                if (Mem == NULL) throw "error allocating mem"; 
                totalSize = _size;
                MemChain ch;
                ch.shift = 0;
                ch.free = 1;
                ch.size = totalSize;
                List.push_front(ch);
        }

        ~MemManagerL1() {
                if (Mem!=NULL) free(Mem); 
        }

        void * alloc(unsigned size) {
//              if (есть память)
                {
                        auto ch=List.begin();
                        for (; ch != List.end();ch++) if (ch->free == 1 && ch->size>=size) break;
                        if (ch != List.end())
                        {                                               // выделим из данного блока нужный кусок и вставим в список ссылку на оставшуюся свободную часть 
                                MemChain ch1;           // остаток
                                ch1.shift = ch->shift+size;
                                ch1.free = 1;
                                ch1.size = ch->size - size;
                                ch->size = size;
                                ch->free = 0;
                                auto ptr = ch;
                                ptr++;
                                List.insert(ptr,ch1);

                                return (Mem + ch->shift);
                        }
                }
                return NULL;
        }
        void  free(void*ptr) {
                if (ptr<Mem && ptr>Mem + totalSize) throw "wrong adr";
                auto ch = List.begin();
                for (; ch != List.end(); ch++) if (Mem + ch->shift == ptr) {
                        ch->free = 1;
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
                        if (ch->free == 1)
                        {
                                c++;
                                if (c == 2)     // предыдущий был тоже свободный
                                {
                                        pr->size += ch->size;
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
                for (auto ch = List.begin(); ch != List.end(); ch++) cout << ch->shift << "    " << ch->size << "    " << ch->free << "\n";
        }

};
