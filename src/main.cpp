#include <iostream>
#include <csignal>
#include "terminal_chars.h"
#include "Tetris/Tetris.h"

void signal_handler(int signal) {
  std::cout << CURSOR_POSITION(23, 0) << FG_RED << "Game Over\n";
  std::cout << RESET;
  exit(signal);
}

int main(int argc, char* argv[]) {
  std::signal(SIGINT, signal_handler);
  Tetris game(10, 20);
  Tetris::clear_screen();
  while(game.IsGameRunning())
  {
    std::cout << FG_YELLOW;
    game.render();
    std::cout << RESET;
  }
  raise(SIGINT);
}