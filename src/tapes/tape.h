#pragma once

#include <cstdint>
#include <cassert>
#include <algorithm>
#include <string>

#include "tape_manager.h"

namespace tapes {

    struct tape_error : std::exception {
        explicit tape_error(std::string msg);

        char const *what();

    private:
        std::string message;

    };

    struct tape {

        /**
         *
         * @return length of tape
         */
        [[nodiscard]] virtual size_t size() const noexcept = 0;

        /**
         * moves cursor to right
         */
        virtual void next() const = 0;

        /**
         * moves cursor to left
         */
        virtual void prev() const = 0;

        /**
         * change cursor's value on x
         * @param x
         */
        virtual void set(int32_t x) = 0;

        /**
         *
         * @return current cursor's value
         */
        [[nodiscard]] virtual int32_t current() const = 0;

        /**
         *
         * @return true if cursor on last element
         */
        [[nodiscard]] virtual bool is_last() const noexcept = 0;

        /**
         *
         * @return true if cursor on first element
         */
        [[nodiscard]] virtual bool is_first() const noexcept = 0;

        virtual ~tape() = default;

        /**
         * move cursor to last element
         */
        void to_last() const noexcept;

        /**
         * move cursor to first element
         */
        void to_first() const noexcept;
    };

    /**
     *
     * @tparam TAPE
     * @tparam MANAGER
     * @param input - input tape
     * @param output - output tape
     * @param manager
     * @param batch_size - size of max memory batch
     */
    void sort(tape const &input, tape &output, tape_manager&& manager, size_t batch_size);

}
