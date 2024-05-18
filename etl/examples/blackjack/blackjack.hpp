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

#include <cstdlib>
#include <ctime>
#include <etl.hpp>
#include <memory>
#include <vector>

using namespace etl;

namespace blackjack
{
enum class Rank : std::uint16_t
{
    ACE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING
};

enum class Suit : std::uint16_t
{
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES,
};

class Card
{
  private:
    Rank _rank;
    Suit _suit;

  public:
    Card(Rank rank, Suit suit) noexcept;

    [[nodiscard]] auto getRank() const noexcept -> Rank;

    [[nodiscard]] auto getSuite() const noexcept -> Suit;
};

class Deck
{
  public:
    using UniqueCard = std::unique_ptr<Card>;

  private:
    /// @brief Here is where we use the etl generic enum iterator class
    using RankIterator = EnumerationIterator<Rank, Rank::ACE, Rank::KING>;
    using SuitIterator = EnumerationIterator<Suit, Suit::HEARTS, Suit::SPADES>;

  private:
    std::vector<UniqueCard> _cards;

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
    auto shuffleDeck() -> void;

    /// @brief Draws a single card from the deck if it isn't emptpy.
    ///
    /// @returns Result<UniqueCard, Error>
    [[nodiscard]] auto drawCard() -> Result<UniqueCard, Error>;
};

class Player
{
  private:
    std::vector<Deck::UniqueCard> _hand;

  public:
    auto addCard(Deck::UniqueCard &&card) noexcept;

    [[nodiscard]] auto getHandValue() -> uint16_t;
};

} // namespace blackjack
