#include "file_tape.h"

#include <utility>
#include <fstream>

namespace tapes {

    void file_tape::next() const {
        if (is_last()) {
            throw tape_error("Cursor on last element");
        }
        ++cursor;
    }

    void file_tape::prev() const {
        if (is_first()) {
            throw tape_error("Cursor on first element");
        }
        --cursor;
    }

    void file_tape::set(int32_t x) {
        array[cursor] = x;
        write_array_to_file();
    }

    file_tape::file_tape(const std::size_t size_, std::string file_name_) :
            array(size_), file_name(std::move(file_name_)) {
        assert(size_ > 0);
        read_file();
    }

    bool file_tape::is_last() const noexcept {
        return cursor + 1 == array.size();
    }

    bool file_tape::is_first() const noexcept {
        return cursor == 0;
    }

    void file_tape::write_array_to_file() {
        std::fstream fs{file_name, std::fstream::out};
        for (auto x: array) {
            fs << x << ' ';
        }
        fs.close();
    }

    void file_tape::read_file() {
        std::fstream fs{file_name, std::fstream::in};
        if (fs.is_open()) {
            for (auto &x: array) {
                fs >> x;
            }
            fs.close();
        } else {
            write_array_to_file();
        }
    }

    int32_t file_tape::current() const noexcept {
        return array[cursor];
    }


    size_t file_tape::size() const noexcept {
        return array.size();
    }

    void file_tape::configure(const std::string& config_file) {
        std::fstream fs{config_file, std::fstream::in};
        if (fs.is_open()) {
            check("NEXT", fs);
            check("=", fs);

        } else {
            throw tape_error("Configuration file didn't find");
        }

    }

    void file_tape::check(std::string const& next_token, std::fstream& fs) {
        std::string tmp;
        fs >> tmp;
        if (tmp != next_token) {
            throw tape_error("Incorrect configure file: expected " + next_token + " but found " + tmp);
        }
    }


}