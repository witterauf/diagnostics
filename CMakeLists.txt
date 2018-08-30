cmake_minimum_required(VERSION 3.10)
project(Diagnostics VERSION 0.1 LANGUAGES CXX)
set(DIAGNOSTICS_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${DIAGNOSTICS_CMAKE_PATH}")
set(DIAGNOSTICS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(DIAGNOSTICS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include(TargetWarnings)

add_library(DiagnosticsLibrary
	include/diagnostics/ConsoleDiagnosticsConsumer.h
	include/diagnostics/Contracts.h
	include/diagnostics/Diagnostic.h
	include/diagnostics/DiagnosticLocation.h
	include/diagnostics/DiagnosticSnippet.h
	include/diagnostics/DiagnosticsReporter.h
	include/diagnostics/DiagnosticsConsumer.h
	include/diagnostics/LineColumnDecoder.h
	include/diagnostics/StandaloneSnippet.h
	src/ConsoleDiagnosticsConsumer.cc
	src/Diagnostic.cc
	src/DiagnosticLocation.cc
	src/DiagnosticsReporter.cc
	src/LineColumnDecoder.cc
	src/StandaloneSnippet.cc
	src/implementation/Utf8LineColumnDecoder.h
	src/implementation/Utf8LineColumnDecoder.cc
)
target_all_warnings(DiagnosticsLibrary)
target_warnings_are_errors(DiagnosticsLibrary)
target_compile_features(DiagnosticsLibrary PUBLIC cxx_std_17)
target_include_directories(DiagnosticsLibrary
	PUBLIC
         $<INSTALL_INTERFACE:include/>
         $<BUILD_INTERFACE:${DIAGNOSTICS_INCLUDE_DIR}/>
	PRIVATE
		"${DIAGNOSTICS_INCLUDE_DIR}/diagnostics"
)
add_library(Diagnostics::Diagnostics ALIAS DiagnosticsLibrary)

add_subdirectory(test)

##[ installation ]#############################################################

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    DiagnosticsConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
configure_file("${DIAGNOSTICS_CMAKE_PATH}/DiagnosticsConfig.cmake.in" DiagnosticsConfig.cmake @ONLY)
    
install(TARGETS DiagnosticsLibrary EXPORT DiagnosticsLibrary
    ARCHIVE DESTINATION "lib/"
    LIBRARY DESTINATION "lib/"
    RUNTIME DESTINATION "bin/"
)
install(DIRECTORY ${DIAGNOSTICS_INCLUDE_DIR} DESTINATION include)
install(EXPORT DiagnosticsLibrary NAMESPACE Diagnostics:: DESTINATION lib/cmake/diagnostics)
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/DiagnosticsConfigVersion.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/DiagnosticsConfig.cmake
    DESTINATION
        lib/cmake/diagnostics
)