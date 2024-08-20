# Terminal Tetris

This is a simple terminal-based implementation of the classic Tetris game. It is designed to be built and run on both Windows and Unix-like systems.

## Features

- Classic Tetris gameplay
- Terminal-based UI
- Build using CMake, with scripts provided for convenience

## Controls

- **`a`**: Move left
- **`s`**: Move down
- **`d`**: Move right
- **`w`**: Rotate
- **`q`**: Quit the game

## Building from Source

### Prerequisites

- **CMake**: Required for generating build files.
- **Compiler**: A C++ compiler (e.g., GCC, Clang, MSVC).
- **Ninja** (Windows only)
- **Make** (Unix only)

### On Windows

1. **Install Prerequisites**: Ensure that you have CMake, a C++ compiler, and Ninja installed.

2. **Clone the Repository**:

    ```sh
    git clone https://github.com/bonsall2004/terminal-tetris.git
    cd terminal-tetris
    ```

3. **Build Using CMake and Ninja**:

    ```sh
    build-win.bat
    ```

   This script will generate the necessary build files using CMake and then build the project using Ninja.

### On Unix-like Systems

1. **Install Prerequisites**: Ensure that you have CMake, `make`, and a C++ compiler installed.

2. **Clone the Repository**:

    ```sh
    git clone https://github.com/bonsall2004/terminal-tetris.git
    cd terminal-tetris
    ```

3. **Build Using CMake and Make**:

    ```sh
   chmod +x ./build-unix.sh
   
    ./build-unix.sh
    ```

   This script will generate the necessary build files using CMake and then build the project using `make`.

## Running the Game

After building the project, you can run the game executable:

- **On Windows**: Navigate to the `binaries/win` directory and run `tetris.exe`.
- **On Unix-like Systems**: Navigate to the `binaries/linux` directory and run `./tetris`.

## Releases

If you prefer not to compile the game from source, you can download precompiled binaries from the [latest releases](https://github.com/bonsall2004/terminal-tetris/releases) page.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
