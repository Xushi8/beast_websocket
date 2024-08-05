add_library(network "")

file(GLOB sources CONFIGURE_DEPENDS src/*.c src/*.cpp)
file(GLOB header CONFIGURE_DEPENDS include/*.h include/*.hpp)

target_sources(network
PRIVATE
    ${sources}
PUBLIC
    ${header}
)

target_include_directories(network 
PUBLIC
./include
)
