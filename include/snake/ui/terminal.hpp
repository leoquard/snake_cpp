#pragma once

#include <string>
#include <string_view>
#include <termios.h>
namespace snake {

class Terminal
{
public:
  Terminal();
  ~Terminal();

  enum class read_result
  {
    Char,
    TIME_OUT,
    Eof,
    ERROR
  };

  Terminal(const Terminal&) = delete;
  Terminal& operator=(const Terminal&) = delete;

  void begin_frame();
  void clean();
  void move_to(int row, int col);
  void put(std::string_view text);
  void present();
  read_result read_char(int timout_ms, char* read_out);

private:
  termios saved_{};
  bool raw_active_ = false;
  std::string frame_;
};
}
