function(target_all_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        # Turn off maybe-initialized because there is a bug since GCC 8 that generates spurious warnings.
        # See https://gcc.gnu.org/bugzilla/show_bug.cgi?id=80635
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wno-long-long -Wno-error=maybe-uninitialized -Wno-maybe-uninitialized -pedantic)
    endif()    
endfunction(target_all_warnings)

function(target_warnings_are_errors target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /WX)
    elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        target_compile_options(${target} PRIVATE -Werror -Wno-error=maybe-uninitialized -Wno-maybe-uninitialized)
    endif()    
endfunction(target_warnings_are_errors)
