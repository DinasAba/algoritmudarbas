#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// Paieškos su grįžimu algoritmas
bool isSafe(int node, vector<int>& colors, vector<vector<int>>& graph, int color) { // apibrėžiama funkcija isSafe, kuri patikrina, ar galima nudažyti nurodytą viršūnę (node) nurodyta spalva (color). Funkcija priima viršūnės numerį, spalvų vektorių, grafą ir spalvą
    for (int k = 0; k < graph.size(); k++) { // pradedamas ciklas, einantis per visas viršūnes
        if (graph[node][k] && color == colors[k]) //  tikrinama, ar tarp viršūnės node ir kitos viršūnės k yra briauna, ir ar viršūnė k jau nudažyta ta pačia spalva
            return false; //  jei rasta tokia pati spalva, grąžinama false, nes negalima nudažyti viršūnės
    }
    return true; // jei spalva galima, grąžinama true
}

bool solve(int node, vector<vector<int>>& graph, vector<int>& colors, int m, int V) { // apibrėžiama rekursinė funkcija solve, kuri bando nudažyti grafą
    if (node == V) // tikrinama, ar visos viršūnės sėkmingai nudažytos
        return true; // jei taip, grąžinama true

    for (int i = 1; i <= m; i++) { // pradedamas ciklas per visas galimas spalvas
        if (isSafe(node, colors, graph, i)) { //  tikrinama, ar galima nudažyti dabartinę viršūnę spalva i
            colors[node] = i; // jei taip, viršūnė nudažoma spalva i
            if (solve(node + 1, graph, colors, m, V)) // rekursiškai bandoma nudažyti kitas viršūnes
                return true; //  jei pavyksta, grąžinama true
            colors[node] = 0; // jei neįmanoma nudažyti, spalva atstatoma į pradinę
        }
    }
    return false; // jei negalima nudažyti, grąžinama false
}

bool graphColoring(vector<vector<int>>& graph, int m, int V) { // apibrėžiama funkcija graphColoring, kuri yra pagrindinė funkcija grafo dažymui
    vector<int> colors(V, 0); // sukuriamas spalvų vektorius, pradinės spalvos yra 0 (nenudažytos)
    if (solve(0, graph, colors, m, V) == false) { // pradedamas bandymas nudažyti grafą nuo pirmos viršūnės
        return false; //  jei neįmanoma nudažyti, grąžinama false
    }

    for (int i = 0; i < V; i++)
        cout << "Virsune " << i + 1 << " ---> Spalva " << colors[i] << endl;
    return true; //  jei pavyko nudažyti, grąžinama true
}

// Godus algoritmas

void greedyColoring(const vector<vector<int>> &graph, int i1) {
    int V = graph.size(); //  į kintamąjį V išsaugomas grafo viršūnių skaičius
    vector<int> result(V, -1); // sukuriamas vektorius result, kuriame bus saugomi kiekvienos viršūnės spalvos, pradinė reikšmė kiekvienai viršūnei yra -1, reiškianti, kad spalva dar nepriskirta
    result[0] = 0; //  pirmajai viršūnei priskiriama spalva 0

    for (int u = 1; u < V; u++) { // pradedamas ciklas, kuris eina per visas viršūnes, pradedant nuo antrosios
        vector<bool> available(V + 1, true); // sukuriamas vektorius available, kuris saugo informaciją, kurios spalvos yra galimos kiekvienai viršūnei
        for (int i = 0; i < V; i++) { //  pradedamas vidinis ciklas per visas viršūnes
            if (graph[u][i] && result[i] != -1) // tikrinama, ar yra briauna tarp viršūnės u ir viršūnės i, ir ar viršūnei i jau priskirta spalva
                available[result[i]] = false; // jei sąlyga patenkinama, atitinkama spalva pažymima kaip nepasiekiama viršūnei u
        }

        int cr; // sukuriamas kintamasis cr spalvos indeksui saugoti.
        for (cr = 1; cr <= V; cr++) { //  pradedamas ciklas ieškant pirmos galimos spalvos.
            if (available[cr]) //  jei spalva cr yra galima, nutraukiama paieška.
                break;
        }

        result[u] = cr; // viršūnei u priskiriama spalva cr.
    }
    for (int u = 0; u < V; u++)
        cout << "Virsune " << u + 1 << " ---> Spalva " << result[u] + 1 << endl;
}



void generateRandomGraph(vector<vector<int>>& graph, int V) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);

    graph.resize(V, vector<int>(V, 0));
    for (int i = 0; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) {
            graph[i][j] = dis(gen);
            graph[j][i] = graph[i][j];
        }
    }
}



int main() {
    int V;
    cout << "Iveskite virsuniu skaiciu (V): ";
    cin >> V;

    vector<vector<int>> graph(V, vector<int>(V));
    cout << "Iveskite grafa eilutemis. Naudokite 1, jei virsune sujungta ir 0, jei ne:" << endl;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            cin >> graph[i][j];
        }
    }

    int choice, m;
    cout << "Pasirinkite algoritma: 1 - Paieska su grizimu, 2 - Godusis algoritmas: ";
    cin >> choice;

    // Pradėkite laiko matavimą
    auto start = high_resolution_clock::now();

    if (choice == 1) {
        cout << "Iveskite maksimalu spalvu skaiciu: ";
        cin >> m;
        if (!graphColoring(graph, m, V))
            cout << "Neimanoma nuspalvinti naudojant " << m << " spalvas." << endl;
    } else if (choice == 2) {
        greedyColoring(graph, V);
    } else {
        cout << "Neteisingas pasirinkimas." << endl;
    }

    // Baigti laiko matavimą
    auto stop = high_resolution_clock::now();

    // Apskaičiuoti vykdymo trukmę
    auto duration = duration_cast<milliseconds>(stop - start);

    // Išvesti vykdymo trukmę
    cout << "Algoritmo vykdymo trukme: " << duration.count() << " millisekundes" << endl;

    cout << "Paspauskite bet kuri klavisa testi..." << endl;
    cin.ignore();
    cin.get();

    return 0;
}


/*
int main() {
    cout << fixed << setprecision(6);

    for (int V = 100; V <= 30000; V += 100) {
        vector<vector<int>> graph;
        generateRandomGraph(graph, V);

        // Matuojame greedyColoring vykdymo laiką
        auto start = high_resolution_clock::now();
        greedyColoring(graph, V);
        auto stop = high_resolution_clock::now();
        auto durationGreedy = duration_cast<microseconds>(stop - start).count() / 1000000.0;

        // Matuojame graphColoring vykdymo laiką
        start = high_resolution_clock::now();
        graphColoring(graph, 5000000, V);
        stop = high_resolution_clock::now();
        auto durationGraph = duration_cast<microseconds>(stop - start).count() / 1000000.0;

        cout << "Grafo dydis: " << V
             << ", Vykdymo laikas (greedyColoring): " << durationGreedy << " sekundės"
             << ", Vykdymo laikas (graphColoring): " << durationGraph << " sekundės" << endl;
    }

    return 0;
}
 */