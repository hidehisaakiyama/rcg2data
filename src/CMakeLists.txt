
add_executable(rcg2data
  main.cpp
  options.cpp
  action_event.cpp
  field_model.cpp
  field_state.cpp
  game_analyzer.cpp
  rcg_reader.cpp
  )


target_include_directories(rcg2data
  PRIVATE
  ${PROJECT_BINARY_DIR}
  PUBLIC
  ${Boost_INCLUDE_DIRS}
  ${LIBRCSC_INCLUDE_DIR}
  )

target_link_libraries(rcg2data
  ${LIBRCSC_LIB}
  Boost::system
  ZLIB::ZLIB
  )

set_target_properties(rcg2data
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/src
  )

install(TARGETS
  rcg2data
  RUNTIME
  DESTINATION ${CMAKE_INSTALL_BINDIR})
