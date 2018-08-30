#include "implementation/Utf8LineColumnDecoder.h"
#include "catch.hpp"

using namespace diagnostics;

namespace {

class TestUtf8LineColumnDecoder : public Utf8LineColumnDecoder
{
public:
    explicit TestUtf8LineColumnDecoder(const uint8_t* source, size_t size)
        : Utf8LineColumnDecoder{ source, size } {}

    auto testDecode(size_t offset, size_t hint, const LineAndColumn& start) const-> LineAndColumn
    {
        return doDecoding(offset, Hint{ hint, start });
    }
};

}

SCENARIO("Decoding ASCII characters", "[Support][LineColumnDecoder][Utf8]")
{
    const std::string TestSequence = "a\nb\rc\r\nd";
    TestUtf8LineColumnDecoder decoder{ reinterpret_cast<const uint8_t*>(TestSequence.c_str()), sizeof(TestSequence.length()) };

    GIVEN("An UTF-8 line/column decoder")
    {
        LineAndColumn startPosition{ 1, 1 };

        WHEN("Encountering a normal character")
        {
            const size_t start = 0;
            const size_t target = start + 1;
            auto before = decoder.testDecode(start, 0, startPosition);
            auto after = decoder.testDecode(target, 0, startPosition);

            THEN("The column count is incremented")
            {
                REQUIRE(after.column == before.column + 1);
            }
            THEN("The line count is unchanged")
            {
                REQUIRE(after.line == before.line);
            }
        }

        WHEN("Encountering a linefeed (\\n)")
        {
            const size_t start = TestSequence.find('\n', 0);
            const size_t target = start + 1;
            auto before = decoder.testDecode(start, 0, startPosition);
            auto after = decoder.testDecode(target, 0, startPosition);

            THEN("The column count is reset to 1")
            {
                REQUIRE(after.column == 1);
            }
            THEN("The line count is incremented")
            {
                REQUIRE(after.line == before.line + 1);
            }
        }

        WHEN("Encountering a carriage return (\\r)")
        {
            const size_t start = TestSequence.find('\r', 0);
            const size_t target = start + 1;
            auto before = decoder.testDecode(start, 0, startPosition);
            auto after = decoder.testDecode(target, 0, startPosition);

            THEN("The column count is reset to 1")
            {
                REQUIRE(after.column == 1);
            }
            THEN("The line count is incremented")
            {
                REQUIRE(after.line == before.line + 1);
            }
        }

        WHEN("Encountering a carriage return+linefeed (\\r\\n")
        {
            const size_t start = TestSequence.find("\r\n", 0);
            const size_t target = start + 2;
            auto before = decoder.testDecode(start, 0, startPosition);
            auto after = decoder.testDecode(target, 0, startPosition);

            THEN("The column count is reset to 1")
            {
                REQUIRE(after.column == 1);
            }
            THEN("The line count is incremented")
            {
                REQUIRE(after.line == before.line + 1);
            }
        }
    }
}