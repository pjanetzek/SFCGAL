#-- configure tests

if( SFCGAL_USE_STATIC_LIBS )
  add_definitions( "-DSFCGAL_USE_STATIC_LIBS" )
endif()

set( SFCGAL_TEST_DIRECTORY "${CMAKE_SOURCE_DIR}/test" )
configure_file( ${SFCGAL_TEST_DIRECTORY}/test_config.h.cmake ${SFCGAL_TEST_DIRECTORY}/test_config.h )

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions( "-frounding-math" )
  
endif(CMAKE_COMPILER_IS_GNUCXX)

#-- build regression tests
if( SFCGAL_BUILD_TESTS )
	add_subdirectory( unit )
	add_subdirectory( regress )
endif()

if( SFCGAL_BUILD_BENCH )
	add_subdirectory( bench )
endif()

# add a custom rule "check" that adds verbosity to ctest
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
