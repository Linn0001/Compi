#ifndef FOLLOW_H
#define FOLLOW_H

#include "grammar.h"
#include "first.h"
#include <map>
#include <set>
#include <string>

using namespace std;

/**
 * Clase Follow
 * Calcula los conjuntos FOLLOW de una gramática libre de contexto.
 * Estos conjuntos indican qué terminales pueden aparecer inmediatamente
 * después de un no terminal en alguna derivación.
 */
class Follow {
public:
    Grammar& grammar;             // Referencia a la gramática
    First& first;                 // Referencia a los conjuntos FIRST
    map<string, set<string>> followSets; // Almacena los conjuntos FOLLOW de cada no terminal

    // Constructor: recibe la gramática y los FIRST calculados
    Follow(Grammar& g, First& f) : grammar(g), first(f) {}

    // Calcula los conjuntos FOLLOW
    void compute();

    // Imprime los conjuntos FOLLOW en consola
    void print();
};

#endif // FOLLOW_H
