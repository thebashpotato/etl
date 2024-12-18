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

namespace Blackjack
{

Card::Card(Rank rank, Suit suit) noexcept : m_rank(rank), m_suit(suit)
{
}

auto Card::rank() const noexcept -> Rank
{
    return m_rank;
}

auto Card::suit() const noexcept -> Suit
{
    return m_suit;
}

Deck::Deck() noexcept
{
    for (const auto &suit : SuitIterator())
    {
        for (const auto &rank : RankIterator())
        {
            m_cards.emplace_back(std::make_unique<Card>(rank, suit));
        }
    }
}

auto Deck::size() -> std::size_t
{
    return m_cards.size();
}

auto Deck::ShuffleDeck() -> void
{
    std::random_device random_number;
    std::mt19937 generator(random_number());
    std::shuffle(m_cards.begin(), m_cards.end(), generator);
}

auto Deck::DrawCard() -> etl::Result<UniqueCard, Error>
{
    if (m_cards.empty())
    {
        return etl::Result<UniqueCard, Error>(Error("Deck is empty", etl::RUNTIME_INFO));
    }
    auto card = std::move(m_cards.back());
    m_cards.pop_back();
    return etl::Result<UniqueCard, Error>(std::move(card));
}

auto Player::AddCard(Deck::UniqueCard &&card) noexcept
{
    m_hand.emplace_back(std::move(card));
}

auto Player::GetHandValue() -> std::uint16_t
{
    constexpr auto best_hand_value = 21;
    constexpr auto remove_value = 10;
    constexpr auto hightest_ace_value = 11;

    std::uint16_t value = 0;
    std::uint16_t aces = 0;

    for (const auto &card : this->m_hand)
    {
        auto card_value = card->rank();
        if (card_value >= Rank::Ten)
        {
            card_value = Rank::Ten;
        }
        else if (card_value == Rank::Ace)
        {
            aces++;
            card_value = static_cast<Rank>(hightest_ace_value);
        }
        value += static_cast<std::uint16_t>(card_value);
    }

    while (value > best_hand_value && aces > 0)
    {
        value -= remove_value;
        aces--;
    }
    return value;
}

} // namespace Blackjack

void DrawTwoCards(Blackjack::Deck &deck, Blackjack::Player &entity)
{
    for (int i = 0; i < 2; ++i)
    {
        if (auto result = deck.DrawCard(); result.is_ok())
        {
            entity.AddCard(std::move(*result.ok()));
        }
        else
        {
            std::cerr << result.err().value().info() << '\n';
        }
    }
}

auto main() -> int
{
    Blackjack::Deck deck;
    deck.ShuffleDeck();

    Blackjack::Player player;
    Blackjack::Player dealer;
    std::cout << "Deck has: " << deck.size() << " cards\n";

    DrawTwoCards(deck, player);
    DrawTwoCards(deck, dealer);

    std::cout << "Player hand value: " << player.GetHandValue() << '\n';
    std::cout << "Dealer hand value: " << dealer.GetHandValue() << '\n';
    std::cout << "Deck has: " << deck.size() << " cards\n";

    return EXIT_SUCCESS;
}
