#include <bits/stdc++.h>
using namespace std;

// Function to create adjacency list from CSV file
unordered_map<string, vector<string>> create_adjList() {
    ifstream iFile("pune_police_stations.csv");
    if (!iFile) {
        cerr << "Error: File could not be opened!" << endl;
        return {}; // Return an empty map
    }

    unordered_map<string, vector<string>> adjacencyList;
    string line, location, nearby;

    // Skip header
    getline(iFile, line);

    // Read each line
    while (getline(iFile, line)) {
        int loc = line.find(',');
        if (loc == string::npos) continue; // Skip malformed lines

        location = line.substr(0, loc);
        nearby = line.substr(loc + 1);

        // Remove extra spaces and quotes
        location.erase(remove(location.begin(), location.end(), '\"'), location.end());
        nearby.erase(remove(nearby.begin(), nearby.end(), '\"'), nearby.end());

        // Ensure all locations are initialized in adjacencyList
        if (adjacencyList.find(location) == adjacencyList.end()) {
            adjacencyList[location] = {};
        }

        // Split nearby locations
        stringstream ss(nearby);
        string neighbor;
        while (getline(ss, neighbor, ',')) {
            //neighbor.erase(remove(neighbor.begin(), neighbor.end()), neighbor.end()); // Trim spaces

            // Ensure neighbor is also added as a key
            if (adjacencyList.find(neighbor) == adjacencyList.end()) {
                adjacencyList[neighbor] = {};
            }

            adjacencyList[location].push_back(neighbor);
        }
    }

    iFile.close();
    return adjacencyList; // Return the adjacency list
}

// Function to perform BFS on the adjacency list
vector<string> bfs(const unordered_map<string, vector<string>>& adjacencyList, const string& start) {
    if (adjacencyList.find(start) == adjacencyList.end()) {
        cout << "Error: Start location not found!" << endl;
        return {};
    }
    vector<string> nearby;
    queue<string> q;
    unordered_set<string> visited;

    q.push(start);
    visited.insert(start);

    cout << "The nearby cities where police stations are located from " << start << " are: ";

    while (!q.empty()) {
        string current = q.front();
        q.pop();
        //cout << current << ",";
        nearby.push_back(current);

        // Check if current node exists in adjacencyList before accessing it
        if (adjacencyList.find(current) != adjacencyList.end()) {
            for (const string& neighbor : adjacencyList.at(current)) {
                if (visited.find(neighbor) == visited.end()) {
                    q.push(neighbor);
                    visited.insert(neighbor);
                }
            }
        }
    }

    //cout << endl;
    return nearby;
}

// Function to print adjacency list
void print_adj_list(const unordered_map<string, vector<string>>& adjacencyList) {
    for (const auto& pair : adjacencyList) {
        cout << pair.first << " -> ";
        for (const string& neighbor : pair.second) {
            cout << neighbor << ", ";
        }
        cout << endl;
    }
}

int main() {
    // Create adjacency list
    unordered_map<string, vector<string>> adjList = create_adjList();

    // Print adjacency list
    //print_adj_list(adjList);

    // Perform BFS starting from "Alandi"
    string start;
    cout << "Enter your current location: ";
    cin >> start;
    int noofnearbys;
    cout << "Enter how many nearest police stations you want to know: ";
    cin >> noofnearbys;
    vector<string> nearby = bfs(adjList, start);
    if(nearby.size() == 0) return 0;
    else if(noofnearbys <= nearby.size()) {
        cout << nearby[0] << ", ";
        for(int i = 1; i < noofnearbys -1; i++) {
            cout << nearby[i] << ",";
        }
        cout << nearby[noofnearbys -1] << endl;
    }
    else {
        cout << "Sorry, the number of police stations nearby " << start << " are " << nearby.size() << endl;
        cout << "They are: " << endl;
        cout << nearby[0] << ", ";
        for(int i = 1; i < noofnearbys -1; i++) {
            cout << nearby[i] << ",";
        }
        cout << nearby[noofnearbys -1] << endl;
    }

    return 0;
}