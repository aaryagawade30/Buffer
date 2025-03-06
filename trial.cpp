#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cmath>

#define EARTH_RADIUS 6371.0  // Earth's radius in kilometers

struct Edge {
    int crime_index;  // Crime Index of the neighboring city
    double distance;  // Distance between the two cities
};

struct City {
    std::string name;
    int crime_index;
    double latitude, longitude;
    std::vector<std::string> neighbors;
};

std::unordered_map<std::string, City> cityMap;
std::unordered_map<std::string, std::vector<std::pair<std::string, Edge>>> adjacencyList;

// Haversine Formula to calculate great-circle distance between two lat/long points
double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * M_PI / 180.0;
    double dLon = (lon2 - lon1) * M_PI / 180.0;

    lat1 = lat1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;

    double a = pow(sin(dLat / 2), 2) + pow(sin(dLon / 2), 2) * cos(lat1) * cos(lat2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS * c;
}

// Load city data from CSV
void loadCitiesFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        std::stringstream ss(line);
        City city;
        std::string crime_index_str, lat_str, lon_str, neighbors_str;

        getline(ss, city.name, ',');
        getline(ss, crime_index_str, ',');
        getline(ss, lat_str, ',');
        getline(ss, lon_str, ',');
        getline(ss, neighbors_str);

        city.crime_index = std::stoi(crime_index_str);
        city.latitude = std::stod(lat_str);
        city.longitude = std::stod(lon_str);

        std::stringstream neighbors_ss(neighbors_str);
        std::string neighbor;
        while (getline(neighbors_ss, neighbor, ';')) {
            city.neighbors.push_back(neighbor);
        }

        cityMap[city.name] = city;
    }

    file.close();
}

// Construct adjacency list
void buildAdjacencyList() {
    for (const auto& cityPair : cityMap) {  
        const std::string& city_name = cityPair.first;
        const City& city = cityPair.second;

        for (const auto& neighbor : city.neighbors) {
            if (cityMap.find(neighbor) != cityMap.end()) {
                double distance = calculateDistance(city.latitude, city.longitude,
                                                    cityMap[neighbor].latitude, cityMap[neighbor].longitude);
                Edge edge = {cityMap[neighbor].crime_index, distance};
                adjacencyList[city_name].push_back({neighbor, edge});
            }
        }
        if (adjacencyList.find(city_name) == adjacencyList.end()) {
            adjacencyList[city_name] = {};
        }
    }
}

void printAdjacencyList() {
    for (const auto& pair : adjacencyList) {  
        const std::string& city = pair.first;
        const auto& neighbors = pair.second;

        std::cout << city << " -> ";
        for (const auto& neighborPair : neighbors) {
            const std::string& neighbor = neighborPair.first;
            const Edge& edge = neighborPair.second;
            std::cout << "[" << neighbor << " {Crime: " << edge.crime_index
                      << ", Distance: " << edge.distance << " km}] ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::string filename = "maharashtra.csv";
    
    loadCitiesFromCSV(filename);
    buildAdjacencyList();
    printAdjacencyList();

    return 0;
}
