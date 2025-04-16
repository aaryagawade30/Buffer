#include <bits/stdc++.h>
#include <stdlib.h>
#include <cstdlib>
using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string normalize(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c) { return tolower(c); });
    result.erase(result.begin(),
                 find_if(result.begin(), result.end(),
                         [](unsigned char c) { return !isspace(c); }));
    result.erase(find_if(result.rbegin(), result.rend(),
                         [](unsigned char c) { return !isspace(c); }).base(),
                 result.end());
    return result;
}

struct Neighbor {
    string location;
    int crimeIndex;
};

unordered_map<string, vector<Neighbor>> createAdjacencyList(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: File '" << filename << "' could not be opened!" << endl;
        return {};
    }

    unordered_map<string, vector<Neighbor>> adjList;
    string line, location, nearbyData;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, location, ',');
        getline(ss, nearbyData);

        string baseLoc = normalize(location);
        vector<Neighbor> neighbors;

        if (!nearbyData.empty() && nearbyData.front() == '"' && nearbyData.back() == '"') {
            nearbyData = nearbyData.substr(1, nearbyData.size() - 2);
        }

        stringstream nearbyStream(nearbyData);
        string entry;
        while (getline(nearbyStream, entry, ',')) {
            size_t sep = entry.find('|');
            if (sep == string::npos) continue;

            string neighbor = normalize(entry.substr(0, sep));
            int crimeIndex = stoi(entry.substr(sep + 1));
            neighbors.push_back({neighbor, crimeIndex});
        }
        adjList[baseLoc] = neighbors;
    }

    file.close();
    return adjList;
}

vector<string> findSafestRoute(unordered_map<string, vector<Neighbor>>& adjList,
                               const string& startRaw, const string& destinationRaw) {
    string start = normalize(startRaw);
    string destination = normalize(destinationRaw);

    if (adjList.find(start) == adjList.end() || adjList.find(destination) == adjList.end()) {
        cout << "Error: One or both locations not found in the dataset!" << endl;
        return {};
    }

    unordered_map<string, int> crimeScore;
    unordered_map<string, string> parent;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;

    for (auto& loc : adjList) crimeScore[loc.first] = INT_MAX;
    crimeScore[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [currentCrime, current] = pq.top(); pq.pop();
        if (current == destination) break;

        for (auto& neighbor : adjList[current]) {
            int newCrime = currentCrime + neighbor.crimeIndex;
            if (newCrime < crimeScore[neighbor.location]) {
                crimeScore[neighbor.location] = newCrime;
                parent[neighbor.location] = current;
                pq.push({newCrime, neighbor.location});
            }
        }
    }

    if (crimeScore[destination] == INT_MAX) {
        cout << "No safe route found!" << endl;
        return {};
    }

    vector<string> path;
    for (string loc = destination; !loc.empty(); loc = parent[loc]) {
        path.push_back(loc);
        if (loc == start) break;
    }
    reverse(path.begin(), path.end());
    return path;
}

unordered_map<string, vector<string>> create_adjList() {
    ifstream iFile("pune_police_stations.csv");
    if (!iFile) {
        cerr << "Error: File 'pune_police_stations.csv' could not be opened!" << endl;
        return {};
    }

    unordered_map<string, vector<string>> adjacencyList;
    string line, location, nearby;
    getline(iFile, line);

    while (getline(iFile, line)) {
        size_t loc = line.find(',');
        if (loc == string::npos) continue;

        location = line.substr(0, loc);
        nearby = line.substr(loc + 1);
        location.erase(remove(location.begin(), location.end(), '\"'), location.end());
        location = normalize(location);

        if (adjacencyList.find(location) == adjacencyList.end()) {
            adjacencyList[location] = {};
        }

        if (!nearby.empty() && nearby.front() == '"' && nearby.back() == '"') {
            nearby = nearby.substr(1, nearby.size() - 2);
        }

        stringstream ss(nearby);
        string neighbor;
        while (getline(ss, neighbor, ',')) {
            size_t pipePos = neighbor.find('|');
            string cleanNeighbor = (pipePos != string::npos)
                ? normalize(neighbor.substr(0, pipePos))
                : normalize(neighbor);
            if (cleanNeighbor.empty()) continue;

            if (adjacencyList.find(cleanNeighbor) == adjacencyList.end()) {
                adjacencyList[cleanNeighbor] = {};
            }

            adjacencyList[location].push_back(cleanNeighbor);
        }
    }

    iFile.close();
    return adjacencyList;
}

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
        string current = q.front(); q.pop();
        nearby.push_back(current);
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

