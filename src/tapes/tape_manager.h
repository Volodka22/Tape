#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace tapes {

    struct tape_manager {

        virtual void add(std::vector<int32_t> const &array) = 0;

        virtual int32_t get_max() = 0;

        [[nodiscard]] virtual bool empty() const noexcept = 0;

    };

}
