#pragma once
#include <iostream>
#include <algorithm>
#include <random>
#include <cstdint>
#include <filesystem>
#include <thread>
#include <mutex>
#include <vector>
#include <sstream>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif
enum class TetrominoType
{
  I, O, T, S, Z, J, L
};

class Tetromino {
    using Board = std::vector<std::vector<int>>;
  public:
    Tetromino();
    void set_type(TetrominoType t);
    void draw_tetromino(Board& vec);
    void rotate();
    [[nodiscard]] std::vector<std::vector<int>> get_current_shape() const;

    int x = 0, y = 0;
    int rotation = 0;
    TetrominoType type;
  private:
    std::vector<std::vector<std::vector<int>>> pieces;
    int previous_x = 0, previous_y = 0, previous_rotation = 0;
};

class Tetris {
  public:
    using Board = std::vector<std::vector<int>>;
    Tetris(uint8_t width, uint8_t height);
    bool draw_board();
    void render(bool track_input = true, size_t delay = 10);
    void move_right();
    void move_left();
    void move_down();
    void rotate();
    static void clear_screen();
    bool check_collision(int16_t new_x, int16_t new_y, const std::vector<std::vector<int>>& new_shape);
    void lock_piece();
    void clear_lines();
    void handle_input();
    bool is_game_over();
    void draw_ui() const;

    [[nodiscard]] int get_score() const;
    [[nodiscard]] const Board& GetCurrentBoard() const;
    [[nodiscard]] const Board& GetStaticBoard() const;
    [[nodiscard]] bool IsGameRunning() const;
    [[nodiscard]] const Tetromino& get_active_piece() const;
    [[nodiscard]] const Tetromino& get_next_piece() const;
    [[nodiscard]] uint8_t get_height() const;
    [[nodiscard]] uint8_t get_width() const;

  private:
    Board current_board;
    Board static_board;
    Tetromino active_piece;
    Tetromino next_piece;
    uint8_t width;
    uint8_t height;
    int score = 0;
    bool game_running = true;
    std::thread gravity_thread;
    std::mutex collision_check_mutex;
};
