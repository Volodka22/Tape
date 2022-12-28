#include <iostream>
#include "tapes/file_tape/file_tape.h"
#include "tapes/file_tape/file_tape_manager.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        std::perror("Expected 4 arguments: input filename, output filename, tapes length, batch size");
        return -1;
    }

    size_t tape_length = strtoull(argv[3], nullptr, 10);
    size_t batch_size = strtoull(argv[4], nullptr, 10);

    tapes::file_tape input(tape_length, std::string(argv[1]));
    tapes::file_tape output(tape_length, std::string(argv[2]));

    tapes::sort(input, output, tapes::file_tape_manager(), batch_size);

    return 0;
}
