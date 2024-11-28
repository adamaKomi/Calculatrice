#pragma once
#include <iostream>
#include <stdexcept>
#include <cctype> // pour isdigit()
#include <string>


template<class MyType>
class Stack {
private:
    MyType* values;// les elements de la pile
    int sommet; // le sommet de la pile
    int capacity; // la capacite max

public:
    Stack();
    MyType Pop(); //pour depiler
    MyType Top() const; // pour connaitre l'element au sommet de la pile

    double evaluateExpression(const std::string& exp);  // pour evaluer une expression
    double applyOperation(double a, double b, char op) const; // pour effectuer des operations binaires

    int priorite(char op) const;// pour trouver la priorité entre les opterateur

    bool isEmpty() const; //verifier si la pile est vide
    bool isFull() const; //verifier si la pile est pleine
    bool validerExpresion(std::string exp); // verifier si l'expression esty valide
    bool isOperator(const char op) const;  // verifier si c'est un operateur
    bool isParenthese(const MyType op) const; //verifier si c'est une parenthese
    bool isValideCaract(const MyType car) const; //verifier si le caractere est valide

    void Push(MyType object); // pour empliler
    void resize(); // augmenter la capacité de la pile
    void PrintStack(); // afficher la pile
};

// Constructeur
template<class MyType>
Stack<MyType>::Stack() : sommet(-1), capacity(10) {
    values = new MyType[capacity];
}


// Retirer un élément de la pile
template<class MyType>
MyType Stack<MyType>::Pop() {
    if (isEmpty()) {
        throw std::underflow_error("Pop : Pile vide !");
    }
    return values[sommet--];
}


// Obtenir l'élément au sommet de la pile
template<class MyType>
MyType Stack<MyType>::Top() const {
    if (isEmpty()) {
        throw std::underflow_error("Top : Pile vide !");
    }
    return values[sommet];
}



// Évaluer une expression arithmétique
template<class MyType>
double Stack<MyType>::evaluateExpression(const std::string& exp) {
    Stack<char> operators;  // Pile pour les opérateurs
    Stack<double> operands; // Pile pour les opérandes


    // verifer la validité de l'expression avant l'evaluation
    this->validerExpresion(exp);


    for (size_t i = 0; i < exp.length(); ++i) {
        char ch = exp[i];

        // Ignorer les espaces
        if (isspace(ch)) continue;

        // Si c'est un chiffre, l'ajouter dans la pile des opérandes
        if (isdigit(ch)) {
            double number = 0;

            // Gérer les nombres a plusieurs chifffres
            while (i < exp.length() && isdigit(exp[i])) {
                number = number * 10 + (exp[i] - '0');
                ++i;
            }
            --i; // Réajuster l'indice
            operands.Push(number);
        }
        // Si c'est une parenthèse ouvrante, l'ajouter dans la pile des opérateurs
        else if (ch == '(') {
            operators.Push(ch);
        }
        // Si c'est une parenthèse fermante
        else if (ch == ')') {
            // Appliquer toutes les opérations jusqu'à rencontrer une parenthèse ouvrante
            while (!operators.isEmpty() && operators.Top() != '(') {
                double b = operands.Pop(); // 2e operande
                double a = operands.Pop(); // 1e operande
                char op = operators.Pop(); // operateur
                try
                {
                    operands.Push(applyOperation(a, b, op));
                }
                catch (const std::invalid_argument& e)
                {
                    std::cout << "Erreur : " << e.what() << std::endl;
                }
            }
            operators.Pop(); // Retirer la parenthèse ouvrante
        }
        // Si c'est un opérateur
        else if (isOperator(ch)) {
            // Gérer la priorité des opérateurs
            while (!operators.isEmpty() && priorite(operators.Top()) >= priorite(ch)) {
                double b = operands.Pop();
                double a = operands.Pop();
                char op = operators.Pop();
                try
                {
                    operands.Push(applyOperation(a, b, op));
                }
                catch (const std::invalid_argument& e)
                {
                    std::cout << "Erreur : " << e.what() << std::endl;
                }
            }
            operators.Push(ch);
        }
        else {
            throw std::invalid_argument("Caractère invalide dans l'expression !");
        }
    }

    // Appliquer les opérations restantes
    while (!operators.isEmpty()) {
        double b = operands.Pop();
        double a = operands.Pop();
        char op = operators.Pop();
        try
        {
            operands.Push(applyOperation(a, b, op));
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << "Erreur : " << e.what() << std::endl;
        }
    }

    return operands.Pop();
}


// Appliquer une opération
template<class MyType>
inline double Stack<MyType>::applyOperation(double a, double b, char op) const
{
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) throw std::invalid_argument("Division par zéro !");
        return a / b;
    default: throw std::invalid_argument("Opérateur invalide !");
    }
}

template<class MyType>
inline int Stack<MyType>::priorite(char op) const
{
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0; // Pour les parenthèses
}


// Vérifier si la pile est vide
template<class MyType>
bool Stack<MyType>::isEmpty() const {
    return this->sommet == -1;
}


// Vérifier si la pile est pleine
template<class MyType>
bool Stack<MyType>::isFull() const {
    return sommet == (capacity - 1);
}


