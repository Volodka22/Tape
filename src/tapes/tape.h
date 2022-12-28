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
    template<typename TAPE,
            typename MANAGER,
            typename = std::enable_if<std::is_base_of_v<tape, TAPE> && std::is_base_of_v<tape_manager, MANAGER>>>
    void sort(TAPE const &input, TAPE &output, MANAGER manager, size_t batch_size) {
        assert(input.size() == output.size());
        input.to_first();
        std::vector<int32_t> array;
        array.reserve(batch_size);
        for (size_t start = 0; start < input.size(); start += batch_size) {
            for (int i = 0; i < batch_size && start + i < input.size(); i++) {
                array.push_back(input.current());
                if (!input.is_last()) {
                    input.next();
                }
            }
            std::sort(array.begin(), array.end());
            manager.add(array);
            array.clear();
        }

        output.to_last();

        while (!manager.empty()) {
            output.set(manager.get_max());
            if (!output.is_first()) {
                output.prev();
            }
        }
    }

}
