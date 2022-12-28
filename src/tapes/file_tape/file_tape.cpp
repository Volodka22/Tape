#include "file_tape.h"

#include <utility>
#include <fstream>
#include <thread>

namespace tapes {

    void file_tape::next() const {
        sleep(sleep_next);
        if (is_last()) {
            throw tape_error("Cursor on last element");
        }
        ++cursor;
        write_array_to_file();
    }

    void file_tape::prev() const {
        sleep(sleep_prev);
        if (is_first()) {
            throw tape_error("Cursor on first element");
        }
        --cursor;
        write_array_to_file();
    }

    void file_tape::set(int32_t x) {
        sleep(sleep_set);
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

    void file_tape::write_array_to_file(bool is_finale) const {
        std::fstream fs{file_name, std::fstream::out};
        for (size_t i = 0; i < array.size(); i++) {
            if(i == cursor && !is_finale) {
                fs << "->";
            }
            fs << array[i] << ' ';
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
        sleep(sleep_get);
        return array[cursor];
    }


    size_t file_tape::size() const noexcept {
        return array.size();
    }

    void file_tape::configure(const std::string &config_file) {
        std::fstream fs{config_file, std::fstream::in};
        if (fs.is_open()) {
            check("NEXT", fs);
            check("=", fs);
            fs >> sleep_next;

            check("PREV", fs);
            check("=", fs);
            fs >> sleep_prev;

            check("SET", fs);
            check("=", fs);
            fs >> sleep_set;

            check("GET", fs);
            check("=", fs);
            fs >> sleep_get;

        } else {
            throw tape_error("Configuration file didn't find");
        }

    }

    void file_tape::check(std::string const &next_token, std::fstream &fs) {
        std::string tmp;
        fs >> tmp;
        if (tmp != next_token) {
            throw tape_error("Incorrect configure file: expected " + next_token + " but found " + tmp);
        }
    }

    void file_tape::sleep(int64_t x) {
        std::this_thread::sleep_for(std::chrono::milliseconds(x));
    }

    file_tape::file_tape(std::size_t size_, std::string file_name_, const std::string& configure_file)
            : file_tape::file_tape(size_, std::move(file_name_)) {
        configure(configure_file);
    }

    file_tape::~file_tape() {
        write_array_to_file(true);
    }

    std::string file_tape::get_path() const noexcept {
        return file_name;
    }


}