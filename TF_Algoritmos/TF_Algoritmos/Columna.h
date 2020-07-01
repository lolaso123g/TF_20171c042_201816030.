#pragma once
#include <vector>
#include <string>
using namespace std;

class Columna {

protected:
	//vector<T> elems;
	int tipo;
	string nombre;
public:

	Columna() {

	}
	~Columna() {

	}
	/*
	int getCantidad() {
		return elems.size();
	}
	void add(T elem) {
		elems.push_back(elem);
	}
	void erase(int pos) {
		elems.erase(elems.begin() + pos);
	}
	T getElem(int pos) {
		return elems[pos];
	}*/
	int getTipo() { return tipo; }
	string getNombre() { return nombre; }
	void setNombre(string name) { nombre = name; }
};
