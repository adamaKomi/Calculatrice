// Calculatrice.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include "Stack.h"

int main() {
    Stack<char> calc;

    try {
        std::string expression = "(4/2)+3 + 5 * (2 - 8)";
        double result = calc.evaluateExpression(expression);

        std::cout << "Résultat de \"" << expression << "\" : " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
    }

    return 0;
}
