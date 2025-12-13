#include <fmt/format.h>
#include <fmt/ranges.h>

#include <array>
#include <functional>
#include <ranges>
#include <set>
#include <span>
#include <string_view>
#include <vector>

namespace bp
{
// Each word maps letters A-Z to numbers 1-26
// Sets for 4 numbers form a "core" sequence number.
// There are 4 operations (+,-,*,/) to apply between the numbers to get core numbers.
// If the result is larger than 4 digis - repeat.
// If the result is impossible (negative, not large enough, fractional) - discard.
// Transform core numbers to letters (1->A, 2->B, ... 26->Z).
// Collect unique letters from all results for a word.

// Looks like first-letters of the result sets - form a phrase.
// STILL WATER TINTS BLANK BOOKS
using std::string_view_literals::operator""sv;
constexpr auto puzzle = std::to_array({
    // clang-format off
     std::to_array({"PIGS"sv, "SAND"sv, "MAIL"sv, "DATE"sv, "HEAD"sv}),
     std::to_array({"CLAM"sv, "PEAK"sv, "HEAT"sv, "JOYA"sv, "WELL"sv, }),
     std::to_array({"TOAD"sv, "CARD"sv, "WILL"sv, "TAPE"sv, "LEGS"sv,}),
     std::to_array({"TREE"sv, "ROAD"sv, "MAID"sv, "SLAB"sv, "ROCK"sv,}),
     std::to_array({"HAND"sv, "VASE"sv, "SAFE"sv, "CLAY"sv, "TOES"sv,}),
});  // clang-format on

constexpr auto toNumbers(std::span<const std::string_view> words)
{
    std::vector<std::vector<std::int64_t>> numbers;
    for (const auto& word : words)
    {
        numbers.push_back(  //
            word
            | std::views::transform(
                [](auto ch) { return static_cast<std::int64_t>(ch - 'A' + 1); })
            | std::ranges::to<std::vector>());
    }
    return numbers;
}

enum Operation
{
    Add,
    Sub,
    Mul,
    Div,
};
constexpr auto format_as(Operation op)
{
    return std::to_underlying(op);
}

constexpr auto solveCoreNumbers()
{
    std::vector<std::vector<std::int64_t>> numbers;

    auto ops = std::to_array({Add, Sub, Mul, Div});
    auto result = std::vector<std::vector<Operation>>{};
    do
    {
        result.push_back(ops | std::ranges::to<std::vector>());
    } while (std::ranges::next_permutation(ops).found);
    return result;
}

auto ops = solveCoreNumbers();
auto applyOps(std::span<Operation> operations, std::span<const std::int64_t> numbers)
{
    namespace rv = std::ranges::views;
    namespace rng = std::ranges;

    return rng::fold_left(  //
        rv::zip(operations, numbers),
        0.0,
        [&](double acc, const auto& val)
        {
            auto [op, num] = val;
            switch (op)
            {
            case Add:
                return acc + num;
            case Sub:
                return acc - num;
            case Mul:
                return acc * num;
            case Div:
                return acc / num;
            }
            return acc;
        });
}

constexpr auto isIntegral(double val)
{
    return val == static_cast<std::int64_t>(val);
}
static_assert(not isIntegral(1.777));
static_assert(isIntegral(123));
static_assert(isIntegral(0));
static_assert(isIntegral(-1));

std::string uniqueCharacters(std::ranges::range auto rng)
{
    return std::ranges::fold_left(  //
        rng,
        std::string{},
        [](auto&& acc, auto ch)
        {
            if (not std::ranges::contains(acc, ch))
                acc.push_back(ch);
            return std::move(acc);
        });
}
}  // namespace bp
int main()
{
    namespace rv = std::ranges::views;
    namespace rng = std::ranges;
    using namespace bp;
    for (auto& row : puzzle)
    {
        auto numbers = toNumbers(row);
        for (const auto& number : numbers)
        {
            auto integralResults =
                ops | rv::transform(std::bind_back(applyOps, number))
                | rv::filter(isIntegral)
                | rv::filter([](auto val)
                             { return val > 0 && val <= 26 || val >= 10000; });

            auto characters = uniqueCharacters(  //
                integralResults
                | rv::transform([](auto val)
                                { return static_cast<char>(val + 'A' - 1); }));

            fmt::print("{},", characters);
        }
        fmt::println("");
    }
}
