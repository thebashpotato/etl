/// The black jack code is from the youtube channel Dave's Garage.
/// Give him a like and subscribe, he makes great content.
///
/// @link Link to the video: https://youtu.be/b8V-WIjlScA
/// @link Original code: https://github.com/davepl/blackjack
///
/// In the video Dave does some manual iteration over a C style enum.
/// At the time stamp 8:30 he says "One unfortunate part about C++
/// is that it doesn't provide a way to get the lowest and highest values in an enumeration".
/// Which he is correct. Since this was a problem I had already solved for myself I though his
/// viewers might like to see a solution to this problem.

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <etl.hpp>
#include <iostream>
#include <memory>
#include <optional>
#include <random>
#include <type_traits>
#include <vector>

using namespace etl;

namespace blackjack {
    enum class Rank : std::uint16_t {
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

    enum class Suit : std::uint16_t {
        HEARTS,
        DIAMONDS,
        CLUBS,
        SPADES,
    };

    class Card {
    private:
        Rank rank_;
        Suit suit_;

    public:
        Card(Rank rank, Suit suit) noexcept : rank_(rank), suit_(suit) {}

        [[nodiscard]] auto getRank() const noexcept -> Rank { return rank_; }

        [[nodiscard]] auto getSuite() const noexcept -> Suit { return suit_; }
    };


    class Deck {
    public:
        using UniqueCard = std::unique_ptr<Card>;

    private:
        /// @brief Here is where we use the etl generic enum iterator class
        using RankIterator =
                EnumerationIterator<Rank, Rank::ACE, Rank::KING>;
        using SuitIterator =
                EnumerationIterator<Suit, Suit::HEARTS, Suit::SPADES>;

    private:
        std::vector<UniqueCard> cards_;

    public:
        /// @brief Builds a 52 card deck of 13 ranks with 4 suits,
        ///
        /// @details Uses the custom EnumerationIterator template class to showcase
        /// the C++ ability to iterate over enums.
        Deck() {
            for (const auto &suit: SuitIterator()) {
                for (const auto &rank: RankIterator()) {
                    cards_.emplace_back(std::make_unique<Card>(rank, suit));
                }
            }
        }

        /// @brief How many cards are in the deck
        [[nodiscard]] auto size() -> std::size_t { return cards_.size(); }

        /// @brief Uses a random number and The classic Mersenne Twister,
        /// random number generator to shuffle the deck.
        auto shuffleDeck() {
            std::random_device random_number;
            std::mt19937 generator(random_number());
            std::shuffle(cards_.begin(), cards_.end(), generator);
        }

        /// @brief Draws a single card from the deck if it isn't emptpy.
        ///
        /// @returns Result<UniqueCard, Error>
        [[nodiscard]] auto drawCard() -> Result<UniqueCard, Error> {
            if (cards_.empty()) {
                return Result<UniqueCard, Error>(Error::create("Deck is empty", RUNTIME_INFO));
            }
            auto card = std::move(cards_.back());
            cards_.pop_back();
            return Result<UniqueCard, Error>(std::move(card));
        }
    };


    class Player {
    private:
        std::vector<Deck::UniqueCard> hand_;

    public:
        auto addCard(Deck::UniqueCard &&card) noexcept {
            hand_.emplace_back(std::move(card));
        }

        [[nodiscard]] auto getHandValue() -> uint16_t {
            constexpr auto best_hand_value = 21;
            constexpr auto remove_value = 10;
            constexpr auto hightest_ace_value = 11;

            uint16_t value = 0;
            uint16_t aces = 0;

            for (const auto &card: this->hand_) {
                auto cardValue = card->getRank();
                if (cardValue >= Rank::TEN) {
                    cardValue = Rank::TEN;
                } else if (cardValue == Rank::ACE) {
                    aces++;
                    cardValue = static_cast<Rank>(hightest_ace_value);
                }
                value += static_cast<uint16_t>(cardValue);
            }

            while (value > best_hand_value && aces > 0) {
                value -= remove_value;
                aces--;
            }
            return value;
        }
    };

}// namespace blackjack


auto draw_two_cards(blackjack::Deck &deck, blackjack::Player &entity) {
    for (int i = 0; i < 2; ++i) {
        if (auto result = deck.drawCard(); result.isOk()) {
            entity.addCard(std::move(result.ok().value()));
        } else {
            std::cerr << result.err().value().info() << '\n';
        }
    }
}

auto main() -> int {
    blackjack::Deck deck;
    deck.shuffleDeck();

    blackjack::Player player;
    blackjack::Player dealer;
    std::cout << "Deck has: " << deck.size() << " cards\n";

    draw_two_cards(deck, player);
    draw_two_cards(deck, dealer);

    std::cout << "Player hand value: " << player.getHandValue() << '\n';
    std::cout << "Dealer hand value: " << dealer.getHandValue() << '\n';
    std::cout << "Deck has: " << deck.size() << " cards\n";

    return EXIT_SUCCESS;
}
