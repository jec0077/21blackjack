/**
 * @file blackjack.h
 * @author Josh Campbell (joshecamp04@gmail.com)
 * @brief Blackjack Game Header file
 * @version 0.1.0
 * @date 2024-05-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef BJ_H
#define BJ_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define exit(N)         \
    {                   \
        fflush(stdout); \
        fflush(stderr); \
        _exit(N);       \
    }

#define N_CARDS 52
#define N_SUITS 4
#define BJ 21

int dealt_card_count = 0;

struct Card
{
    char suit; // (S)PADE, (H)EART, (C)LUB, (D)IAMOND
    char rank; // A, K, Q, J, T, 9-2
};

void print_card(struct Card c)
{
    printf("%c %c\n", c.suit, c.rank);
    return;
}

struct Hand
{
    struct Card cards[BJ];
    int value;
    bool empty;
    bool at21;
};

struct Hand *init_hand()
{
    struct Hand *new_hand = malloc(sizeof(struct Hand));
    memset(new_hand, 0x00, sizeof(struct Hand));
    new_hand->empty = true;
    new_hand->at21 = false;

    return new_hand;
}

int get_hand_value(struct Hand *h)
{
    int value = 0;
    if (h->empty)
    {
        return value;
    }

    for (int i = 0; i < BJ; i++)
    {
        if (isalpha(h->cards[i].rank))
        {
            if (h->cards[i].rank == 'T' || h->cards[i].rank == 'K' || h->cards[i].rank == 'Q' || h->cards[i].rank == 'J')
            {
                value += 10;
            }
            else if (h->cards[i].rank == 'A')
            {
                if (value > 10)
                {
                    value++;
                }
                else
                {
                    value += 11;
                }
            }
        }
        else if (h->cards[i].rank != 0)
        {
            value += h->cards[i].rank - '0';
        }
    }

    return value;
}

void print_hand(struct Hand *h)
{
    for (int i = 0; i < BJ; i++)
    {
        if (h->empty)
        {
            printf("HAND IS EMPTY!\n");
            return;
        }

        if (h->cards[i].rank == 0)
        {
            printf("\n");
            return;
        }

        printf("%c,%c  ", h->cards[i].suit, h->cards[i].rank);
    }
}

void clear_hand(struct Hand *h)
{
    memset(h, 0x00, sizeof(struct Hand));
    h->empty = true;
}

struct Deck
{
    struct Card pack[N_CARDS];
};

struct Deck *generate_deck()
{
    struct Deck *new_deck = malloc(sizeof(struct Deck));
    memset(new_deck, 0x00, sizeof(struct Deck));

    char suits[N_SUITS] = {'S', 'H', 'C', 'D'};
    char ranks[N_CARDS / N_SUITS] = {'A', 'K', 'Q', 'J', 'T', '9', '8', '7', '6', '5', '4', '3', '2'};

    int count = 0;
    for (int i = 0; i < N_SUITS; i++)
    {
        for (int j = 0; j < (N_CARDS / N_SUITS); j++)
        {
            new_deck->pack[count].suit = suits[i];
            new_deck->pack[count].rank = ranks[j];
            count++;
        }
    }

    return new_deck;
}

void shuffle_deck(struct Deck *d)
{
    srand(time(0));
    for (int i = N_CARDS - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        struct Card temp = d->pack[i];
        d->pack[i] = d->pack[j];
        d->pack[j] = temp;
    }
    return;
}

void print_deck(struct Deck *d)
{
    for (int i = 0; i < N_CARDS; i++)
    {
        print_card(d->pack[i]);
    }
}

void draw_card(struct Deck *d, struct Hand *h)
{
    int i = 0;
    while (h->cards[i].rank != 0)
    {
        i++;
    }
    h->cards[i] = d->pack[dealt_card_count];
    dealt_card_count++;

    if (dealt_card_count > N_CARDS)
    {
        dealt_card_count = 0;
        shuffle_deck(d);
    }

    h->empty = false;
}

int bust_or_blackjack(struct Hand *h)
{
    if (get_hand_value(h) == BJ)
    {
        h->at21 = true;
        return 1;
    }
    else if (get_hand_value(h) > BJ)
    {
        return -1;
    }

    return 0;
}

void player_input(struct Deck *d, struct Hand *h)
{
    // OPTIONS: Hit Stand Doubledown Split X:surrender
    char input = '0';
    while (get_hand_value(h) != BJ)
    {
        input = '0';
        printf("H - Hit\n");
        printf("S - Stand\n");
        printf("MAKE YOUR MOVE: ~");
        scanf(" %c", &input);
        printf("\n");

        if (toupper(input) == 'H')
        {
            printf("You HIT\n");
            draw_card(d, h);
            print_hand(h);
            printf("Hand total is: %d\n\n", get_hand_value(h));

            int bob = bust_or_blackjack(h);
            if (bob > 0)
            {
                printf("BLACKJACK!!!\n");
                return;
            }
            else if (bob < 0)
            {
                printf("BUSTED!!!\n\n");
                return;
            }
        }
        else if (toupper(input) == 'S')
        {
            printf("You STAND\n");
            print_hand(h);
            printf("Hand total is: %d\n\n", get_hand_value(h));
            return;
        }
    }
}

void setup_hands(struct Deck *d, struct Hand *dealer, struct Hand **hs, int numofplayers)
{

    for (int i = 0; i < numofplayers; i++)
    {
        clear_hand(hs[i]);
        draw_card(d, hs[i]);
        draw_card(d, hs[i]);
        printf("PLAYER %d @ %d\n", i + 1, get_hand_value(hs[i]));
        print_hand(hs[i]);
        printf("\n");
    }

    clear_hand(dealer);
    draw_card(d, dealer);
    printf("DEALER @ %d\n", get_hand_value(dealer));
    print_hand(dealer);
}

void who_won(struct Hand *dealer, struct Hand **hs, int numofplayers)
{
    return;
}

/**
 * Card: class - int:suit, int:rank, bool:ace; funcs - get_suit, get_rank, get_card
 * Hand: class - cards[], int:value; funcs - add_card, draw_card, get_value
 * Deck:
 * make_bet
 * deal_cards
 * hit
 * stand
 * game_step
 * game_exit
 * player_input
 * intro
 *
 */

#endif