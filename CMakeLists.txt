cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "2" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.1.${PATCH_VERSION})

project(join_server_prg VERSION ${PROJECT_VESRION})

find_package(Boost COMPONENTS unit_test_framework  thread REQUIRED)
add_executable(join_server main.cpp
    server.cpp
    server.h)

add_executable(test_bulk test_bulk.cpp)


set_target_properties(join_server test_bulk  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)
set_target_properties(test_bulk PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(join_server
    ${Boost_LIBRARIES}
)
target_link_libraries(test_bulk
    ${Boost_LIBRARIES}
)

if (MSVC)
    target_compile_options(join_server PRIVATE
        /W4
    )
    target_compile_options(join_prg PRIVATE
        /W4
    )
    target_compile_options(test_bulk PRIVATE
        /W4
    )
else ()
    target_compile_options(join_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
#    target_compile_options(bulk_prg PRIVATE
#        -Wall -Wextra -pedantic -Werror
#    )
    target_compile_options(test_bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS join_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_bulk test_bulk)
