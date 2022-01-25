# Skydiving Video Editor
[![macOS](https://svgshare.com/i/ZjP.svg)](https://svgshare.com/i/ZjP.svg)
![C++](https://img.shields.io/badge/-c++-black?logo=c%2B%2B&style=social)

## What is this?
*Honestly, I'm not entirely sure yet.*

This project is initially intended as a personal re-introduction to C++ and Object-Oriented development.
The plan is to actively learn about some modern C++ features, video processing, etc., developing feature ideas as I go.

Longer term, I hope for the project to become a feature-rich, easy-to-use utility that actually solves a problem! 

## Documentation
- [Ideas for the project](ideas.md)
- [Tasks](tasks.md) *(this isn't Trello or Jira-ready yet...)*
- [Useful references, links, and tips](references.md)

## Development

> **NOTE:<br /> For now, this project is being developed and run solely on an M1 (Apple Silicon) MacBook Pro.<br />There is no guarantee it will work on anything else.**<br />
> *^ adds "Docker development environment" to  `ideas.md` ^*

### Requirements
*Soon-to-be-updated*

### Getting started
1. Clone the repository
    ```
    git clone https://github.com/mafull/skydiving-video-editor.git
    ```
1. ... *Soon-to-be-updated*


### Directory structure
```
scripts -> Utilities for development/deployment
src     -> Source and header files compiled into the final executable
tests   -> Test files
```

### Source Control Strategy
Let's keep it simple...
* `main` is the primary branch
* `feature/<name>` is for developing new features
* `bugfix/<name>` is for fixing bugs
