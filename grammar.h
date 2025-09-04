#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <map>
#include <iostream>
#include "utils.h" // para funciones split() y trim()

using namespace std;

/**
 * Clase Grammar
 * Representa una gramática libre de contexto.
 * Permite cargar reglas desde archivo, imprimirlas
 * y generar la tabla LL(1) con ayuda de First y Follow.
 */

class Grammar {
public:
    set<string> terminals;        // Conjunto de terminales
    set<string> nonTerminals;     // Conjunto de no terminales
    string initialState;           // Símbolo inicial
    vector<string> rules;          // Reglas de producción en crudo (como strings)

    Grammar() {}

    // Cargar gramática desde archivo de texto
    bool loadFromFile(const string& filename);

    // Imprimir gramática cargada
    void print();

};

#endif // GRAMMAR_H
