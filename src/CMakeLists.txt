
add_executable(rcgaction
  main.cpp
  composite_handler.cpp
  )


target_include_directories(rcgaction
  PRIVATE
  ${PROJECT_BINARY_DIR}
  PUBLIC
  ${Boost_INCLUDE_DIRS}
  ${LIBRCSC_INCLUDE_DIR}
  )

target_link_libraries(rcgaction
  ${LIBRCSC_LIB}
  Boost::system
  ZLIB::ZLIB
  )

set_target_properties(rcgaction
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/src
  )
