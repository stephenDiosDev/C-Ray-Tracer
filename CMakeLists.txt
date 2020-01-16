cmake_minimum_required(VERSION 3.0)
project(RayTracing_Simple VERSION 1.0 LANGUAGES C CXX)

#[[
        OpenGL
]]
find_package(OpenGL REQUIRED)

#[[
        GLFW
]]
set(GLFW_DIR external/glfw)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory(${GLFW_DIR})

#[[
        glad
]]
set(GLAD_DIR
    external/glad
    )
set(GLAD_SOURCES
    external/glad/src/glad.c
    )
add_library(glad ${GLAD_SOURCES})
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)

#[[
	stb image
]]
set(STB_DIR
	external/stb
	)

#[[
        Headers
]]
set(HEADERS
   include/vec3f.hpp
   include/vec2f.hpp
   include/mat3f.hpp
   include/mat4f.hpp
   include/grid2.hpp
   include/ray.hpp
   include/ray_intersect.hpp
   include/plane.hpp
   include/sphere.hpp
   include/vec2i.hpp
   include/common_matrices.hpp
   include/triangle.hpp
   include/triangle.tpp
   include/obj_mesh.hpp
   include/obj_mesh_file_io.hpp
   include/image.hpp
   include/timer.hpp
   )

#[[
        Sources
]]
set(SOURCES
    src/main.cpp
    src/vec3f.cpp
    src/vec2f.cpp
    src/vec2i.cpp
    src/mat4f.cpp
    src/mat3f.cpp
	src/ray.cpp
    src/ray_intersect.cpp
    src/sphere.cpp
	src/plane.cpp
    src/common_matrices.cpp
    src/triangle.cpp
    src/obj_mesh.cpp
    src/obj_mesh_file_io.cpp
    src/image.cpp
    )

#[[
        Executable
]]
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE include
    PRIVATE external
    PRIVATE ${GLFW_DIR}/include
    PRIVATE ${GLAD_DIR}/include
	PRIVATE ${STB_DIR}
    )

target_compile_definitions(${PROJECT_NAME}
    PRIVATE -DGLFW_INCLUDE_NONE
    )

if(MSVC)
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE -D_USE_MATH_DEFINES
		PRIVATE -DNOMINMAX
        )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    MACOSX_BUNDLE TRUE
    MACOSX_FRAMEWORK_IDENTIFIER org.cmake.${PROJECT_NAME}
    )

target_compile_definitions(${PROJECT_NAME}
    PRIVATE GLFW_INCLUDE_NONE
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${OPENGL_gl_LIBRARY}
    PRIVATE glfw
    PRIVATE ${GLFW_LIBRARIES}
    PRIVATE glad
    PRIVATE ${GLAD_LIBRARIES}
    PRIVATE ${CMAKE_DL_LIBS}
    )



