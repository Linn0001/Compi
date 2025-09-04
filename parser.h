#ifndef PARSER_H
#define PARSER_H

#include "table.h"
#include <stack>
#include <vector>
#include <string>

/**
 * Clase Parser
 * Implementa un parser LL(1) que analiza una secuencia de tokens
 * usando la tabla predictiva generada por la clase Table.
 */
class Parser {
public:
    Table& table;      // Referencia a la tabla LL(1) utilizada para parsear
    int startSymbol; // ID del símbolo inicial de la gramática

    // Constructor: recibe la tabla LL(1)
    Parser(Table& t, int inicial) : table(t), startSymbol(inicial) {}

    // parse
    bool parse(vector<string>& tokens);
};

#endif // PARSER_H
