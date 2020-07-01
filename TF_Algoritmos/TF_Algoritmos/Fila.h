#pragma once
#include "Columna.h"
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class Fila {
	int numero;
public:
	Fila(int num){
		numero = num;
	}
	~Fila(){}
	int getNumeroFila() { return numero; }
	int getInt(vector<Columna*> columnas, int posC) {
		if (columnas[posC]->getTipo() == 1) { return ((CInt*)columnas[posC])->getElem(numero); }
		return -1; // si no encuentra
	}
	string getString(vector<Columna*> columnas, int posC) {
		if (columnas[posC]->getTipo() == 2) { return ((CStr*)columnas[posC])->getElem(numero); }
		return "no se encuentra";
	}
	float getFloat(vector<Columna*> columnas, int posC) {
		if (columnas[posC]->getTipo() == 3) { return ((CFlt*)columnas[posC])->getElem(numero); }
		return -1;
	}
	void printFila(vector<Columna*> columnas) {
			for (int i = 0; i < columnas.size(); i++) {
				if (columnas[i]->getTipo() == 1) { cout << setw(16) << ((CInt*)columnas[i])->getElem(numero) ; }
				else if (columnas[i]->getTipo() == 2) { cout << setw(16) << ((CStr*)columnas[i])->getElem(numero) ; }
				else if (columnas[i]->getTipo() == 3) { cout << setw(16) << ((CFlt*)columnas[i])->getElem(numero) ; }
			}
		cout << endl;
	}
	/*void expFila(vector<Columna*> columnas, fstream archivo) {
		for (int i = 0; i < columnas.size(); i++) {
			if (columnas[i]->getTipo() == 1) { archivo << ((CInt*)columnas[i])->getElem(numero); }
			else if (columnas[i]->getTipo() == 2) { archivo << ((CStr*)columnas[i])->getElem(numero); }
			else if (columnas[i]->getTipo() == 3) { archivo << ((CFlt*)columnas[i])->getElem(numero); }
		}
	}*/
};