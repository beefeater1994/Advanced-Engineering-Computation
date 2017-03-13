add_library(stringParser parserClass.cpp parserClass.h)

add_library(Fgets Fgets.cpp Fgets.h)

add_executable(parserSample main.cpp)
target_link_libraries(parserSample stringParser Fgets)

