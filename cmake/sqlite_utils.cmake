include(compiler_utils)

# A function to add SQLite library target
function(sqlite_add_library PROJECT_ROOT)
    set(SQLITE_ROOT ${PROJECT_ROOT}/3rdparty/cpp/sqlite3)

    # add sqlite library
    add_subdirectory(${SQLITE_ROOT} sqlite3)

    set(SQLITE_LIBRARY SQLite3 PARENT_SCOPE)
    set(SQLITE_INCDIR ${SQLITE_ROOT} PARENT_SCOPE)
endfunction()
