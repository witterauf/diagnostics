set(DIAGNOSTICS_INCLUDE "../include/diagnostics")

set(DIAGNOSTICS_SOURCES
	${DIAGNOSTICS_INCLUDE}/ConsoleDiagnosticsConsumer.h
	${DIAGNOSTICS_INCLUDE}/Contracts.h
	${DIAGNOSTICS_INCLUDE}/Diagnostic.h
	${DIAGNOSTICS_INCLUDE}/DiagnosticLocation.h
	${DIAGNOSTICS_INCLUDE}/DiagnosticSnippet.h
	${DIAGNOSTICS_INCLUDE}/DiagnosticsReporter.h
	${DIAGNOSTICS_INCLUDE}/DiagnosticsConsumer.h
	${DIAGNOSTICS_INCLUDE}/LineAndColumn.h
	ConsoleDiagnosticsConsumer.cc
	Diagnostic.cc
	DiagnosticLocation.cc
	DiagnosticsReporter.cc
)
set(SUPPORT_SOURCES
	${DIAGNOSTICS_INCLUDE}/support/LineColumnDecoder.h
	${DIAGNOSTICS_INCLUDE}/support/StandaloneSnippet.h
	${DIAGNOSTICS_INCLUDE}/support/StandaloneSnippetBuilder.h
	support/LineColumnDecoder.cc
	support/StandaloneSnippet.cc
	support/StandaloneSnippetBuilder.cc
	support/implementation/Utf8LineColumnDecoder.h
	support/implementation/Utf8LineColumnDecoder.cc
)

add_library(DiagnosticsLibrary
	${DIAGNOSTICS_SOURCES}
	${SUPPORT_SOURCES}
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

source_group("Diagnostics" FILES ${DIAGNOSTICS_SOURCES})
source_group("Diagnostics\\Support" FILES ${SUPPORT_SOURCES})


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