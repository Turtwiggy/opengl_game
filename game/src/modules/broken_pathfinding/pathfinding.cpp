// // header
// #include "pathfinding.hpp"

// // std lib
// #include <deque>
// #include <set>

// template<typename T>
// std::vector<T>
// reconstruct_path(std::map<T, T>& came_from, const T& current)
// {
//   T internal = current;

//   std::vector<T> total_path{ internal };

//   while (came_from.find(internal) != came_from.end()) {
//     internal = came_from[internal];

//     total_path.push_back(internal);
//   }

//   std::reverse(total_path.begin(), total_path.end());

//   return total_path;
// }

// template<typename T>
// float
// heuristic(const T& start, const T& fin)
// {
//   return (start - fin).length();
// }

// template<typename T>
// std::optional<T>
// any_of_a_in_b(const std::vector<T>& a, const std::vector<T>& b)
// {
//   for (auto& i : a) {
//     for (auto& j : b) {
//       if (i == j)
//         return i;
//     }
//   }

//   return std::nullopt;
// }

// /// this function was originally designed for an arbitrary linked graph and could be made significantly faster
// template<typename T>
// std::vector<T>
// get_shortest_path(entt::registry& registry,
//                   SINGLETON_TilemapComponent& tmap,
//                   glm::ivec2 start,
//                   glm::ivec2 fin,
//                   int cap = -1)
// {
//   std::vector<T> ret;

//   if (start == fin) {
//     return { start };
//   }

//   std::set<T> closed_set;
//   std::vector<T> open_set = { start };

//   std::map<T, T> came_from;

//   std::map<T, float> g_score;

//   g_score[start] = 0.f;

//   std::deque<std::pair<T, float>> sorted;

//   sorted.push_back({ start, 0.f });

//   int num_explored = 0;

//   while (sorted.size() > 0) {
//     T current_sys = std::move(sorted[0].first);

//     sorted.pop_front();

//     if (current_sys == fin)
//       return reconstruct_path(came_from, current_sys);

//     if (closed_set.find(current_sys) != closed_set.end())
//       continue;

//     closed_set.insert(current_sys);

//     std::vector<std::pair<T, int>> flinks;
//     flinks.reserve(8);

//     auto check_apply = [&](glm::ivec2 offset) {
//       if (offset.x() < 0 || offset.y() < 0 || offset.x() >= tmap.dim.x() || offset.y() >= tmap.dim.y())
//         return;

//       auto& found = tmap.all_entities[offset.y() * tmap.dim.x() + offset.x()];

//       int cost = 0;

//       for (entt::entity& i : found) {
//         /*if(!i.passable)
//             return;*/

//         if (!registry.has<collidable>(i))
//           continue;

//         collidable& coll = registry.get<collidable>(i);

//         if (coll.cost == -1)
//           return;

//         cost = std::max(cost, coll.cost);
//       }

//       flinks.push_back({ offset, cost });
//     };

//     check_apply(current_sys + glm::ivec2{ -1, 0 });
//     check_apply(current_sys + glm::ivec2{ 1, 0 });
//     check_apply(current_sys + glm::ivec2{ 0, -1 });
//     check_apply(current_sys + glm::ivec2{ 0, 1 });
//     check_apply(current_sys + glm::ivec2{ -1, -1 });
//     check_apply(current_sys + glm::ivec2{ 1, -1 });
//     check_apply(current_sys + glm::ivec2{ 1, 1 });
//     check_apply(current_sys + glm::ivec2{ -1, 1 });

//     for (auto& [next_sys, cost] : flinks) {
//       if (closed_set.find(next_sys) != closed_set.end())
//         continue;

//       auto looking_gscore = g_score.find(next_sys);
//       float found_gscore = g_score[current_sys] + sqrtf((next_sys - current_sys).squared_length()) + cost;

//       float testing_gscore = FLT_MAX;

//       if (looking_gscore != g_score.end()) {
//         testing_gscore = looking_gscore->second;
//       }

//       if (found_gscore >= testing_gscore)
//         continue;

//       came_from[next_sys] = current_sys;

//       g_score[next_sys] = found_gscore;

//       sorted.push_back({ next_sys, found_gscore + heuristic(next_sys, fin) });
//       // sorted.push_back({next_sys, found_gscore});

//       for (auto& i : sorted) {
//         if (i.first == next_sys) {
//           i.second = found_gscore + heuristic(next_sys, fin);
//           // i.second = found_gscore;
//           break;
//         }
//       }

//       std::sort(sorted.begin(), sorted.begin(), [](const std::pair<T, float>& p1, const std::pair<T, float>& p2) {
//         return p1.second < p2.second;
//       });

//       num_explored++;

//       if (num_explored > cap && cap != -1)
//         return ret;
//     }
//   }

//   return ret;
// }

// std::optional<std::vector<glm::ivec2>>
// a_star(entt::registry& registry, SINGLETON_TilemapComponent& tmap, glm::ivec2 first, glm::ivec2 finish)
// {
//   if (first == finish)
//     return {};

//   auto found = get_shortest_path<glm::ivec2>(registry, tmap, { first }, { finish }, 10000);

//   if (found.size() == 0)
//     return std::nullopt;

//   return found;
// }