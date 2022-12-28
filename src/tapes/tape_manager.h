#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace tapes {

    /**
     * menage temporary tape
     */
    struct tape_manager {

        /**
         * create temporary tape and set array's values
         * @param array
         */
        virtual void add(std::vector<int32_t> const &array) = 0;

        /**
         * extract max value from manager
         * @return max element
         */
        virtual int32_t get_max() = 0;

        /**
         *
         * @return true if tape manager has no values
         */
        [[nodiscard]] virtual bool empty() const noexcept = 0;

    };

}
