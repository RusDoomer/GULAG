#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Language {
private:
    std::u32string char_array;
    // int8 should be large enough for -1 to 39
    std::vector<int8_t> lookup_table;
    char32_t max_codepoint;

public:
    explicit Language(const std::u32string& definition);

    // Char -> Index
    inline int8_t get_index(char32_t c) const {
        if (c > max_codepoint) {
            return -1;
        }
        return lookup_table[c];
    }

    // Index -> Char
    inline char32_t get_char(int8_t index) const {
        if (index < 0 || index > 39) {
            return U'~'; // null character
        }
        return char_array[index * 2];
    }
};
