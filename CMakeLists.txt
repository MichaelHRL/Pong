cmake_minimum_required(VERSION 3.12.1)

project(Pong LANGUAGES CXX)

add_library(PongLibrary STATIC Source/Opponent.cpp Source/Ball.cpp Source/Player.cpp Source/Game.cpp)
set(SFMLPath "C:/ThirdParty/SFML-2.5.0/") # This needs to be changed for different setups!
string(CONCAT SFMLIncludePath ${SFMLPath} "include")
target_include_directories(PongLibrary PUBLIC Include ${SFMLIncludePath})
target_compile_features(PongLibrary PUBLIC cxx_std_17)
string(CONCAT SFMLAudioLibPath ${SFMLPath} "bin/sfml-audio-2.dll")
string(CONCAT SFMLOpenALLibPath ${SFMLPath} "bin/openal32.dll")
string(CONCAT SFMLGraphicsLibPath ${SFMLPath} "bin/sfml-graphics-2.dll")
string(CONCAT SFMLWindowLibPath ${SFMLPath} "bin/sfml-window-2.dll")
string(CONCAT SFMLSystemLibPath ${SFMLPath} "bin/sfml-system-2.dll")
target_link_libraries(PongLibrary PUBLIC 
  ${SFMLAudioLibPath} 
  ${SFMLOpenALLibPath}
  ${SFMLGraphicsLibPath}
  ${SFMLWindowLibPath}
  ${SFMLSystemLibPath}
)

add_executable(PongApplication Application/Main.cpp)
target_link_libraries(PongApplication PUBLIC PongLibrary)