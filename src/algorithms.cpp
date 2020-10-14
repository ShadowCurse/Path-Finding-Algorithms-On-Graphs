#include "algorithms.h"

#include <ranges>
#include <set>
#include <math.h>

namespace algorithms {

auto Kruskal(const std::vector<Node> &nodes, std::vector<Edge> &edges) -> std::pair<std::vector<Edge *>,
                                                                                    std::vector<Edge * >> {
  std::vector<Edge *> visited_edges;
  std::vector<Edge *> path;
  std::vector<int> parent_nodes(nodes.size());
  std::iota(std::begin(parent_nodes), std::end(parent_nodes), 0);

  std::vector<Edge *> sorted_edges(edges.size());
  std::transform(std::begin(edges), std::end(edges), std::begin(sorted_edges), [](auto &edge) { return &edge; });
  std::sort(std::begin(sorted_edges),
            std::end(sorted_edges),
            [](const auto &e1, const auto &e2) { return e1->length < e1->length; });

  if (!edges.empty()) {
    for (auto &edge: sorted_edges) {
      if (!edge->is_shortcut) {
        auto source_id = edge->source->id;
        auto dest_id = edge->dest->id;
        if (parent_nodes[source_id] != parent_nodes[dest_id]) {
          path.push_back(edge);
          auto old_parent = parent_nodes[dest_id];
          for (auto &parent : parent_nodes) {
            if (parent == old_parent)
              parent = parent_nodes[source_id];
          }
        } else {
          visited_edges.push_back(edge);
        }
      }
    }
  }
  return {std::move(visited_edges), std::move(path)};
}

auto Dijkstra(const std::vector<Node> &nodes, std::vector<Edge> &edges, int start, int end) -> std::tuple<std::vector<
    Edge *>, std::vector<Edge *>, std::vector<int>> {
  std::vector<Edge *> visited_edges;
  std::vector<bool> edge_run_number(nodes.size(), false);
  std::vector<int> parent_id(nodes.size(), -1);
  std::vector<int> min_distance(nodes.size(), std::numeric_limits<int>::max());
  min_distance[start] = 0;
  edge_run_number[start] = true;
  std::set<std::pair<int, int> > active_vertices;
  active_vertices.insert({0, start});
  while (!active_vertices.empty()) {
    auto curr_node_id = active_vertices.begin()->second;
    if (curr_node_id == end) {
      break;
    }
    active_vertices.erase(active_vertices.begin());
    for (auto &edge_id : nodes[curr_node_id].edges) {
      auto &edge = edges[edge_id];
      if (curr_node_id != edge.dest->id && !edge.is_shortcut) {
        if (!edge_run_number[edge.dest->id]) {
          min_distance[edge.dest->id] = min_distance[curr_node_id] + edge.length;
          active_vertices.insert({min_distance[edge.dest->id], edge.dest->id});
          edge_run_number[edge.dest->id] = true;
          parent_id[edge.dest->id] = curr_node_id;
        } else {
          if (min_distance[edge.dest->id] > min_distance[curr_node_id] + edge.length) {
            active_vertices.erase({min_distance[edge.dest->id], edge.dest->id});
            min_distance[edge.dest->id] = min_distance[curr_node_id] + edge.length;
            active_vertices.insert({min_distance[edge.dest->id], edge.dest->id});
            parent_id[edge.dest->id] = curr_node_id;
          }
        }
        visited_edges.push_back(&edge);
      }
    }
  }
  std::vector<Edge *> path;
  if (edge_run_number[end]) {
    auto start_node = nodes[end].id;
    while (start_node != nodes[start].id) {
      for (auto &edge_id : nodes[start_node].edges) {
        auto &edge = edges[edge_id];
        if (edge.source->id == parent_id[start_node]) {
          path.push_back(&edge);
        }
      }
      start_node = parent_id[start_node];
    }
  }
  return {std::move(visited_edges), std::move(path), std::move(parent_id)};
}

auto Reverce_Dijkstra(const std::vector<Node> &nodes,
                      std::vector<Edge> &edges,
                      int start,
                      int end) -> std::vector<int> {
//  std::vector<Edge *> visited_edges;
  std::vector<bool> edge_run_number(nodes.size(), false);
  std::vector<int> parent_id(nodes.size(), -1);
  std::vector<int> min_distance(nodes.size(), std::numeric_limits<int>::max());
  min_distance[start] = 0;
  edge_run_number[start] = true;
  std::set<std::pair<int, int> > active_vertices;
  active_vertices.insert({0, start});
  while (!active_vertices.empty()) {
    auto curr_node_id = active_vertices.begin()->second;
    if (curr_node_id == end) {
      break;
    }
    active_vertices.erase(active_vertices.begin());
    for (auto &edge_id : nodes[curr_node_id].edges) {
      auto &edge = edges[edge_id];
      if (curr_node_id != edge.source->id && !edge.is_shortcut) {
        if (!edge_run_number[edge.source->id]) {
          min_distance[edge.source->id] = min_distance[curr_node_id] + edge.length;
          active_vertices.insert({min_distance[edge.source->id], edge.source->id});
          edge_run_number[edge.source->id] = true;
          parent_id[edge.source->id] = curr_node_id;
        } else {
          if (min_distance[edge.source->id] > min_distance[curr_node_id] + edge.length) {
            active_vertices.erase({min_distance[edge.source->id], edge.source->id});
            min_distance[edge.source->id] = min_distance[curr_node_id] + edge.length;
            active_vertices.insert({min_distance[edge.source->id], edge.source->id});
            parent_id[edge.source->id] = curr_node_id;
          }
        }
//        visited_edges.push_back(edge);
      }
    }
  }
//  std::vector<Edge *> path;
//  if (edge_run_number[end]) {
//    auto start_node = nodes[end].id;
//    while (start_node != nodes[start].id) {
//      for (auto &edge : nodes[start_node].edges) {
//        if (edge.dest->id == parent_id[start_node]) {
//          path.push_back(edge);
//        }
//      }
//      start_node = parent_id[start_node];
//    }
//  }
//  return {std::move(visited_edges), std::move(path)};
  return std::move(parent_id);
}

auto Double_Dijkstra(const std::vector<Node> &nodes,
                     std::vector<Edge> &edges,
                     int start,
                     int end) -> std::tuple<std::vector<Edge *>,
                                            std::vector<Edge *>,
                                            std::vector<Edge * >> {
  int dijkstra_meet_id = -1;
  int source;
  int dest;
  int min_forward;
  int min_backward;
  std::vector<Edge *> forward_visited_edges;
  std::vector<Edge *> backward_visited_edges;
  std::vector<bool> forward_edge_run_number(nodes.size(), false);
  std::vector<bool> backward_edge_run_number(nodes.size(), false);
  std::vector<int> forward_parent_id(nodes.size());
  std::vector<int> backward_parent_id(nodes.size());
  std::vector<int> forward_min_distance(nodes.size());
  std::vector<int> backward_min_distance(nodes.size());
  forward_min_distance[start] = 0;
  backward_min_distance[end] = 0;
  std::set<std::pair<int, int> > forward_active_vertices;
  std::set<std::pair<int, int> > backward_active_vertices;
  forward_active_vertices.insert({0, start});
  backward_active_vertices.insert({0, end});
  while (!forward_active_vertices.empty() && !backward_active_vertices.empty()) {
    source = forward_active_vertices.begin()->second;
    dest = backward_active_vertices.begin()->second;
    forward_active_vertices.erase(forward_active_vertices.begin());
    backward_active_vertices.erase(backward_active_vertices.begin());
    //forward dijkstra
    for (auto &edge_id : nodes[source].edges) {
      auto &edge = edges[edge_id];
      if (source != edge.dest->id && !edge.is_shortcut) {
        if (!forward_edge_run_number[edge.dest->id]) {
          forward_min_distance[edge.dest->id] = forward_min_distance[source] + edge.length;
          forward_active_vertices.insert({forward_min_distance[edge.dest->id], edge.dest->id});
          forward_edge_run_number[edge.dest->id] = true;
          forward_parent_id[edge.dest->id] = source;
        } else {
          if (forward_min_distance[edge.dest->id] > forward_min_distance[source] + edge.length) {
            forward_active_vertices.erase({forward_min_distance[edge.dest->id], edge.dest->id});
            forward_min_distance[edge.dest->id] = forward_min_distance[source] + edge.length;
            forward_active_vertices.insert({forward_min_distance[edge.dest->id], edge.dest->id});
            forward_parent_id[edge.dest->id] = source;
          }
        }
        forward_visited_edges.push_back(&edge);
        if (!backward_edge_run_number[edge.dest->id])
          continue;
        if (!forward_active_vertices.empty()) {
          min_forward = forward_active_vertices.begin()->first;
        } else {
          min_forward = forward_min_distance[edge.dest->id];
        }
        if (!backward_active_vertices.empty()) {
          min_backward = backward_active_vertices.begin()->first;
        } else {
          min_backward = backward_min_distance[edge.dest->id];
        }
        if (forward_min_distance[edge.dest->id] + backward_min_distance[edge.dest->id]
            <= min_forward + min_backward) {
          dijkstra_meet_id = edge.dest->id;
          break;
        }
      }
    }
    if (dijkstra_meet_id != -1)
      break;
    //backward dijkstra
    for (auto &edge_id : nodes[dest].edges) {
      auto &edge = edges[edge_id];
      if (dest != edge.source->id && !edge.is_shortcut) {
        if (!backward_edge_run_number[edge.source->id]) {
          backward_min_distance[edge.source->id] = backward_min_distance[dest] + edge.length;
          backward_active_vertices.insert({backward_min_distance[edge.source->id], edge.source->id});
          backward_edge_run_number[edge.source->id] = true;
          backward_parent_id[edge.source->id] = dest;
        } else {
          if (backward_min_distance[edge.source->id] > backward_min_distance[dest] + edge.length) {
            backward_active_vertices.erase({backward_min_distance[edge.source->id], edge.source->id});
            backward_min_distance[edge.source->id] = backward_min_distance[dest] + edge.length;
            backward_active_vertices.insert({backward_min_distance[edge.source->id], edge.source->id});
            backward_parent_id[edge.source->id] = dest;
          }
        }
        backward_visited_edges.push_back(&edge);
        if (!forward_edge_run_number[edge.source->id])
          continue;
        if (!forward_active_vertices.empty()) {
          min_forward = forward_active_vertices.begin()->first;
        } else {
          min_forward = forward_min_distance[edge.dest->id];
        }
        if (!backward_active_vertices.empty()) {
          min_backward = backward_active_vertices.begin()->first;
        } else {
          min_backward = backward_min_distance[edge.dest->id];
        }
        if (forward_min_distance[edge.source->id] + backward_min_distance[edge.source->id]
            <= min_forward + min_backward) {
          dijkstra_meet_id = edge.source->id;
          break;
        }
      }
    }
    if (dijkstra_meet_id != -1)
      break;
  }
  std::vector<Edge *> path;
  if (dijkstra_meet_id != -1) {
    auto start_node = nodes[end].id;
    while (start_node != nodes[start].id) {
      for (auto &edge_id : nodes[start_node].edges) {
        auto &edge = edges[edge_id];
        if (edge.source->id == forward_parent_id[start_node]) {
          path.push_back(&edge);
        }
      }
      start_node = forward_parent_id[start_node];
    }
    start_node = nodes[end].id;
    while (start_node != nodes[start].id) {
      for (auto &edge_id : nodes[start_node].edges) {
        auto &edge = edges[edge_id];
        if (edge.dest->id == backward_parent_id[start_node]) {
          path.push_back(&edge);
        }
      }
      start_node = backward_parent_id[start_node];
    }
  }
  return {std::move(forward_visited_edges), std::move(backward_visited_edges), std::move(path)};
}

auto AStar(const std::vector<Node> &nodes,
           std::vector<Edge> &edges,
           int start,
           int end) -> std::pair<std::vector<Edge *>,
                                 std::vector<Edge * >> {
  std::vector<Edge *> visited_edges;
  std::vector<int> parent_id(nodes.size());
  std::vector<int> min_distance(nodes.size());
  std::vector<bool> closed_set(nodes.size(), false);
  std::vector<bool> open_set(nodes.size(), false);
  min_distance[start] = 0;
  std::set<std::pair<int, int> > active_vertices;
  active_vertices.insert({0, start});
  while (!active_vertices.empty()) {
    auto source = active_vertices.begin()->second;
    if (source == end) {
      closed_set[source] = true;
      break;
    }
    active_vertices.erase(active_vertices.begin());
    open_set[source] = false;
    closed_set[source] = true;
    for (const auto &edge_id : nodes[source].edges) {
      auto &edge = edges[edge_id];
      if (source != edge.dest->id && !edge.is_shortcut) {
        if (closed_set[edge.dest->id])
          continue;
        auto full_path = min_distance[source] + edge.length;
        if (!open_set[edge.dest->id]) {
          auto distance = std::pow(edge.dest->x - nodes[end].x, 2)
              + std::pow(edge.dest->y - nodes[end].y, 2);
          min_distance[edge.dest->id] = full_path + distance;
          active_vertices.insert({min_distance[edge.dest->id], edge.dest->id});
          open_set[edge.dest->id] = true;
        } else {
          if (full_path >= min_distance[edge.dest->id])
            continue;
        }
        parent_id[edge.dest->id] = source;
        min_distance[edge.dest->id] = full_path;
        visited_edges.push_back(&edge);
      }
    }
  }
  std::vector<Edge *> path;
  if (closed_set[end]) {
    auto start_node = nodes[end].id;
    while (start_node != nodes[start].id) {
      for (const auto &edge_id : nodes[start_node].edges) {
        auto &edge = edges[edge_id];
        if (edge.source->id == parent_id[start_node]) {
          path.push_back(&edge);
        }
      }
      start_node = parent_id[start_node];
    }
  }
  return {std::move(visited_edges), std::move(path)};
}

void Arc_Flags_Preprocessing(const std::vector<Node> &nodes,
                             std::vector<Edge> &edges) {
  std::vector<Edge *> bounding_edges;
  for (auto &edge: edges) {
    if (edge.source->arc_flag_color == edge.dest->arc_flag_color)
      edge.arc_flags[edge.source->arc_flag_color] = true;
    else
      bounding_edges.push_back(&edge);
  }
  for (auto &edge: bounding_edges) {
    int color = edge->dest->arc_flag_color;
    auto parent_id = Reverce_Dijkstra(nodes, edges, edge->source->id, -1);
    for (int i(0); i < nodes.size(); ++i) {
      int destination = i;
      int parent = parent_id[i];
      if (parent == -1)
        continue;
      do {
        for (auto &edge_id: nodes[destination].edges) {
          auto &edge = edges[edge_id];
          if (edge.dest->id == parent && !edge.is_shortcut) {
            edge.arc_flags[color] = true;
          }
        }
        destination = parent;
        parent = parent_id[destination];
      } while (parent != -1);
    }
  }
}

auto Arc_Flag_Dijkstra(const std::vector<Node> &nodes,
                       std::vector<Edge> &edges,
                       int start,
                       int end) -> std::pair<std::vector<Edge *>,
                                             std::vector<Edge * >> {
  const auto end_color = nodes[end].arc_flag_color;
  std::vector<Edge *> visited_edges;
  std::vector<bool> edge_run_number(nodes.size(), false);
  std::vector<int> parent_id(nodes.size(), -1);
  std::vector<int> min_distance(nodes.size(), std::numeric_limits<int>::max());
  min_distance[start] = 0;
  edge_run_number[start] = true;
  std::set<std::pair<int, int> > active_vertices;
  active_vertices.insert({0, start});
  while (!active_vertices.empty()) {
    auto source = active_vertices.begin()->second;
    if (source == end) {
      break;
    }
    active_vertices.erase(active_vertices.begin());
    for (const auto &edge_id : nodes[source].edges) {
      auto &edge = edges[edge_id];
      if (source != edge.dest->id && !edge.is_shortcut && edge.arc_flags[end_color]) {
        if (!edge_run_number[edge.dest->id]) {
          min_distance[edge.dest->id] = min_distance[source] + edge.length;
          active_vertices.insert({min_distance[edge.dest->id], edge.dest->id});
          edge_run_number[edge.dest->id] = true;
          parent_id[edge.dest->id] = source;
        } else {
          if (min_distance[edge.dest->id] > min_distance[source] + edge.length) {
            active_vertices.erase({min_distance[edge.dest->id], edge.dest->id});
            min_distance[edge.dest->id] = min_distance[source] + edge.length;
            active_vertices.insert({min_distance[edge.dest->id], edge.dest->id});
            parent_id[edge.dest->id] = source;
          }
        }
        visited_edges.push_back(&edge);
      }
    }
  }
  std::vector<Edge *> path;
  if (edge_run_number[end]) {
    auto start_node = nodes[end].id;
    while (start_node != nodes[start].id) {
      for (const auto &edge_id : nodes[start_node].edges) {
        auto &edge = edges[edge_id];
        if (edge.source->id == parent_id[start_node]) {
          path.push_back(&edge);
        }
      }
      start_node = parent_id[start_node];
    }
  }
  return {std::move(visited_edges), std::move(path)};
}

auto Constractional_Heirarchy_Prepricessing(std::vector<Node> &nodes, std::vector<Edge> &edges) -> std::vector<
    Edge> {
  std::vector<std::vector<int>> CHP(nodes.size());
  for (auto &node: nodes) {
    auto[visited_nodes, path, parents_id] = Dijkstra(nodes, edges, node.id, -1);
    CHP[node.id] = std::move(parents_id);
  }
  std::vector<int> weights(nodes.size(), 0);
  for (int i(0); i < nodes.size(); ++i) {
    for (int j(0); j < nodes.size(); ++j) {
      if (CHP[i][j] != -1) {
        auto parent = CHP[i][j];
        do {
          weights[parent] += 1;
          parent = CHP[i][parent];
        } while (parent != -1);
      }
    }
  }
  for (int i(0); i < nodes.size(); ++i) {
    nodes[i].weight = weights[i];
  }
  std::vector<Node_Length> node_weight(nodes.size());
  for (int i(0); i < nodes.size(); ++i) {
    node_weight[i].node = &nodes[i];
    node_weight[i].length = weights[i];
  }
  std::sort(std::begin(node_weight), std::end(node_weight));

  //creating shortcuts
  std::vector<std::vector<bool>> shortcut_map(nodes.size(), std::vector(nodes.size(), false));
  for (const auto &edge: edges) {
    shortcut_map[edge.source->id][edge.dest->id] = true;
  }

  std::vector<Edge> shortcuts;
  for (int i(0); i < nodes.size(); ++i) {
    for (int j(0); j < node_weight[i].node->edges.size(); ++j) {
      auto edge1_id = node_weight[i].node->edges[j];
      auto edge1 = edges[edge1_id];
      for (int k(0); k < node_weight[i].node->edges.size(); ++k) {
        auto edge2_id = node_weight[i].node->edges[k];
        auto edge2 = edges[edge2_id];
        if (edge1.dest != edge2.dest && edge1.source != edge2.source
            && edge1.dest != edge2.source) {//selection edges
          if (edge1.dest == node_weight[i].node) {//finding out whitch direction
            if (edge1.source->weight >= node_weight[i].length
                && edge2.dest->weight >= node_weight[i].length) {// && edge1->source->weight <= edge2->dest->weight){
              if (!shortcut_map[edge1.source->id][edge2.dest->id]) {
                shortcut_map[edge1.source->id][edge2.dest->id] = true;
                auto &edge = shortcuts.emplace_back(edge1.source, edge2.dest, edges.size());
                edge.length = edge1.length + edge2.length;
                edge.is_shortcut = true;
                edge.shortcut_edges_ids[0] = edge2_id;
                edge.shortcut_edges_ids[1] = edge1_id;
              }
            }
          } else {
            if (edge2.source->weight >= node_weight[i].length
                && edge1.dest->weight >= node_weight[i].length) {// && edge2->source->weight <= edge1->dest->weight){
              if (!shortcut_map[edge2.source->id][edge1.dest->id]) {
                shortcut_map[edge2.source->id][edge1.dest->id] = true;
                auto &edge = shortcuts.emplace_back(edge2.source, edge1.dest, edges.size());
                edge.length = edge1.length + edge2.length;
                edge.is_shortcut = true;
                edge.shortcut_edges_ids[0] = edge1_id;
                edge.shortcut_edges_ids[1] = edge2_id;
              }
            }
          }
        }
      }
    }
  }

  //calc levels
  for (int i(0); i < nodes.size(); ++i) {
    int max(0);
    for (int j(0); j < node_weight[i].node->edges.size(); ++j) {
      if (edges[node_weight[i].node->edges[j]].dest != node_weight[i].node
          && edges[node_weight[i].node->edges[j]].dest->level > max)
        max = edges[node_weight[i].node->edges[j]].dest->level;
      else {
        if (edges[node_weight[i].node->edges[j]].source != node_weight[i].node
            && edges[node_weight[i].node->edges[j]].source->level > max)
          max = edges[node_weight[i].node->edges[j]].source->level;
      }
    }
    node_weight[i].node->level = max + 1;
  }
  return std::move(shortcuts);
}

auto CHP_Double_Dijkstra(const std::vector<Node> &nodes,
                         std::vector<Edge> &edges,
                         int start,
                         int end) -> std::tuple<std::vector<Edge *>,
                                                std::vector<Edge *>,
                                                std::vector<Edge * >> {
  int dijkstra_meet_id = -1;
  int source;
  int dest;
  int min_forward;
  int min_backward;
  std::vector<Edge *> forward_visited_edges;
  std::vector<Edge *> backward_visited_edges;
  std::vector<bool> forward_edge_run_number(nodes.size(), false);
  std::vector<bool> backward_edge_run_number(nodes.size(), false);
  std::vector<int> forward_parent_id(nodes.size());
  std::vector<int> backward_parent_id(nodes.size());
  std::vector<int> forward_min_distance(nodes.size());
  std::vector<int> backward_min_distance(nodes.size());
  forward_min_distance[start] = 0;
  backward_min_distance[end] = 0;
  std::set<std::pair<int, int> > forward_active_vertices;
  std::set<std::pair<int, int> > backward_active_vertices;
  forward_active_vertices.insert({0, start});
  backward_active_vertices.insert({0, end});
  while (!forward_active_vertices.empty() || !backward_active_vertices.empty()) {
    if (!forward_active_vertices.empty()) {
      source = forward_active_vertices.begin()->second;
      forward_active_vertices.erase(forward_active_vertices.begin());
      //forward dijkstra
      for (auto &edge_id : nodes[source].edges) {
        auto edge = edges[edge_id];
        if (source != edge.dest->id && nodes[source].level < edge.dest->level) {// && !edge.is_shortcut){
          if (!forward_edge_run_number[edge.dest->id]) {
            forward_min_distance[edge.dest->id] = forward_min_distance[source] + edge.length;
            forward_active_vertices.insert({forward_min_distance[edge.dest->id], edge.dest->id});
            forward_edge_run_number[edge.dest->id] = true;
            forward_parent_id[edge.dest->id] = source;
          } else {
            if (forward_min_distance[edge.dest->id] > forward_min_distance[source] + edge.length) {
              forward_active_vertices.erase({forward_min_distance[edge.dest->id], edge.dest->id});
              forward_min_distance[edge.dest->id] = forward_min_distance[source] + edge.length;
              forward_active_vertices.insert({forward_min_distance[edge.dest->id], edge.dest->id});
              forward_parent_id[edge.dest->id] = source;
            }
          }
          forward_visited_edges.push_back(&edge);
          if (!backward_edge_run_number[edge.dest->id])
            continue;
          if (!forward_active_vertices.empty()) {
            min_forward = forward_active_vertices.begin()->first;
          } else {
            min_forward = forward_min_distance[edge.dest->id];
          }
          if (!backward_active_vertices.empty()) {
            min_backward = backward_active_vertices.begin()->first;
          } else {
            min_backward = backward_min_distance[edge.dest->id];
          }
          if (forward_min_distance[edge.dest->id] + backward_min_distance[edge.dest->id]
              <= min_forward + min_backward) {
            dijkstra_meet_id = edge.dest->id;
            break;
          }
        }
      }
      if (dijkstra_meet_id != -1)
        break;
    }
    if (!backward_active_vertices.empty()) {
      //backward dijkstra
      dest = backward_active_vertices.begin()->second;
      backward_active_vertices.erase(backward_active_vertices.begin());
      for (auto &edge_id : nodes[dest].edges) {
        auto edge = edges[edge_id];
        if (dest != edge.source->id && nodes[dest].level < edge.source->level) {// && !edge.is_shortcut){
          if (!backward_edge_run_number[edge.source->id]) {
            backward_min_distance[edge.source->id] = backward_min_distance[dest] + edge.length;
            backward_active_vertices.insert({backward_min_distance[edge.source->id], edge.source->id});
            backward_edge_run_number[edge.source->id] = true;
            backward_parent_id[edge.source->id] = dest;
          } else {
            if (backward_min_distance[edge.source->id] > backward_min_distance[dest] + edge.length) {
              backward_active_vertices.erase({backward_min_distance[edge.source->id], edge.source->id});
              backward_min_distance[edge.source->id] = backward_min_distance[dest] + edge.length;
              backward_active_vertices.insert({backward_min_distance[edge.source->id], edge.source->id});
              backward_parent_id[edge.source->id] = dest;
            }
          }
          backward_visited_edges.push_back(&edge);
          if (!forward_edge_run_number[edge.source->id])
            continue;
          if (!forward_active_vertices.empty()) {
            min_forward = forward_active_vertices.begin()->first;
          } else {
            min_forward = forward_min_distance[edge.dest->id];
          }
          if (!backward_active_vertices.empty()) {
            min_backward = backward_active_vertices.begin()->first;
          } else {
            min_backward = backward_min_distance[edge.dest->id];
          }
          if (forward_min_distance[edge.source->id] + backward_min_distance[edge.source->id]
              <= min_forward + min_backward) {
            dijkstra_meet_id = edge.source->id;
            break;
          }
        }
      }
      if (dijkstra_meet_id != -1)
        break;
    }
  }
  std::vector<Edge *> path;
  if (dijkstra_meet_id != -1) {
    auto start_node = nodes[end].id;
    while (start_node != nodes[start].id) {
      for (auto &edge_id : nodes[start_node].edges) {
        auto edge = edges[edge_id];
        if (edge.source->id == forward_parent_id[start_node]) {
          path.push_back(&edge);
        }
      }
      start_node = forward_parent_id[start_node];
    }
    start_node = nodes[end].id;
    while (start_node != nodes[start].id) {
      for (auto &edge_id : nodes[start_node].edges) {
        auto edge = edges[edge_id];
        if (edge.dest->id == backward_parent_id[start_node]) {
          path.push_back(&edge);
        }
      }
      start_node = backward_parent_id[start_node];
    }
  }
  return {std::move(forward_visited_edges), std::move(backward_visited_edges), std::move(path)};
}

}