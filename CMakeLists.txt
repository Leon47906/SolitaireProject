cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main WIN32 src/main.cpp)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Window sfml-main)

install(TARGETS main DESTINATION bin)   # Ensures solitaire.exe is installed
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/Sprites DESTINATION bin/src)
include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "NSIS") # Windows Installer
set(CPACK_PACKAGE_NAME "Solitaire")
set(CPACK_PACKAGE_VERSION "1.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Solitaire")
include(CPack)