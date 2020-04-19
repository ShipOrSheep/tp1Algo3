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
// La idea de verificar las resistencias es que solo importa medir la resistencia que se va a romper mas rapido, la cual es la mas chica de las actuales
// Chequear cual es la que se romperia mas rapido es O(1)
void FBAux(int i, int k, int Ractual, vector<int> *res){
	if (Ractual - w[i] < 0){
		res.push_back(0);
	} else {
		res.push_back(k);
	}
	if(i == n){return;} // Llegue al fondo del arbol de recursion. Ya puse 0 o k en el vector.
	int Rproxima = min(Ractual - w[i], r[i]);
    res.push_back( max(FBAux(i+1, k, Rproxima, &res), FB(i+1, k+1, Rproxima, &res)) )
	return;
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
int FB(){
	vector<int> res; // no nos preocupamos por usar memoria dinamica, <vector> ya la usa
	FBAux(0, 0, R, &res);
	return maximo(res); // el maximo del vector
}
// Estimo que la complejidad es O(2^n) por FBAux + O(2^n) por el maximo.
// Me imagino que se puede hacer FBAux de forma divide and conquer, y ahi podriamos justificar la complejidad con el teorema maestro. Deberiamos modificar el algoritmo como para que arme las secuencias y subsecuencias. El paso recursivo seria separado en 2 probl. de n/2 tamano. Una vez que volvamos del paso recursivo, usamos un merge para unir las secuencias de los dos lados. Eso tendria una complejidad. Despues buscariamos la secuencia con mejor k, lo que seria O(2^n * n) (2^n casos de n largo).
//---------------------------------------------------------------------------------


// Ajuste sobre el algoritmo anterior
int FB(int i, int W, int k, int minR, bool aplastados)
{
    // Caso base.    
    if (i == n) return (W <= R && !aplastados) ? k : 0;

    // Recursión.
    bool aplastadosAux = aplastados || (minR - w[i] < 0);
    return max(FB(i+1, W, k, minR, aplastados), FB(i+1, W+w[i], k+1, min(minR - w[i], r[i]), aplastadosAux));
}

//---------------------------------------------------------------------------------
bool poda_factibilidad = true;
bool poda_optimalidad = true;
int BT(){
	vector<int> res; // no nos preocupamos por usar memoria dinamica, <vector> ya la usa
	int max;
	BTAux(0, 0, R, &res, poda_factibilidad, poda_optimalidad, &max);
	return maximo(res); // el maximo del vector
}
//---------------------------------------------------------------------------------
void FBAux(int i, int k, int Ractual, vector<int> *res, bool poda_factibilidad, bool poda_optimalidad, int *max){
	if(poda_factibilidad){
		if (Ractual - w[i] < 0){
			return;
		} else {
			res.push_back(k);
		}
	}
/*	if(poda_optimalidad){
		if( Ractual - w[i] < 0 ){
			return;
		} else {
			//
		}
	}
*/
// No se me ocurre la poda por optimalidad
	if(i == n){return;} // Llegue al fondo del arbol de recursion. Ya puse 0 o k en el vector.
	int Rproxima = min(Ractual - w[i], r[i]);
    res.push_back( max(FBAux(i+1, k, Rproxima, &res, poda_factibilidad, poda_optimalidad, &max), FB(i+1, k+1, Rproxima, &res, poda_factibilidad, poda_optimalidad, &max)) )
	return;
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
    
    switch (algoritmo) {
        case "FB": 
            res = FB(0, 0, 0, R, false);
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
