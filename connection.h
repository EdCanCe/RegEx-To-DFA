/**
 * @file connection.h
 * @author Edmundo Canedo Cervantes - A01645576
 * @brief El archivo contiene la clase que permite usar
 * conexiones entre nodos.
 *
 * Complejidad asintótica: O(1)
 *
 * @date 2025-03-14
 */
#ifndef CONNECTION_H
#define CONNECTION_H

#include<iostream>

class Connection {
   private:
    // Guarda el nodo hacia el cuál se conectará
    int node;

    // Guarda el peso de la conexión
    char weight;

   public:
    Connection(int, char);
    int getNode();
    char getWeight();
};

/**
 * @brief Constructor de una conexión, crea una conexión.
 *
 * @param connectedNode El nodo hacia dónde se conectará.
 * @param connectedWeight El peso de la conexión.
 *
 * Complejidad asintótica: O(1)
 */
Connection::Connection(int connectedNode, char connectedWeight) {
    node = connectedNode;
    weight = connectedWeight;
}

/**
 * @brief Obtiene el nodo de la conexión.
 *
 * @return int El nodo de la conexión.
 *
 * Complejidad asintótica: O(1)
 */
int Connection::getNode() { return node; }

/**
 * @brief Obtiene el peso de la conexión.
 *
 * @return char El peso de la conexión.
 *
 * Complejidad asintótica: O(1)
 */
char Connection::getWeight() { return weight; }

#endif