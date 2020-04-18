#include "fuerzabruta.h"

//----------------------------------------------------------------------------------
vector< vector< objeto > > permutaciones(vector< objeto > A){
	reutrn permutacionesAux(A, 0);
}
//----------------------------------------------------------------------------------
vector< vector< objeto > > permutacionesAux(vector< objeto > A, unsigned int indice){
	vector< vector < objeto > > res = {};
	if (A.length() == indice){
		res = {A}; // necesitamos hacer una deep copy, para que lo que estemos asignando en res no sea un A que en los proximos momentos se va a modificar. Sino que sea una foto del momento actual. Podriamos hacerlo llamando a malloc o a new.
	} else {
		for (unsigned int i = indice; i < A.length();i++){
			swap(A[i], A[indice]);
			res = res++permutacionesAux(A,index+1);// No estoy muy seguro de como escribir la concatenacion. Tal vez deba hacer una funcion yo.
			swap(A[i], A[indice]);
		}
	return res;
}
//----------------------------------------------------------------------------------
