
// void
// player::ability_boost(GameObject2D& player, const KeysAndState& keys, const float delta_time_s)
// {
//   if (keys.boost_held) {
//     // Boost when shift pressed
//     player.shift_boost_time_left -= delta_time_s;
//     player.shift_boost_time_left = player.shift_boost_time_left < 0.0f ? 0.0f : player.shift_boost_time_left;
//   } else {
//     // Recharge when shift released
//     player.shift_boost_time_left += delta_time_s;
//     // Cap limit
//     player.shift_boost_time_left =
//       player.shift_boost_time_left > player.shift_boost_time ? player.shift_boost_time :
//       player.shift_boost_time_left;
//   }

//   if (keys.boost_held && player.shift_boost_time_left > 0.0f) {
//     player.velocity *= player.velocity_boost_modifier;
//   }
// };