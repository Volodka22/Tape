add_executable(
        Tape
        main.cpp
        tapes/tape.h
        tapes/file_tape/file_tape.h
        tapes/file_tape/file_tape.cpp
        tapes/file_tape/file_tape_manager.h
        tapes/file_tape/file_tape_manager.cpp
        tapes/tape_manager.h
        tapes/tape.cpp
)

add_library(
        tape_lib
        main.cpp
        tapes/tape.h
        tapes/file_tape/file_tape.h
        tapes/file_tape/file_tape.cpp
        tapes/file_tape/file_tape_manager.h
        tapes/file_tape/file_tape_manager.cpp
        tapes/tape_manager.h
        tapes/tape.cpp
)


target_include_directories(tape_lib PUBLIC include)
