cmake_minimum_required(VERSION 3.28.3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_CXX_COMPILER "clang++-17")
set(CMAKE_CXX_FLAGS "-Ofast -ffast-math -flto -march=native -mtune=native -fstack-protector -msse2 -mavx -g -Wall -Wextra")
set(CMAKE_LINKER "lld-17")

project(
    nyisip
    VERSION 1.0.0
    DESCRIPTION "Program untuk menyisipkan suatu file ke dalam suatu file."
    LANGUAGES CXX
)

file(
	GLOB_RECURSE 
	SOURCE_FILES
	"src/*.cc"
	"src/**/*.cc"
)

include_directories("include")

add_executable(
    nyisip
    ${SOURCE_FILES}
)
