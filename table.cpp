#include "table.h"
#include "utils.h"
#include <iostream>
#include <stack>
#include <iomanip>

using namespace std;

/**
 * print()
 * Imprime la tabla LL(1) de manera formateada.
 * Filas: No terminales, Columnas: terminales
 */
void Table::print() {
    // Encabezado con los terminales
    cout << setw(5) << "NT/T";
    for (size_t j = 0; j < tsVec.size(); ++j) {
        cout << setw(10) << tsVec[j];
    }
    cout << "\n";

    // Separador visual
    cout << string(5 + 10 * tsVec.size(), '-') << "\n";

    // Recorrer no terminales
    for (size_t i = 0; i < ntsVec.size(); ++i) {
        cout << setw(5) << ntsVec[i];

        for (size_t j = 0; j < tsVec.size(); ++j) {
            auto it = parserTable.find({(int)i, (int)j});
            if (it != parserTable.end()) {
                if (it->second.empty()) {
                    cout << setw(10) << "''";
                } else {
                    string rhsStr;
                    for (auto s : it->second) {
                        rhsStr += (s.type == NONTERMINAL ? ntsVec[s.value] : tsVec[s.value]) + " ";
                    }
                    cout << setw(10) << rhsStr;
                }
            }else {
                cout << setw(10) << "-";
            }
        }
        cout << "\n";
    }
}

/**
 * Constructor Table
 * Construye la tabla LL(1) a partir de la gramática, los First y Follow.
 */
Table::Table(const Grammar& g, const First& first, const Follow& follow) {
    // -------------------------
    // Asignar IDs a los no terminales
    // -------------------------
    int id = 0;
    for (auto nt : g.nonTerminals) {
        ntMap[nt] = id++;
        ntsVec.push_back(nt);
    }

    // -------------------------
    // Asignar IDs a los terminales
    // -------------------------
    id = 0;
    for (auto t : g.terminals) {
        termMap[t] = id++;
        tsVec.push_back(t);
    }

    // -------------------------
    // Construcción de la tabla LL(1)
    // -------------------------
    for (auto ruleStr : g.rules) {
        size_t pos = ruleStr.find("->");
        if (pos == string::npos) continue;

        string lhs = trim(ruleStr.substr(0, pos));       // LHS
        string rhs = trim(ruleStr.substr(pos + 2));      // RHS

        vector<string> alternatives = split(rhs, '|');

        for (auto alt : alternatives) {
            alt = trim(alt);
            // Manejo de epsilon
            if (alt.empty() || alt == "''" || alt == "ε") {
                for (auto term : follow.followSets.at(lhs)) {
                    int lhsId = getNonTerminalId(lhs);
                    int termId = getTerminalId(term == "''" ? "$" : term);
                    parserTable[{lhsId, termId}] = {}; // epsilon
                }
                continue;
            }

            // -------------------------
            // Calcular First(α)
            // -------------------------
            set<string> firstAlpha;
            bool canBeEpsilon = true;
            vector<string> symbols = split(alt, ' ');

            for (auto s : symbols) {
                s = trim(s);
                set<string> firstS;
                if (g.nonTerminals.count(s)) firstS = first.firstSets.at(s);
                else firstS.insert(s);

                for (auto f : firstS)
                    if (f != "''") firstAlpha.insert(f);

                if (firstS.count("''") == 0) { canBeEpsilon = false; break; }
            }

            // -------------------------
            // Insertar producción en la tabla
            // -------------------------
            int lhsId = getNonTerminalId(lhs);
            for (auto term : firstAlpha) {
                int tid = getTerminalId(term);
                parserTable[{lhsId, tid}] = {};
                for (auto sym : symbols) {
                    if (ntMap.count(sym)) parserTable[{lhsId, tid}].push_back(Symbol(NONTERMINAL, ntMap[sym]));
                    else if (termMap.count(sym)) parserTable[{lhsId, tid}].push_back(Symbol(TERMINAL, termMap[sym]));
                }
            }

            // Si α puede derivar ε, agregar Follow(LHS)
            if (canBeEpsilon) {
                for (auto term : follow.followSets.at(lhs)) {
                    int tid = getTerminalId(term == "''" ? "$" : term);
                    parserTable[{lhsId, tid}] = {}; // epsilon
                }
            }
        }
    }
}
