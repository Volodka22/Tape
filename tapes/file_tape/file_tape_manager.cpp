
#include "file_tape_manager.h"

#include <utility>

namespace tapes {

    void file_tape_manager::add(std::vector<int32_t> const &array) {
        std::string file_path = "./tmp/tape" +  std::to_string(tapes.size());
        if(!config_path.empty()) {
            tapes.emplace_back(array.size(), file_path, config_path);
        } else {
            tapes.emplace_back(array.size(), file_path);
        }
        for (int32_t x: array) {
            tapes.back().set(x);
            if (!tapes.back().is_last()) {
                tapes.back().next();
            }
        }
    }

    int32_t file_tape_manager::get_max() {
        assert(!empty());
        auto max_tape = tapes.begin();
        auto cur = tapes.begin()->current();
        for (auto x = std::next(tapes.begin()); x != tapes.end(); x++) {
            auto other = x->current();
            if (other > cur) {
                max_tape = x;
                cur = other;
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

    file_tape_manager::file_tape_manager(std::string tmp_path_) : tmp_path(std::move(tmp_path_)) {}

    file_tape_manager::file_tape_manager(std::string tmp_path_, std::string config_path_)
            : tmp_path(std::move(tmp_path_)), config_path(std::move(config_path_)) {}

}