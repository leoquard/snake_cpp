#include "snake/ui/terminal.hpp"
#include <cstddef>
#include <cstdio>
#include <optional>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

namespace snake{
namespace  {
    
    void write_all(std::string_view text)
    {
        while(text.size() > 0)
        {
            ssize_t len = ::write(STDOUT_FILENO, text.data(), text.size());
            if(len > 0) text.remove_prefix(static_cast<std::size_t>(len));
            else if ((len == -1) && (errno == EINTR)) continue;
            else return;
        }
    }
}

Terminal::Terminal()
{
    if(::tcgetattr(STDIN_FILENO, &saved_) != 0) return;
    termios raw = saved_;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    if(::tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) != 0) return;
    raw_active_ = true; 
    write_all("\033[?1049h\033[?25l\033[2J\033[H");
}

Terminal::~Terminal()
{
    if(!raw_active_) return;
    write_all("\033[?25h\033[?1049l");
    if(::tcsetattr(STDIN_FILENO, TCSAFLUSH, &saved_) != 0) std::perror("tcsetattr");
    raw_active_ = false;    
}

void Terminal::clean()
{
    write_all("\033[2J\033[H");
}

void Terminal::move_to(int row, int col)
{
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "\033[%d;%dH", row + 1, col + 1);//返回写出的字符个数
    if(len > 0 && len < static_cast<int>(sizeof(buf)))
    write_all(std::string_view(buf, static_cast<std::size_t>(len)));
}

void Terminal::put(std::string_view text)
{
    write_all(text);
}

std::optional<char> Terminal::read_char()
{
    char c;
    if(::read(STDIN_FILENO, &c, 1) == 1) return c;
    else return std::nullopt;
}

}


