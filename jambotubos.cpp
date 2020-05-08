#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <set>
#include <map>

using namespace std;

// Información de la instancia a resolver.
int R, W, n;
vector <int> r;
vector <int> w;

// R: resistencia de Jambotubo.
// i: posicion del elemento en la secuencia de productos.
// r: arreglo que tiene en cada posición la resistencia del producto.
// w: arreglo que tiene en cada posición el peso del producto.
// W: peso acumulado de los productos.
// k: cantidad de elementos seleccionados.
/*

//---------------------------------------------------------------------------------
// Version alt.
// La idea es que R es la resistencia de un elemento con peso cero, 
// el cual es anterior al primer elemento que viene por la cinta
// De esa forma sacamos el caso de "revisar si se rompe el tubo"
void FBAux(int i, int k, int Ractual){
	if( i == n ){
		if (Ractual - w[i] < 0){
			return 0;
		} else {
			return k;
		}
	} else {
	int Rproxima = min(Ractual - w[i], r[i]);
    return  max(FBAux(i+1, k, Rproxima, &res), FB(i+1, k+1, Rproxima, &res));
	}
}
//---------------------------------------------------------------------------------
// Asi se llamaria a FB con la forma de arriba
int FB(){
	return FBAux(0, 0, R);
}
//---------------------------------------------------------------------------------
int maximoVector(vector<int> vec){
	int max = 0;
	for(int i = 0;i< vec.length();i++){
		if (vec[i] > max){
			max = vec[i];
		}
	}
	return max;
}
//---------------------------------------------------------------------------------
*/

// Version Principal
// Algoritmo FB sin generar vector de elementos
int FB(int i, int W, int k, int minR, bool aplastados)
{
    // Caso base.    
    if (i == n) return (W <= R && !aplastados) ? k : 0;

    // Recursión.
    bool aplastadosAux = aplastados || (minR - w[i] < 0);
    return max(FB(i+1, W, k, minR, aplastados), FB(i+1, W+w[i], k+1, min(minR - w[i], r[i]), aplastadosAux));
}

//---------------------------------------------------------------------------------
bool poda_factibilidad = false;
bool poda_optimalidad = false;

// Version principal
//Algoritmo BT sin generar vector de elementos
int BT(int i, int W, int k, int kOptimo, int minR)
{
    // Caso base.    
    if (i == n) return (W <= R && minR >= 0) ? kOptimo : 0;

    // Poda por factibilidad.
    //Si rompe la bolsa o hay aplastados
    if (poda_factibilidad && (W > R || minR < 0)) return 0;

    // Poda por optimalidad.
    //Si no es posible superar al optimo
    if (poda_optimalidad && (kOptimo > (k + n-i-1))) return 0;

    // Recursión.
    return max(BT(i+1, W, k, kOptimo, minR), BT(i+1, W+w[i], k+1, max(kOptimo, k+1), min(minR - w[i], r[i])));
}

/*
int BT(){
	int res = BTAux(0, 0, R, poda_factibilidad, poda_optimalidad, &max);
	if(res < 0){
		res = 0;
	}
	return res;
}
//---------------------------------------------------------------------------------
// Version alternativa
void BTAux(int i, int k, int Ractual, poda_f, poda_o){
	if(poda_f){
		if (Ractual - w[i] < 0){
			return k-1;
		}
	int Rproxima = min(Ractual - w[i], r[i]);
    return max(FBAux(i+1, k, Rproxima, &res), FB(i+1, k+1, Rproxima, &res));
	}
	if(poda_o){
		// ? De que manera entiendo la optimalidad?
	}
}
*/

// Programación Dinámica (debe ser top down)
// TODO no deberiamos tener "podas" Son casos base.
vector<vector<int>> m; // Matriz para memoria de PD
int DP(int i, int W, int k, int minR)
{
    // Caso base.    
    if (i == n) return (W <= R && minR >= 0) ? k : 0;

    // Poda por factibilidad.
    //Si rompe la bolsa o hay aplastados
    if (poda_factibilidad && (W > R || minR < 0)) return 0;

    // Poda por optimalidad.
    //Si no es posible superar al optimo
    if (poda_optimalidad && (m[i][W] > (k + n-i-1))) return 0;

    // Recursión.
    if (m[i][W] == -1) {
        m[i][W] = max(DP(i+1, W, k, minR), DP(i+1, W+w[i], k+1, min(minR - w[i], r[i])));
    }
    
    return m[i][W];
}   

