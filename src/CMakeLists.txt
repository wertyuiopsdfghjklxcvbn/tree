add_subdirectory(ast)
add_executable(tree 
    main.cpp
    Parser.cpp
    Operators.cpp
    KeyWords.cpp
    Logging.cpp
    EToken.cpp
    IRGenerator.cpp
    StreamParser.cpp
)

target_link_libraries(tree ast)

