#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "util.h"
using namespace std;

/**
 * @class Preprocessing
 * 
 * @brief Clase que maneja el preprocesamiento de una expresión regular.
 * 
 * Guarda los métodos y atributos necesarios para convertir una expresión
 * regular en su método prefijo.
 */
class Preprocessing {
    private:
        // Guarda la expresión regular original
        string originalRegex;

        // Guarda los caracteres que usa la expresión
        vector<char> characters;

        // Guarda los diferentes operadores que se pueden usar
        vector<char> operators; 
        
        char getOperator(string);
        int getSeparation(string);
        vector<string> getOperands(string);
        string cleanRegex(string);
        string notation(string);
        
        public:
        Preprocessing(string, string);
        string notation();
};

/**
 * @brief Constructor del preprocesador, crea un preprocesador.
 * 
 * 
 * @param regex Expresión regular original a preprocesar.
 * @param usedCharacters Los caracteres de los cuales está formada
 * la expresión regular.
 * 
 * Complejidad asintótica: O(n)
 */
Preprocessing::Preprocessing(string regex, string usedCharacters) {
    originalRegex = regex;

    // Añade los diferentes operadores que puede tener la expresión regular
    operators.push_back('|');
    operators.push_back('*');
    operators.push_back('+');

    // Añade los caracteres de los cuales está formada la expresión a su vector
    for (auto i : usedCharacters) { 
        characters.push_back(i);
    }
}

/**
 * @brief Obtiene el operador principal a realizar dentro de un fragmento de
 * una expresión regular.
 * 
 * @param regex Fragmento de expresión regular a evaluar.
 * @return char El operador prinvcipal de dicho framento.
 * 
 * Complejidad asintótica: O(n)
 */
char Preprocessing::getOperator(string regex) {
    // Contador de paréntesis para no contar expresiones dentro de paréntesis
    int parenthesis = 0; 

    // Verifica si es un 'or' al contar los símbolos que lo representen 
    // fuera de paréntesis
    for (int i = 0; i < regex.size(); i++) { 
        if (regex[i] == '(') parenthesis++;
        else if (regex[i] == ')') parenthesis--;
        else if (regex[i] == '|' and parenthesis == 0) return '|';
    }

    if (regex[regex.size() - 1] == '+') return '+';
    if (regex[regex.size() - 1] == '*') return '*';

    // En caso de que no sea otro operador, se regresa como operador
    // de concatenar
    return '_';
}

/**
 * @brief Obtiene el índice donde se separa una parte de la expresión de su
 * otra mitad debido al uso de un operador 'or'.
 * 
 * @param regex Fragmento de expresión regular a evaluar.
 * @return int El índice del punto de separación.
 * 
 * Complejidad asintótica: O(n)
 */
int Preprocessing::getSeparation(string regex) {
    // Contador de paréntesis para no contar expresiones dentro de paréntesis
    int parenthesis = 0; 

    // Encuentra el índice donde se encuentra el punto de separación de
    // la expresión
    for (int i = 0; i < regex.size(); i++) { 
        if (regex[i] == '(') parenthesis++;
        else if (regex[i] == ')') parenthesis--;
        else if (regex[i] == '|' and parenthesis == 0) return i;
    }

    return 0;
}

/**
 * @brief Obtiene los operandos de un fragmento de una expresión regular.
 * 
 * @param regex Fragmento de expresión regular a evaluar.
 * @return vector<string> Los operandos de dicho fragmento.
 * 
 * Complejidad asintótica: O(n²)
 */
vector<string> Preprocessing::getOperands(string regex) {
    // Guarda los operandos que se tienen en el fragmento de la 
    // expresión regular
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
                if (regex[j] == ')') parenthesis--;
                else if (regex[j] == '(') parenthesis++;
                j++;
            }

            j--;
            extra = j - i;
        }

        // En caso de que el siguiente elemento sea un operador como
        // 'uno o más', se le añade otro caracter más
        if(j < (regex.size() - 1) && in(operators, regex[j + 1])){
            extra += 1;
        }

        operands.push_back(regex.substr(i, extra+1));
        i += extra;
    }

    return operands;
}

/**
 * @brief Obtiene la expresión quitando los paréntesis innecesarios a partir
 * de un fragmento de una expresión regular.
 * 
 * @param regex Fragmento de expresión regular a evaluar.
 * @return string El fragmento sin paréntesis innecesarios.
 * 
 * Complejidad asintótica: O(n²)
 */
string Preprocessing::cleanRegex(string regex) {
    int parenthesis = 0;
    int counter = 0;

    for (int i = 0; i < regex.size(); i++) {
        if (regex[i] == '(') {
            parenthesis++;
            counter++;
        }
        else if (regex[i] == ')') {
            parenthesis--;
            counter++;
        }

        // En caso de que el paréntesis no sea el elemento final, significa
        // que SI es necesario el paréntesis
        if(parenthesis == 0 && i < regex.size() - 1) return regex;
    }

    if (counter == 0) return regex;
    return cleanRegex(regex.substr(1, regex.size()-2));
}


/**
 * @brief Obtiene una expresión regular en su formato prefijo a partir
 * de un fragmento de una expresión regular.
 * 
 * @param regex Fragmento de expresión regular a evaluar.
 * @return string El fragmento en formato prefijo.
 * 
 * Complejidad asintótica: O(n³)
 */
string Preprocessing::notation(string regex) {
    regex = cleanRegex(regex);
    vector<string> operands = getOperands(regex);
    char operation = getOperator(regex);
    string newRegex(1, operation);

    // En caso de que tenga más de un operador, pero no sea un 'or',
    // significa que es una concatenación enrealidad
    if (operands.size() > 1 && operation != '|') operation = '_';

    switch (operation){
        case '|': {
            int idSeparation = getSeparation(regex);
            string firstOperand = regex.substr(0, idSeparation);
            string secondOperand = regex.substr(idSeparation + 1);
            newRegex += "(" + notation(firstOperand) + ")" + "(" + notation(secondOperand) + ")";
            break;
        }

        case '_': {
            // Si es solo un elemento, se regresa a si mismo
            if(operands.size() == 1){
                newRegex = "." + operands[0];
                return newRegex;
            }
            for(auto i:operands){
                newRegex += "(" + notation(i) + ")";
            }
            break;
        }

        default: {
            string operand = operands[0].substr(0, operands[0].size() - 1);
            newRegex += "(" + notation(operand) + ")";
            break;
        }
            
    }    

    return newRegex;
}

/**
 * @brief Obtiene una expresión regular en su formato prefijo a partir
 * de una expresión regular.
 * 
 * @return string La expresión en formato prefijo.
 * 
 * Complejidad asintótica: O(n³)
 */
string Preprocessing::notation() {
    return notation(originalRegex);
}

#endif