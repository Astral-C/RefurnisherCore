## Refurnisher Core
### What is this?
A C++ library for handling a variety of formats from Luigi's Mansion

#### Haven't you done this before?
Yeah, though previous attempts were unfocused and poorly written due to lack of experience.

### Goals
Provide functions for converting to/from in-game formats to intermediate formats, keeping bloat to a minimum. 

### Features
 - JMP <-> JSON Serialization and Deserialization
 - (Planned) Collision generation from model formats and a custom intermediate format
 - (Planned) Bin Model Loading/Saving


### How to build
Clone the repository with `git clone --recursive`, then simply run `cmake .` and `make`. 

Currently there are no options to disable building and running the tests, though this does not matter as the library will still build properly regardless. This will be fixed in the future.