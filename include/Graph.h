#ifndef DA_PROJ1_GRAPH_H
#define DA_PROJ1_GRAPH_H

#include <vector>
#include <queue>

#include "StationEdge.h"

class Graph;

/**
 * @brief A directed multigraph representing a railway network.
 */
class Graph {
    /**
     * @brief A vector that stores the nodes(stations) of the graph.
     */
    std::vector<Station*> stationSet;

public:
    /**
     * @brief Creates an empty graph.
     */
    Graph(){};

    /**
     * @brief Get the vector where all the nodes are stored.
     *
     * @return The vector.
     */
    std::vector<Station*> getStationSet() const;

    /**
     * @brief Populates the graph with the information from the csv files in the dataset.
     */
    void fill();

    /**
     * @brief Reads the stations from the file and adds them into the graph.
     */
    void readStations();

    /**
     * @brief Reads the Network(Edges) from the file and adds them into the graph.
     */
    void readNetwork();

    /**
     * @brief Removes a station from the graph.
     *
     * @param station The station that is going to be removed.
     * @return True if the station was successfully removed.
     * @return False otherwise.
     */
    bool removeStation(const Station* station);

    /**
     * @brief Adds a station to the graph.
     *
     * @param name The name of the station.
     * @param district The district where the station belongs.
     * @param municipality The municipality where the station belongs.
     * @param township The township where the station belongs.
     * @param line The main line that the station serves.
     * @return True if the station was successfully added.
     * @return False otherwise.
     */
    bool addStation(const std::string& name, const std::string& district, const std::string& municipality, const std::string& township, const std::string& line);

    /**
     * @brief Adds a Line to the graph.
     *
     * @param origin The name of the station where the line starts.
     * @param dest The name of the station where the line ends.
     * @param capacity The number of trains that can simultaneously travel on this line.
     * @param service The type of service that it provides, which can be either STANDARD or ALFA PENDULAR.
     * @return True if the line was successfully added.
     * @return False otherwise.
     */
    bool addLine(const std::string& origin, const std::string& dest, const double& capacity, const std::string& service);

    /**
     * @brief Gets a station with a given name if it exists.
     * @param name The name of the station.
     * @return The station if it exists.
     * @return A null pointer if it doesn't.
     */
    Station* findStation(const std::string& name) const;

    /**
     * @brief Adds a bidirectional line to a graph.
     *
     * @param origin The origin of the line.
     * @param dest The destination of the line.
     * @param capacity The number of trains that can simultaneously travel on this line.
     * @param service The type of service that it provides, which can be either STANDARD or ALFA PENDULAR.
     * @return True if the line was successfully added.
     * @return False otherwise.
     */
    bool addBidirectionalLine(const std::string& origin, const std::string& dest, const double& capacity, const std::string& service);

    /**
     * @brief Gets the maximum number of trains that can simultaneously travel between two stations.
     * @param source The name of the origin station.
     * @param target The name of the final station.
     * @return -2 if either the origin station does not exist, the final station does not exist or the final station is the same as the final station.
     * @return -1 if there is no path that connects both origin and final stations.
     * @return The maximum flow between both stations.
     */
    double maxFlow(const std::string& source, const std::string& target);

    /**
     * @brief Finds an augmenting path between two stations.
     * @param s The origin station.
     * @param t The final station.
     * @return True if exists an augmenting path.
     * @return False otherwise.
     */
    bool findAugmentingPath(Station* s, Station* t);
    double findMinResidualAlongPath(Station* s, Station* t);
    void augmentFlowAlongPath(Station* s, Station* t, double f);
    void testAndVisit(std::queue<Station*>& q, Edge* e, Station* w, double residual);
    bool dfs(const std::string& source, const std::string& dest);
    bool dfsVisit(Station* s, const std::string& dest);
    std::vector<std::pair<double, std::pair<std::string, std::string>>> fullMaxFlow();
    std::vector<std::pair<std::string, double>> topDistricts(int n);
    std::vector<std::pair<std::string, double>> topMunicipalities(int n);
    /**
     * @brief Finds the maximum number of trains that can travel simultaneously to a specific station from the entire railway network.
     * @param dest The name of the station.
     * @return
     */
    double maxFlowGridToStation(const std::string& dest);
};

#endif