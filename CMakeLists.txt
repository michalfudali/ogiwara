cmake_minimum_required (VERSION 3.18)

project ("ogiwara")

add_executable (ogiwara "ogiwara.cpp")

target_compile_features(ogiwara PUBLIC cxx_std_20)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(ogiwara PRIVATE Threads::Threads)

find_package(Boost REQUIRED log log_setup)

target_link_libraries(ogiwara PRIVATE ${Boost_LIBRARIES})
