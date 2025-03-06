#include <bits/stdc++.h>

using namespace std;

// Graph representation (Adjacency List)
class Graph {
public:
    unordered_map<int, vector<pair<int, int>>> adj; // Node -> [(Neighbor, Weight)]

    void addEdge(int u, int v, int weight) {
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight}); // Bidirectional
    }

    // BFS to find the nearest police station
    int findNearestPoliceStation(int start, vector<int>& policeStations) {
        queue<int> q;
        unordered_map<int, bool> visited;
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            // If node is a police station, return it
            if (find(policeStations.begin(), policeStations.end(), node) != policeStations.end()) {
                return node;
            }

            for (auto& neighbor : adj[node]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
        return -1; // No police station found
    }

    // Dijkstra’s Algorithm for safest & shortest path
    vector<int> dijkstra(int start, int end, unordered_map<int, int>& crimeScore) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        unordered_map<int, int> dist;
        unordered_map<int, int> parent;
        vector<int> path;

        // Initialize distances
        for (auto& node : adj) {
            dist[node.first] = INT_MAX;
        }
        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            int currDist = pq.top().first;
            int node = pq.top().second;
            pq.pop();

            if (node == end) break; // Destination reached

            for (auto& neighbor : adj[node]) {
                int nextNode = neighbor.first;
                int edgeWeight = neighbor.second + crimeScore[nextNode]; // Adjusting with crime score

                if (currDist + edgeWeight < dist[nextNode]) {
                    dist[nextNode] = currDist + edgeWeight;
                    pq.push({dist[nextNode], nextNode});
                    parent[nextNode] = node;
                }
            }
        }

        // Reconstruct path
        int temp = end;
        while (temp != start) {
            path.push_back(temp);
            temp = parent[temp];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());

        return path;
    }
};

int main() {
    Graph cityGraph;

    // Adding roads (Node1, Node2, Travel Time in minutes)
    cityGraph.addEdge(1, 2, 5);
    cityGraph.addEdge(1, 3, 2);
    cityGraph.addEdge(2, 4, 3);
    cityGraph.addEdge(3, 4, 6);
    cityGraph.addEdge(4, 5, 2);
    
    vector<int> policeStations = {5}; // Police station at node 5
    unordered_map<int, int> crimeScore = {{1, 1}, {2, 5}, {3, 2}, {4, 7}, {5, 0}}; // Higher value = unsafe

    int userLocation = 1;
    int destination = 5;

    // Find nearest police station
    int nearestStation = cityGraph.findNearestPoliceStation(userLocation, policeStations);
    cout << "Nearest Police Station: " << nearestStation << endl;

    // Find safest & shortest path
    vector<int> safestPath = cityGraph.dijkstra(userLocation, destination, crimeScore);

    cout << "Safest Route: ";
    for (int node : safestPath) {
        cout << node << " ";
    }
    cout << endl;

    return 0;
}
