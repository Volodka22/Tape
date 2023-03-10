#include <gtest/gtest.h>
#include <fstream>
#include "../src/tapes/file_tape/file_tape.h"
#include "../src/tapes/file_tape/file_tape_manager.h"

void test(std::string const &input, std::string const &output, size_t tape_length, size_t batch_size) {

    const char *input_test = "input_test";
    std::fstream input_stream{input_test, std::fstream::out};
    input_stream << input;
    input_stream.close();
    tapes::file_tape input_tape(tape_length, input_test);

    const char *output_test = "output_test";
    tapes::file_tape output_tape(tape_length, output_test);

    tapes::sort(input_tape, output_tape, tapes::file_tape_manager("./tmp/tape"), batch_size);

    std::string my_out;

    std::fstream output_stream{output_test, std::fstream::in};
    std::getline(output_stream, my_out);
    output_stream.close();

    remove(output_test);
    remove(input_test);

    ASSERT_EQ(my_out, output);
}

TEST(TapeTests, TestBatchEqLength) {
    test("           7|    1       |    1       |-12         |    6       |   11       |    3       |   8        |     9      |   11       |",
         "-12         |1           |1           |3           |6           |7           |8           |9           |11          |11          |",
         10,
         10
    );
}

TEST(TapeTests, TestBatchGtLength) {
    test("           7|    1       |    1       |-12         |    6       |   11       |    3       |   9        |     2      |   8        |",
         "-12         |1           |1           |2           |3           |6           |7           |8           |9           |11          |",
         10,
         20
    );
}

TEST(TapeTests, TestLengthDivideByBatch) {
    test("           7|    1       |    5       |-12         |    6       |   11       |    3       |   1        |     2      |   3        |",
         "-12         |1           |1           |2           |3           |3           |5           |6           |7           |11          |",
         10,
         2
    );
}

TEST(TapeTests, TestLengthNotDivideByBatch) {
    test("           7|    1       |    5       |-12         |    6       |   11       |    3       |   1        |     2      |   3        |",
         "-12         |1           |1           |2           |3           |3           |5           |6           |7           |11          |",
         10,
         3
    );
}

TEST(TapeTests, TestBatchIsOne) {
    test("           9|    1       |    5       |-12         |    6       |   11       |    3       |   1        |     2      |   3        |",
         "-12         |1           |1           |2           |3           |3           |5           |6           |9           |11          |",
         10,
         1
    );
}

TEST(TapeTests, TestOneLength) {
    test("           9| ",
         "9           |",
         1,
         10
    );
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