template <class MyType>
inline bool Stack<MyType>::validerExpresion(std::string exp)
{

    int parenthese = 0;
    MyType var1, var2, top;
    size_t lg = exp.length();
    // Créer une pile temporaire car la pile courante sera utilisee dans la calculatrice
    Stack<MyType> tmpStack;


    // Ajouter chaque caractère de la chaîne dans la pile
    for (int i = lg - 1; i >= 0; i--)
    {
        if (isspace(exp[i])) continue;//ignorer les espaces
        this->Push(exp[i]);
        tmpStack.Push(exp[i]);
    }

    if (tmpStack.isEmpty())// verifier si la pile est non vide
    {
        throw std::invalid_argument("Entrez une expression non vide!!");
    }

    //afficher l'expression et sa longeur
    std::cout << "********************************************************" << std::endl;
    std::cout << "Longueur : " << tmpStack.sommet + 1 << std::endl;
    tmpStack.PrintStack();
    std::cout << std::endl;



    //verifer s'il y a un operateur au debut
    if (isOperator(tmpStack.Top()))
    {
        top = tmpStack.Pop();
        if (top == '*' || top == '/')
            throw std::invalid_argument("Invalide : irregularité au debut de l'expression");
        //top = tmpStack.Pop();// le suivant
    }

    // Vérifier les parenthèses
    while (!tmpStack.isEmpty())
    {
        var1 = tmpStack.Pop();

        if (!isValideCaract(var1))
        {
            //throw std::invalid_argument("Caractère invalide : " + std::to_string(var1));
            throw std::invalid_argument("Caractère invalide : " + std::string(1, var1));
        }
        // Si les parenthèses sont déséquilibrées
        else if (parenthese < 0)
        {
            throw std::invalid_argument("Invalide : problème de parenthèses");
        }
        // traiter tout ce qui est autour de la parenthese ouvrante
        if (var1 == '(')
        {
            var2 = tmpStack.Pop();
            //caractere invalide
            if (!isValideCaract(var2))
            {
                throw std::invalid_argument("Caractère invalide : " + std::string(1, var2));
            }
            // Ex : ()   (/   (*
            else if (var2 == ')' || var2 == '/' || var2 == '*')
            {
                throw std::invalid_argument("Invalide : problème de parenthèses : " + std::string(1, var1) + std::string(1, var2));

            }
            else
            {
                tmpStack.Push(var2);
            }
            parenthese++;
        }
        // traiter tout ce qui est autour de la parenthese fermante
        else if (var1 == ')')
        {
            parenthese--;
        }
        // traiter tout ce qui est autour des operateurs
        else if (isOperator(var1))
        {
            // le cas d'un operateur suivie par le vide; Ex: x+y+
            if (tmpStack.isEmpty())
            {
                throw std::invalid_argument("Invalide : operande manquante après \"" + std::string(1, var1) + "\"");
            }

            var2 = tmpStack.Pop();

            //caractere invalide
            if (!isValideCaract(var2))
            {
                throw std::invalid_argument("Invalide : caractère invalide \"" + std::string(1, var2) + "\"");
            }
            // le cas d'un operateur suivie d'une parenthese fermante; Ex: +)
            else if (var2 == ')')
            {
                throw std::invalid_argument("Invalide : operande manquante après \"" + std::string(1, var1) + "\" dans les parenthèses");
            }
            // Ex : +* ou +/ ou -/
            else if (isOperator(var2))
            {
                throw std::invalid_argument("Invalide Invalide : succession invalide d'operateur : " + std::string(1, var1) + std::string(1, var2));
            }
            else {
                tmpStack.Push(var2);
            }
        }
    }


    // Vérification finale des parenthèses ouvertes
    if (parenthese != 0)
    {
        throw std::invalid_argument("Invalide :  problème de parenthèses");
    }

    std::cout << "Expression valide !\n" << std::endl;
    return true;


}


// Vérifier si un caractère est un opérateur
template<class MyType>
bool Stack<MyType>::isOperator(const char op) const {
    return op == '+' || op == '-' || op == '*' || op == '/';
}


template<class MyType>
inline bool Stack<MyType>::isParenthese(const MyType op) const
{
    return op == '(' || op == ')';
}



template<class MyType>
inline bool Stack<MyType>::isValideCaract(const MyType car) const
{
    return std::isdigit(car) || isOperator(car) || isParenthese(car);
}


// Ajouter un élément dans la pile
template<class MyType>
void Stack<MyType>::Push(MyType object) {
    if (isFull()) {
        resize();
    }
    values[++sommet] = object;
}


// Redimensionner la pile
template<class MyType>
void Stack<MyType>::resize() {
    capacity *= 2;
    MyType* newValues = new MyType[capacity];
    for (int i = 0; i <= sommet; i++) {
        newValues[i] = values[i];
    }
    delete[] values;
    values = newValues;
}


// Afficher la pile (pour le débogage)
template<class MyType>
inline void Stack<MyType>::PrintStack()
{
    if (this->isEmpty()) {
        std::cout << "La pile est vide" << std::endl;
        return;
    }
    for (int i = this->sommet; i >= 0; i--)
    {
        std::cout << this->values[i] << " ";
    }

    std::cout << std::endl;

}