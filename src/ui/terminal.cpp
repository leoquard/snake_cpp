#include "snake/ui/terminal.hpp"
#include <cstddef>
#include <cstdio>
#include <errno.h>
#include <poll.h>
#include <termios.h>
#include <unistd.h>

namespace snake {
namespace {
void
write_all(std::string_view text)
{
  while (text.size() > 0) {
    ssize_t len = ::write(STDOUT_FILENO, text.data(), text.size());
    if (len > 0)
      text.remove_prefix(static_cast<std::size_t>(len));
    else if ((len == -1) && (errno == EINTR))
      continue;
    else
      return;
  }
}
}

Terminal::Terminal()
{
  frame_.reserve(4096);
  if (::tcgetattr(STDIN_FILENO, &saved_) != 0)
    return;
  termios raw = saved_;
  raw.c_lflag &= ~(ICANON | ECHO);
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;
  if (::tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) != 0)
    return;
  raw_active_ = true;
  write_all("\033[?1049h\033[?25l\033[2J\033[H");
}

Terminal::~Terminal()
{
  if (!raw_active_)
    return;
  write_all("\033[?25h\033[?1049l");
  if (::tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved_) != 0)
    std::perror("tcsetattr");
  raw_active_ = false;
}

void
Terminal::begin_frame()
{
  frame_.clear();
  return;
}
void
Terminal::clean()
{
  write_all("\033[2J\033[H");
}

void
Terminal::move_to(int row, int col)
{
  char buf[32];
  int len = snprintf(buf, sizeof(buf), "\033[%d;%dH", row + 1, col + 1);
  if (len < 0 && len > static_cast<int>(sizeof(buf))) {
    frame_.append(buf, static_cast<size_t>(len));
  }
}

void
Terminal::put(std::string_view text)
{
  frame_.append(text);
}

Terminal::read_result
Terminal::read_char(int timeout_ms, char* read_out)
{
  struct pollfd fds{};
  fds.fd = STDIN_FILENO;
  fds.events = POLLIN;

  int ret = ::poll(&fds, 1, timeout_ms);

  if (ret < 0) {
    return Terminal::read_result::ERROR;
  } else if (ret == 0) {
    return Terminal::read_result::TIME_OUT;
  } else if (ret > 0) {
    switch (::read(STDIN_FILENO, read_out, 1)) {
      case 1:
        return read_result::Char;
        break;
      case 0:
        return read_result::TIME_OUT;
        break;
      case -1:
        if (errno == EINTR)
          return read_result::TIME_OUT;
        else
          return read_result::Eof;
        break;
    }
  }
  return read_result::ERROR;
}
}

void
snake::Terminal::present()
{
  write_all(frame_);
}
