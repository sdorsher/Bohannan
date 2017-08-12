
#include "ShiftRegister2.h"


ShiftRegister2::ShiftRegister2()
{
	k = 0;
	Size = 0;
}

ShiftRegister2::~ShiftRegister2()
{
	RemoveAll();
}

ShiftRegister2::ShiftRegister2(int size)
{
	k = 0;
	Size = size;
	storage.resize(Size);
	for(int j=0;j<Size;j++) 
		storage[j] = 0.0;
}

void ShiftRegister2::Shift()
{
	k = k--;
	if(k<0) k = Size-1;
}

double& ShiftRegister2::operator [] (int index)
{
	int j;
	j = (k+index)%Size;
	return storage[j];
}

void ShiftRegister2::RemoveAll()
{
	storage.clear();
}

void ShiftRegister2::SetSize(int size)
{
	storage.resize(size);
	Size = size;
	for(int j=0;j<Size;j++) storage[j] = 0.0;
}


