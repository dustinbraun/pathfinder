#pragma once

#include <cstdint>

namespace nav2 {

using VertexId = uint16_t;

constexpr VertexId VERTEX_ID_NONE = 0xFFFF;

constexpr VertexId VERTEX_ID_MAX = 0xFFFE;

} // namespace nav2
