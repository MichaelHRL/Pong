cmake_minimum_required(VERSION 3.12.1)

project(Pong LANGUAGES CXX)

add_library(PongLibrary STATIC Source/Opponent.cpp Source/Ball.cpp Source/Player.cpp Source/Game.cpp)
target_include_directories(PongLibrary PUBLIC Include "C:/ThirdParty/SFML-2.5.0/include")
target_compile_features(PongLibrary PUBLIC cxx_std_17)
target_link_libraries(PongLibrary PUBLIC "C:/ThirdParty/SFML-2.5.0/bin/sfml-graphics-2.dll" "C:/ThirdParty/SFML-2.5.0/bin/sfml-window-2.dll" "C:/ThirdParty/SFML-2.5.0/bin/sfml-system-2.dll")

add_executable(PongApplication Application/Main.cpp)
target_link_libraries(PongApplication PUBLIC PongLibrary)