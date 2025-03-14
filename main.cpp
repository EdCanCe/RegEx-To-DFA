#include <iostream>
#include "preprocess.h"
using namespace std;

int main(){
    string characters;
    cout << "Enter the characters in your regular expression: ";
    cin >> characters;

    string regex;
    cout << "Type your regular expression: ";
    cin >> regex;

    Preprocessing preprocessed(regex, characters);

    cout << preprocessed.notation();
}