#ifndef TABLE_H
#define TABLE_H

#include "grammar.h"
#include "first.h"
#include "follow.h"
#include <map>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/**
 * Tipo de símbolo: terminal o no terminal
 */
enum SymbolType { TERMINAL, NONTERMINAL };

/**
 * Estructura Symbol
 * Representa un símbolo de la gramática en la tabla LL(1)
 */
struct Symbol {
    SymbolType type; // Indica si es terminal o no terminal
    int value;       // ID del terminal o no terminal
    Symbol(SymbolType t, int v) : type(t), value(v) {}
};

/**
 * Clase Table
 * Representa la tabla predictiva LL(1) de la gramática.
 * Permite mapear pares (NoTerminal, Terminal) a la producción correspondiente.
 */
class Table {
public:
    // -------------------------
    // Atributos públicos
    // -------------------------
    map<pair<int,int>, vector<Symbol>> parserTable; // Tabla LL(1)
    map<string,int> ntMap;   // Mapeo: NonTerminal -> ID
    vector<string> ntsVec;   // Vector para obtener NonTerminal desde ID
    map<string,int> termMap; // Mapeo: Terminal -> ID
    vector<string> tsVec;    // Vector para obtener Terminal desde ID

    // -------------------------
    // Constructor
    // -------------------------
    Table(const Grammar& g, const First& first, const Follow& follow);

    // -------------------------
    // Métodos públicos
    // -------------------------
    void print(); // Imprime la tabla LL(1) de manera legible

    // Obtiene el ID de un NoTerminal
    int getNonTerminalId(const string& nt) const {
        auto it = ntMap.find(nt);
        return (it != ntMap.end()) ? it->second : -1;
    }

    // Obtiene el ID de un Terminal
    int getTerminalId(const string& t) const {
        auto it = termMap.find(t);
        return (it != termMap.end()) ? it->second : -1;
    }
};

#endif // TABLE_H
