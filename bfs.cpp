#include <bits/stdc++.h>
using namespace std;

// Function to normalize strings (trim whitespace and convert to lowercase)
string normalize(const string& s) {
    string result = s;
    
    // Convert to lowercase
    transform(result.begin(), result.end(), result.begin(), 
              [](unsigned char c) { return tolower(c); });
    
    // Trim leading whitespace
    result.erase(result.begin(), 
                 find_if(result.begin(), result.end(), 
                         [](unsigned char c) { return !isspace(c); }));
    
    // Trim trailing whitespace
    result.erase(find_if(result.rbegin(), result.rend(), 
                         [](unsigned char c) { return !isspace(c); }).base(), 
                 result.end());
    
    return result;
}

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
        size_t loc = line.find(',');
        if (loc == string::npos) continue; // Skip malformed lines

        location = line.substr(0, loc);
        nearby = line.substr(loc + 1);

        // Remove quotes and normalize location name
        location.erase(remove(location.begin(), location.end(), '\"'), location.end());
        location = normalize(location);

        // Ensure all locations are initialized in adjacencyList
        if (adjacencyList.find(location) == adjacencyList.end()) {
            adjacencyList[location] = {};
        }

        // Remove surrounding quotes if present
        if (!nearby.empty() && nearby.front() == '"' && nearby.back() == '"') {
            nearby = nearby.substr(1, nearby.size() - 2);
        }

        // Split nearby locations
        stringstream ss(nearby);
        string neighbor;
        while (getline(ss, neighbor, ',')) {
            // Find the index of the pipe character if present
            size_t pipePos = neighbor.find('|');
            string cleanNeighbor;
            
            if (pipePos != string::npos) {
                // Extract just the location name before the pipe
                cleanNeighbor = normalize(neighbor.substr(0, pipePos));
            } else {
                cleanNeighbor = normalize(neighbor);
            }

            // Skip empty neighbors
            if (cleanNeighbor.empty()) continue;

            // Ensure neighbor is also added as a key
            if (adjacencyList.find(cleanNeighbor) == adjacencyList.end()) {
                adjacencyList[cleanNeighbor] = {};
            }

            adjacencyList[location].push_back(cleanNeighbor);
        }
    }

    iFile.close();
    return adjacencyList;
}

// Function to perform BFS on the adjacency list
vector<string> bfs(const unordered_map<string, vector<string>>& adjacencyList, const string& startRaw) {
    string start = normalize(startRaw);
    
    if (adjacencyList.find(start) == adjacencyList.end()) {
        cout << "Error: Start location \"" << startRaw << "\" not found!" << endl;
        return {};
    }
    
    vector<string> nearby;
    queue<string> q;
    unordered_set<string> visited;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        string current = q.front();
        q.pop();
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

    return nearby;
}

// Function to print adjacency list (for debugging)
void print_adj_list(const unordered_map<string, vector<string>>& adjacencyList) {
    cout << "Adjacency List:" << endl;
    cout << "----------------" << endl;
    for (const auto& pair : adjacencyList) {
        cout << "\"" << pair.first << "\" -> ";
        for (size_t i = 0; i < pair.second.size(); i++) {
            cout << "\"" << pair.second[i] << "\"";
            if (i < pair.second.size() - 1) cout << ", ";
        }
        cout << endl;
    }
    cout << "----------------" << endl;
}

int main() {
    // Create adjacency list
    unordered_map<string, vector<string>> adjList = create_adjList();

    // Uncomment to debug the adjacency list
    // print_adj_list(adjList);
    
    string start;
    cout << "Enter your current location: ";
    // Using the same input method as in the Dijkstra implementation
    getline(cin, start);
    
    int noofnearbys;
    cout << "Enter how many nearest police stations you want to know: ";
    // For numeric input, we still use cin >>
    cin >> noofnearbys;
    
    vector<string> nearby = bfs(adjList, start);
    
    if (nearby.empty()) {
        return 0;
    } 
    else {
        cout << "The nearby cities where police stations are located from " << start << " are: ";
        
        int count = min((int)nearby.size(), noofnearbys);
        
        for (int i = 0; i < count; i++) {
            cout << nearby[i];
            if (i < count - 1) cout << ", ";
        }
        cout << endl;
        
        if (noofnearbys > nearby.size()) {
            cout << "Note: Only " << nearby.size() << " police stations were found." << endl;
        }
    }

    return 0;
}