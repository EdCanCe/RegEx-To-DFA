/**
 * @file main.cpp
 * @author Edmundo Canedo Cervantes - A01645576
 * @brief Usa las clases NFA y Preprocessing para formular un DFA a partir de
 * una expresión regular.
 *
 * Complejidad asintótica: O(2^n)
 *
 * @date 2025-03-14
 */
#include <iostream>
#include <vector>

#include "dfa.h"
#include "nfa.h"
#include "preprocess.h"
using namespace std;

int main() {
    string characters;
    cout << "Enter the characters in your regular expression: ";
    cin >> characters;

    string regex;
    cout << "Type your regular expression: ";
    cin >> regex;

    Preprocessing preprocessed(regex, characters);

    cout << "----RESULTS----\n"
         << "INPUT:\n"
         << regex << "\n\n";

    NFA nfa(preprocessed.notation());
    pair<int, int> startFinish = nfa.print();

    DFA dfa(nfa.getConnections(), startFinish.first, startFinish.second,
            vector<char>(characters.begin(), characters.end()));
    dfa.print();
}