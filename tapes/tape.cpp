
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
}
