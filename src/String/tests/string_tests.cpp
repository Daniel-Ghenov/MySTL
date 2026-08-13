#include <gtest/gtest.h>
#include <sstream>
#include "MyString.h"

using mystd::string;

TEST(StringTest, DefaultConstructedStringIsEmpty) {
    string s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0u);
}

TEST(StringTest, ConstructedFromCStringHasCorrectContent) {
    string s("hello");
    EXPECT_EQ(s.size(), 5u);
    EXPECT_STREQ(s.c_str(), "hello");
}

TEST(StringTest, ConstructedFromLongCStringHasCorrectContent) {
    const char* longText = "this is a much longer string than the short-string buffer";
    string s(longText);
    EXPECT_EQ(s.size(), std::string(longText).size());
    EXPECT_STREQ(s.c_str(), longText);
}

TEST(StringTest, IndexOperatorReturnsCorrectCharacters) {
    string s("abcde");
    EXPECT_EQ(s[0], 'a');
    EXPECT_EQ(s[4], 'e');
}

TEST(StringTest, AtThrowsOutOfRangeForInvalidIndex) {
    string s("abc");
    EXPECT_THROW(s.at(100), std::out_of_range);
}

TEST(StringTest, AtReturnsCorrectCharacter) {
    string s("abc");
    EXPECT_EQ(s.at(1), 'b');
}

TEST(StringTest, FrontAndBackReturnCorrectCharacters) {
    string s("abcde");
    EXPECT_EQ(s.front(), 'a');
    EXPECT_EQ(s.back(), 'e');
}

TEST(StringTest, CopyConstructorProducesIndependentEqualCopy) {
    string original("hello");
    string copy(original);

    EXPECT_TRUE(copy == original);

    copy[0] = 'j';
    EXPECT_EQ(original[0], 'h') << "Mutating the copy should not affect the original";
}

TEST(StringTest, CopyAssignmentProducesIndependentEqualCopy) {
    string original("hello");
    string copy;
    copy = original;

    EXPECT_TRUE(copy == original);

    copy[0] = 'j';
    EXPECT_EQ(original[0], 'h') << "Mutating the copy should not affect the original";
}

TEST(StringTest, MoveConstructorTransfersContent) {
    string original("hello world");
    string moved(std::move(original));

    EXPECT_STREQ(moved.c_str(), "hello world");
}

TEST(StringTest, AssignFromCStringReplacesContent) {
    string s("old value");
    s = "new value";
    EXPECT_STREQ(s.c_str(), "new value");
}

TEST(StringTest, PlusEqualsAppendsOtherString) {
    string s("foo");
    string other("bar");
    s += other;
    EXPECT_STREQ(s.c_str(), "foobar");
}

TEST(StringTest, AppendReturnsReferenceAndAppendsContent) {
    string s("foo");
    string other("bar");
    s.append(other);
    EXPECT_STREQ(s.c_str(), "foobar");
}

TEST(StringTest, OperatorPlusConcatenatesTwoStrings) {
    string a("foo");
    string b("bar");
    string result = a + b;
    EXPECT_STREQ(result.c_str(), "foobar");
}

TEST(StringTest, PushBackAppendsSingleCharacter) {
    string s("ab");
    s.push_back('c');
    EXPECT_STREQ(s.c_str(), "abc");
    EXPECT_EQ(s.size(), 3u);
}

TEST(StringTest, PopBackRemovesLastCharacter) {
    string s("abc");
    s.pop_back();
    EXPECT_EQ(s.size(), 2u);
    EXPECT_STREQ(s.c_str(), "ab");
}

TEST(StringTest, AssignReplacesContent) {
    string s("foo");
    string other("replacement");
    s.assign(other);
    EXPECT_STREQ(s.c_str(), "replacement");
}

TEST(StringTest, ClearEmptiesTheString) {
    string s("hello");
    s.clear();
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0u);
}

TEST(StringTest, EqualityComparesContent) {
    string a("hello");
    string b("hello");
    string c("world");

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(StringTest, EqualityWithCStringComparesContent) {
    string a("hello");
    EXPECT_TRUE(a == "hello");
    EXPECT_FALSE(a == "world");
}

TEST(StringTest, RelationalOperatorsMatchLexicographicOrder) {
    string a("apple");
    string b("banana");

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
}

TEST(StringTest, CompareReturnsZeroForEqualStrings) {
    string a("same");
    string b("same");
    EXPECT_EQ(a.compare(b), 0);
}

TEST(StringTest, EmptyReflectsWhetherStringHasContent) {
    string empty;
    string nonEmpty("x");

    EXPECT_TRUE(empty.empty());
    EXPECT_FALSE(nonEmpty.empty());
}

TEST(StringTest, SwapExchangesContents) {
    string a("aaa");
    string b("bbb");
    a.swap(b);

    EXPECT_STREQ(a.c_str(), "bbb");
    EXPECT_STREQ(b.c_str(), "aaa");
}

TEST(StringTest, StreamInsertionWritesContent) {
    string s("printed");
    std::ostringstream oss;
    oss << s;
    EXPECT_EQ(oss.str(), "printed");
}

TEST(StringTest, StreamExtractionReadsWhitespaceDelimitedToken) {
    std::istringstream iss("hello world");
    string s;
    iss >> s;
    EXPECT_STREQ(s.c_str(), "hello");
}

TEST(StringTest, GetlineReadsFullLine) {
    std::istringstream iss("hello world\nsecond line");
    string s;
    mystd::getline(iss, s, '\n');
    EXPECT_STREQ(s.c_str(), "hello world");
}

TEST(StringTest, LengthMatchesSize) {
    string s("hello");
    EXPECT_EQ(s.length(), s.size());
}
