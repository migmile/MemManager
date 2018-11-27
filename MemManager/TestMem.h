void WriteMem(void * adr, unsigned len)
{
	int * ptr = (int *)adr;
	int count = len / sizeof(int);
	for (int i = 0; i < count; i++) ptr[i] = i;
}
void ReadMem(void * adr, unsigned len)
{
	int * ptr = (int *)adr;
	int count = len / sizeof(int);
	for (int i = 0; i < count; i++) if (ptr[i] != i) throw "wrong value!";
}