# Tetris (C++ / raylib)

A small, single-player Tetris clone written in C++ using raylib. It’s aimed at learners and hobbyists who want a clean, readable example of a complete “game loop + gameplay rules + simple UI” desktop game.

## Screenshots

<div align="center">
  <img src="PlayingGame.png" alt="Gameplay" width="45%" />
  &nbsp;&nbsp;&nbsp;&nbsp;
  <img src="GameOver.png" alt="Game Over" width="45%" />
</div>

## Key Features

- Falling tetromino gameplay with the 7 standard pieces (I, O, T, S, Z, J, L)
- Rotation and collision checks against a 20×10 grid
- Next-piece preview
- Score tracking (line clears + soft drop points)
- Game-over detection + restart on key press
- Background music + sound effects (rotate + line clear)

## System Overview

### Architecture

```text
┌───────────────────────────────────────────────────────────────┐
│ main loop (src/main.cpp)                                      │
│ - InitWindow + SetTargetFPS                                   │
│ - per-frame: HandleInput + Draw                               │
│ - every ~0.3s: MoveBlockDown (gravity)                        │
└───────────────┬───────────────────────────────┬───────────────┘
                │                               │
                v                               v
┌──────────────────────────────┐     ┌───────────────────────────┐
│ Game (src/game.*)            │     │ Audio (raylib)            │
│ - current/next block         │     │ - music stream            │
│ - score + gameOver           │     │ - rotate/clear sounds     │
│ - movement/rotation/locking  │     └───────────────────────────┘
└───────────────┬──────────────┘
                │
                v
┌───────────────────────────────────────────────────────────────┐
│ Grid (src/grid.*)                                             │
│ - 20x10 int matrix (0 = empty, >0 = block id/color)           │
│ - row clear + shift down                                      │
└───────────────────────────────────────────────────────────────┘
```

### Components and responsibilities

- Entry point and render loop: [src/main.cpp](src/main.cpp)

  - Creates the window, loads the font, runs the main loop at 60 FPS.
  - Every ~0.3s triggers gravity (automatic downward movement).
  - Draws UI labels and delegates game rendering to `Game::Draw()`.

- Game rules / state machine: [src/game.h](src/game.h) and [src/game.cpp](src/game.cpp)

  - Owns the `Grid`, current block, next block, score, and game-over flag.
  - Loads and plays audio assets on startup.
  - Handles player inputs (left/right/down/rotate) and validates movement.
  - Locks pieces into the grid and clears filled rows.

- Grid storage and row clearing: [src/grid.h](src/grid.h) and [src/grid.cpp](src/grid.cpp)

  - Maintains a fixed 20×10 integer matrix (`grid[row][col]`).
  - `0` means empty; non-zero values map to block IDs / colors.
  - Clears full rows and shifts rows downward.

- Tetromino geometry and movement: [src/block.h](src/block.h) and [src/block.cpp](src/block.cpp)

  - Stores per-rotation cell coordinates in a 4×4 local grid.
  - Applies row/column offsets to place the block in the playfield.

- Tetromino definitions (piece shapes): [src/blocks.cpp](src/blocks.cpp)

  - Defines the 7 tetromino subclasses and their rotation states.

- Color palette: [src/colors.h](src/colors.h) and [src/colors.cpp](src/colors.cpp)

  - Defines UI background colors and the piece color lookup table.

- Simple coordinate struct: [src/position.h](src/position.h) and [src/position.cpp](src/position.cpp)

### Data flow (frame lifecycle)

1. `main()` processes audio stream updates and polls keyboard input.
2. `Game::HandleInput()` translates key presses into gameplay actions.
3. Every 0.3 seconds, `Game::MoveBlockDown()` applies gravity.
4. When a block can’t move down, `Game::LockBlock()` writes it into `Grid::grid`, checks for game over, then clears rows.
5. `Game::Draw()` renders the grid, current block, and next-block preview.

## Tech Stack

- Language: C++ (compiled with `-std=c++14` via the Makefile)
- Game / rendering / input / audio: raylib
- Build system: GNU Make (raylib’s cross-platform Makefile template)
- Editor tooling: VS Code tasks and launch configs

## Repository Structure

- [src/](src/) — all gameplay and rendering source code
- [Font/](Font/) — font asset loaded at runtime
- [Sounds/](Sounds/) — audio assets loaded at runtime
- [Makefile](Makefile) — build configuration (desktop-focused)
- [.vscode/](.vscode/) — VS Code tasks/build + debug launch configurations
- [PlayingGame.png](PlayingGame.png), [GameOver.png](GameOver.png) — screenshots
- [LICENSE](LICENSE) — MIT license

## Getting Started (Local Development)

### Prerequisites

- A C++ compiler
  - macOS: Xcode Command Line Tools (clang)
  - Linux: g++ or clang
  - Windows: MinGW (w64devkit) or another toolchain capable of linking raylib
- raylib installed on your system (or available via your toolchain)
- Make

### Install steps

1. Clone the repository.
2. Install raylib for your platform.

This repo is already set up to compile via `make` (see [Makefile](Makefile)) and to run builds via VS Code tasks (see [.vscode/tasks.json](.vscode/tasks.json)).

### Environment variables

This project does not use environment variables for configuration. Runtime resources are loaded from relative paths.

| Name   | Purpose | Example | Required? |
| ------ | ------- | ------- | --------- |
| (none) | (none)  | (n/a)   | No        |

### Run commands

The Makefile expects two key variables:

- `PROJECT_NAME` — output binary name
- `OBJS` — list/glob of source files to compile (this repo uses `src/*.cpp`)

