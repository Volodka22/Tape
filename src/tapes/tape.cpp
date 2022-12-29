
#include "tape.h"

namespace tapes {
    char const *tape_error::what() {
        return message.c_str();
    }

    tape_error::tape_error(std::string msg) : message(std::move(msg)) {}

    void tape::to_last() const noexcept {
        while (!is_last()) {
            next();
        }
    }

    void tape::to_first() const noexcept {
        while (!is_first()) {
            prev();
        }
    }

    void sort(tape const &input, tape &output, tape_manager &&manager, size_t batch_size) {
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
