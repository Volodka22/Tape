#include <gtest/gtest.h>
#include <fstream>
#include "../src/tapes/file_tape/file_tape.h"
#include "../src/tapes/file_tape/file_tape_manager.h"

void test(std::string const &input, std::string const &output, size_t tape_length, size_t batch_size) {

    std::fstream input_stream{"input_test", std::fstream::out};
    input_stream << input;
    input_stream.close();

    tapes::file_tape input_tape(tape_length, "input_test");
    tapes::file_tape output_tape(tape_length, "output_test");

    tapes::sort(input_tape, output_tape, tapes::file_tape_manager("./tmp/tape"), batch_size);

    std::string my_out;

    std::fstream output_stream{"output_test", std::fstream::in};
    while (!output_stream.eof()) {
        std::string tmp;
        output_stream >> tmp;
        if (!tmp.empty()) {
            my_out += tmp + " ";
        }
    }
    output_stream >> my_out;
    output_stream.close();

    ASSERT_EQ(my_out, output);
}

TEST(TapeTests, TestBatchEqLengh) {
    test(">7 1 5 4 6 11 3 9 2 8 ", ">1 2 3 4 5 6 7 8 9 11 ", 10, 10);
}

TEST(TapeTests, TestBatchGtLengh) {
    test(">7 1 1 -12 6 11 3 9 2 8 ", ">-12 1 1 2 3 6 7 8 9 11 ", 10, 20);
}

TEST(TapeTests, TestLengthDivideByBatch) {
    test(">7 1 5 -12 6 11 3 1 2 3 ", ">-12 1 1 2 3 3 5 6 7 11 ", 10, 2);
}

TEST(TapeTests, TestLengthNotDivideByBatch) {
    test(">7 1 5 -12 6 11 3 1 2 3 ", ">-12 1 1 2 3 3 5 6 7 11 ", 10, 3);
}

TEST(TapeTests, TestBatchIsOne) {
    test(">7 1 5 -12 6 11 3 1 2 3 ", ">-12 1 1 2 3 3 5 6 7 11 ", 10, 1);
}

TEST(TapeTests, TestOneLngth) {
    test(">-128 ", ">-128 ", 1, 2);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
