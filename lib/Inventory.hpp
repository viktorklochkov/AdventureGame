#pragma once

#include <string>

namespace adv_sk {

struct InventoryItem {
  std::string name{};
  std::string use_message{};
  bool is_visible{false};

  auto operator<=>(const InventoryItem& item) const = default;
};

}// namespace adv_sk
