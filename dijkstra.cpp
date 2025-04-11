#include <bits/stdc++.h>
using namespace std;

struct Neighbor {
    string location;
    int crimeIndex;
};

string normalize(const string& s) {
    string out;
    for (char c : s) out += tolower(c);
    out.erase(out.begin(), find_if(out.begin(), out.end(), [](int ch) { return !isspace(ch); }));
    out.erase(find_if(out.rbegin(), out.rend(), [](int ch) { return !isspace(ch); }).base(), out.end());
    return out;
}

unordered_map<string, vector<Neighbor>> createAdjacencyList(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: File could not be opened!" << endl;
        exit(1);
    }

    unordered_map<string, vector<Neighbor>> adjList;
    string line, location, nearbyData;

    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, location, ',');
        getline(ss, nearbyData);

        string baseLoc = normalize(location);
        vector<Neighbor> neighbors;

        // Remove surrounding quotes if any
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

vector<string> findSafestRoute(
    unordered_map<string, vector<Neighbor>>& adjList,
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
        int currentCrime = pq.top().first;
        string current = pq.top().second;
        pq.pop();

        if (current == destination) break;

        for (auto& neighbor : adjList[current]) {
            int newCrime = currentCrime + neighbor.crimeIndex;
            if (newCrime < crimeScore[neighbor.location]) {
                crimeScore[neighbor.location] = newCrime;
                pq.push({newCrime, neighbor.location});
                parent[neighbor.location] = current;
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

int main() {
    string filename = "pune_cities_crime_index.csv";
    unordered_map<string, vector<Neighbor>> adjList = createAdjacencyList(filename);

    cout << "Enter start location: ";
    string start;
    getline(cin, start);

    cout << "Enter destination: ";
    string destination;
    getline(cin, destination);

    vector<string> route = findSafestRoute(adjList, start, destination);

    if (!route.empty()) {
        cout << "Safest route based on crime index: ";
        for (const auto& loc : route) cout << loc << " -> ";
        cout << "END" << endl;
    }

    return 0;
}