#pragma once

#include "terminal.hpp"

namespace snake {

enum class Key
{
  None,
  Quit,
  Up,
  Down,
  Left,
  Right,
  Unknown
};

Key
decode_key(snake::Terminal& term, int timeout_ms);
}
