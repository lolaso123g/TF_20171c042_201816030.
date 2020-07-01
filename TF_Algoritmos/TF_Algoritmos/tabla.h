#pragma once
#include <iostream>
#include "Columna.h"
#include <string>
#include <vector>
#include <iomanip>
#include "Columna.h"
#include "CInt.h"
#include "CStr.h"
#include "CFlt.h"
#include "Fila.h"
#include "arbIndex.h"
#include <algorithm>
#include <sstream>
#include <fstream>
using namespace std;
using namespace System;

class Table {
	vector<Columna*> columnas;
	vector<Fila*> filas;
	vector<AVLBST<Fila*, int, nullptr>*> arbolesInt;
	vector<AVLBST<Fila*, string, nullptr>*> arbolesStr;
	vector<AVLBST<Fila*, float, nullptr>*> arbolesFlt;
	vector<Fila*> filtro1, filtro2;
	int cantColumnas;
	int cantFilas;
	function<void(Fila*)> print;

public:
	Table() {
		cantColumnas = 0;
		cantFilas = 0;
		print = [&](Fila* a) {a->printFila(columnas); };
	}
	Table(vector<Columna*> columnas, vector<Fila*> filas) {
		this->columnas = columnas;
		this->filas = filas;
		cantColumnas = columnas.size();
		cantFilas = filas.size();
		print = [&](Fila* a) {a->printFila(columnas); };
	}
	~Table() {}
	int getNumColumnas() {
		return columnas.size();
	}
	void setColumnas() {
		string nombreCol;
		int tipo;
		do {
			cout << "Ingrese la cantidad de columnas: ";
			cin >> cantColumnas;
			cin.ignore();
		} while (cantColumnas < 0);
		for (int i = 0; i < cantColumnas; i++) {
			cout << "Ingrese el nombre de la columna " << i + 1 << ": ";
			//cin >> nombreCol;
			getline(cin, nombreCol);
			do {
				cout << "Ingrese el tipo de la columna " << i + 1 << " (1:int, 2:string, 3:float): ";
				cin >> tipo;
				cin.ignore();
			} while (tipo > 3 && tipo < 1);
			if (tipo == 1) { columnas.push_back(new CInt(nombreCol)); }
			else if (tipo == 2) { columnas.push_back(new CStr(nombreCol)); }
			else { columnas.push_back(new CFlt(nombreCol)); }
		}
	}
	void addFila() {
		string elem, tipoDD;
		for (int i = 0; i < columnas.size(); i++) {
			if (columnas[i]->getTipo() == 1) { tipoDD = "int"; }
			else if (columnas[i]->getTipo() == 2) { tipoDD = "string"; }
			else { tipoDD = "float"; }
			cout << "Ingrese el valor (columna " << i + 1 << "| " << columnas[i]->getNombre() << "| " << tipoDD <<"): ";
			getline(cin,elem);
			if (columnas[i]->getTipo() == 1) { ((CInt*)columnas[i])->add(stoi(elem)); }
			else if (columnas[i]->getTipo() == 2) { transform(elem.begin(), elem.end(), elem.begin(), ::toupper); ((CStr*)columnas[i])->add(elem); }
			else if (columnas[i]->getTipo() == 3) { ((CFlt*)columnas[i])->add(stof(elem)); }
		}
		filas.push_back(new Fila(cantFilas));
		cantFilas++;
	}
	void mostrar() {
		for (int i = 0; i < columnas.size(); i++) {
			cout << setw(16) << columnas[i]->getNombre();
		}
		cout << endl;
		print = [&](Fila* a) {a->printFila(columnas); };
		for (auto f : filas) {
			print(f);
			cout << endl;
		}
	}
	void indexar() {
		int numColumna = 0;
		do {
			cout << "Ingrese en numero de la Columna a indexar: ";
			cin >> numColumna;
		} while (cantColumnas < 0 && cantColumnas > columnas.size());
		auto k1 = [&](Fila* a, int numColumna) { return a->getInt(columnas, numColumna - 1); };
		auto k2 = [&](Fila* a, int numColumna) { return a->getString(columnas, numColumna - 1); };
		auto k3 = [&](Fila* a, int numColumna) { return a->getFloat(columnas, numColumna - 1); };
		auto toString1 = [&](Fila* a, int numColumna) { return to_string(a->getInt(columnas, numColumna - 1)); };
		auto toString2 = [&](Fila* a, int numColumna) { return a->getString(columnas, numColumna - 1); };
		auto toString3 = [&](Fila* a, int numColumna) { 
			stringstream ss;
			ss << a->getFloat(columnas, numColumna - 1);
			return ss.str();
		};
		if (columnas[numColumna-1]->getTipo() == 1) { 
			AVLBST<Fila*, int, nullptr>* arbol = new AVLBST<Fila*, int, nullptr>(k1, toString1); 
			arbol->setNColumna(numColumna);
			for (auto fila : filas) {
				arbol->add(fila);
			}
			arbolesInt.push_back(arbol);
		}
		else if (columnas[numColumna - 1]->getTipo() == 2) { 
			AVLBST<Fila*, string, nullptr>* arbol = new AVLBST<Fila*, string, nullptr>(k2, toString2);
			arbol->setNColumna(numColumna);
			for (auto fila : filas) {
				arbol->add(fila);
			}
			arbolesStr.push_back(arbol);
		}
		else if (columnas[numColumna - 1]->getTipo() == 3) { 
			AVLBST<Fila*, float, nullptr>* arbol = new AVLBST<Fila*, float, nullptr>(k3, toString3);
			arbol->setNColumna(numColumna);
			for (auto fila : filas) {
				arbol->add(fila);
			}
			arbolesFlt.push_back(arbol);
		}
	}
	vector<Columna*> getColumnas(vector<int> numC) {
		vector<Columna*> seleccion;
		for (int i = 0; i < numC.size(); i++) {
			seleccion.push_back(columnas[numC[i] - 1]);
		}
		return seleccion;
	}
	vector<Columna*> getColumnas() {
		return columnas;
	}
	vector<Fila*> ordenar() {
		vector<Fila*> enOrden;
		int numColumna = 0;
		do {
			cout << "Ingrese en numero de la Columna a ordenar: ";
			cin >> numColumna;
		} while (cantColumnas < 0 && cantColumnas > columnas.size());
		for (int i = 0; i < arbolesInt.size(); i++) {
			if (arbolesInt[i]->getNColumna() == numColumna) {
				arbolesInt[i]->inOrder(enOrden);
				system("pause");
				break;
			}
		}
		for (int i = 0; i < arbolesStr.size(); i++) {
			if (arbolesStr[i]->getNColumna() == numColumna) {
				arbolesStr[i]->inOrder(enOrden);
				system("pause");
				break;
			}
		}
		for (int i = 0; i < arbolesFlt.size(); i++) {
			if (arbolesFlt[i]->getNColumna() == numColumna) {
				arbolesFlt[i]->inOrder(enOrden);
				system("pause");
				break;
			}
		}
		return enOrden;
	}
	vector<Fila*> getFilas() {
		return filas;
	}
	/*void ordenar() {
		int numColumna = 0;
		auto print = [&](Fila* a) {a->printFila(columnas); };
		do {
			cout << "Ingrese en numero de la Columna a ordenar: ";
			cin >> numColumna;
		} while (cantColumnas < 0 && cantColumnas > columnas.size());
		for (int i = 0; i < arbolesInt.size(); i++) {
			if (arbolesInt[i]->getNColumna() == numColumna) {
				arbolesInt[i]->inOrder(print);
				system("pause");
				break;
			}
		}
		for (int i = 0; i < arbolesStr.size(); i++) {
			if (arbolesStr[i]->getNColumna() == numColumna) {
				arbolesStr[i]->inOrder(print);
				system("pause");
				break;
			}
		}
		for (int i = 0; i < arbolesFlt.size(); i++) {
			if (arbolesFlt[i]->getNColumna() == numColumna) {
				arbolesFlt[i]->inOrder(print);
				system("pause");
				break;
			}
		}
	}*/
	void slowFiltr(int numColumna, int tipoF, string valor) {
		if (tipoF == 1) {
			if (filtro1.empty()) {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 1) {
						if (filas[ix]->getInt(columnas, numColumna - 1) > stoi(valor)) { filtro1.push_back(filas[ix]); }
					}
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if (filas[ix]->getString(columnas, numColumna - 1) > valor) { filtro1.push_back(filas[ix]); }
					}
					if (columnas[numColumna - 1]->getTipo() == 3) {
						if (filas[ix]->getFloat(columnas, numColumna - 1) > stof(valor)) { filtro1.push_back(filas[ix]); }
					}
				}
			}
			else {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 1) {
						if (filas[ix]->getInt(columnas, numColumna - 1) > stoi(valor)) { filtro2.push_back(filas[ix]); }
					}
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if (filas[ix]->getString(columnas, numColumna - 1) > valor) { filtro2.push_back(filas[ix]); }
					}
					if (columnas[numColumna - 1]->getTipo() == 3) {
						if (filas[ix]->getFloat(columnas, numColumna - 1) > stof(valor)) { filtro2.push_back(filas[ix]); }
					}
				}
			}
		}
		else if (tipoF == 2) {
			if (filtro1.empty()) {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 1) {
						if (filas[ix]->getInt(columnas, numColumna - 1) < stoi(valor)) { filtro1.push_back(filas[ix]); }
					}
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if (filas[ix]->getString(columnas, numColumna - 1) < valor) { filtro1.push_back(filas[ix]); }
					}
					if (columnas[numColumna - 1]->getTipo() == 3) {
						if (filas[ix]->getFloat(columnas, numColumna - 1) < stof(valor)) { filtro1.push_back(filas[ix]); }
					}
				}
			}
			else {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 1) {
						if (filas[ix]->getInt(columnas, numColumna - 1) < stoi(valor)) { filtro2.push_back(filas[ix]); }
					}
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if (filas[ix]->getString(columnas, numColumna - 1) < valor) { filtro2.push_back(filas[ix]); }
					}
					if (columnas[numColumna - 1]->getTipo() == 3) {
						if (filas[ix]->getFloat(columnas, numColumna - 1) < stof(valor)) { filtro2.push_back(filas[ix]); }
					}
				}
			}
		}
		else if (tipoF == 3) {
			if (filtro1.empty()) {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 1) {
						if (filas[ix]->getInt(columnas, numColumna - 1) == stoi(valor)) { filtro1.push_back(filas[ix]); }
					}
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if (filas[ix]->getString(columnas, numColumna - 1) == valor) { filtro1.push_back(filas[ix]); }
					}
					if (columnas[numColumna - 1]->getTipo() == 3) {
						if (filas[ix]->getFloat(columnas, numColumna - 1) == stof(valor)) { filtro1.push_back(filas[ix]); }
					}
				}
			}
			else {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 1) {
						if (filas[ix]->getInt(columnas, numColumna - 1) == stoi(valor)) { filtro2.push_back(filas[ix]); }
					}
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if (filas[ix]->getString(columnas, numColumna - 1) == valor) { filtro2.push_back(filas[ix]); }
					}
					if (columnas[numColumna - 1]->getTipo() == 3) {
						if (filas[ix]->getFloat(columnas, numColumna - 1) == stof(valor)) { filtro2.push_back(filas[ix]); }
					}
				}
			}
		}
		else if (tipoF == 4) {
			if (filtro1.empty()) {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if (filas[ix]->getString(columnas, numColumna - 1)[0] == valor[0]) { filtro1.push_back(filas[ix]); }
					}
				}
			}
			else {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if (filas[ix]->getString(columnas, numColumna - 1)[0] == valor[0]) { filtro2.push_back(filas[ix]); }
					}
				}
			}
		}
		else if (tipoF == 5) {
			if (filtro1.empty()) {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if (filas[ix]->getString(columnas, numColumna - 1).back() == valor.back()) { filtro1.push_back(filas[ix]); }
					}
				}
			}
			else {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if (filas[ix]->getString(columnas, numColumna - 1).back() == valor.back()) { filtro2.push_back(filas[ix]); }
					}
				}
			}
		}
		else if (tipoF == 6) {
			if (filtro1.empty()) {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if ((filas[ix]->getString(columnas, numColumna - 1)).find(valor) != -1) { filtro1.push_back(filas[ix]); }
					}
				}
			}
			else {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if ((filas[ix]->getString(columnas, numColumna - 1)).find(valor) != -1) { filtro2.push_back(filas[ix]); }
					}
				}
			}
		}
		else if (tipoF == 7) {
			if (filtro1.empty()) {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if ((filas[ix]->getString(columnas, numColumna - 1)).find(valor) == -1) { filtro1.push_back(filas[ix]); }
					}
				}
			}
			else {
				for (int ix = 0; ix < cantFilas; ix++) {
					if (columnas[numColumna - 1]->getTipo() == 2) {
						transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
						if ((filas[ix]->getString(columnas, numColumna - 1)).find(valor) == -1) { filtro2.push_back(filas[ix]); }
					}
				}
			}
		}
	}
	void filtrar() {
		bool arbol = false;
		int numColumna = 0;
		string valor;
		char opcionFiltro;
		do {
			cout << "Ingrese el numero de la columna a filtrar: ";
			cin >> numColumna;
		} while (cantColumnas < 0 && cantColumnas > columnas.size());
		cout << endl << "1. Mayor a:" << endl << "2. Menor a:" << endl << "3. Igual a:" << endl
			 << "4. Empieza con:" << endl << "5. Termina con:" << endl << "6. Contenido en:" << endl 
			 << "7. No esta contenido en:" << endl;
		cout << "Elegir Filtro (0 para borrar filtros previos): ";
		cin >> opcionFiltro;

		switch (opcionFiltro) {
		case '0':
			filtro1.clear();
			filtro2.clear();
			break;
		case '1':
			cout << "Mayor que: ";
			cin >> valor;
			for (int i = 0; i < arbolesInt.size(); i++) {
				if (arbolesInt[i]->getNColumna() == numColumna) {
					arbol = true;
					if (filtro1.empty()) { arbolesInt[i]->greaterThan(filtro1, stoi(valor)); }
					else { arbolesInt[i]->greaterThan(filtro2, stoi(valor)); }
					break;
				}
			}
			for (int i = 0; i < arbolesStr.size(); i++) {
				if (arbolesStr[i]->getNColumna() == numColumna) {
					arbol = true;
					transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
					if (filtro1.empty()) { arbolesStr[i]->greaterThan(filtro1, valor); }
					else { arbolesStr[i]->greaterThan(filtro2, valor); }
					break;
				}
			}
			for (int i = 0; i < arbolesFlt.size(); i++) {
				if (arbolesFlt[i]->getNColumna() == numColumna) {
					arbol = true;
					if (filtro1.empty()) { arbolesFlt[i]->greaterThan(filtro1, stof(valor)); }
					else { arbolesFlt[i]->greaterThan(filtro2, stof(valor)); }
					break;
				}
			}
			if (!arbol) { slowFiltr(numColumna, 1, valor); }
			break;
		case '2':
			cout << "Menor que: ";
			cin >> valor;
			for (int i = 0; i < arbolesInt.size(); i++) {
				if (arbolesInt[i]->getNColumna() == numColumna) {
					arbol = true;
					if (filtro1.empty()) { arbolesInt[i]->lowerThan(filtro1, stoi(valor)); }
					else { arbolesInt[i]->lowerThan(filtro2, stoi(valor)); }
					break;
				}
			}
			for (int i = 0; i < arbolesStr.size(); i++) {
				if (arbolesStr[i]->getNColumna() == numColumna) {
					arbol = true;
					transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
					if (filtro1.empty()) { arbolesStr[i]->lowerThan(filtro1, valor); }
					else { arbolesStr[i]->lowerThan(filtro2, valor); }
					break;
				}
			}
			for (int i = 0; i < arbolesFlt.size(); i++) {
				if (arbolesFlt[i]->getNColumna() == numColumna) {
					arbol = true;
					if (filtro1.empty()) { arbolesFlt[i]->lowerThan(filtro1, stof(valor)); }
					else { arbolesFlt[i]->lowerThan(filtro2, stof( valor)); }
					break;
				}
			}
			if (!arbol) { slowFiltr(numColumna, 2, valor); }
			break;
		case '3':
			cout << "Igual que: ";
			cin >> valor;
			for (int i = 0; i < arbolesInt.size(); i++) {
				if (arbolesInt[i]->getNColumna() == numColumna) {
					arbol = true;
					if (filtro1.empty()) { arbolesInt[i]->equalTo(filtro1, stoi(valor)); }
					else { arbolesInt[i]->equalTo(filtro2, stoi(valor)); }
					break;
				}
			}
			for (int i = 0; i < arbolesStr.size(); i++) {
				if (arbolesStr[i]->getNColumna() == numColumna) {
					arbol = true;
					transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
					if (filtro1.empty()) { arbolesStr[i]->equalTo(filtro1, valor); }
					else { arbolesStr[i]->equalTo(filtro2, valor); }
					break;
				}
			}
			for (int i = 0; i < arbolesFlt.size(); i++) {
				if (arbolesFlt[i]->getNColumna() == numColumna) {
					arbol = true;
					if (filtro1.empty()) { arbolesFlt[i]->equalTo(filtro1, stof(valor)); }
					else { arbolesFlt[i]->equalTo(filtro2, stof(valor)); }
					break;
				}
			}
			if (!arbol) { slowFiltr(numColumna, 3, valor); }
			break;
		case '4':
			cout << "Empieza con: ";
			cin >> valor;
			for (int i = 0; i < arbolesInt.size(); i++) {
				if (arbolesInt[i]->getNColumna() == numColumna) {
					arbol = true;
					if (filtro1.empty()) { arbolesInt[i]->startsWith(filtro1, valor); }
					else { arbolesInt[i]->startsWith(filtro2, valor); }
					break;
				}
			}
			for (int i = 0; i < arbolesStr.size(); i++) {
				if (arbolesStr[i]->getNColumna() == numColumna) {
					arbol = true;
					transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
					if (filtro1.empty()) { arbolesStr[i]->startsWith(filtro1, valor); }
					else { arbolesStr[i]->startsWith(filtro2, valor); }
					break;
				}
			}
			for (int i = 0; i < arbolesFlt.size(); i++) {
				if (arbolesFlt[i]->getNColumna() == numColumna) {
					arbol = true;
					if (filtro1.empty()) { arbolesFlt[i]->startsWith(filtro1, valor); }
					else { arbolesFlt[i]->startsWith(filtro2, valor); }
					break;
				}
			}
			if (!arbol) { slowFiltr(numColumna, 4, valor); }
			break;
		case '5':
			cout << "Termina con: ";
			cin >> valor;
			for (int i = 0; i < arbolesInt.size(); i++) {
				if (arbolesInt[i]->getNColumna() == numColumna) {
					arbol = true;
					if (filtro1.empty()) { arbolesInt[i]->endsIn(filtro1, valor); }
					else { arbolesInt[i]->endsIn(filtro2, valor); }
					break;
				}
			}
			for (int i = 0; i < arbolesStr.size(); i++) {
				if (arbolesStr[i]->getNColumna() == numColumna) {
					arbol = true;
					transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
					if (filtro1.empty()) { arbolesStr[i]->endsIn(filtro1, valor); }
					else { arbolesStr[i]->endsIn(filtro2, valor); }
					break;
				}
			}
			for (int i = 0; i < arbolesFlt.size(); i++) {
				if (arbolesFlt[i]->getNColumna() == numColumna) {
					arbol = true;
					if (filtro1.empty()) { arbolesFlt[i]->endsIn(filtro1, valor); }
					else { arbolesFlt[i]->endsIn(filtro2, valor); }
					break;
				}
			}
			if (!arbol) { slowFiltr(numColumna, 5, valor); }
			break;
		case '6':
			cout << "Incluido en: ";
			cout << endl << "Ingrese: ";
			cin >> valor;
			for (int i = 0; i < arbolesStr.size(); i++) {
				if (arbolesStr[i]->getNColumna() == numColumna) {
					arbol = true;
					transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
					if (filtro1.empty()) { arbolesStr[i]->includedIn(filtro1, valor); }
					else { arbolesStr[i]->includedIn(filtro2, valor); }
					break;
				}
			}
			if (!arbol) { slowFiltr(numColumna, 6, valor); }
			break;
		case '7':
			cout << "No incluido en: ";
			cout << endl << "Ingrese: ";
			cin >> valor;
			for (int i = 0; i < arbolesStr.size(); i++) {
				if (arbolesStr[i]->getNColumna() == numColumna) {
					arbol = true;
					transform(valor.begin(), valor.end(), valor.begin(), ::toupper);
					if (filtro1.empty()) { arbolesStr[i]->notIncludedIn(filtro1, valor); }
					else { arbolesStr[i]->notIncludedIn(filtro2, valor); }
					break;
				}
			}
			if (!arbol) { slowFiltr(numColumna, 7, valor); }
			break;
		default:
			cout << "Opcion no válida";
			break;
		}
	}
	void mostrarFiltro() {
		print = [&](Fila* a) {a->printFila(columnas); };
		if (!(filtro1.empty()) && !(filtro2.empty())) {
			for (int i = 0; i < filtro1.size(); i++) {
				for (int j = 0; j < filtro2.size(); j++) {
					if (filtro1[i] == filtro2[j]) {
						print(filtro1[i]);
						break;
					}
				}
			}
		}
		else if(!(filtro1.empty())){
			for (auto f : filtro1) {
				print(f);
			}
		}
		else if(!(filtro2.empty())) {
			for (auto f : filtro2) {
				print(f);
			}
		}
	}
	void exportarArch() {
		//fstream archivo;
		string fileName, tipoDD;
		cout << "Ingrese el nombre del archivo: ";
		cin >> fileName;
		ofstream archivo(fileName + ".txt", ios::trunc);
		//archivo.open(fileName + ".txt", ios::out || ios::trunc);
		
		if (archivo.fail()) {
			cout << "no se pudo abrir" << endl << endl << endl;
			system("pause");
		}
		else {
			for (int i = 0; i < columnas.size(); i++) {
				if (columnas[i]->getTipo() == 1) { tipoDD = "int"; }
				else if (columnas[i]->getTipo() == 2) { tipoDD = "string"; }
				else { tipoDD = "float"; }
				archivo << tipoDD << " ";
			} archivo << "." << endl;
			for (int i = 0; i < columnas.size(); i++) {
				archivo << columnas[i]->getNombre() << " ";
			} archivo << "." << endl;
			for (auto f : filas) {
				for (int i = 0; i < columnas.size(); i++) {
					if (columnas[i]->getTipo() == 1) { archivo << ((CInt*)columnas[i])->getElem(f->getNumeroFila()) << " "; }
					else if (columnas[i]->getTipo() == 2) { archivo << ((CStr*)columnas[i])->getElem(f->getNumeroFila()) << " "; }
					else if (columnas[i]->getTipo() == 3) { archivo << ((CFlt*)columnas[i])->getElem(f->getNumeroFila()) << " "; }
				} archivo << endl;
			}
		}
		archivo.close();
	}
	// funciones que facilitan lectura de archivo y creacion de un objeto tabla en base al mismo
	///////////////////////////////////////////////////////////////
	void addColumna(int tipo) {
		if (tipo == 1) { columnas.push_back(new CInt()); }
		else if (tipo == 2) { columnas.push_back(new CStr()); }
		else { columnas.push_back(new CFlt()); }
		cantColumnas++;
	}
	void nombrarColumna(int numeroColumna, string name) {
		columnas[numeroColumna]->setNombre(name);
	}
	void addFila(int idx, string elem) {
		if (columnas[idx]->getTipo() == 1) { ((CInt*)columnas[idx])->add(stoi(elem)); }
		else if (columnas[idx]->getTipo() == 2) { transform(elem.begin(), elem.end(), elem.begin(), ::toupper); ((CStr*)columnas[idx])->add(elem); }
		else if (columnas[idx]->getTipo() == 3) { ((CFlt*)columnas[idx])->add(stof(elem)); }
	}
	void masFila() {
		filas.push_back(new Fila(cantFilas));
		cantFilas++;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
};
