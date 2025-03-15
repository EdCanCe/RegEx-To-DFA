#include <iostream>
#include <vector>
#include "preprocess.h"
#include "nfa.h"
#include "dfa.h"
using namespace std;

int main(){
    string characters;
    cout << "Enter the characters in your regular expression: ";
    cin >> characters;

    string regex;
    cout << "Type your regular expression: ";
    cin >> regex;

    Preprocessing preprocessed(regex, characters);

    cout << "----RESULTS----\n" << "INPUT:\n" << regex << "\n\n";

    NFA nfa(preprocessed.notation());
    pair<int, int> startFinish = nfa.print();

    DFA dfa(nfa.getConnections(), startFinish.first, startFinish.second, vector<char>(characters.begin(), characters.end()));
    dfa.print();
}