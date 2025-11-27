include(compiler_utils)

# A function to add SQLite library target
function(sqlite_add_library SQLITE_ROOT)
    # add sqlite library
    add_subdirectory(${SQLITE_ROOT} sqlite3)
endfunction()
