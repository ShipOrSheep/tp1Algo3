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

//---------------------------------------------------------------------------------
// Primero lee como llamo a FB
void FBAux(int i, int k, int Ractual, vector<int> *res){
    if (i == n) return Ractual - w[i] < 0 ? k : 0; // en vez de return, deberia ser un append 0 o k al vector de respuestas
	if (Ractual - w[i] < 0){
		return 0; // en vez de return, deberia ser un append 0 al vector de respuestas
	} else {
		return k; // en vez de return, deberia ser un append k al vector de respuestas
	}
	int Rproxima = min(Ractual - w[i], r[i]);
    return max(FBAux(i+1, k, Rproxima, &res), FB(i+1, k+1, Rproxima, &res)); // en vez de return, deberia ser un append k al vector de respuestas
}
//---------------------------------------------------------------------------------
int maximo(vector<int> vec){
	int max = 0;
	for(int i = 0;i< vec.length();i++){
		if (vec[i] > max){
			max = vec[i];
		}
	}
	return max;
}
//---------------------------------------------------------------------------------
// Este esria el nuevo FB
int FB(int i, int W, int k){
	vector<int> res;
	FBAux(0, 0, R, &res);
	return maximo(res); // el maximo del vector
}
//---------------------------------------------------------------------------------
// No elimine tu FB por las dudas
int FB(int i, int W, int k)
{
    // Caso base.
	// if (verificaResistencias(arregloDeEstePaso)){
		return k;
	} else {
		return 0;
	}
    if (i == n) return W <= R ? k : 0;

    // Recursión.
    return max(FB(i+1, W, k), FB(i+1, W+w[i], k+1));
}

// Recibe por parámetro qué algoritmos utilizar para la ejecución separados por espacios.
// Imprime por clog la información de ejecución de los algoritmos.
// Imprime por cout el resultado de algun algoritmo ejecutado.
//---------------------------------------------------------------------------------
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
    
    switch (algoritmo) {
        case "FB": 
            res = FB(0, 0, 0);
            break;
        default:
            break;
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