Examples:

Build (release):

```bash
make PROJECT_NAME=main OBJS=src/*.cpp
```

Build (debug):

```bash
make PROJECT_NAME=main OBJS=src/*.cpp BUILD_MODE=DEBUG
```

Run:

```bash
./main
```

Notes:

- On macOS, the Makefile automatically uses `clang++` when `PLATFORM_OS=OSX` (detected via `uname`).
- If raylib is installed via Homebrew on Apple Silicon, the Makefile includes `/opt/homebrew/include` and `/opt/homebrew/lib` automatically when those paths exist.

Testing / linting:

- There are no unit tests or lint targets in this repository today.

## Configuration

### Build configuration

Primary build configuration lives in [Makefile](Makefile).

Notable variables you can override at build time:

- `BUILD_MODE` — `DEBUG` or `RELEASE` (affects `-g/-O0` vs `-s/-O1`)
- `PLATFORM` — defaults to `PLATFORM_DESKTOP`
- `RAYLIB_LIBTYPE` — `STATIC` or `SHARED` (mostly relevant on Linux)
- `DESTDIR`, `RAYLIB_INSTALL_PATH`, `RAYLIB_H_INSTALL_PATH` — where raylib is installed

### VS Code configuration

- Build tasks: [.vscode/tasks.json](.vscode/tasks.json)
  - Provides `build debug` and `build release` tasks across Windows/macOS/Linux.
- Debug/Run launch configs: [.vscode/launch.json](.vscode/launch.json)
  - Uses the output binary name of the active file base name.

## Usage

### Core gameplay workflow

1. Start the game (run the compiled binary).
2. A piece spawns at the top and falls automatically every 0.3 seconds.
3. Use the arrow keys to position and rotate pieces.
4. Fill rows to clear them and increase your score.
5. If a new piece cannot be placed, the game shows “Game Over”.
6. Press any key to reset and play again.

### Controls

- Left Arrow: move piece left
- Right Arrow: move piece right
- Down Arrow: soft drop (moves down once + +1 score)
- Up Arrow: rotate
- Any key (when game over): restart

## API / Interfaces

This is a local desktop game. There is no network API.

## Data Model

There is no external database.

In-memory state:

- `Grid::grid` is a 20×10 integer matrix where `0` is empty and non-zero values represent block IDs (used to look up colors).
- `Game` owns `currentBlock`, `nextBlock`, `score`, and `gameOver`.

## Testing & Quality

- Tests: none present.
- CI: none present.
- Recommended manual checks:
  - Confirm font loads correctly on launch.
  - Confirm audio device opens and music plays.
  - Confirm row clearing and scoring works for 1–3 line clears.

## Deployment

No deployment configuration is present. This project is currently intended to be compiled and run locally as a native desktop executable.

If you want distributable builds, consider adding:

- A deterministic build output directory
- A packaging step (e.g., zip + assets; or platform-native bundling)

## Troubleshooting / FAQ

### The window opens but crashes or shows missing assets

The game loads assets using relative paths:

- Font loaded from Font/ARCADECLASSIC.TTF
- Audio loaded from Sounds/music.mp3, Sounds/rotate.mp3, Sounds/clear.mp3

Run the executable from the repository root so those paths resolve correctly.

### Compilation fails with missing raylib headers

- Ensure raylib headers are discoverable by your compiler.
- The Makefile adds include paths for `/opt/homebrew/include` automatically if present (macOS), but other setups may require adjusting `DESTDIR` or installing raylib into a standard include path.

### Linking fails on macOS

The Makefile links raylib and macOS frameworks:

- `-framework OpenGL -framework OpenAL -framework Cocoa -framework IOKit`

If you installed raylib somewhere non-standard, ensure your library path is discoverable (or add to `LDFLAGS`).

### No sound / audio device issues

Audio is initialized during `Game` construction. If the audio device cannot be opened, behavior may vary by OS/driver.
Confirm your system audio is working and that raylib is built with audio support.

## Roadmap

This repo does not contain an issues list or explicit TODO roadmap. Based on current code structure, likely next steps are:

- Add scoring for 4-line clears (currently only 1–3 lines are awarded points)
- Add a “hard drop” control
- Add level/speed progression (gravity interval is currently fixed at 0.3s)
- Add a proper build output directory and release packaging

## License

MIT License — see [LICENSE](LICENSE).

## Contributing

Contributions are welcome.

Suggested workflow:

1. Fork the repo.
2. Create a feature branch.
3. Keep changes focused and consistent with existing code style.
4. Open a pull request with a short description and screenshots if UI changes.

---

## Evidence Notes

Primary sources used to write this README:

- [src/main.cpp](src/main.cpp)
- [src/game.h](src/game.h) and [src/game.cpp](src/game.cpp)
- [src/grid.h](src/grid.h) and [src/grid.cpp](src/grid.cpp)
- [src/block.h](src/block.h) and [src/block.cpp](src/block.cpp)
- [src/blocks.cpp](src/blocks.cpp)
- [src/colors.h](src/colors.h) and [src/colors.cpp](src/colors.cpp)
- [Font/](Font/) and [Sounds/](Sounds/)
- [Makefile](Makefile)
- [.vscode/tasks.json](.vscode/tasks.json) and [.vscode/launch.json](.vscode/launch.json)
- [LICENSE](LICENSE)

Needs confirmation:

- Windows installation story: the repo includes Windows-specific VS Code task settings, but does not include a pinned raylib binary or package-manager configuration. If you want a guaranteed Windows setup, confirm your intended toolchain (w64devkit vs MSVC) and raylib installation method, then document it here.
