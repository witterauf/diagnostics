add_executable(LineColumnDecoderUnitTest
	LineColumnDecoderTest.cc
	Utf8LineColumnDecoderTest.cc
	${CATCH_MAIN}
)
target_include_directories(LineColumnDecoderUnitTest PRIVATE "${DIAGNOSTICS_SOURCE_DIR}")
target_link_libraries(LineColumnDecoderUnitTest PRIVATE Diagnostics::Diagnostics Catch)

add_executable(DiagnosticSnipperUnitTests
	StandaloneSnippetBuilderTest.cc
	${CATCH_MAIN}
)
target_include_directories(DiagnosticSnipperUnitTests PRIVATE "${DIAGNOSTICS_SOURCE_DIR}")
target_link_libraries(DiagnosticSnipperUnitTests PRIVATE Diagnostics::Diagnostics Catch)