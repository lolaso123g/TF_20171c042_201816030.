#pragma once
#include "Columna.h"


class CInt : public Columna
{
	vector<int> elems;

public:
	CInt(string name) {
		tipo = 1;
		nombre = name;
	}
	CInt() { tipo = 1; }
	~CInt() {}
	int getCantidad() {
		return elems.size();
	}
	void add(int elem) {
		elems.push_back(elem);
	}
	void erase(int pos) {
		elems.erase(elems.begin() + pos);
	}
	int getElem(int pos) {
		return elems[pos];
	}
};

