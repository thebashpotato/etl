/// The black jack code is from the youtube channel Dave's Garage.
/// Give him a like and subscribe, he makes great content.
///
/// @link Link to the video: https://youtu.be/b8V-WIjlScA
/// @link Original code: https://github.com/davepl/blackjack
///
/// In the video Dave does some manual iteration over a C style enum.
/// At the time stamp 8:30 he says "One unfortunate part about C++
/// is that it doesn't provide a way to get the lowest and highest values in an enumeration".
/// Which he is correct. Since this was a problem I had already solved for myself I thought
/// I would share my solution with him and the world (if anyone ever sees this).
#pragma once

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <etl.hpp>
#include <memory>
#include <vector>

namespace Blackjack
{

/// @brief Create a basic Error class which inherits from the etl::BaseError
class Error : public etl::BaseError
{
  public:
    explicit Error(const std::string_view &msg) noexcept : etl::BaseError(msg)
    {
    }

    Error(const std::string_view &msg, const etl::detail::SourceCodeLocation &slc) noexcept : etl::BaseError(msg, slc)
    {
    }
};

enum class Rank : std::uint8_t
{
    Ace,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
};

enum class Suit : std::uint8_t
{
    Hearts,
    Diamonds,
    Clubs,
    Spades,
};

class Card
{
  public:
    Card(Rank rank, Suit suit) noexcept;

    [[nodiscard]] auto rank() const noexcept -> Rank;

    [[nodiscard]] auto suit() const noexcept -> Suit;

  private:
    Rank m_rank;
    Suit m_suit;
};

class Deck
{
  public:
    using UniqueCard = std::unique_ptr<Card>;

  private:
    /// @brief Here is where we use the etl generic enum iterator class
    using RankIterator = etl::EnumerationIterator<Rank, Rank::Ace, Rank::King>;
    using SuitIterator = etl::EnumerationIterator<Suit, Suit::Hearts, Suit::Spades>;

  public:
    /// @brief Builds a 52 card deck of 13 ranks with 4 suits,
    ///
    /// @details Uses the custom EnumerationIterator template class to showcase
    /// the C++ ability to iterate over enums.
    Deck() noexcept;

    /// @brief How many cards are in the deck
    [[nodiscard]] auto size() -> std::size_t;

    /// @brief Uses a random number and The classic Mersenne Twister,
    /// random number generator to shuffle the deck.
    void ShuffleDeck();

    /// @brief Draws a single card from the deck if it isn't emptpy.
    ///
    /// @returns Result<UniqueCard, Error>
    [[nodiscard]] auto DrawCard() -> etl::Result<UniqueCard, Error>;

  private:
    std::vector<UniqueCard> m_cards;
};

class Player
{
  public:
    auto AddCard(Deck::UniqueCard &&card) noexcept;

    [[nodiscard]] auto GetHandValue() -> uint16_t;

  private:
    std::vector<Deck::UniqueCard> m_hand;
};

} // namespace Blackjack
