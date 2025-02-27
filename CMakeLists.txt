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
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Projeto_Lixeira_AutoSustentavel C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
file(GLOB_RECURSE SRC_FILES "src/*/*.c")
add_executable(Projeto_Lixeira_AutoSustentavel
    main.c
    ${SRC_FILES}
)

pico_set_program_name(Projeto_Lixeira_AutoSustentavel "Projeto_Lixeira_AutoSustentavel")
pico_set_program_version(Projeto_Lixeira_AutoSustentavel "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Projeto_Lixeira_AutoSustentavel 1)
pico_enable_stdio_usb(Projeto_Lixeira_AutoSustentavel 1)

pico_generate_pio_header(Projeto_Lixeira_AutoSustentavel ${CMAKE_CURRENT_LIST_DIR}/pios/ws2812.pio)

# Add the standard library to the build
target_link_libraries(Projeto_Lixeira_AutoSustentavel
    pico_stdlib)

# Add the standard include files to the build
target_include_directories(Projeto_Lixeira_AutoSustentavel PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/includes/
        ${CMAKE_CURRENT_LIST_DIR}/includes/trash/
        ${CMAKE_CURRENT_LIST_DIR}/includes/utils
        ${CMAKE_CURRENT_LIST_DIR}/assets/fonts/
)

# Add any user requested libraries
target_link_libraries(Projeto_Lixeira_AutoSustentavel
    hardware_i2c
    hardware_timer
    hardware_pwm
    hardware_adc
    hardware_pio
    hardware_clocks
)

pico_add_extra_outputs(Projeto_Lixeira_AutoSustentavel)

