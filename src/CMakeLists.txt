SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(SOURCE_FILE road_label.cc main.cc)  

add_executable(road_label ${SOURCE_FILE} )
TARGET_LINK_LIBRARIES(road_label ${REQUIRED_LIBRARIES})



