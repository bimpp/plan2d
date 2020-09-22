set(CODE_FILE_LIST
    ${BIMPP_PLAN2D_PATH_SRC_FILE_LIST}
    plan2d.cpp
    )
add_executable(plan2d ${CODE_FILE_LIST})

target_include_directories(plan2d PRIVATE
    ${Boost_INCLUDE_DIR}
    ${BIMPP_PLAN2D_PATH_INC}
    )

set_target_properties(plan2d PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIMPP_PLAN2D_PATH_OUTPUT_BIN})
