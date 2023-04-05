//
// Created by Henrique Silva on 19/03/2023.
//

#ifndef DA_PROJ1_STATIONEDGE_H
#define DA_PROJ1_STATIONEDGE_H

#include <string>
#include <vector>

class Edge;

/************************* Station  **************************/

/**
 * @brief Represents a station for a public railway network.
 */
class Station {
private:
    /**
     * @brief The name of the station.
     */
    std::string name;

    /**
     * @brief The district where the station belongs.
     */
    std::string district;

    /**
     * @brief The municipality where the station belongs.
     */
    std::string municipality;

    /**
     * @brief The township where the station belongs.
     */
    std::string township;

    /**
     * @brief The main line where the station serves.
     */
    std::string line;

    /**
     * @brief A vector with the outgoing edges.
     */
    std::vector<Edge*> adj;

    /**
     * @brief A vector with the incoming edges.
     */
    std::vector<Edge*> incoming;

    /**
     * @brief A visited attribute. Used for maxFlow and DFS.
     */
    bool visited;

    /**
     * @brief A path that contains the edge taken to get to this station. Used for maxFlow.
     */
    Edge* path;

    /**
     * @brief The maximum number of trains that can simultaneously arrive at this station.
     */
    double maximumFlow;

    /**
     * @brief The cost that takes to get from a station to this station. Need for Dijkstra.
     */
    double cost;

public:
    /**
     * @brief A constructor that initializes a station with a name, district, municipality, township and line.
     *
     * @note This constructor initializes path as nullptr, visited as false and maximumFlow as 0.
     *
     * @param name The name of the station.
     * @param district The district where the station belongs.
     * @param municipality The municipality where the station belongs.
     * @param township The township where the station belongs.
     * @param line The line that the station serves.
     */
    Station(const std::string& name, const std::string& district, const std::string& municipality, const std::string& township, const std::string& line);

    /**
     * @brief Gets the station name.
     *
     * @return The station name.
     */
    std::string getName() const;

    /**
     * @brief Gets the district where the station belongs.
     *
     * @return The district of the station.
     */
    std::string getDistrict() const;

    /**
     * @brief Gets the municipality where the station belongs.
     *
     * @return The municipality of the station.
     */
    std::string getMunicipality() const;

    /**
     * @brief Gets the township where the station belongs.
     *
     * @return The township of the station.
     */
    std::string getTownShip() const;

    /**
     * @brief Gets the line that serves the station.
     *
     * @return The line of the station.
     */
    std::string getLine() const;

    /**
     * @brief Gets the outgoing edges of this station.
     *
     * @return The outgoing edges.
     */
    std::vector<Edge*> getAdj() const;

    /**
     * @brief Gets the incoming edges of this station.
     *
     * @return The incoming edges.
     */
    std::vector<Edge*> getIncoming() const;

    /**
     * @brief Connects two stations by adding a line (edge).
     *
     * @param dest The destination station.
     * @param capacity The number of trains that can simultaneously travel between this station and the destination station.
     * @param service The type of service that line provides, which can be either STANDARD or ALFA PENDULAR.
     * @return The edge if it was successfully created.
     * @return Null pointer otherwise.
     */
    Edge* addLine(Station* dest, const double capacity, const std::string& service);

    /**
     * @brief Removes an edge (line) from this station that connects to other station.
     *
     * @param name The name of the other station which edge is going to be removed.
     * @return True if the edge was successfully removed.
     * @return False otherwise.
     */
    bool removeEdge(std::string name);

    /**
     * @brief Removes all outgoing edges from this station.
     */
    void removeOutgoingEdges();

    /**
     * @brief Removes and stores the edge that leaves this station and arrives the destination station.
     *
     * @param dest The destination station.
     * @return The Edge if it exists.
     * @return Null pointer if it does not exist.
     */
    Edge* removeAndStoreEdge(Station* dest);

    /**
     * @brief Deletes an edge (line) from the destination station.
     *
     * @param edge The edge that is going to get deleted.
     */
    void deleteEdge(Edge* edge);

    /**
     * @brief Set this station as visited or not visited.
     *
     * @param The visited attribute.
     */
    void setVisited(bool vis);

    /**
     * @brief Sees if this station is visited.
     *
     * @return True if this station is visited.
     * @return False otherwise.
     */
    bool isVisited() const;

    /**
     * @brief Sets the path that contains the edge taken to get to this station.
     *
     * @param path The edge taken to get to this station.
     */
    void setPath(Edge* path);

    /**
     * @brief Gets the path that contains the edge taken to get to this station.
     *
     * @return The edge taken to get to this station.
     * @return Null pointer if the edge does not exist.
     */
    Edge* getPath() const;

    /**
     * @brief Sets the maximum flow of this station.
     *
     * @param maximumFlow The value of the maximum flow.
     */
    void setMaximumFlow(double maximumFlow);


    /**
     * @brief Gets the maximum flow of this station.
     *
     * @return The maximum flow of this station.
     */
    double getMaximumFlow() const;

    void setCost(double cost);

    double getCost() const;

    bool operator<(Station& station) const;

    /**
     * @brief Index of this station in Priority Queue. Need for Dijkstra.
     */
    int queueIndex;
};

/************************* Edge  **************************/

class Edge {
private:
    /**
     * @brief The station where the edge starts.
     */
    Station* origin;

    /**
     * @brief The station where the edge ends.
     */
    Station* dest;

    /**
     * @brief The maximum number of trains that can simultaneously travel in this edge.
     */
    double capacity;

    /**
     * @brief The type of service that it provides, which can be either STANDARD or ALFA PENDULAR.
     */
    std::string service;

    /**
     * @brief The edge that connects the destination station to this station.
     */
    Edge* reverse;

    /**
     * @brief The flow that this edge supports. Used for maxFlow.
     */
    double flow;
public:
    /**
     * @brief Constructor that initializes an edge with an origin station, a destination station, a capacity and a service.
     *
     * @param origin The station where this edge starts.
     * @param dest The station where this edge ends.
     * @param capacity The maximum number of trains that can simultaneously travel in this edge.
     * @param service The type of service that it provides, which can be either STANDARD or ALFA PENDULAR.
     */
    Edge(Station* origin, Station* dest, const double capacity, const std::string& service);

    /**
     * @brief Gets the station where this edge starts.
     *
     * @return The station.
     */
    Station* getOrigin() const;

    /**
     * @brief Gets the station where this edge ends.
     *
     * @return The station.
     */
    Station* getDest() const;

    /**
     * @brief Gets the maximum number of trains that can simultaneously travel in this edge (capacity).
     *
     * @return The capacity.
     */
    double getCapacity() const;

    /**
     * @brief Gets the service that this edge provides.
     *
     * @return The name of the service.
     */
    std::string getService() const;

    /**
     * @brief Sets the reverse edge that connects the destination station to this station.
     *
     * @param reverse The edge.
     */
    void setReverse(Edge* reverse);

    /**
     * @brief Gets the reverse edge that connects the destination station to this station.
     *
     * @return The edge.
     */
    Edge* getReverse() const;

    /**
     * @brief Gets the current number of trains that can travel in this edge (flow).
     *
     * @return The flow.
     */
    double getFlow() const;

    /**
     * @brief Sets the current number of trains that can travel in edge (flow).
     *
     * @param flow The flow.
     */
    void setFlow(double flow);
};

#endif
