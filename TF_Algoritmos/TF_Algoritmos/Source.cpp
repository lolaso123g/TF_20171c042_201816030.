#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include "Columna.h"
#include "tabla.h"

using namespace std;
using namespace System;

Table* importarArch() {
	string fileName, tipoDD;
	cout << "Ingrese el nombre del archivo: ";
	cin >> fileName;
	ifstream archivo(fileName + ".txt");
	Table* newTable = new Table();

	if (archivo.fail()) {
		cout << "no se pudo abrir" << endl << endl << endl;
		system("pause");
	}
	else {
		string s = "";
		while (s != ".") {
			archivo >> s;
			if (s == "int") { newTable->addColumna(1); }
			else if (s == "string") { newTable->addColumna(2); }
			else if (s == "float") { newTable->addColumna(3); }
		} 
		s = "";
		int idxColumna = 0;
		while (s != ".") {
			archivo >> s;
			if (s != ".") { 
				newTable->nombrarColumna(idxColumna, s);
				idxColumna++;
			}
		}
		s = "";
		/*while (archivo >> s) {
			for (int i = 0; i < newTable->getNumColumnas(); i++) {
				newTable->addFila(i, s);
			}
		}*/
		int contadorF, contadorC;
		contadorC = contadorF = 0;
		while (archivo >> s) {
			//cout << s << endl;
			newTable->addFila(contadorC, s);
			contadorC++;
			if (contadorC == newTable->getNumColumnas()) { contadorC = 0; newTable->masFila(); }
		}
		//system("pause");
	}
	archivo.close();
	return newTable;
}

int main() {
	bool bandera = false;
	bool t = false;
	bool repetido = false;
	char tecla;
	vector<Columna*> columnas;
	vector<Table*> tablas;
	int nCol, idxT, tempidx;
	vector<int> numC;
	string elem, nombreCol, tipoDD;
	Table* tabla = new Table();
	tablas.push_back(tabla);
	idxT = 0;
	do
	{
		system("cls");
		cin.clear();
		cout << "Menu" << endl;
		cout << "-----------" << endl << endl;
		cout << "\t1 .- Definir columnas." << endl;
		cout << "\t2 .- Agregar Fila." << endl;
		cout << "\t3 .- Indexar Columna #." << endl;
		cout << "\t4 .- Seleccion de datos por columnas." << endl;
		cout << "\t5 .- Ordenamiento de datos por columnas." << endl;
		cout << "\t6 .- Filtrar." << endl;
		cout << "\t7 .- Exportar datos a archivo." << endl;
		cout << "\t8 .- Leer tabla." << endl;
		cout << "\t9 .- Elegir Tabla. " << endl;
		cout << "\t10 .- Salir" << endl << endl;
		
		tablas[idxT]->mostrar();
		cout << "----------------------------------------------------------" << endl;
		tablas[idxT]->mostrarFiltro();

		cout << "Elije una opcion: ";
		cin >> tecla;
		cin.ignore();

		switch (tecla)
		{
		case '1':
			system("cls");
			if (!(t)) {
				tabla->setColumnas();
				t = true;
			}
			else {
				cout << "Ya se ingresaron las columnas" << endl;
				system("pause");
			}
			break;

		case '2':
			system("cls");
			
			tablas[idxT]->addFila();
			break;

		case '3':
			system("cls");
			
			tablas[idxT]->indexar();
			break;
		case '4':
			system("cls");
			do {
				repetido = false;
				cout << "Ingrese columna a seleccionar (0 para terminar): ";
				cin >> nCol;
				if (nCol > 0 && nCol <= tablas[idxT]->getNumColumnas()) { 
					for (int i = 0; i < numC.size(); i++) {
						if (nCol == numC[i]) {
							repetido = true;
						}
					}
					if (!(repetido)) { numC.push_back(nCol); }
				}
			} while (nCol != 0);
			tablas.push_back(new Table(tablas[idxT]->getColumnas(numC), tablas[idxT]->getFilas()));
			t = true;
			break;

		case '5':
			system("cls");
			tablas.push_back(new Table(tablas[idxT]->getColumnas(), tablas[idxT]->ordenar()));
			break;

		case '6':
			system("cls");
			tablas[idxT]->filtrar();
			break;
		case '7':
			system("cls");
			tablas[idxT]->exportarArch();
			break;
		case '8':
			system("cls");

			tablas.push_back(importarArch());
			break;
		case '9':
			system("cls");

			do {
				cout << "Ingrese # de la tabla a utilizar (1 es la original - se se crea una segunda tabla esta sera # 2 | 0 para regresar): ";
				cin >> tempidx;
				if (tempidx == 0)
					break;
			} while (tempidx < 0 || tempidx > tablas.size());
			idxT = tempidx - 1;
			break;
		case '0':
			bandera = true;

			//exit(1);
			break;

		default:
			system("cls");
			cout << "Opcion no valida.\a\n";
			break;
		}

	} while (bandera != true);

	system("pause");
	return 0;
}