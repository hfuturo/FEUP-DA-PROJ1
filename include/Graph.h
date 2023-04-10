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
     * @brief Get the vector where all the stations are stored.
     *
     * @return A vector containing all the stations of the graph.
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
     * @brief Gets the maximum number of trains that can simultaneously travel between two stations by apllying the Edmonds-Karp Algorithm.
     *
     * @note This function was implemented by Gonçalo Leão.
     * @note Complexity time: O(VE^2)
     *
     * @param source The name of the origin station.
     * @param target The name of the final station.
     * @return -2 if either the origin station does not exist, the final station does not exist or the final station is the same as the final station.
     * @return -1 if there is no path that connects both origin and final stations.
     * @return The maximum flow between both stations.
     */
    double maxFlow(const std::string& source, const std::string& target);

    /**
     * @brief Finds an augmenting path between two stations.
     *
     * @note This function was implemented by Gonçalo Leão.
     *
     * @param s The origin station.
     * @param t The final station.
     * @return True if exists an augmenting path.
     * @return False otherwise.
     */
    bool findAugmentingPath(Station* s, Station* t);

    /**
     * @brief Calculates how much more flow is allowed in each edge of the augmenting path.
     *
     * @note This function was implemented by Gonçalo Leão.
     *
     * @param s The origin station.
     * @param t The destination station.
     * @return The minimal residual flow.
     */
    double findMinResidualAlongPath(Station* s, Station* t);

    /**
     * @brief Finds the flow of a augmented path.
     *
     * @note This function was implemented by Goncaçalo Leão.
     *
     * @param s The origin station.
     * @param t The destination station.
     * @param f The minimal residual flow of this augmented path.
     */
    void augmentFlowAlongPath(Station* s, Station* t, double f);

    /**
     * @brief This function evaluates if the staion w is optimal to insert into the queue. (BFS)
     *
     * @note This function was implemented by Gonçalo Leão.
     *
     * @param q Queue where we are going to add stations.
     * @param e Edge that we took to get to station w.
     * @param w Station we are going to evaluate.
     * @param residual The remaining capacity of the station w.
     */
    void testAndVisit(std::queue<Station*>& q, Edge* e, Station* w, double residual);

    /**
     * @brief Aplly the DFS algorithm to see if a path between source and dest exist. We can apply DFS in all paths, STANDARD paths and ALFA PENDULAR paths.
     *
     * @note Complexity time: O(V+E)
     *
     * @param source The origin station's name
     * @param dest The destination station's name
     * @param service The service we want to execute the DFS. "All" executes dfs in all paths. "Standard" executes dfs only in standard paths. "ALFA PENDULAR" executes dfs only in alfa pendular paths.
     * @return True if a path exists.
     * @return False otherwise.
     */
    bool dfs(const std::string& source, const std::string& dest, const std::string& service);

    /**
     * @brief Executes the "heavy work" of the dfs algorithm in all paths.
     *
     * @param s The origin station.
     * @param dest The destination station's name.
     * @return True if a path exists.
     * @return False otherwise.
     */
    bool dfsVisit(Station* s, const std::string& dest);

    /**
     * @brief Executes the "heavy work" of the dfs algorithm in "ALFA PENDULAR" paths.
     *
     * @param s The origin station.
     * @param dest The destination station's name.
     * @return True if a path exists.
     * @return False otherwise.
     */
    bool dfsVisitAlfaPendular(Station* s, const std::string& dest);

    /**
     * @brief Executes the "heavy work" of the dfs algorithm in "STANDARD" paths.
     *
     * @param s The origin station.
     * @param dest The destination station's name.
     * @return True if a path exists.
     * @return False otherwise.
     */
    bool dfsVisitStandard(Station* s, const std::string& dest);

    /**
     * @brief Executes the Edmonds-Karp algorithm in all pair of stations to find the pair of stations that require the most amount of trains.
     *
     * @note Complexity time: O(V^3 * E^2).
     *
     * @return A vector containing the number of trains and the pair of station's name.
     */
    std::vector<std::pair<double, std::pair<std::string, std::string>>> fullMaxFlow();

    /**
     * @brief Executes the Edmonds-Karp algorithm to find the top (n) districts with the most flow of trains.
     *
     * @note Complexity time: O(V^3 * E^2).
     *
     * @param n The number of the districts that we want to see.
     * @return A vector containing a pair with the district name and the respective flow of trains.
     */
    std::vector<std::pair<std::string, double>> topDistricts(int n);

    /**
     * @brief Executes the Edmonds-Karp algorithm to find the top (n) municipalities with the most flow of trains.
     *
     * @note Complexity time: O(V^3 * E^2).
     *
     * @param n The number of the districts that we want to see.
     * @return A vector containing a pair with the district name and the respective flow of trains.
     */
    std::vector<std::pair<std::string, double>> topMunicipalities(int n);

    /**
     * @brief Finds the maximum number of trains that can travel simultaneously to a specific station from the entire railway network.
     *
     * @note Complexity time: O(VE^2).
     *
     * @param dest The name of the station.
     * @return
     */
    double maxFlowGridToStation(const std::string& dest);

    /**
     * @brief Finds the path that connects two stations which cost less to the company while maximizes the number of trains that can travel.
     *
     * @note If there is no path that connects the origin station and the destination station, the service will be uninitialized.
     * @note Complexity time: O(ElogV)
     *
     * @param origin The origin station's name.
     * @param dest The destination station's name.
     * @param service The service that the path took.
     * @return A pair which contains the cost and the most amount of trains that can travel simultaneously between origin and destination.
     */
    std::pair<double, double> maxFlowMinCost(const std::string& origin, const std::string& dest, std::string& service);

    /**
     * @brief Calculates the maximum number of trains that can simultaneously travel between two stations by apllying the Edmonds-Karp Algorithm in a subgraph.
     *
     * @note Complexity time: O(VE^2)
     *
     * @param linesToRemove A vector that contains a pair of the station's name that are going to have the edges that connect them removed.
     * @param origin The origin station's name.
     * @param dest The destination station's name.
     * @return The max flow between both stations.
     */
    double maxFlowSubGraph(const std::vector<std::pair<std::string, std::string>>& linesToRemove, const std::string& origin, const std::string& dest);

    /**
     * @brief Finds the minimal cost path between two stations. We can apply in "STANDARD" paths and "ALFA PENDULAR" paths.
     *
     * @note Complexity time: O(ElogV)
     *
     * @param source The origin station.
     * @param dest The destination station.
     * @param service The service we want to execute the Dijkstra. It can either be "STANDARD" or "ALFA PENDULAR".
     */
    void dijkstra(Station* source, Station* dest, const std::string& service);

    /**
     * @brief Provides the top (n) stations that were affected by the lines removed.
     *
     * @param linesToRemove A vector that contains a pair of the station's name that are going to have the edges that connect them removed.
     * @param n The number of stations that we want to see affected.
     * @param error Variable to be initialized if an error occurred.
     * @return A vector containing a pair which have the station and the number of trains affected.
     */
    std::vector<std::vector<std::pair<Station*, double>>> topStationsAffected(const std::vector<std::pair<std::string, std::string>> &linesToRemove, const int n, bool& error);

    /**
     * @brief Calculate the maximum number of trains that can travel between a specific path.
     *
     * @param origin The origin station.
     * @param dest The destination station.
     * @param nPath The number of lines that it took to get to the destination station. Must be initialized in this function.
     * @return The maximum number of trains that can travel from origin to dest.
     */
    double calculateCost(Station* origin, Station* dest, int& nPath) const;
};

#endif