# Create the memorypool library
add_library(memory_pool src/memory_pool.c)

# Include the header files directory
target_include_directories(memory_pool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)