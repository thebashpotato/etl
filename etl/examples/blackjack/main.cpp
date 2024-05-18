#include "blackjack.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <etl.hpp>
#include <iostream>
#include <memory>
#include <random>
#include <utility>

namespace blackjack
{

Card::Card(Rank rank, Suit suit) noexcept : _rank(rank), _suit(suit)
{
}

auto Card::getRank() const noexcept -> Rank
{
    return _rank;
}

auto Card::getSuite() const noexcept -> Suit
{
    return _suit;
}

Deck::Deck() noexcept
{
    for (auto const &suit : SuitIterator())
    {
        for (auto const &rank : RankIterator())
        {
            _cards.emplace_back(std::make_unique<Card>(rank, suit));
        }
    }
}

auto Deck::size() -> std::size_t
{
    return _cards.size();
}

auto Deck::shuffleDeck() -> void
{
    std::random_device random_number;
    std::mt19937 generator(random_number());
    std::shuffle(_cards.begin(), _cards.end(), generator);
}

auto Deck::drawCard() -> Result<UniqueCard, Error>
{
    if (_cards.empty())
    {
        return Result<UniqueCard, Error>(Error::create("Deck is empty", RUNTIME_INFO));
    }
    auto card = std::move(_cards.back());
    _cards.pop_back();
    return Result<UniqueCard, Error>(std::move(card));
}

auto Player::addCard(Deck::UniqueCard &&card) noexcept
{
    _hand.emplace_back(std::move(card));
}

auto Player::getHandValue() -> uint16_t
{
    constexpr auto best_hand_value = 21;
    constexpr auto remove_value = 10;
    constexpr auto hightest_ace_value = 11;

    uint16_t value = 0;
    uint16_t aces = 0;

    for (const auto &card : this->_hand)
    {
        auto cardValue = card->getRank();
        if (cardValue >= Rank::TEN)
        {
            cardValue = Rank::TEN;
        }
        else if (cardValue == Rank::ACE)
        {
            aces++;
            cardValue = static_cast<Rank>(hightest_ace_value);
        }
        value += static_cast<uint16_t>(cardValue);
    }

    while (value > best_hand_value && aces > 0)
    {
        value -= remove_value;
        aces--;
    }
    return value;
}

} // namespace blackjack

auto draw_two_cards(blackjack::Deck &deck, blackjack::Player &entity)
{
    for (int i = 0; i < 2; ++i)
    {
        if (auto result = deck.drawCard(); result.is_ok())
        {
            entity.addCard(std::move(result.ok().value()));
        }
        else
        {
            std::cerr << result.err().value().info() << '\n';
        }
    }
}

auto main() -> int
{
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
