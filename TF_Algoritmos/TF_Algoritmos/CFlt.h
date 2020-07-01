#pragma once
#include "Columna.h"


class CFlt : public Columna
{
	vector<float> elems;
	
public:
	CFlt(string name) {
		tipo = 3;
		nombre = name;
	}
	CFlt() { tipo = 3; }
	~CFlt() {}
	int getCantidad() {
		return elems.size();
	}
	void add(float elem) {
		elems.push_back(elem);
	}
	void erase(int pos) {
		elems.erase(elems.begin() + pos);
	}
	float getElem(int pos) {
		return elems[pos];
	}
};

