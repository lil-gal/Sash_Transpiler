cmake_minimum_required(VERSION 3.10)

# Set the project name and language
project(MyCProject C)

# Specify the C standard (e.g., C11 or C99)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add Directories
include_directories(Transpiler Lexer Parser Generator)
# Add the executable target and its source files
add_executable(MyProjectExecutable program.c Transpiler/Lexer/Lexer.c Transpiler/Parser/Parser.c Transpiler/Generator/Generator.c )
# Transpiler/methods.c Transpiler/file.c