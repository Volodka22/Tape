
#include "file_tape_manager.h"

namespace tapes {

    void file_tape_manager::add(std::vector<int32_t> const &array) {
        std::string file_path = TMP_PATH + std::to_string(tapes.size());
        tapes.emplace_back(array.size(), file_path);
        for (int32_t x : array) {
            tapes.back().set(x);
            if (!tapes.back().is_last()) {
                tapes.back().next();
            }
        }
    }

    int32_t file_tape_manager::get_max() {
        auto max_tape = tapes.begin();
        for (auto x = std::next(tapes.begin()); x != tapes.end(); x++) {
            if (x->current() > max_tape->current()) {
                max_tape = x;
            }
        }

        auto res = max_tape->current();

        if (max_tape->is_first()) {
            tapes.erase(max_tape);
        } else {
            max_tape->prev();
        }

        return res;
    }

    bool file_tape_manager::empty() const noexcept {
        return tapes.empty();
    }

}