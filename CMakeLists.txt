# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(fun_steps C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(fun_steps fun_steps.c bmi160.c led_manager.c)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
# Générer le fichier .pio.h à partir de ws2812.pio
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/ws2812.pio")
    pico_generate_pio_header(fun_steps ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)
else()
    message(FATAL_ERROR "ws2812.pio not found in ${CMAKE_CURRENT_LIST_DIR}")
endif()

# Ajouter les sources WS2812
# Les fichiers sources sont déjà listés dans add_executable, donc cette section est supprimée pour éviter les doublons.

# Ajouter le chemin d'inclusion pour le fichier généré
target_include_directories(fun_steps PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/generated
)

pico_set_program_name(fun_steps "fun_steps_c")
pico_set_program_version(fun_steps "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(fun_steps 1)
pico_enable_stdio_usb(fun_steps 0)

# Add the standard library to the build
target_link_libraries(fun_steps PRIVATE
        pico_stdlib
        hardware_pio
        hardware_i2c
        )

pico_add_extra_outputs(fun_steps)

