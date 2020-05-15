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
int kOptimo = 0;
// Version principal
//Algoritmo BT sin generar vector de elementos
int BT(int i, int W, int k, int minR)
{
    // Caso base.
    if (i == n)  {
        if (W <= R && minR >= 0) {
            kOptimo = max(kOptimo, k);
            return k;
        }else{
            return 0;
        }
    }

    // Poda por factibilidad.
    //Si rompe la bolsa o hay aplastados
    if (poda_factibilidad && (W > R || minR < 0)) return 0;

    // Poda por optimalidad.
    //Si no es posible superar al optimo
    if (poda_optimalidad && (kOptimo >= (k + n-i-1))) return 0;

    // Recursión.
    return max(BT(i+1, W, k, minR), BT(i+1, W+w[i], k+1, min(minR - w[i], r[i])));
}

//---------------------------------------------------------------------------------
vector<vector<int>> m; // Matriz para memoria de PD

int DP(int i, int Ractual)
{
	if (Ractual < 0) {return -1;} // Si se rompe el tubo o algun elemento devuelvo -1, pues debo terminar y restarlo al contador de PD.
	
	if (i == n ) {return 0;} // Caso base en el que no se rompe ningun objeto.
	
	if (m[i][Ractual] == -1) {
	    int Rproximo = min(Ractual-w[i], r[i]);
        m[i][Ractual] = max(DP(i+1, Ractual), 1 + DP(i+1, Rproximo));
	}
	
	return m[i][Ractual];
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
        res = BT(0, 0, 0, R);
	}
	else if (algoritmo == "BT-F")
	{   
        poda_factibilidad = true;
        res = BT(0, 0, 0, R);
	}
	else if (algoritmo == "BT-O")
	{   
        poda_optimalidad = true;
        res = BT(0, 0, 0, R);
	}
	else if (algoritmo == "DP")
	{
        //Inicializo la matriz
		m = vector<vector<int>>(n, vector<int>(R+1, -1));
        
		// Obtenemos la solucion optima.
		res = DP(0, R);
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
