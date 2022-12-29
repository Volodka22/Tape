#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../tape.h"

namespace tapes {

    struct file_tape : tape {

        size_t size() const noexcept override;

        file_tape(std::size_t size_, std::string file_name_);

        file_tape(std::size_t size_, std::string file_name_, const std::string &configure_file);

        void next() const override;

        void prev() const override;

        void set(int32_t x) override;

        int32_t current() const noexcept override;

        bool is_last() const noexcept override;

        bool is_first() const noexcept override;

        ~file_tape() override;

        void configure(const std::string &config_file);

        std::string get_path() const noexcept;

    private:
        const std::string file_name;
        size_t size_;
        mutable size_t cursor = 0;
        mutable FILE* file;
        mutable int32_t cur = 0;


        mutable int64_t sleep_next = 0;
        mutable int64_t sleep_prev = 0;
        mutable int64_t sleep_set = 0;
        mutable int64_t sleep_get = 0;

        const int32_t SPACE_SIZE = 12;

        static void sleep(int64_t x);

        void read_int() const;
        void write_int(int32_t) const;

        static void check(std::string const &next_token, std::fstream &fs);

    };


}
