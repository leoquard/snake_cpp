#include "snake/ui/input.hpp"
namespace snake {
Key
decode_key(snake::Terminal& term, int timeout_ms)

{
  char ch;
  snake::Terminal::read_result res = term.read_char(timeout_ms, &ch);
  switch (res) {
    case snake::Terminal::read_result::Char:
      switch (ch) {
        case 'q':
          return Key::Quit;
        case 'w':
          return Key::Up;
        case 'a':
          return Key::Left;
        case 's':
          return Key::Down;
        case 'd':
          return Key::Right;
        default:
          return Key::Unknown;
      }
    case snake::Terminal::read_result::ERROR:
      return Key::Quit;
    case snake::Terminal::read_result::Eof:
      return Key::Quit;
    case snake::Terminal::read_result::TIME_OUT:
      return Key::None;
    default:
      return Key::Unknown;
  }
}
}
