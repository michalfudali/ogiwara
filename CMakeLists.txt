cmake_minimum_required (VERSION 3.8)

set (CMAKE_C_STANDARD_REQUIRED C17)
set (CMAKE_CXX_STANDARD_REQUIRED C++20)

project ("ogiwara")

# Dodaj źródło do pliku wykonywalnego tego projektu.
add_executable (ogiwara "ogiwara.cpp")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(ogiwara PRIVATE Threads::Threads)

find_package(Boost REQUIRED log log_setup)
target_link_libraries(ogiwara PRIVATE ${Boost_LIBRARIES})
