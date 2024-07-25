# C++17 Generator PoC

This PoC shows design of the generated code for the following Zserio features:

- Structure Types
- Choice Types
- Union Types
- Parameterized Types
- Templates
- Packed Arrays
- Offsets
- Functions

## Directory Structure

| Folder/File  | Description |
|--------------|-------------|
| └─`gen` | Contains PoC of generated C++ sources |
| └─`runtime` | Contains Zserio C++ runtime library |
| `CMakeLists.txt` | CMake configuration file |
| `main.cpp` | Simple main application |
| `test.zs` | Zserio schema which was used to generate C++ sources |
| `README.md` | This readme file in Markdown |

## Usage

```
mkdir build
cd build
cmake ..
cmake --build .
./cpp17_poc
```
