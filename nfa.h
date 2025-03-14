/**
 * @file nfa.h
 * @author Edmundo Canedo Cervantes - A01645576
 * @brief El archivo contiene la clase que permite obtener un NFA
 * a partir de una expresión regular en formato prefijo
 * 
 * Complejidad asintótica: ?
 * 
 * @date 2025-03-14
 */
#ifndef NFA_H
#define NFA_H

#include <iostream>
#include <vector>
#include <map>
#include "util.h"
using namespace std;

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
 */
int Connection::getNode(){
    return node;
}

/**
 * @brief Obtiene el peso de la conexión.
 * 
 * @return char El peso de la conexión.
 */
char Connection::getWeight(){
    return weight;
}

/**
 * @class NFA
 * 
 * @brief Clase que maneja la formación de un NFA.
 * 
 * Guarda los métodos y atributos para obtener un NFA basándose
 * en una expresión regular en formato prefijo.
 */
class NFA {
    private:
        // Guarda la expresión regular original en formato prefijo
        string originalRegex;

        // Guarda las conexiones que hay entre los diferentes nodos
        map<int, vector<Connection>> connections;
        int noConnections;

        // Guarda los diferentes operadores que se pueden usar
        vector<char> operators;

        void newNode();
        void connect(int, int, char);
        vector<string> getOperands(string);
        char getChar(string);
        void model(int, string);

    public:
        NFA(string);
        void model();
        void print();
};

NFA::NFA(string regex) {
    
}

void NFA::newNode() {

}

void NFA::connect(int node1, int node2, char weight) {

}

vector<string> NFA::getOperands(string regex) {
    vector<string> operands;

    return operands;
}

char NFA::getChar(string regex) {
    return 0;
}

void NFA::model(int startNode, string regex) {

}

void NFA::model() {

}

void NFA::print() {

}

#endif