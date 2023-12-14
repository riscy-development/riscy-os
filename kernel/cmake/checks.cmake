include(CheckCSourceCompiles)

# ----- VLA Support -----
check_c_source_compiles(
    "
    int foo(int n) {
        volatile int arr[n + 5];
        return arr[n];
    }
    "
    HAVE_VLA
)

if(NOT HAVE_VLA)
    message(FATAL_ERROR "VLAs are not supported, cannot continue with build.")
endif()
