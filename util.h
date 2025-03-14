#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
using namespace std;

/**
 * @brief Verifica si un elemento se encuentra dentro de un vector.
 * 
 * @param v El vector con elementos.
 * @param element El elemento a buscar en el vector.
 * @return true El elemento se encuentra en el vector.
 * @return false El elemento no se encuentra en el vector.
 * 
 * Complejidad asintótica: O(n)
 */
template <class T>
bool in(vector<T> v, T element){
    for(auto i:v){
        if(i == element) return true;
    }
    return false;
}

#endif