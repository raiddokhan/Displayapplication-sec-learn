# # configure_vtk.cmake
# set(VTK_DIR "${VTK_DIR}")
# find_package(VTK REQUIRED NO_MODULE)

# configure_vtk.cmake
set(VTK_DIR "${VTK_DIR}")
find_package(VTK REQUIRED NO_MODULE)

# Include directories for VTK
include_directories(${VTK_INCLUDE_DIRS})

# Link libraries for VTK
foreach(lib ${VTK_LIBRARIES})
    target_link_libraries(MyProject PRIVATE ${lib})
endforeach()

message(STATUS "VTK configured successfully.")R
