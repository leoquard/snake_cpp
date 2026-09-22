#include "snake/ui/terminal.hpp"
#include <signal.h>

namespace snake {
    namespace  {
        volatile sig_atomic_t go_running = 1;
        void on_signal(int) { go_running = 0; }

        void install_signal_handler(){
            struct sigaction sig{}; 
            sig.sa_handler = on_signal;
            sigemptyset(&sig.sa_mask);
            sig.sa_flags = 0;
            ::sigaction(SIGINT, &sig, nullptr);
            ::sigaction(SIGTERM, &sig, nullptr);
        }
        
    }

}

int main()
{
    snake::install_signal_handler();
    
    snake::Terminal term;

    int kBoardRows = 16;
    int kBoardCols = 40;

    for(int rows = 0, cols = 0;cols < kBoardCols + 2;cols++)
    {
        term.move_to( rows, cols);
        if(cols == 0 || cols == kBoardCols + 1)
        {
            term.put("+");
        }
        else 
        {
            term .put("-");
        }

    }
    for(int rows = kBoardRows + 1, cols = 0;cols < kBoardCols + 2;cols++)
    {
        term.move_to( rows, cols);
        if(cols == 0 || cols == kBoardCols + 1)
        {
            term.put("+");
        }
        else 
        {
            term.put("-");
        }

    }
    for(int rows = 1, cols = 0;rows < kBoardRows + 1;rows++)
    {
        term.move_to( rows, cols);
        term.put("|");
    }
    for(int rows = 1, cols = kBoardCols + 1;rows < kBoardRows + 1;rows++)
    {
        term.move_to( rows, cols);
        term.put("|");
    }

    term.move_to(6, 6);
    term.put("click 'q' to exit...");

    while(snake::go_running)
    {
        auto ch = term.read_char();
        if(!ch) break;
        if(* ch == 'q') break;
    }

    return 0;
}
