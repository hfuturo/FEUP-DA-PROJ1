#ifndef DA_PROJ1_CONSTANTS_H
#define DA_PROJ1_CONSTANTS_H

#include <string>

/**
 * @brief Where the stations are stored.
*/
const std::string STATIONS_FILE_PATH = "../dataset/stations.csv";

/**
 * @brief Where the network(edges) are stored.
*/
const std::string NETWORK_FILE_PATH = "../dataset/network.csv";

/**
 * @brief The cost of a standard train per train and per segment
 */
const int STANDARD_COST = 2;

/**
 * @brief The cost of a alfa pendular train per train and per segment
 */
const int ALFA_PENDULAR_COST = 4;

#endif