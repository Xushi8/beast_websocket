find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.81.0 CONFIG REQUIRED COMPONENTS headers)
