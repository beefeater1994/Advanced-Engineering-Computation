add_library(shooting_game_lib shoot-3-class.cpp shoot-3-class.h)
target_include_directories(shooting_game_lib PUBLIC .)
target_link_libraries(shooting_game_lib simplewindow ysglfontdata)
