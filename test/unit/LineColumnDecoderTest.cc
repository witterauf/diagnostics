#include "diagnostics/support/LineColumnDecoder.h"
#include "catch.hpp"

using namespace diagnostics;

namespace {

class TestLineColumnDecoder : public LineColumnDecoder
{
public:
    explicit TestLineColumnDecoder(const uint8_t* source, size_t size)
        : LineColumnDecoder{ source, size } {}

    mutable std::vector<Hint> providedHints;

protected:
    auto doDecoding(size_t offset, const Hint& hint) const -> LineAndColumn override
    {
        providedHints.push_back(hint);
        return { offset + 1, 1 };
    }

    auto doDecoding(const LineAndColumn& position, const Hint& hint) const -> std::optional<size_t> override
    {
        providedHints.push_back(hint);
        return position.line - 1;
    }
};

auto operator==(const LineColumnDecoder::Hint& a, const LineColumnDecoder::Hint& b)
{
    return a.offset == b.offset && a.position.line == b.position.line && a.position.column == b.position.column;
}

auto operator!=(const LineColumnDecoder::Hint& a, const LineColumnDecoder::Hint& b)
{
    return !(a == b);
}

}

using Catch::Matchers::Equals;

SCENARIO("Caching previous decoding results", "[Support][LineColumnDecoder]")
{
    static constexpr char TestInput[] = "abcdefghijklmnopqrstuvwxyz";
    TestLineColumnDecoder decoder{ reinterpret_cast<const uint8_t*>(TestInput), sizeof(TestInput) + 1 };

    GIVEN("The decoder has no hints yet")
    {
        WHEN("Decoding an offset")
        {
            static constexpr size_t TestOffset = 10;
            auto result = decoder.decode(TestOffset);

            THEN("The search is hinted to start from offset 0")
            {
                REQUIRE(decoder.providedHints[0].offset == 0);
            }
            THEN("The search is hinted to start from position 1:1")
            {
                REQUIRE(decoder.providedHints[0].position.line == 1);
                REQUIRE(decoder.providedHints[0].position.column == 1);
            }
            THEN("The result is stored as a new hint")
            {
                auto const r = std::vector<LineColumnDecoder::Hint>{ LineColumnDecoder::Hint{ 10, result } };
                REQUIRE_THAT(decoder.hints(), Equals(r));
            }
        }

        WHEN("Decoding a line/column pair that exists")
        {
            const LineAndColumn TestPosition{ 10, 1 };
            auto result = decoder.offset(TestPosition);

            THEN("The search is hinted to start from offset 0")
            {
                REQUIRE(decoder.providedHints[0].offset == 0);
            }
            THEN("The search is hinted to start from position 1:1")
            {
                REQUIRE(decoder.providedHints[0].position.line == 1);
                REQUIRE(decoder.providedHints[0].position.column == 1);
            }
            THEN("The result is stored as a new hint")
            {
                REQUIRE(result.has_value());
                auto const r = std::vector<LineColumnDecoder::Hint>{ LineColumnDecoder::Hint{ *result, TestPosition } };
                REQUIRE_THAT(decoder.hints(), Equals(r));
            }
        }
    }

    GIVEN("The decoder has at least one hint")
    {
        decoder.addHint({ 10, { 11, 1 } });
        decoder.addHint({ 15, { 16, 1 } });
        decoder.addHint({ 20, { 21, 1 } });

        WHEN("Decoding an offset that is before all hints")
        {
            static constexpr size_t OffsetBeforeHints = 5;
            auto result = decoder.decode(OffsetBeforeHints);

            THEN("The search is hinted to start from offset 0")
            {
                REQUIRE(decoder.providedHints[0].offset == 0);
            }
            THEN("The search is hinted to start from position 1:1")
            {
                REQUIRE(decoder.providedHints[0].position.line == 1);
                REQUIRE(decoder.providedHints[0].position.column == 1);
            }
        }
        WHEN("Decoding an offset that is after all hints")
        {
            auto const testHint = decoder.hints().back();
            auto const offsetAfterHints = testHint.offset + 1;
            auto result = decoder.decode(offsetAfterHints);

            THEN("The search is hinted to start from the last hint offset")
            {
                REQUIRE(decoder.providedHints[0].offset == testHint.offset);
            }
            THEN("The search is hinted to start from that position")
            {
                REQUIRE(decoder.providedHints[0].position.line == testHint.position.line);
                REQUIRE(decoder.providedHints[0].position.column == testHint.position.column);
            }
        }
        WHEN("Decoding an offset within the hints")
        {
            auto const testHint = decoder.hints()[1];
            const size_t OffsetWithinHints = testHint.offset + 1;
            auto result = decoder.decode(OffsetWithinHints);

            THEN("The search is hinted to start from the next-lowest offset")
            {
                REQUIRE(decoder.providedHints[0].offset == testHint.offset);
            }
            THEN("The search is hinted to start from that position")
            {
                REQUIRE(decoder.providedHints[0].position.line == testHint.position.line);
                REQUIRE(decoder.providedHints[0].position.column == testHint.position.column);
            }
        }
    }
}