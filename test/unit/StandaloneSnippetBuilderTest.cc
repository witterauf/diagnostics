#include "diagnostics/support/StandaloneSnippetBuilder.h"
#include "diagnostics/support/LineColumnDecoder.h"
#include "catch.hpp"

using namespace diagnostics;

static constexpr char TestInput[] = R"txt(1-
2--
3---
4----
5-----)txt";

SCENARIO("Extraction of snippet code", "[Support][StandaloneSnippetBuilder][StandaloneSnippet]")
{
    auto const* input = reinterpret_cast<const uint8_t*>(TestInput);
    auto const size = sizeof(TestInput);
    auto decoder = LineColumnDecoder::make(input, size);
    StandaloneSnippetBuilder builder;
    builder.setSource(input, size);
    builder.setDecoder(decoder.get());

    GIVEN("Building within full line mode on and using line/column pairs")
    {
        builder.onlyFullLines(true);

        WHEN("Using a source range within one line")
        {
            builder.setSourceRange(LineAndColumn{ 4, 2 }, LineAndColumn{ 4, 3 });
            builder.setCursor(LineAndColumn{ 4, 2 });

            THEN("The entire line is returned")
            {
                auto snippet = builder.build();

                REQUIRE(snippet->get() == "4----");
            }
        }
        WHEN("Using a source range spanning more than one line")
        {
            builder.setSourceRange(LineAndColumn{ 4, 2 }, LineAndColumn{ 5, 3 });
            builder.setCursor(LineAndColumn{ 4, 2 });

            THEN("All lines are returned")
            {
                auto snippet = builder.build();

                REQUIRE(snippet->get() == "4----\n5-----");
            }
        }
    }
}