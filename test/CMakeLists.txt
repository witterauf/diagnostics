add_library(Catch STATIC
	catch_main.cc
)
target_include_directories(Catch
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(unit)