void displayTopKSafeDangerousPlaces(const string& filename, int k) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: File '" << filename << "' could not be opened!" << endl;
        return;
    }

    unordered_map<string, int> crimeIndexMap;
    string line, location, nearbyData;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, location, ',');
        getline(ss, nearbyData);

        location = normalize(location);

        if (!nearbyData.empty() && nearbyData.front() == '"' && nearbyData.back() == '"') {
            nearbyData = nearbyData.substr(1, nearbyData.size() - 2);
        }

        stringstream nearbyStream(nearbyData);
        string entry;
        int totalCrime = 0, count = 0;
        while (getline(nearbyStream, entry, ',')) {
            size_t sep = entry.find('|');
            if (sep == string::npos) continue;
            int crime = stoi(entry.substr(sep + 1));
            totalCrime += crime;
            count++;
        }

        if (count > 0)
            crimeIndexMap[location] = totalCrime / count;
    }

    file.close();

    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> safestPQ;
    priority_queue<pair<int, string>> dangerousPQ;

    for (const auto& [loc, avgCrime] : crimeIndexMap) {
        safestPQ.push({avgCrime, loc});
        dangerousPQ.push({avgCrime, loc});
    }

    cout << "\nTop " << k << " Safest Places:" << endl;
    for (int i = 0; i < k && !safestPQ.empty(); ++i) {
        auto [crime, loc] = safestPQ.top(); safestPQ.pop();
        cout << i + 1 << ". " << loc << " (Avg Crime Index: " << crime << ")" << endl;
    }

    cout << "\nTop " << k << " Most Dangerous Places:" << endl;
    for (int i = 0; i < k && !dangerousPQ.empty(); ++i) {
        auto [crime, loc] = dangerousPQ.top(); dangerousPQ.pop();
        cout << i + 1 << ". " << loc << " (Avg Crime Index: " << crime << ")" << endl;
    }
}

void runBFSModule() {
    clearScreen();
    cout << "=========== FIND NEAREST POLICE STATIONS ===========" << endl;
    unordered_map<string, vector<string>> adjList = create_adjList();

    string start;
    cout << "Enter your current location: ";
    getline(cin, start);

    int noofnearbys;
    cout << "Enter how many nearest police stations you want: ";
    cin >> noofnearbys;
    cin.ignore();

    vector<string> nearby = bfs(adjList, start);
    if (nearby.empty()) {
        cout << "No nearby police stations found or invalid location." << endl;
    } else {
        int count = min((int)nearby.size(), noofnearbys);
        for (int i = 0; i < count; i++) {
            cout << i + 1 << ". " << nearby[i] << endl;
        }
    }
    cout << "\nPress Enter to return to main menu..."; cin.get();
}

void runDijkstraModule() {
    clearScreen();
    cout << "=============== FIND SAFEST ROUTE ===============" << endl;
    string filename = "pune_cities_crime_index.csv";
    unordered_map<string, vector<Neighbor>> adjList = createAdjacencyList(filename);

    string start, destination;
    cout << "Enter start location: "; getline(cin, start);
    cout << "Enter destination: "; getline(cin, destination);

    vector<string> route = findSafestRoute(adjList, start, destination);
    if (!route.empty()) {
        cout << "\nSafest route based on crime index:\n";
        for (size_t i = 0; i < route.size(); i++) {
            if (i > 0) cout << " -> ";
            cout << route[i];
        }
        cout << endl;
    }

    cout << "\nPress Enter to return to main menu..."; cin.get();
}

void runChatbotModule() {
    clearScreen();
    cout << "========= WOMEN'S SAFETY CHATBOT =========\n";
    system("python chat.py");
    cout << "\nReturning to main menu...\n";
}

void displayMenu() {
    clearScreen();
    cout << "======== WOMEN'S SAFETY APPLICATION ========\n";
    cout << "1. Find Nearest Police Stations\n";
    cout << "2. Find Safest Route\n";
    cout << "3. Chat with Safety Assistant\n";
    cout << "4. Display Top K Safe and Dangerous Places\n";
    cout << "5. Exit\n";
    cout << "Enter your choice (1-5): ";
}

int main() {
    int choice;
    bool running = true;

    while (running) {
        displayMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: runBFSModule(); break;
            case 2: runDijkstraModule(); break;
            case 3: runChatbotModule(); break;
            case 4: {
                int k;
                cout << "Enter value of K: ";
                cin >> k;
                cin.ignore();
                displayTopKSafeDangerousPlaces("pune_cities_crime_index.csv", k);
                cout << "\nPress Enter to return to menu..."; cin.get();
                break;
            }
            case 5:
                cout << "Thank you. Stay safe!" << endl;
                running = false;
                break;
            default:
                cout << "Invalid choice. Press Enter to continue..."; cin.get();
        }
    }

    return 0;
}
