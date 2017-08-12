#pragma once
#include <vector>

class ShiftRegister2 
{
public:
	ShiftRegister2(void);
	~ShiftRegister2(void);
	ShiftRegister2(int size);
	int Size;
	void RemoveAll();
	void SetSize(int size);
	void Shift();
	int k;		// current zero pointer index
	std::vector<double> storage;
	double& operator [](int index);
};



