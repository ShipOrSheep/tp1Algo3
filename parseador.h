#include <vector>
#include <iostream>
#include <string>

typedef struct objeto{
	unsigned int peso;
	unsigned int resistencia;
} objeto;
//----------------------------------------------------------------------------------
vector< objeto > parsearEntrada(){
	string line;
	getline(cin, line);
	string cantidadObjetos;
	string resistenciaTubo;
	getline(line, cantidadObjetos, " ");
	getline(line, resistenciaTubo, "\n");
	for(;getline(cin, line);){
		string pesoObjeto;
		string resistenciaObjeto;
		getline(line, pesoObjeto, " ");
		getline(line, resistenciaObjeto, "\n");
	}
}
