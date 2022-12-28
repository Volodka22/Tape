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

        [[nodiscard]] virtual size_t size() const noexcept = 0;

        virtual void next() const = 0;

        virtual void prev() const = 0;

        virtual void set(int32_t x) = 0;

        [[nodiscard]] virtual int32_t current() const = 0;

        [[nodiscard]] virtual bool is_last() const noexcept = 0;

        [[nodiscard]] virtual bool is_first() const noexcept = 0;

        virtual ~tape() = default;

        void to_last() const noexcept;

        void to_first() const noexcept;
    };

    template<typename TAPE,
            typename MANAGER,
            typename = std::enable_if<std::is_base_of_v<tape, TAPE> && std::is_base_of_v<tape_manager, MANAGER>>>
    void sort(TAPE const &input,
              TAPE &output, MANAGER manager, size_t batch_size) {
        assert(input.size() == output.size());
        input.to_first();
        std::vector<int32_t> array(batch_size);
        for (size_t start = 0; start < input.size(); start += batch_size) {
            for (int i = 0; i < batch_size && start + i < input.size(); i++) {
                array[i] = input.current();
                if (!input.is_last()) {
                    input.next();
                }
            }
            std::sort(array.begin(), array.end());
            manager.add(array);
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
