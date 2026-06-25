cmake_minimum_required(VERSION 3.20)

project(Oasis LANGUAGES C CXX)

add_library(Oasis STATIC
	Libraries/glad.c
	Source/Oasis/Camera.cpp
	Source/Oasis/GraphicalObject.cpp
	Source/Oasis/IOModel.cpp
	Source/Oasis/InputManager.cpp
	Source/Oasis/Light.cpp
	Source/Oasis/Model.cpp
	Source/Oasis/RealTimePlot.cpp
	Source/Oasis/RenderSystem.cpp
	Source/Oasis/Shader.cpp
	Source/Oasis/Texture.cpp
	Source/Headers/Camera.h
	Source/Headers/GraphicalObject.h
	Source/Headers/IOModel.h
	Source/Headers/InputManager.h
	Source/Headers/Light.h
	Source/Headers/Model.h
	Source/Headers/RealTimePlot.h
	Source/Headers/RenderSystem.h
	Source/Headers/Shader.h
	Source/Headers/Texture.h
	Source/Shaders/PointShader3D.fs
	Source/Shaders/PointShader3D.vs
)

target_compile_features(Oasis PUBLIC cxx_std_20)

target_include_directories(Oasis
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/Source/Headers"
		"${CMAKE_CURRENT_SOURCE_DIR}/Libraries/Include"
)

find_package(OpenGL REQUIRED)
target_link_libraries(Oasis PUBLIC OpenGL::GL)

set(OASIS_BUNDLED_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Libraries/lib")
set(OASIS_CAN_USE_BUNDLED_MSVC_DEPS OFF)

if(WIN32 AND MSVC
	AND EXISTS "${OASIS_BUNDLED_LIB_DIR}/glfw3.lib"
	AND EXISTS "${OASIS_BUNDLED_LIB_DIR}/freetype.lib")
	set(OASIS_CAN_USE_BUNDLED_MSVC_DEPS ON)
endif()

option(OASIS_FORCE_SYSTEM_DEPS "Ignore Oasis bundled Windows libraries and use package-manager/system dependencies." OFF)

if(OASIS_CAN_USE_BUNDLED_MSVC_DEPS AND NOT OASIS_FORCE_SYSTEM_DEPS)
	add_library(oasis_bundled_glfw3 STATIC IMPORTED)
	set_target_properties(oasis_bundled_glfw3 PROPERTIES
		IMPORTED_LOCATION "${OASIS_BUNDLED_LIB_DIR}/glfw3.lib"
	)

	add_library(oasis_bundled_freetype STATIC IMPORTED)
	set_target_properties(oasis_bundled_freetype PROPERTIES
		IMPORTED_LOCATION "${OASIS_BUNDLED_LIB_DIR}/freetype.lib"
	)

	target_link_libraries(Oasis
		PUBLIC
			oasis_bundled_glfw3
			oasis_bundled_freetype
			user32
			gdi32
			shell32
	)
else()
	find_package(glfw3 CONFIG QUIET)
	if(TARGET glfw)
		target_link_libraries(Oasis PUBLIC glfw)
	elseif(TARGET glfw3)
		target_link_libraries(Oasis PUBLIC glfw3)
	else()
		find_package(PkgConfig REQUIRED)
		pkg_check_modules(GLFW3 REQUIRED IMPORTED_TARGET glfw3)
		target_link_libraries(Oasis PUBLIC PkgConfig::GLFW3)
	endif()

	find_package(Freetype REQUIRED)
	target_link_libraries(Oasis PUBLIC Freetype::Freetype)
endif()

if(WIN32)
	target_compile_definitions(Oasis PUBLIC NOMINMAX)
endif()

if(MSVC)
	target_compile_options(Oasis PRIVATE /W3)
else()
	target_compile_options(Oasis PRIVATE -Wall -Wextra -Wpedantic)
endif()
