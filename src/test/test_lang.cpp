#include "lang.hpp"
#include <iostream>
#include <cassert>
#include <string>
#include <stdexcept>
#include <vector>

// macro magic, I didn't write these
#define ASSERT_THROWS(expr, ExceptionType) \
    do { \
        bool threw = false; \
        try { (expr); } \
        catch (const ExceptionType&) { threw = true; } \
        catch (...) {} \
        if (!threw) { \
            std::cerr << "FAIL: Expected " << #expr << " to throw " << #ExceptionType << "\n"; \
            assert(false); \
        } \
    } while(0)

#define ASSERT_EQ(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "FAIL: " << #actual << " != " << #expected \
                      << " (Actual: " << (long long)(actual) \
                      << ", Expected: " << (long long)(expected) << ")\n"; \
            assert(false); \
        } \
    } while(0)

const std::u32string VALID_LANG_80 =
    U"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ,<.>/?;:'\"[{]}\\|-_=+1!2@3#4$";

const std::u32string UNICODE_LANG_80 =
    U"аАбБвВгГдДеЕёЁжЖзЗиИйЙкКлЛмМнНоОпПрРсСтТуУфФхХцЦчЧшШщЩъЪыЫьЬэЭюЮяЯ,<.>/?;:'\"[{]}";



void test_constructor_valid() {
    Language lang(VALID_LANG_80);
    Language lang_unicode(UNICODE_LANG_80);
}

void test_constructor_invalid_length() {
    std::u32string too_short = U"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ,<.>/?;:'\"[{]}\\|-_=+";
    ASSERT_THROWS(Language{too_short}, std::invalid_argument);

    std::u32string too_long = U"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ,<.>/?;:'\"[{]}\\|-_=+1!2@3#4$5%6^7&8*9(0)";
    ASSERT_THROWS(Language{too_long}, std::invalid_argument);
}

void test_constructor_invalid_chars() {
    std::u32string has_space_start = U" aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ,<.>/?;:'\"[{]}\\|-_=+1!2@3#4";
    ASSERT_THROWS(Language{has_space_start}, std::invalid_argument);
    std::u32string has_space_mid = U"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrR sStTuUvVwWxXyYzZ,<.>/?;:'\"[{]}\\|-_=+1!2@3#4";
    ASSERT_THROWS(Language{has_space_mid}, std::invalid_argument);
    std::u32string has_space_end = U"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ,<.>/?;:'\"[{]}\\|-_=+1!2@3#4 ";
    ASSERT_THROWS(Language{has_space_end}, std::invalid_argument);

    std::u32string has_tilde_start = U"~aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ,<.>/?;:'\"[{]}\\|-_=+1!2@3#4";
    ASSERT_THROWS(Language{has_tilde_start}, std::invalid_argument);
    std::u32string has_tilde_mid = U"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ~,<.>/?;:'\"[{]}\\|-_=+1!2@3#4";
    ASSERT_THROWS(Language{has_tilde_mid}, std::invalid_argument);
    std::u32string has_tilde_end = U"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ,<.>/?;:'\"[{]}\\|-_=+1!2@3#4~";
    ASSERT_THROWS(Language{has_tilde_end}, std::invalid_argument);
}

void test_get_index_valid() {
    Language lang(VALID_LANG_80);

    // Test first
    ASSERT_EQ(lang.get_index(U'a'), 0);
    ASSERT_EQ(lang.get_index(U'A'), 0);

    // Test middle
    ASSERT_EQ(lang.get_index(U'q'), 16);
    ASSERT_EQ(lang.get_index(U'Q'), 16);

    // Test last
    ASSERT_EQ(lang.get_index(U'4'), 39);
    ASSERT_EQ(lang.get_index(U'$'), 39);
}

void test_get_index_invalid() {
    Language lang(VALID_LANG_80);

    // Test character within the max_codepoint range, but NOT in the language
    ASSERT_EQ(lang.get_index(U'7'), -1);

    // Test the undefined specifically
    ASSERT_EQ(lang.get_index(U' '), -1);
    ASSERT_EQ(lang.get_index(U'~'), -1);

    // Test character far outside the max_codepoint range
    ASSERT_EQ(lang.get_index(U'Я'), -1);
}

void test_get_char_valid() {
    Language lang(VALID_LANG_80);

    // Should return the UNSHIFTED character
    ASSERT_EQ(lang.get_char(0), U'a');
    ASSERT_EQ(lang.get_char(16), U'q');
    ASSERT_EQ(lang.get_char(39), U'4');
}

void test_get_char_invalid() {
    Language lang(VALID_LANG_80);

    // Lower out-of-bounds
    ASSERT_EQ(lang.get_char(-1), U'~');
    ASSERT_EQ(lang.get_char(-100), U'~');

    // Upper out-of-bounds
    ASSERT_EQ(lang.get_char(40), U'~');
    ASSERT_EQ(lang.get_char(100), U'~');
}

void test_high_unicode_scaling() {
    Language lang(UNICODE_LANG_80);

    // (Cyrillic a)
    ASSERT_EQ(lang.get_index(U'а'), 0);
    ASSERT_EQ(lang.get_char(0), U'а');

    ASSERT_EQ(lang.get_index(U']'), 39);
    ASSERT_EQ(lang.get_char(39), U']');

    ASSERT_EQ(lang.get_index(U'a'), -1); // Latin 'a'
}

int main() {
    std::cout << "[RUNNING] test_constructor_valid...\n";
    test_constructor_valid();

    std::cout << "[RUNNING] test_constructor_invalid_length...\n";
    test_constructor_invalid_length();

    std::cout << "[RUNNING] test_constructor_invalid_chars...\n";
    test_constructor_invalid_chars();

    std::cout << "[RUNNING] test_get_index_valid...\n";
    test_get_index_valid();

    std::cout << "[RUNNING] test_get_index_invalid...\n";
    test_get_index_invalid();

    std::cout << "[RUNNING] test_get_char_valid...\n";
    test_get_char_valid();

    std::cout << "[RUNNING] test_get_char_invalid...\n";
    test_get_char_invalid();

    std::cout << "[RUNNING] test_high_unicode_scaling...\n";
    test_high_unicode_scaling();

    std::cout << "\n\033[32m[SUCCESS]\033[0m All Language module tests passed successfully!\n";
    return 0;
}
