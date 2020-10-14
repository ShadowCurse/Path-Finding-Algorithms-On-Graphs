#ifndef PATH_FINDING_ALGORITHMS_ON_GRAPHS_SRC_ALGORITHMS_H_
#define PATH_FINDING_ALGORITHMS_ON_GRAPHS_SRC_ALGORITHMS_H_

#include <vector>
#include <tuple>
#include "node.h"
#include "edge.h"

namespace algorithms {

auto Kruskal(const std::vector<Node> &nodes, std::vector<Edge> &edges) -> std::pair<std::vector<Edge *>,
                                                                                    std::vector<Edge * >>;

auto Dijkstra(const std::vector<Node> &nodes, std::vector<Edge> &edges, int start, int end) -> std::tuple<std::vector<
    Edge *>, std::vector<Edge *>, std::vector<int>>;
auto Reverce_Dijkstra(const std::vector<Node> &nodes, std::vector<Edge> &edges, int start, int end) -> std::vector<int>;
auto Double_Dijkstra(const std::vector<Node> &nodes,
                     std::vector<Edge> &edges,
                     int start,
                     int end) -> std::tuple<std::vector<Edge *>, std::vector<Edge *>, std::vector<Edge * >>;
auto AStar(const std::vector<Node> &nodes,
           std::vector<Edge> &edges,
           int start,
           int end) -> std::pair<std::vector<Edge *>,
                                 std::vector<Edge * >>;
void Arc_Flags_Preprocessing(const std::vector<Node> &nodes,
                             std::vector<Edge> &edges);
auto Arc_Flag_Dijkstra(const std::vector<Node> &nodes,
                       std::vector<Edge> &edges,
                       int start,
                       int end) -> std::pair<std::vector<Edge *>,
                                             std::vector<Edge * >>;
auto Constractional_Heirarchy_Prepricessing(const std::vector<Node> &nodes,
                                            const std::vector<Edge> &edges) -> std::vector<Edge>;
auto CHP_Double_Dijkstra(const std::vector<Node> &nodes,
                         std::vector<Edge> &edges,
                         int start,
                         int end) -> std::tuple<std::vector<Edge *>,
                                                std::vector<Edge *>,
                                                std::vector<Edge * >>;

};

#endif //PATH_FINDING_ALGORITHMS_ON_GRAPHS_SRC_ALGORITHMS_H_
