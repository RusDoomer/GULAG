#include "lang.hpp"
#include <stdexcept>

Language::Language(const std::u32string& definition) {

    if (definition.length() != 80) {
        throw std::invalid_argument("Language must be exactly 80 characters.");
    }

    max_codepoint = 0;

    for (char32_t c : definition) {
        if (c == U'~' || c == U' ') {
            throw std::invalid_argument("Language cannot contain '~' or space.");
        }
        // find max codepoint to reduce size of look-up table
        // (1.1M possible characters)
        if (c > max_codepoint) {
            max_codepoint = c;
        }
    }

    // initialized to -1 for not found
    lookup_table.assign(max_codepoint + 1, -1);

    for (size_t i = 0; i < definition.length(); ++i) {
        // characters are paired so divide by 2 for index
        lookup_table[definition[i]] = static_cast<int8_t>(i / 2);
    }

    // for reverse lookups
    char_array = definition;
}
