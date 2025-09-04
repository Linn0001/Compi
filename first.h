#ifndef FIRST_H
#define FIRST_H

#include "grammar.h"
#include <map>
#include <set>
#include <string>

using namespace std;

/**
 * Clase First
 * Calcula los conjuntos FIRST de una gramática libre de contexto.
 * Estos conjuntos indican los posibles terminales con los que puede comenzar
 * una derivación de cada no terminal.
 */
class First {
public:
    Grammar& grammar;                         // Referencia a la gramática
    map<string, set<string>> firstSets;       // Almacena los conjuntos FIRST de cada no terminal

    // Constructor: recibe la gramática
    First(Grammar& g) : grammar(g) {}

    // Calcula los conjuntos FIRST
    void compute();

    // Imprime los conjuntos FIRST en consola
    void print();
};

#endif // FIRST_H
