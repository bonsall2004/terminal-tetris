#include "Tetris.h"
#include "terminal_chars.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <cstdint>
#include <filesystem>

#ifdef _WIN32
bool is_key_pressed() {
  return _kbhit() != 0;
}

int get_pressed_key() {
  return _getch();
}
#else
bool is_key_pressed() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

int get_pressed_key() {
    struct termios oldt, newt;
    int ch;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

std::ostringstream frame_buffer;
std::ostringstream off_screen_buffer;

void swap_buffer()
{
  frame_buffer.str("");
  frame_buffer << off_screen_buffer.str();
  off_screen_buffer.str("");
}

int generate_random_int(int min, int max)
{
  std::random_device rd;
  std::mt19937 engine(rd());

  std::uniform_int_distribution<int> distribution(min, max);

  return distribution(engine);
}

Tetromino::Tetromino()
{
  set_type(static_cast<TetrominoType>(generate_random_int(0, 6)));
}

void Tetromino::set_type(TetrominoType t)
{
  type = t;
  switch(type)
  {
  case TetrominoType::I:
    pieces = { { { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 } },
               { { 0, 0, 0, 0 }, { 1, 1, 1, 1 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, },
               { { 0, 0, 1, 0 }, { 0, 0, 1, 0 }, { 0, 0, 1, 0 }, { 0, 0, 1, 0 }, },
               { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 1, 1, 1, 1 }, { 0, 0, 0, 0 }, } };
    break;
  case TetrominoType::O:
    pieces = { { { 0, 0, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } } };
    break;
  case TetrominoType::T:
    pieces = { { { 0, 1, 0, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
               { { 0, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 } },
               { { 0, 0, 0, 0 }, { 1, 1, 1, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 } },
               { { 0, 1, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 } } };
    break;
  case TetrominoType::S:
    pieces = { { { 0, 1, 1, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
               { { 1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 } } };
    break;
  case TetrominoType::Z:
    pieces = { { { 1, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },

               { { 0, 1, 0, 0 }, { 1, 1, 0, 0 }, { 1, 0, 0, 0 }, { 0, 0, 0, 0 } } };
    break;
  case TetrominoType::J:
    pieces = { { { 1, 0, 0, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },

               { { 0, 1, 1, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 } },

               { { 0, 0, 0, 0 }, { 1, 1, 1, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 0 } },

               { { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 1, 1, 0, 0 }, { 0, 0, 0, 0 } } };
    break;
  case TetrominoType::L:
    pieces = { { { 0, 0, 1, 0 }, { 1, 1, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },

               { { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 1, 0 }, { 0, 0, 0, 0 } },

               { { 0, 0, 0, 0 }, { 1, 1, 1, 0 }, { 1, 0, 0, 0 }, { 0, 0, 0, 0 } },

               { { 1, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 } } };
    break;
  }
}

void Tetromino::draw_tetromino(Board& vec)
{
  rotation = rotation % pieces.size();

  for(int k = 0; k < 4; ++k)
  {
    for(int l = 0; l < 4; ++l)
    {
      if(pieces[previous_rotation][k][l] != 0) vec[previous_y + k][previous_x + l] = 0;
    }
  }

  for(int k = 0; k < 4; ++k)
  {
    for(int l = 0; l < 4; ++l)
    {
      if(rotation > pieces.size() || k > pieces[rotation].size() || l > pieces[rotation][k].size()) continue;
      if(pieces[rotation][k][l] != 0) vec[y + k][x + l] = 1;
    }
  }
  previous_y = y;
  previous_x = x;
  previous_rotation = rotation;
}

void Tetromino::rotate()
{
  rotation = (rotation + 1) % pieces.size();
}

std::vector<std::vector<int>> Tetromino::get_current_shape() const
{
  return pieces[rotation];
}

Tetris::Tetris(uint8_t width, uint8_t height) : width(width), height(height)
{
  current_board.resize(height);
  static_board.resize(height);
  for(int i = 0; i < height; ++i)
  {
    current_board[i].resize(width);
    static_board[i].resize(width);
    for(int j = 0; j < width; ++j)
    {
      current_board[i][j] = 0;
      static_board[i][j] = 0;
    }
  }
  next_piece.set_type(static_cast<TetrominoType>(generate_random_int(0, 6)));
  gravity_thread = std::thread([this]()
  {
    while(this->game_running)
    {
      move_down();
      std::this_thread::sleep_for(std::chrono::milliseconds(675));
    }
  });
}
bool Tetris::draw_board()
{
  off_screen_buffer << "\033[H";
  off_screen_buffer << "+";
  for(int j = 0; j < width * 2; ++j)
  {
    off_screen_buffer << '-';
  }
  off_screen_buffer << "+\n";
  for(int i = 0; i < height; ++i)
  {
    off_screen_buffer << '|';
    for(int j = 0; j < width; ++j)
    {
      off_screen_buffer << (current_board[i][j] == 0 ? " ." : "[]");
    }
    off_screen_buffer << "|\n";
  }

  off_screen_buffer << "+";
  for(int j = 0; j < width * 2; ++j)
  {
    off_screen_buffer << '-';
  }
  off_screen_buffer << "+\n";

  off_screen_buffer.flush();
  return false;
}

void Tetris::render(bool track_input, size_t delay)
{
  if(!game_running) return;
  current_board = static_board;

  if(track_input) handle_input();
  active_piece.draw_tetromino(current_board);
  draw_board();
  draw_ui();

  std::cout.flush();
  frame_buffer << CURSOR_POSITION(0, 0);
  std::cout << frame_buffer.str();
  swap_buffer();
  std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

void Tetris::move_right()
{
  if(check_collision(active_piece.x + 1, active_piece.y, active_piece.get_current_shape()))
  {
    return;
  }
  active_piece.x += 1;
  std::this_thread::sleep_for(std::chrono::milliseconds(25));
}

void Tetris::move_left()
{
  if(check_collision(active_piece.x - 1, active_piece.y, active_piece.get_current_shape()))
  {
    return;
  }
  active_piece.x -= 1;
  std::this_thread::sleep_for(std::chrono::milliseconds(25));
}

void Tetris::move_down()
{
  if(check_collision(active_piece.x, active_piece.y + 1, active_piece.get_current_shape()))
  {
    lock_piece();
    if(is_game_over())
    {
      game_running = false;
      return;
    }
    return;
  }

  active_piece.y += 1;
  std::this_thread::sleep_for(std::chrono::milliseconds(25));
}

void Tetris::rotate()
{
  Tetromino backup = active_piece;
  active_piece.rotate();
  if(check_collision(active_piece.x, active_piece.y, active_piece.get_current_shape()))
  {
    active_piece = backup;
  }
}

void Tetris::clear_screen()
{

  off_screen_buffer << "\033[2J\033[H";
  off_screen_buffer.flush();
}

bool Tetris::check_collision(int16_t new_x, int16_t new_y, const std::vector<std::vector<int>>& new_shape)
{
  std::lock_guard<std::mutex> lock(collision_check_mutex);

  int shape_height = new_shape.size();
  int shape_width = new_shape[0].size();

  for(int i = 0; i < shape_height; ++i)
  {
    for(int j = 0; j < shape_width; ++j)
    {
      if(new_shape[i][j] != 0)
      {
        int x = new_x + j;
        int y = new_y + i;

        if(x < 0 || x >= width || y < 0 || y >= height)
        {
          return true;
        }

        if(static_board[y][x] != 0)
        {
          return true;
        }
      }
    }
  }
  return false;
}

void Tetris::lock_piece()
{
  auto shape = active_piece.get_current_shape();
  int shape_height = shape.size();
  int shape_width = shape[0].size();

  active_piece.draw_tetromino(current_board);
  if(is_game_over())
  {
    game_running = false;
    return;
  }
  clear_lines();
  static_board = current_board;
  active_piece = next_piece;
  next_piece = Tetromino();
}

void Tetris::clear_lines()
{
  int lines_cleared = 0;
  bool line_was_cleared = false;

  auto is_free_standing = [&](int x, int y)->bool
  {
    if(current_board[y][x] == 0) return false;

    bool left_empty = (x == 0 || current_board[y][x - 1] == 0);
    bool right_empty = (x == width - 1 || current_board[y][x + 1] == 0);
    bool above_empty = (y == 0 || current_board[y - 1][x] == 0);
    bool below_empty = (y == height - 1 || current_board[y + 1][x] == 0);

    return left_empty && right_empty && below_empty && above_empty;
  };

  for(int i = height - 1; i >= 0; --i)
  {
    bool line_full = true;

    for(int j = 0; j < width; ++j)
    {
      if(current_board[i][j] == 0)
      {
        line_full = false;
        break;
      }
    }

    if(line_full)
    {
      ++lines_cleared;
      line_was_cleared = true;
      for(int k = i; k > 0; --k)
      {
        current_board[k] = current_board[k - 1];
      }
      current_board[0].assign(width, 0);

      ++i;
    }
  }

  if(line_was_cleared)
  {
    for(int y = height - 2; y >= 0; --y)
    {
      for(int x = 0; x < width; ++x)
      {
        if(is_free_standing(x, y))
        {
          int drop = 0;
          while(y + drop + 1 < height && current_board[y + drop + 1][x] == 0)
          {
            ++drop;
          }

          if(drop > 0)
          {
            current_board[y + drop][x] = current_board[y][x];
            current_board[y][x] = 0;
          }
        }
      }
    }
  }

  score += lines_cleared * lines_cleared;
}

void Tetris::handle_input()
{
  if(is_key_pressed())
  {
    switch(get_pressed_key())
    {
    case 'a':
      move_left();
      break;
    case 'd':
      move_right();
      break;
    case 's':
      move_down();
      break;
    case 'w':
      rotate();
      break;
    }
  }
}

bool Tetris::is_game_over()
{
  std::vector<std::vector<int>> shape = next_piece.get_current_shape();
  int spawn_x = next_piece.x;
  int spawn_y = next_piece.y;

  const int board_width = current_board[0].size();
  const int board_height = current_board.size();

  for(int row = 0; row < shape.size(); ++row)
  {
    for(int col = 0; col < shape[row].size(); ++col)
    {
      if(shape[row][col] != 0)
      {
        int board_x = spawn_x + col;
        int board_y = spawn_y + row;
        if(board_x < 0 || board_x >= board_width || board_y < 0 || board_y >= board_height)
        {
          game_running = false;
          return true;
        }
        if(current_board[board_y][board_x] != 0)
        {
          game_running = false;
          return true;
        }
      }
    }
  }
  game_running = true;
  return false;
}

void Tetris::draw_ui() const
{

  int x_offset = (width * 2) + 4;
  off_screen_buffer << "\033[1;" << x_offset << "H";
  off_screen_buffer << "+--------------------------+";

  std::string score_str = std::to_string(score);
  off_screen_buffer << "\033[2;" << x_offset << "H";
  off_screen_buffer << "| Score: " << score_str;
  off_screen_buffer << std::string(18 - score_str.length(), ' ');
  off_screen_buffer << "|";

  off_screen_buffer << "\033[3;" << x_offset << "H";
  off_screen_buffer << "|                          |";

  off_screen_buffer << "\033[4;" << x_offset << "H";
  off_screen_buffer << "| Next Piece:              |\n";
  off_screen_buffer << "\033[5;" << x_offset << "H";
  off_screen_buffer << "+--------------------------+\n";
  off_screen_buffer << "\033[6;" << x_offset << "H";
  off_screen_buffer << "|                          |\n";

  std::vector<std::vector<int>> shape = next_piece.get_current_shape();
  for(int i = 0; i < 4; ++i)
  {
    off_screen_buffer << "\033[" << std::to_string(7 + i) << ";" << x_offset << "H";
    off_screen_buffer << "|         ";
    for(int j = 0; j < 4; ++j)
    {
      off_screen_buffer << (shape[i][j] == 0 ? " ." : "[]");
    }
    off_screen_buffer << "         |";
  }
  off_screen_buffer << "\033[11;" << x_offset << "H";
  off_screen_buffer << "|                          |\n";
  off_screen_buffer << "\033[12;" << x_offset << "H";
  off_screen_buffer << "+--------------------------+\n";

  off_screen_buffer.flush();
}

int Tetris::get_score() const
{
  return score;
}
const Tetris::Board& Tetris::GetCurrentBoard() const
{
  return current_board;
}
const Tetris::Board& Tetris::GetStaticBoard() const
{
  return static_board;
}
bool Tetris::IsGameRunning() const
{
  return game_running;
}
const Tetromino& Tetris::get_active_piece() const
{
  return active_piece;
}
const Tetromino& Tetris::get_next_piece() const
{
  return next_piece;
}
uint8_t Tetris::get_height() const
{
  return height;
}
uint8_t Tetris::get_width() const
{
  return width;
}
