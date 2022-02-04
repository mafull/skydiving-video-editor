# Skydiving Video Editor
[![macOS](https://svgshare.com/i/ZjP.svg)](https://svgshare.com/i/ZjP.svg)
![C++](https://img.shields.io/badge/-c++-black?logo=c%2B%2B&style=social)

## What is this?
*Honestly, I'm not entirely sure yet.*

This project is initially intended as a personal re-introduction to C++ and Object-Oriented development.
The plan is to actively learn about some modern C++ features, video processing, etc., developing project ideas as I go.

Longer term, I hope for the project to become a feature-rich, easy-to-use utility that actually solves a problem! 

## Documentation
- [Ideas for the project](ideas.md)
- [Tasks](tasks.md) *(this isn't Trello or Jira-ready yet...)*
- [Useful references, links, and tips](references.md)

## Development

> **NOTE:<br /> For now, this project is being developed and run solely on an M1 (Apple Silicon) MacBook Pro, in VS Code. There is no guarantee it will work on anything else.**<br />
> *^ adds "Docker development environment" to  `ideas.md` ^*

### Requirements

#### CMake
```
brew install cmake
```
#### VSCode extensions
* [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
* [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

#### FFmpeg
```
brew install ffmpeg
```

### Getting started
Ensure all requirements are met before following this.
```
git clone https://github.com/mafull/skydiving-video-editor.git
cd skydiving-video-editor

# Build the project
./scripts/configure
./scripts/build

# Run it
./scripts/run <directory_to_search_for_video_files>
```

#### VS Code's CMake Tools
This extension will automatically re-run `cmake` whenever you save a change to CMakelists.txt.

You can trigger a build by clicking `Build` on the bottom task bar, or from the extension itself.

### Directory structure
```
scripts -> Utilities for development/deployment
src     -> Source and header files compiled into the final executable
tests   -> Test files
```

### Source Control Strategy
Let's keep it simple...
* `main` is the primary branch
* `feature/<name>` for developing new features
* `bugfix/<name>` for fixing bugs
