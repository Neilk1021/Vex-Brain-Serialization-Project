# CMakeList.txt : CMake project for ReadProj, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (ReadProj "ReadProj.cpp" "ReadProj.h" "Button.cpp")

# TODO: Add tests and install targets if needed.
 find_package(matplot++ CONFIG REQUIRED)
 target_link_libraries(ReadProj PRIVATE Matplot++::cimg std::filesystem Matplot++::matplot Matplot++::nodesoup)