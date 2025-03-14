#include <iostream>
#include "preprocess.h"
using namespace std;

int main(){
    string characters;
    cout << "Enter the characters in your regular expression: ";
    cin >> characters;

    string regex;
    Preprocessing preprocessing = Preprocessing("(a|b)*abb", "ab");
    cout << preprocessing.notation() << "\n";

    preprocessing = Preprocessing("a+b((abc)|a+)*", "abc");
    cout << preprocessing.notation();
}