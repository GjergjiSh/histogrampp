set(HISTOGRAM_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/histogram.h)

add_library(histogram INTERFACE ${HISTOGRAM_HEADER})

target_include_directories(histogram INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)