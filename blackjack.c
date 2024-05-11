/**
 * @file blackjack.c
 * @author Josh Campbell (joshecamp04@gmail.com)
 * @brief Blackjack Game main C file
 * @version 0.3.0
 * @date 2024-05-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "blackjack.h"

int main(int argc, char *argv[])
{
    int num_players = 0;
    if (argc == 1)
    {
        num_players++;
    }
    else
    {
        if (atoi(argv[1]) > 3)
        {
            num_players += 3;
        }
        else
        {
            num_players += atoi(argv[1]);
        }
    }

    printf("BLACKJACK %d PLAYER(s)\n\n", num_players);

    struct Hand *dealer = init_hand();
    struct Hand *players[num_players];
    for (int i = 0; i < num_players; i++)
    {
        players[i] = init_hand();
    }
    struct Deck *main_deck = generate_deck();
    shuffle_deck(main_deck);

    char tobecontinued = 'Y';

    while (toupper(tobecontinued) != 'N')
    {
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("DEALING NEW CARDS...\n\n");
        setup_hands(main_deck, dealer, players, num_players);
        printf("~~~~~~~~~~~~~~~~~~~~~~~~~\n");

        for (int i = 0; i < num_players; i++)
        {
            printf("(!) PLAYER %d's turn! You're @ %d\n", i + 1, get_hand_value(players[i]));
            player_input(main_deck, players[i]);
        }

        printf("(!) DEALER's turn! They're @ %d\n", get_hand_value(dealer));
        auto_play(main_deck, dealer);

        who_won(dealer, players, num_players);

        printf("\n(!) CONTINUE? Y/N ");
        scanf(" %c", &tobecontinued);
    }

    end_game(main_deck, dealer, players, num_players);
    return 0;
}