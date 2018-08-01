set(PROJECT_NAME "SharedCode")
set(BASE_LIBRARY_NAME "SharedCode")


####################################################
# Настройка CMake сборки
####################################################
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# Информация о текущих настройках
function(print_current_cmake_config)
    # https://cmake.org/Wiki/CMake_Useful_Variables
    message("******** Project compilation settings ***********")
    message("System = ${CMAKE_SYSTEM}")
    message("System name = ${CMAKE_SYSTEM_NAME}")
    message("System version = ${CMAKE_SYSTEM_VERSION}")
    message("CMake Generator = ${CMAKE_GENERATOR}")
    message("CMake build type = ${CMAKE_BUILD_TYPE}")
    message("CC compiler = ${CMAKE_C_COMPILER}")
    message("CXX compiler = ${CMAKE_CXX_COMPILER}")
    message("CC Flags = ${CMAKE_C_FLAGS}")
    message("CC Flags (Debug) = ${CMAKE_C_FLAGS_DEBUG}")
    message("CC Flags (Release) = ${CMAKE_C_FLAGS_RELEASE}")
    message("CXX Flags = ${CMAKE_CXX_FLAGS}")
    message("CXX Flags (Debug) = ${CMAKE_CXX_FLAGS_DEBUG}")
    message("CXX Flags (Release) = ${CMAKE_CXX_FLAGS_RELEASE}")
endfunction()

# Цвета
option(CMAKE_COLOR_MAKEFILE "Color makefile" ON)

# Инициализация проекта
project(${PROJECT_NAME})

enable_testing()



####################################################
# Библиотеки в системе
####################################################
message("******** System libs search ***********")

# # Список фреймворков OSX для MoltenVK
# if(APPLE)
#     set(LIBS_LINK_NAMES ${LIBS_LINK_NAMES}
#         "-framework Metal"
#         "-framework IOSurface"
#         "-framework QuartzCore"
#         "-framework IOKit"
#         "-framework Foundation")
# endif()

##############
# Системные библиотеки
if(UNIX)
    set(LIBS_LINK_NAMES ${LIBS_LINK_NAMES} m)
endif()



####################################################
# Библиотеки из исходников
####################################################
message("******** Source libraries search ***********")

##############
# GLEW library
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/glew")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include")
set(BASE_LIBRARY_INCLUDES ${BASE_LIBRARY_INCLUDES} "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include")
set(LIBS_LINK_NAMES ${LIBS_LINK_NAMES} libglew)
add_definitions(-DGLEW_STATIC)


##############
# GLFW library
# Устанавливаем нужные нам настройки
message("GLFW including")
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
# if(APPLE)
#     option(GLFW_VULKAN_STATIC "Use the Vulkan loader statically linked into application" ON)
# else()
#     option(GLFW_VULKAN_STATIC "Use the Vulkan loader statically linked into application" OFF)
# endif()
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
if (WIN32)
    option(GLFW_USE_HYBRID_HPG "Force use of high-performance GPU on hybrid systems" ON)
endif()
if (APPLE)
    option(GLFW_USE_CHDIR "Make glfwInit chdir to Contents/Resources" ON)
    option(GLFW_USE_MENUBAR "Populate the menu bar on first window creation" ON)
    option(GLFW_USE_RETINA "Use the full resolution of Retina displays" ON)
endif()
if (UNIX AND NOT APPLE)
    option(GLFW_USE_WAYLAND "Use Wayland for window creation" OFF)
    option(GLFW_USE_MIR     "Use Mir for window creation" OFF)
endif()
if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" ON)
endif()

# Добавляем библиотеку
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include")
set(BASE_LIBRARY_INCLUDES ${BASE_LIBRARY_INCLUDES} "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include")
set(LIBS_LINK_NAMES ${LIBS_LINK_NAMES} glfw ${GLFW_LIBRARIES})
message("GLFW link libraries list: ${GLFW_LIBRARIES}")


##############
# ImGUI library
message("ImGUI including")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui")
set(BASE_LIBRARY_INCLUDES ${BASE_LIBRARY_INCLUDES} ${IMGUI_LIBRARY_INCLUDES})
set(LIBS_LINK_NAMES ${LIBS_LINK_NAMES} ${IMGUI_LIBRARY_NAME} ${IMGUI_LIBRARY_LINK_LIBS})
message("ImGUI added")


####################################################
# Настройка сборки текущего проекта
####################################################
# Не кастомные флаги компиляции
if(APPLE)
    set(PROJECT_LINK_FLAGS "-stdlib=libc++")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -Wall -g")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2 -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -Wall -g -stdlib=libc++ -std=c++11")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -Wall -stdlib=libc++ -std=c++11")
elseif (UNIX AND NOT APPLE)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -O0 -Wall -g")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O2 -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -Wall -g -std=c++11")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -Wall -std=c++11")
elseif (MSVC)
    # Сразу отключаем ненужные предупреждения на Windows
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    # микрософту не нужен флаг -std=c++11
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O0 /Wall /g")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /O0 /Wall /g")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O0 /Wall /g")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /O0 /Wall /g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Wall")
    message("Microsoft VS flags")
elseif (WIN32)
    # микрософту не нужен флаг -std=c++11 +  -stdlib=libc++??
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -Wall -std=c99")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -Wall -std=c99")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -std=c99")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -std=c++11")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=c++11")
    message("MinGW flags")
endif ()

# Вывод информации о настройке копиляции (Функция в CMake/Helpers.cmake)
print_current_cmake_config()



####################################################
# Исходники
####################################################
set(ALL_SOURCES
    src/Helpers.h
    src/Helpers.cpp)

source_group("Sources" FILES ${ALL_SOURCES})

set(BASE_LIBRARY_INCLUDES ${BASE_LIBRARY_INCLUDES} "${CMAKE_CURRENT_SOURCE_DIR}/src")

# тип сборки
#if(NOT CMAKE_BUILD_TYPE)
#  set(CMAKE_BUILD_TYPE Release CACHE STRING "Release" FORCE)
#endif()


####################################################
# Генерация
####################################################
message("******** Generating started ***********")
# Из найденных исходников выставляем генерацию бинарника
add_library(${BASE_LIBRARY_NAME} STATIC ${ALL_SOURCES})

# Конкретные пути к бинарнику
set_target_properties(${BASE_LIBRARY_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
)

# Библиотеки, с которыми будем ликовать
# target_link_libraries(${BASE_LIBRARY_NAME} ${LIBS_LINK_NAMES})

set(BASE_LIBRARY_LINK_LIBS ${BASE_LIBRARY_LINK_LIBS} ${BASE_LIBRARY_NAME} ${LIBS_LINK_NAMES})

# Export
set(BASE_LIBRARY_NAME ${BASE_LIBRARY_NAME}  PARENT_SCOPE)
set(BASE_LIBRARY_LINK_LIBS ${BASE_LIBRARY_LINK_LIBS}  PARENT_SCOPE)
set(BASE_LIBRARY_INCLUDES ${BASE_LIBRARY_INCLUDES} PARENT_SCOPE)

