
set(SOURCES tc-tool.c)
add_executable(tc-tool ${SOURCES})
target_link_libraries(tc-tool soem)
install(TARGETS tc-tool DESTINATION bin)
