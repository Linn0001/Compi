#include "parser.h"
#include <iostream>
#include <iomanip>

using namespace std;

/**
 * parse
 * Realiza el análisis sintáctico LL(1) de la secuencia de tokens usando la tabla predictiva.
 * @param tokens: vector de terminales a analizar
 * @return true si la secuencia es aceptada por la gramática, false en caso contrario
 */
bool Parser::parse(vector<string>& tokens) {
    // Convertir tokens a IDs de terminales
    vector<int> input;
    for (auto& tok : tokens) {
        int tid = table.getTerminalId(tok);
        if (tid < 0) {
            cerr << "[Parser] Token desconocido: " << tok << endl;
            return false;
        }
        input.push_back(tid);
    }

    // Asegurar que la entrada termine con símbolo de fin $
    int dollarId = table.getTerminalId("$");
    if (input.empty() || input.back() != dollarId)
        input.push_back(dollarId);

    // Inicializar pila con $ y el símbolo inicial
    stack<Symbol> st;
    st.push(Symbol(TERMINAL, dollarId));
    st.push(Symbol(NONTERMINAL, startSymbol));

    size_t ip = 0; // índice de lectura de tokens

    // Ancho para imprimir tabla de derivación
    const int widthPila = 25;
    const int widthEntrada = 25;
    const int widthRegla = 30;

    // Encabezado de la tabla de derivación
    cout << "\n=== Tabla de derivación ===\n";
    cout << left << setw(widthPila) << "Pila"
         << setw(widthEntrada) << "Entrada"
         << setw(widthRegla) << "Regla aplicada" << "\n";
    cout << string(widthPila + widthEntrada + widthRegla, '-') << "\n";

    // Ciclo principal del parseo
    while (!st.empty()) {
        Symbol top = st.top();        // símbolo en la cima de la pila
        int lookahead = input[ip];    // siguiente token de entrada

        // Convertir pila a string para impresión
        stack<Symbol> tmpSt = st;
        vector<string> tmpVec;
        while (!tmpSt.empty()) {
            Symbol s = tmpSt.top(); tmpSt.pop();
            tmpVec.push_back(s.type==NONTERMINAL ? table.ntsVec[s.value] : table.tsVec[s.value]);
        }
        string pilaStr;
        for (auto it = tmpVec.rbegin(); it != tmpVec.rend(); ++it)
            pilaStr += *it + " ";

        // Construir string de la entrada restante
        string entradaStr;
        for (size_t j = ip; j < input.size(); ++j)
            entradaStr += table.tsVec[input[j]] + " ";

        // Caso: símbolo terminal en la cima
        if (top.type == TERMINAL) {
            if (top.value == lookahead) {
                // Coincide: hacer "match"
                cout << left << setw(widthPila) << pilaStr
                     << setw(widthEntrada) << entradaStr
                     << setw(widthRegla) << ("Match: " + table.tsVec[top.value]) << "\n";
                st.pop();
                ip++;
            } else {
                // Error de coincidencia
                cerr << "[Parser] Error: esperaba '"
                     << table.tsVec[top.value] << "' pero llegó '"
                     << table.tsVec[lookahead] << "'\n";
                return false;
            }
        } 
        // Caso: símbolo no terminal en la cima
        else {
            auto it = table.parserTable.find({top.value, lookahead});
            if (it == table.parserTable.end()) {
                cerr << "[Parser] Error: no hay regla para "
                     << table.ntsVec[top.value] << " con lookahead="
                     << table.tsVec[lookahead] << "\n";
                return false;
            }

            st.pop();
            const auto& rhs = it->second; // RHS de la regla

            // Convertir RHS a string para impresión
            string rhsStr;
            if (rhs.empty()) rhsStr = "ε"; // epsilon
            else {
                for (auto s : rhs)
                    rhsStr += (s.type==NONTERMINAL ? table.ntsVec[s.value] : table.tsVec[s.value]) + " ";
            }

            cout << left << setw(widthPila) << pilaStr
                 << setw(widthEntrada) << entradaStr
                 << setw(widthRegla) << (table.ntsVec[top.value] + " -> " + rhsStr) << "\n";

            // Apilar RHS en orden inverso
            for (auto rit = rhs.rbegin(); rit != rhs.rend(); ++rit)
                st.push(*rit);
        }
    }

    // Verificar si toda la entrada fue consumida
    if (ip == input.size()) {
        cout << "\n[Parser] Análisis exitoso \n";
        return true;
    } else {
        cerr << "[Parser] Error sintactico \n";
        return false;
    }
}
