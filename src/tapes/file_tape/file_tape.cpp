#include "file_tape.h"

#include <utility>
#include <fstream>
#include <thread>

namespace tapes {

    void file_tape::next() const {
        sleep(sleep_next);
        cursor++;
        read_int();
    }

    void file_tape::prev() const {
        sleep(sleep_prev);
        if (is_first()) {
            throw tape_error("Cursor on first element");
        }
        cursor--;
        fseek(file, -26, SEEK_CUR);
        read_int();
    }

    void file_tape::set(int32_t x) {
        sleep(sleep_set);
        fseek(file, -SPACE_SIZE - 1, SEEK_CUR);
        write_int(x);
    }

    file_tape::file_tape(const std::size_t _size, std::string file_name_) :
            size_(_size), file_name(std::move(file_name_)) {
        file = fopen(file_name.c_str(), "r+");
        if (file == nullptr) {
            file = fopen(file_name.c_str(), "w+");
            for (size_t i = 0; i < size_; i++) {
                write_int(0);
            }
            fseek(file, 0, SEEK_SET);
        }

        read_int();

        assert(size_ > 0);
    }

    bool file_tape::is_last() const noexcept {
        return cursor + 1 == size();
    }

    bool file_tape::is_first() const noexcept {
        return cursor == 0;
    }

    int32_t file_tape::current() const noexcept {
        sleep(sleep_get);
        return cur;
    }


    size_t file_tape::size() const noexcept {
        return size_;
    }

    void file_tape::configure(const std::string &config_file) {
        std::fstream config_stream{config_file, std::fstream::in};
        if (config_stream.is_open()) {
            check("NEXT", config_stream);
            config_stream >> sleep_next;

            check("PREV", config_stream);
            config_stream >> sleep_prev;

            check("SET", config_stream);
            config_stream >> sleep_set;

            check("GET", config_stream);
            config_stream >> sleep_get;

        } else {
            throw tape_error("Configuration file didn't find");
        }

    }

    void file_tape::check(std::string const &next_token, std::fstream &config_stream) {
        std::string tmp;
        config_stream >> tmp;
        if (tmp != next_token) {
            throw tape_error("Incorrect configure file: expected " + next_token + " but found " + tmp);
        }
    }

    void file_tape::sleep(int64_t x) {
        std::this_thread::sleep_for(std::chrono::milliseconds(x));
    }

    file_tape::file_tape(std::size_t size_, std::string file_name_, const std::string &configure_file)
            : file_tape::file_tape(size_, std::move(file_name_)) {
        configure(configure_file);
    }

    std::string file_tape::get_path() const noexcept {
        return file_name;
    }

    void file_tape::read_int() const {
        std::string s;
        for (size_t i = 0; i < SPACE_SIZE; i++) {
            int x = fgetc(file);
            if (x != ' ' && x != '\t' && x != '\n' && x != '\r') {
                s += (char)x;
            }
        }
        char *p;
        cur = static_cast<int32_t>(strtol(s.c_str(), &p, 10));
        if (*p != 0) {
            throw tape_error("Incorrect input (not a number): in cell " + std::to_string(cursor));
        }
        if (fgetc(file) != '|') {
            throw tape_error("Incorrect input (lose \'|\' ): in cell " + std::to_string(cursor));
        }
    }

    void file_tape::write_int(int32_t x) const {
        std::string s = std::to_string(x);
        size_t pos = 0;
        for (auto a : s) {
            fputc(a, file);
            pos++;
        }
        while (pos < SPACE_SIZE) {
            fputc(' ', file);
            pos++;
        }
        fputc('|', file);
        cur = x;
        fflush(file);
    }

    file_tape::~file_tape() {
        fclose(file);
    }


}
