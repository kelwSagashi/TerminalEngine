cmake_minimum_required(VERSION 3.10)

project(TerminalEngine)

# Defina a saída do executável na pasta principal
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

# Adicione o arquivo algebra.cpp à lista de fontes
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/_3dFunctions.cpp
    ${CMAKE_SOURCE_DIR}/src/Sprite.cpp
    ${CMAKE_SOURCE_DIR}/src/Camera.cpp
    ${CMAKE_SOURCE_DIR}/src/EngineInput.cpp
    ${CMAKE_SOURCE_DIR}/src/consoleTerminal.cpp
    ${CMAKE_SOURCE_DIR}/src/Draw3d.cpp
    ${CMAKE_SOURCE_DIR}/src/DrawBasics.cpp
    ${CMAKE_SOURCE_DIR}/src/Engine.cpp
    ${CMAKE_SOURCE_DIR}/src/Renderer.cpp
)

# Define a opção LOADOBJ com valor padrão 0 (desativado)
option(LOADOBJ "Ativar carregamento de objeto" 0)

# Crie o executável
add_executable(TerminalEngine ${SOURCES})

# Define a macro de compilação LOADOBJ com base na opção correspondente
target_compile_definitions(TerminalEngine PRIVATE LOADOBJ=${LOADOBJ})

# Inclua a pasta includes no caminho de inclusão
target_include_directories(TerminalEngine PRIVATE ${CMAKE_SOURCE_DIR}/includes)

# Defina a saída do executável na pasta desejada
set_target_properties(TerminalEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
    # RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}
)