//Algoritmo DP alternativo
int DPAlt(int i, int W) //, int minR)
{
	if (i == 0 || W <= 0) {return 0;}
	
	// Este caso no hace falta verlo, ya lo cubris cuando haces  DPAlt( i-1, W-w[i-1] ) en el llamado recursivo
// Creo que te estas confundiendo entre el caso actual y el siguiente, porque para que funcione lo que te dije en la linea anterior, deberias hacer DPAlt( i-1, W-w[i] )
	if (W - w[i-1] < 0) {return 0;}
	
	if (m[i][W] == -1) {
//	    int minRAux = (i == 1) ? r[i] : min(minR-w[i], r[i]);
//	    m[i][W] = max(DPAlt(i-1, W, minR), 1 + DPAlt(i-1, W-w[i], minRAux));
        m[i][W] = max(DPAlt(i-1, W), 1 + DPAlt(i-1, W-w[i-1]));
	}
	
	return m[i][W];
}

//---------------------------------------------------------------------------------
// Algoritmo DP alternativo, de Javier, pero modificado por Jona para que vaya desde atras para adelante
int DPAlt_Aux(int i, int Ractual, int cantElem) // cantElem es la cantidad total de elementos
{
	if (i == cantElem || Ractual < 0) {return 0;} // en vez de Ractual <= 0, pongo Ractual < 0, pues el caso en que Ractual es = a cero no se aplasta el objeto asociado a esa resistencia. Un objeto solo se aplasta si se sobrepaso su resistencia
	
	if (m[i][W] == -1) {
	    int Rproximo = min(Ractual-w[i], r[i]);
        m[i][W] = max(DPAlt_Aux(i+1, Ractual), 1 + DPAlt_Aux(i+1, Rproximo));
	}
	
	return m[i][Ractual];
}
int DPAlt(){
	return DPAlt_Aux(0,R,n); // Empiezo con la resistencia del jambotubo, es como si fuera un elemento que viene por la cinta, pero con peso cero
}

//---------------------------------------------------------------------------------
// Recibe por parámetro qué algoritmos utilizar para la ejecución separados por espacios.
// Imprime por clog la información de ejecución de los algoritmos.
// Imprime por cout el resultado de algun algoritmo ejecutado.
int main(int argc, char** argv)
{
	// Leemos el parametro que indica el algoritmo a ejecutar.
	map<string, string> algoritmos_implementados = {
		{"FB", "Fuerza Bruta"}, {"BT", "Backtracking con podas"}, {"BT-F", "Backtracking con poda por factibilidad"}, 
		{"BT-O", "Backtracking con poda por optimalidad"}, {"DP", "Programacion dinámica"}
	};

	// Verificar que el algoritmo pedido exista.
	if (argc < 2 || algoritmos_implementados.find(argv[1]) == algoritmos_implementados.end())
	{
		cerr << "Algoritmo no encontrado: " << argv[1] << endl;
		cerr << "Los algoritmos existentes son: " << endl;
		for (auto& alg_desc: algoritmos_implementados) cerr << "\t- " << alg_desc.first << ": " << alg_desc.second << endl;
		return 0;
	}
	string algoritmo = argv[1];

    // Leemos el input.
    cin >> n >> R;
    r.assign(n, 0);
    w.assign(n, 0);
    
    for (int i = 0; i < n; ++i) {
        cin >> w[i] >> r[i];
    }

    // Ejecutamos el algoritmo y obtenemos su tiempo de ejecución.
	int res = 0;
	
    auto start = chrono::steady_clock::now();
    
    if (algoritmo == "FB")
	{
        res = FB(0, 0, 0, R, false);
	}
	else if (algoritmo == "BT")
	{
        poda_factibilidad = true;
        poda_optimalidad = true;
        res = BT(0, 0, 0, 0, R);
	}
	else if (algoritmo == "BT-F")
	{   
        poda_factibilidad = true;
        res = BT(0, 0, 0, 0, R);
	}
	else if (algoritmo == "BT-O")
	{   
        poda_optimalidad = true;
        res = BT(0, 0, 0, 0, R);
	}
	else if (algoritmo == "DP")
	{
        //Inicializo la matriz
		m = vector<vector<int>>(n, vector<int>(R+1, -1));
        
        poda_factibilidad = true;
        poda_optimalidad = true;
        
		// Obtenemos la solucion optima.
		res = DP(0, 0, 0, R);
	}
	else
	{
	    //Nada
	}
	
	auto end = chrono::steady_clock::now();
	double total_time = chrono::duration<double, milli>(end - start).count();

	// Imprimimos el tiempo de ejecución por stderr.
	clog << total_time << endl;

    // Imprimimos el resultado por stdout.
    cout << (res >= 0 ? res : -1) << endl;
    return 0;
}
//---------------------------------------------------------------------------------
