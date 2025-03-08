#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

// Function to load graph from CSV file
unordered_map<string, vector<string>> loadGraph(const string &filename) {
    unordered_map<string, vector<string>> graph;
    ifstream file(filename);
    string line, location, nearby_list;

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, location, ',');
        getline(ss, nearby_list); // Read nearby locations as a single string

        // Remove quotes if present
        if (!nearby_list.empty() && nearby_list.front() == '"' && nearby_list.back() == '"') {
            nearby_list = nearby_list.substr(1, nearby_list.size() - 2);
        }
        
        stringstream ss_nearby(nearby_list);
        string nearby;
        vector<string> edges;
        
        while (getline(ss_nearby, nearby, ',')) {
            // Trim spaces around names
            nearby.erase(0, nearby.find_first_not_of(" "));
            nearby.erase(nearby.find_last_not_of(" ") + 1);
            edges.push_back(nearby);
        }
        
        graph[location] = edges;
    }

    file.close();
    return graph;
}

// Function to print the adjacency list
void printGraph(const unordered_map<string, vector<string>> &graph) {
    for (const auto &pair : graph) {
        cout << pair.first << " -> ";
        for (size_t i = 0; i < pair.second.size(); i++) {
            cout << pair.second[i] << (i < pair.second.size() - 1 ? ", " : "");
        }
        cout << endl;
    }
}

int main() {
    string filename = "pune_police_stations_with_distances.csv";
    unordered_map<string, vector<string>> graph = loadGraph(filename);
    printGraph(graph);
    return 0;
}
