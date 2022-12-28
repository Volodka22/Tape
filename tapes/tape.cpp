
#include "tape.h"

namespace tapes {

    tape_error::tape_error(const char *msg) : message(msg) {}

    char const *tape_error::what() {
        return message;
    }

    tape_error::tape_error(const std::string &msg) : message(msg.c_str()) {}

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
