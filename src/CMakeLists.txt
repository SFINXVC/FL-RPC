cmake_minimum_required(VERSION 3.15)

add_definitions(-D__CMAKE_ROOT_DIRECTORY__="${CMAKE_CURRENT_SOURCE_DIR}/..")

if (NOT WIN32)
    message(FATAL_ERROR, "dude, go uninstall ur linux and go install windows already, ur os sucks (lol)")
endif()

file(GLOB SOURCE_FILES
    *.cc
    **/*.cc
    **/**/*.cc
    # discord/discord_rpc.cc

    # utils/process_monitor.cc
    # utils/system_tray.cc

    # gui/settings.c

    # main.cc
)

file(GLOB HEADER_FILES
    *.hh
    **/*.hh
    **/**/*.hh
    # const/config.hh

    # discord/discord_rpc.hh

    # utils/process_monitor.hh
    # utils/system_tray.hh

    # gui/settings.hh
)

add_executable(${PROJECT_NAME} 
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

add_compile_definitions(${PROJECT_NAME} SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG)

# r u lagging? no fuck it got stuck again
target_link_libraries(${PROJECT_NAME} PUBLIC
    spdlog fmt discord-rpc
)

target_include_directories(${PROJECT_NAME} PRIVATE .
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/discord/include
)