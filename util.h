/**
 * @file util.cpp
 * @author Edmundo Canedo Cervantes - A01645576
 * @brief Métodos adicionales útiles para correr el programa.
 *
 * Complejidad asintótica: O(n)
 *
 * @date 2025-03-14
 */
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