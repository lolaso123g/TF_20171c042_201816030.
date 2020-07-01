#pragma once
#include "Columna.h"
#include <string>

class CStr : public Columna
{
	vector <string> elems;

public:
	CStr(string name) {
		tipo = 2;
		nombre = name;
	}
	CStr() { tipo = 2; }
	~CStr() {}
	int getCantidad() {
		return elems.size();
	}
	void add(string elem) {
		elems.push_back(elem);
	}
	void erase(int pos) {
		elems.erase(elems.begin() + pos);
	}
	string getElem(int pos) {
		return elems[pos];
	}
};

