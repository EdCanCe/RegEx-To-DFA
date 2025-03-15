/**
 * @file dfa.h
 * @author Edmundo Canedo Cervantes - A01645576
 * @brief El archivo contiene la clase que permite obtener un DFA
 * a partir de un NFA.
 *
 * Complejidad asintótica: O(?)
 *
 * @date 2025-03-14
 */
#ifndef DFA_H
#define DFA_H

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "connection.h"
#include "util.h"
using namespace std;

/**
 * @class DFA
 * 
 * @brief Obtiene el modelo DFA a partir de una matriz de adyacencias de un NFA.
 * 
 * Guarda los métodos y atributos para poder obtener un DFA basándose en una matriz de adyacencias de un NFA.
 * 
 */
class DFA {
   private:
    // Las conexiones que se tenían en el NFA
    map<int, vector<Connection>> nfaConnections;

    // Los nodos de inicio y final del NFA
    int startNode;
    int endNode;

    // Los elementos que guardan los estados del DFA
    vector<vector<int>> states;

    // Las conexiones que tienen los estados entre sí
    map<int, vector<Connection>> statesConnections;

    // Estados de aceptación del DFA
    vector<int> acceptingStates;

    // Caracteres usados en la expresión regular en la que se basan
    vector<char> characters;

    vector<int> getEpsilonConnections(int, vector<int>&, vector<int>&);
    vector<int> getConnections(int, char);
    int eClosure(vector<int>);
    vector<int> move(int, char);
    void doState(int);
    string getChar(int);
    void model();

   public:
    DFA(map<int, vector<Connection>>, int, int, vector<char>);
    void print();
};

/**
 * @brief Constructor de una clase usada para modelar un autómata finito determinista a partir de una matriz de adyacencias de un NFA, crea dicho modelo.
 * 
 * @param connections La matriz de adyacencias.
 * @param start El nodo inicial del modelo NFA.
 * @param end El nodo final del modelo NFA.
 * @param chars Los caracteres usados en la expresión regular en que se basan.
 * 
 * Complejidad asintótica: O(1)
 */
DFA::DFA(map<int, vector<Connection>> connections, int start, int end,
         vector<char> chars) {
    nfaConnections = connections;
    startNode = start;
    endNode = end;
    characters = chars;
}

vector<int> DFA::getEpsilonConnections(int index, vector<int>& visited,
                                       vector<int>& queue) {
    if (in(visited, index)) return queue;  // Evita ciclos infinitos
    visited.push_back(index);
    queue.push_back(index);

    for (auto i : nfaConnections[index]) {
        if (i.getWeight() == '#') {
            getEpsilonConnections(i.getNode(), visited, queue);
        }
    }

    // Elimina duplicados usando un set
    set<int> unique(queue.begin(), queue.end());
    return vector<int>(unique.begin(), unique.end());
}

vector<int> DFA::getConnections(int index, char weight) {
    vector<int> fullArray;

    for (auto i : nfaConnections[index]) {
        if (i.getWeight() == weight) {
            fullArray.push_back(i.getNode());
        }
    }

    // Elimina duplicados usando un set
    set<int> unique(fullArray.begin(), fullArray.end());
    return vector<int>(unique.begin(), unique.end());
}

int DFA::eClosure(vector<int> indexArray) {
    vector<int> fullArray;
    bool isEnd = false;

    for (auto i : indexArray) {
        vector<int> visited;
        vector<int> epsilonStates =
            getEpsilonConnections(i, visited, fullArray);
        fullArray.insert(fullArray.end(), epsilonStates.begin(),
                         epsilonStates.end());
    }

    // Elimina duplicados usando un set
    set<int> unique(fullArray.begin(), fullArray.end());
    fullArray = vector<int>(unique.begin(), unique.end());

    // Verifica si el estado contiene el nodo final del NFA
    if (in(fullArray, endNode)) {
        isEnd = true;
    }

    // Busca si el estado ya existe
    sort(fullArray.begin(), fullArray.end());
    for (int i = 0; i < states.size(); ++i) {
        sort(states[i].begin(), states[i].end());
        if (states[i] == fullArray) {
            if (isEnd && !in(acceptingStates, i)) {
                acceptingStates.push_back(i);
            }
            return i;
        }
    }

    // Si no existe, agrega el nuevo estado
    states.push_back(fullArray);
    if (isEnd) {
        acceptingStates.push_back(states.size() - 1);
    }
    return states.size() - 1;
}

vector<int> DFA::move(int startState, char weight) {
    vector<int> fullArray;
    for (int i : states[startState]) {
        vector<int> newStates = getConnections(i, weight);
        fullArray.insert(fullArray.end(), newStates.begin(), newStates.end());
    }

    // Elimina duplicados usando un set
    set<int> unique(fullArray.begin(), fullArray.end());
    return vector<int>(unique.begin(), unique.end());
}

void DFA::doState(int state) {
    for (char i : characters) {
        vector<int> nodes = move(state, i);

        // Si no hay nodos, no se puede conectar
        if (nodes.empty()) continue;

        int endState = eClosure(nodes);
        statesConnections[state].push_back(Connection(endState, i));
    }
}

string DFA::getChar(int number) {
    string state = "";
    while (number >= 0) {
        state = char(number % 26 + 'A') + state;
        number /= 26;
        number--;
    }
    return state;
}

void DFA::model() {
    eClosure({startNode});  // Calcula el e-closure del estado de inicio

    int i = 0;
    int j = 1;
    while (i < j) {
        doState(i);
        i++;
        j = states.size();
    }
}

void DFA::print() {
    model();  // Construye el DFA automáticamente

    cout << "\nDFA:\n";
    for (auto i : statesConnections) {
        cout << getChar(i.first) << " => [";
        for (int j = 0; j < i.second.size(); j++) {
            cout << "('" << getChar(i.second[j].getNode()) << "', '"
                 << i.second[j].getWeight() << "')";
            if (j < i.second.size() - 1) cout << ", ";
        }
        cout << "]\n";
    }

    // Estados de aceptación
    set<int> accepted(acceptingStates.begin(), acceptingStates.end());
    acceptingStates = vector<int>(accepted.begin(), accepted.end());
    cout << "Accepting states: [";
    for (int i = 0; i < acceptingStates.size(); ++i) {
        cout << "'" << getChar(acceptingStates[i]) << "'";
        if (i < acceptingStates.size() - 1) cout << ", ";
    }
    cout << "]\n";
}

#endif