#include "support/implementation/Utf8LineColumnDecoder.h"
#include "catch.hpp"

using namespace diagnostics;

namespace {

class TestUtf8LineColumnDecoder : public Utf8LineColumnDecoder
{
public:
    explicit TestUtf8LineColumnDecoder(const uint8_t* source, size_t size)
        : Utf8LineColumnDecoder{ source, size } {}

    auto testDecode(size_t offset, size_t hint, const LineAndColumn& start) const -> LineAndColumn
    {
        return doDecoding(offset, Hint{ hint, start });
    }

    auto testDecode(const LineAndColumn& position, const Hint& hint) const
    {
        return doDecoding(position, hint);
    }
};

}

SCENARIO("Decoding ASCII characters", "[Support][LineColumnDecoder][Utf8]")
{
    const std::string TestSequence = "1\n2\r3\r\n4";
    TestUtf8LineColumnDecoder decoder{ reinterpret_cast<const uint8_t*>(TestSequence.c_str()), sizeof(TestSequence.length()) };
    const LineAndColumn startPosition{ 1, 1 };
    const LineColumnDecoder::Hint startHint{ 0, startPosition };

    GIVEN("A line/column pair to decode into an offset")
    {
        WHEN("The pair exists and is non-special")
        {
            const LineAndColumn target{ 3, 1 };
            auto const correctOffset = TestSequence.find('3');
            auto const offset = decoder.testDecode(target, startHint);

            THEN("The result is valid")
            {
                REQUIRE(offset.has_value());

                AND_THEN("The found position is the requested position")
                {
                    REQUIRE(offset->position == target);
                }
                AND_THEN("The found offset is correct")
                {
                    REQUIRE(offset->offset == correctOffset);
                }
            }
        }
        WHEN("The pair's column does not exist and the pair is non-special")
        {
            const LineAndColumn target{ 3, 3 };
            auto const result = decoder.testDecode(target, startHint);

            THEN("The result is invalid")
            {
                REQUIRE(!result.has_value());
            }
        }

        WHEN("The pair points to the end of a line")
        {
            auto const target = LineAndColumn::endOfLine(3);
            auto const correctOffset = TestSequence.find('3') + 1;
            auto const correctPosition = LineAndColumn{ 3, 2 };
            auto const result = decoder.testDecode(target, startHint);

            THEN("The result is valid")
            {
                REQUIRE(result.has_value());

                AND_THEN("The found position has the correct line/column")
                {
                    REQUIRE(result->position == correctPosition);
                }
                AND_THEN("The found offset is correct")
                {
                    REQUIRE(result->offset == correctOffset);
                }
            }
        }
    }

    GIVEN("An offset to decode into a line/column pair")
    {
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