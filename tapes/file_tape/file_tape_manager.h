#pragma once

#include <list>

#include "file_tape.h"
#include "../tape_manager.h"

namespace tapes {

    struct file_tape_manager : tape_manager {

        void add(std::vector<int32_t> const &array) override;

        int32_t get_max() override;

        [[nodiscard]] bool empty() const noexcept override;

    private:
        std::list<file_tape> tapes;
    };

}

const std::string TMP_PATH = "./tmp/tape";