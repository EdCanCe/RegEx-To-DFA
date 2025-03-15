/**
 * @file nfa.h
 * @author Edmundo Canedo Cervantes - A01645576
 * @brief El archivo contiene la clase que permite obtener un NFA
 * a partir de una expresión regular en formato prefijo
 *
 * Complejidad asintótica: O(n²)
 *
 * @date 2025-03-14
 */
#ifndef NFA_H
#define NFA_H

#include <iostream>
#include <map>
#include <utility>
#include <vector>

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

/**
 * @class NFA
 *
 * @brief Clase que maneja la formación de un NFA.
 *
 * Guarda los métodos y atributos para obtener un NFA basándose en una expresión
 * regular en formato prefijo.
 */
class NFA {
   private:
    // Guarda la expresión regular original en formato prefijo
    string originalRegex;

    // Guarda las conexiones que hay entre los diferentes nodos
    map<int, vector<Connection>> connections;
    int nodes;

    // Guarda los diferentes operadores que se pueden usar
    vector<char> operators;

    int newNode();
    void connect(int, int, char);
    vector<string> getOperands(string);
    char getChar(string);
    pair<int, int> model(int, string);

   public:
    NFA(string);
    void print();
    map<int, vector<Connection>> getConnections();
};

/**
 * @brief Contructor de una clase usada para modelar un aotómata finito no
 * determinista a partir de una expresión regular, crea dicho modelo.
 *
 * @param regex La expresión regular en formato prefijo.
 *
 * Complejidad asintótica: O(1)
 */
NFA::NFA(string regex) {
    originalRegex = regex;

    operators.push_back('|');
    operators.push_back('*');
    operators.push_back('+');
    operators.push_back('_');
    operators.push_back('.');

    nodes = 0;
}

/**
 * @brief Obtiene el índice de un nuevo nodo para conectar.
 *
 * @return int El índice del nuevo nodo.
 *
 * Complejidad asintótica: O(1)
 */
int NFA::newNode() {
    nodes++;
    return nodes - 1;
}

/**
 * @brief Conecta dos nodos con un peso entre ellos.
 *
 * @param node1 El nodo que se va a conectar.
 * @param node2 El nodo que recibe la conexión.
 * @param weight El peso de la conexión.
 *
 * Complejidad asintótica: O(1)
 */
void NFA::connect(int node1, int node2, char weight) {
    connections[node1].push_back(Connection(node2, weight));
}

/**
 * @brief Obtiene los operandos de un fragmento de una expresión regular.
 *
 * @param regex Fragmento de expresión regular a evaluar.
 * @return vector<string> Los operandos de dicho fragmento.
 *
 * Complejidad asintótica: O(n)
 */
vector<string> NFA::getOperands(string regex) {
    // Guarda los operandos que se tienen en el fragmento de la expresión
    // regular
    vector<string> operands;

    // Itera por toda la expresión para encontrar sus operandos
    for (int i = 0; i < regex.size(); i++) {
        // Cantidad de caracteres extra que tiene de tamaño el operando
        int extra = 0;
        int j = i;

        // En caso de que sea paréntesis, obtiene todo lo que tiene adentro
        if (regex[i] == '(') {
            j++;
            int parenthesis = 1;

            // Itera hasta terminar el paréntesis
            while (parenthesis != 0) {
                if (regex[j] == ')')
                    parenthesis--;
                else if (regex[j] == '(')
                    parenthesis++;
                j++;
            }

            j--;
            extra = j - i;

            // Se añade el operando dictado por sus paréntesis
            operands.push_back(regex.substr(i + 1, extra));
        }
        i += extra;
    }

    return operands;
}

/**
 * @brief Obtiene el caracter en expresiones con el formato .X
 *
 * @param regex La expresión regular a evaluar
 * @return char Su caracter
 *
 * Complejidad asintótica: 1
 */
char NFA::getChar(string regex) { return regex[1]; }

/**
 * @brief Obtiene el modelo NFA a partir de un fragmento de una expresión
 * regular.
 *
 * @param startNode El nodo inicial donde se va a basar el inicio de la
 * operación.
 * @param regex El fragmento de la expresión regular a evaluar.
 * @return pair<int, int> Los nodos de inicio y final de la expresión regular.
 *
 * Complejidad asintótica: O(n²)
 */
pair<int, int> NFA::model(int startNode, string regex) {
    char currentOperation = regex[0];
    vector<string> operands = getOperands(regex);

    // Los nodos de inicio y final de los operandos de la expresión
    vector<pair<int, int>> regexNodes;

    // Hace la operación de cada operando
    for (auto i : operands) {
        pair<int, int> travelNode = model(startNode, i);
        startNode = newNode();
        regexNodes.push_back(travelNode);
    }

    int endNode = newNode();

    // Realiza la operación actual
    switch (currentOperation) {
        case '|': {
            connect(startNode, regexNodes[0].first, '#');
            connect(startNode, regexNodes[1].first, '#');
            connect(regexNodes[0].second, endNode, '#');
            connect(regexNodes[1].second, endNode, '#');
            break;
        }

        case '_': {
            startNode = regexNodes[0].first;
            endNode = regexNodes[regexNodes.size() - 1].second;
            for (int i = 0; i < regexNodes.size() - 1; i++) {
                connect(regexNodes[i].second, regexNodes[i + 1].first, '#');
            }
            break;
        }

        case '*': {
            int midNode = regexNodes[0].first;
            connect(startNode, midNode, '#');
            connect(startNode, endNode, '#');
            connect(regexNodes[0].second, midNode, '#');
            connect(regexNodes[0].second, endNode, '#');
            break;
        }

        case '+': {
            int midNode = regexNodes[0].first;
            connect(startNode, midNode, '#');
            connect(regexNodes[0].second, midNode, '#');
            connect(regexNodes[0].second, endNode, '#');
            break;
        }

        case '.': {
            connect(startNode, endNode, getChar(regex));
            break;
        }
    }

    return pair<int, int>(startNode, endNode);
}

/**
 * @brief Imrpime el modelo NFA a partir de una expresión regular en formato
 * prefijo.
 *
 * Complejidad asintótica O(n)
 */
void NFA::print() {
    newNode();
    pair<int, int> startFinish(model(0, originalRegex));

    cout << "NFA:\n";
    for (int i = 0; i < nodes - 1; i++) {
        if (empty(connections[i])) continue;
        cout << i << " => [";
        for (int j = 0; j < connections[i].size(); j++) {
            cout << "(" << connections[i][j].getNode() << ", '"
                 << connections[i][j].getWeight() << "')";
            if (j < connections[i].size() - 1) cout << ", ";
        }
        cout << "]\n";
    }
    cout << "Accepting state: " << startFinish.second << "\n";
}

/**
 * @brief Obtiene las conexiones realizadas en el NFA.
 *
 * @return map<int, vector<Connection>> Las conexiones.
 */
map<int, vector<Connection>> NFA::getConnections() { return connections; }

#endif