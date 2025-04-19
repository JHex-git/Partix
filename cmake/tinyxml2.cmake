if (MSVC)
    set(tinyxml2_INSTALL_PKGCONFIG OFF CACHE BOOL "Install with MSVC will throw error when configure" FORCE)  
endif()
set(tinyxml2_BUILD_TESTING OFF CACHE BOOL "Disable tinyxml2 unit test build." FORCE)