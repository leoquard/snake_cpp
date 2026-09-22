#pragma once

#include <optional>
#include <termios.h>
#include <string_view>

namespace snake{


    class Terminal{
        public:
            Terminal();
            ~Terminal();

            Terminal(const Terminal&) = delete;
            Terminal& operator=(const Terminal&) = delete;

            void clean();
            void move_to(int row, int col);
            void put(std::string_view text);
            std::optional<char> read_char();
        private:
            termios saved_{};
            bool raw_active_ = false;
    };
}
