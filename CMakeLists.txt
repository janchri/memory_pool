cmake_minimum_required(VERSION 3.10.0)

# Define project name and version
project(App VERSION 0.1.0 LANGUAGES C)

# Add subdirectory for the memorypool library
add_subdirectory(lib)

# Define the main executable
add_executable(app src/main.c)

# Link memorypool library to the executable
target_link_libraries(app PRIVATE memory_pool)
