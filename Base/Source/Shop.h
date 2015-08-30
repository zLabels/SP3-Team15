#pragma once
#include <iostream>
#include <string>

using namespace std;

class CShop
{
private:
	int Cash;
public:
	CShop(void);
	CShop(int);
	~CShop(void);

	void SetCash(int);
	int GetCash(void);
};

