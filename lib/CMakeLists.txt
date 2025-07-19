set(SOURCES
        Game.cpp
        Direction.cpp
        Map.cpp
        Player.cpp
        Room.cpp
)

string(REPLACE ".cpp" ".hpp" HEADERS "${SOURCES}")
add_library(GameLogic SHARED ${SOURCES})

if (BUILD_TESTS)
    include(GoogleTest)
    enable_testing()
    set(TEST_SOURCES
            Game.test.cpp)

    add_executable(GameLogicTests ${TEST_SOURCES})
    target_link_libraries(GameLogicTests PRIVATE GameLogic gtest_main)
    gtest_discover_tests(GameLogicTests TEST_PREFIX GameLogic)
endif ()
