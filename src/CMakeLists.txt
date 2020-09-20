set(CODE_FILE_LIST
    ${BIMPP_PLAN2D_PATH_SRC_FILE_LIST}
    demo.cpp
    )
add_executable(demo ${CODE_FILE_LIST})

target_include_directories(demo PRIVATE
    ${Boost_INCLUDE_DIR}
    ${BIMPP_PLAN2D_PATH_INC}
    )

set_target_properties(demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIMPP_PLAN2D_PATH_OUTPUT_BIN})
