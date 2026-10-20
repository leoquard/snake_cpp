#include "snake/ui/input.hpp"
#include "snake/ui/terminal.hpp"

#include <signal.h>

namespace snake {

volatile sig_atomic_t go_running = 1;

constexpr int kBoardRows = 16;
constexpr int kBoardCols = 40;
constexpr int kFrameMs = 80;

int snakerows = kBoardRows / 2;
int snakecols = kBoardCols / 2;

int dr = 0;
int dc = -1;

void
on_signal(int)
{
  go_running = 0;
}

void
install_signal_handler()
{
  struct sigaction sig{};
  sig.sa_handler = on_signal;
  sigemptyset(&sig.sa_mask);
  sig.sa_flags = 0;
  ::sigaction(SIGINT, &sig, nullptr);
  ::sigaction(SIGTERM, &sig, nullptr);
}

void
refresh(Terminal& term)
{
  for (int rows = 0, cols = 0; cols < snake::kBoardCols; cols++) {
    term.move_to(rows, cols);
    if (cols == 0 || cols == snake::kBoardCols - 1) {
      term.put("+");
    } else {
      term.put("-");
    }
  }
  for (int rows = snake::kBoardRows - 1, cols = 0; cols < snake::kBoardCols;
       cols++) {
    term.move_to(rows, cols);
    if (cols == 0 || cols == snake::kBoardCols - 1) {
      term.put("+");
    } else {
      term.put("-");
    }
  }
  for (int rows = 1, cols = 0; rows < snake::kBoardRows - 1; rows++) {
    term.move_to(rows, cols);
    term.put("|");
    term.put(std::string(kBoardCols - 2, ' '));
    term.put("|");
  }
}
} // namespace snake

int
main()
{
  snake::install_signal_handler();

  snake::Terminal term;

  /*
for (int i = 0; i < snakelen; i++) {
  term.move_to(snakerows, snakecols + i);
  term.put("*");
}
  */

  term.present();

  while (snake::go_running) {
    term.begin_frame();

    term.move_to(18, 10);
    term.put("TIME");

    term.move_to(18, 30);
    term.put("SCORE");

    term.move_to(snake::snakerows, snake::snakecols);
    term.put("@");

    snake::refresh(term);
    int nr = snake::snakerows + snake::dr;
    int nc = snake::snakecols + snake::dc;

    if (nr > 0 && nr < snake::kBoardRows - 1 && nc > 0 &&
        nc < snake::kBoardCols - 1) {
      snake::snakerows = nr;
      snake::snakecols = nc;
    }

    term.move_to(snake::snakerows, snake::snakecols);
    term.put("@");

    switch (snake::decode_key(term, snake::kFrameMs)) {
      case snake::Key::Quit:
        snake::go_running = 0;
        return 0;
      case snake::Key::Unknown:
        break;
      case snake::Key::None:
        break;
      case snake::Key::Down:
        snake::dc = 0;
        snake::dr = +1;
        break;
      case snake::Key::Left:
        snake::dc = -1;
        snake::dr = 0;
        break;
      case snake::Key::Right:
        snake::dc = +1;
        snake::dr = 0;
        break;
      case snake::Key::Up:
        snake::dc = 0;
        snake::dr = -1;
        break;
      default:
        break;
    }
    term.present();
  }

  return 0;
}
