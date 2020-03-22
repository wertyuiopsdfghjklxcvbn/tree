add_subdirectory(AST)
add_executable(tree 
    main.cpp
    Parser.cpp
    Operators.cpp
    KeyWords.cpp
)

target_link_libraries(tree AST)

