/*Use a struct to define a card as an enumerated member that is its suit value and a short that is its pips value.

Write a function that randomly shuffles the deck.

Submit your work as a text file. 

Then deal out 7 card hands and evaluate the probability that a hand has no pair, one pair, two pair, three of a kind, full house and 4 of a kind.  
This is a Monte Carlo method to get an approximation to these probabilities.  Use at least 1 million randomly generated hands.*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUITS 4
#define PIPS 13
#define DECK_SIZE (SUITS * PIPS)

#define DECKS_NUMBER 200000
#define HAND_SIZE 7

typedef enum suit
{
    hearts,
    diamonds,
    spades,
    clubs
} suit;

typedef struct playing_card
{
    suit suit;
    short pip;
} card;

struct hand_stats
{
    unsigned long total;
    unsigned long no_pair;
    unsigned long one_pair;
    unsigned long three_of_kind;
    unsigned long two_pair;
    unsigned long four_of_kind;
    unsigned long full_house;
};

void shuffle_deck(card *deck, int length)
{
    for (int i = 1;  i < length;  ++i) {
        int j = rand() % (i+1);
        /* swap cards i and j */
        card tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }
}

void evaluate_hand(card* hand, int length, struct hand_stats *stats)
{
    unsigned value_count[PIPS] = { 0 }; /* how many cards of each pip value */
    for (int i = 0;  i < length;  ++i) {
        ++value_count[hand[i].pip];
    }

    unsigned count[SUITS+1] = { 0 }; /* how many singletons, pairs etc */
    for (int i = 0;  i < PIPS;  ++i) {
        ++count[value_count[i]];
    }

    if (count[4]) {
        ++stats->four_of_kind;
    } else if (count[3] && count[2] || count[3] >= 2) {
        ++stats->full_house;
    } else if (count[3]) {
        ++stats->three_of_kind;
    } else if (count[2] >= 2) {
        ++stats->two_pair;
    } else if (count[2]) {
        ++stats->one_pair;
    } else {
        ++stats->no_pair;
    }
    ++stats->total;
}


int main(void)
{
    srand((unsigned)time(0));

    card deck[DECK_SIZE];

    int i = 0;
    for (short j = 1;  j <= PIPS;  ++j) {
        for (short k = 1;  k <= SUITS;  ++k) {
            deck[i].suit = k;
            deck[i].pip = j;
            ++i;
        }
    }

    struct hand_stats stats = { 0, 0, 0, 0, 0, 0, 0 };
    for (int i = 0;  i < DECKS_NUMBER;  ++i) {
        shuffle_deck(deck, DECK_SIZE);
        for (int j = 0;  j + HAND_SIZE < DECK_SIZE;  j += HAND_SIZE) {
            evaluate_hand(deck+j, HAND_SIZE, &stats);
        }
    }

    /* now print the results */
    const long double total = stats.total;
    printf("No pair probablity =  %Lf\n",        stats.no_pair       / total);
    printf("One pair probability = %Lf\n",       stats.one_pair      / total);
    printf("Two pair probability = %Lf\n",       stats.two_pair      / total);
    printf("Three of a kind probablity = %Lf\n", stats.three_of_kind / total);
    printf("Four of a kind probability = %Lf\n", stats.four_of_kind  / total);
    printf("Full house probability = %Lf\n",     stats.full_house    / total);
}
