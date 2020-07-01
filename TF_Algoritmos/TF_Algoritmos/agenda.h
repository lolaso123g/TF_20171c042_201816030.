#pragma once

class Agenda
{
private:
	fstream file;
	vector<string> split(const string& source, char token);
	bool fileExists(const string& ruta);

public:
	Agenda(const string& ruta);
	void agregar(Table& persona);
	void agregarTodas(const vector<Table>& personas);
	vector<Table> leerTodo();
	~Agenda();//Destructor de la clase
};

Agenda::Agenda(const string& ruta)
{
	if (fileExists(ruta))
	{
		file.open(ruta.c_str(), fstream::in | fstream::out | fstream::app);
	}
	else
	{
		file.open(ruta.c_str(), fstream::in | fstream::out | fstream::trunc);
	}
}

vector<string> Agenda::split(const string& source, char token)
{
	std::string sentence = "";
	vector<string> result;

	for (char c : source)
	{
		if (c == token)
		{
			result.push_back(sentence);
			sentence = "";
			continue;
		}
		sentence.push_back(c);
	}
	return result;
}


bool Agenda::fileExists(const string& ruta)
{
	fstream infile(ruta.c_str());
	return infile.good();
}

void Agenda::agregar(Table& persona)
{
	if (file.is_open())
	{
		file << persona. << /*"," << persona.getApellido() << "," << persona.getCedula() << "," <<*/ endl;
	}
}

void Agenda::agregarTodas(const vector<Table>& personas)
{
	if (file.is_open())
	{
		for (Table p : personas)
		{
			file << p.getNombre() /*<< "," << p.getApellido() << "," << p.getCedula() << ","*/ << endl;
		}
	}
}

vector<Table> Agenda::leerTodo()
{
	vector<Table> personas;
	if (file.good())
	{
		file.seekg(0, file.beg);
		string linea;
		while (getline(file, linea))
		{
			vector<string> tokens = split(linea, ',');
			string nombre = tokens[0];
			string apellido = tokens[1];
			string cedula = tokens[2];

			/*Columna p(nombre, apellido, cedula);
			personas.push_back(p);                           */

		}
	}
	return personas;
}

Agenda::~Agenda()
{
	file.close();
}
