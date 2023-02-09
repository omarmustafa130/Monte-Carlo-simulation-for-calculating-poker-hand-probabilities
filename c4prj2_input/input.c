#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"
#include "cards.h"
#include "future.h"

deck_t * hand_from_string(const char * str, future_cards_t * fc)
{
    size_t len = strlen(str);
    deck_t * lineDeck = malloc(sizeof(*lineDeck));
    lineDeck->cards = NULL;
    lineDeck->n_cards = 0;
    for(size_t i = 0; i<len; i++)
    {
        if(str[i] == '\n' || str[i] == ' ')
            continue;
        else
        {      
            if(str[i] == '?')
            {
                i++;
                char num[strlen(str)];
                int n=0;
                while(!((str[i] == '\n')||(str[i] == ' '))) 
                {
                    num[n]=str[i];
                    i++;
                    n++;
                }
                num[n]='\0';
                add_future_card(fc,atoi(num),add_empty_card(lineDeck)) ;

            }
            else
            {
                card_t c = card_from_letters(str[i], str[i+1]);
                add_card_to(lineDeck, c);
                i++;
            }
            
        }
    }

    if(lineDeck->n_cards <5)
    {
        perror("Not enough cards in deck!\n");
        return NULL;
    }
    return lineDeck;
}


deck_t ** read_input(FILE * f, size_t * n_hands, future_cards_t * fc)
{
    deck_t ** decks = NULL;
    char * line = NULL;
    size_t sz = 0;
    size_t idx = 0;
    while((getline(&line, &sz, f))>=0)
    {
        decks = realloc(decks, (idx+1) * sizeof(*decks));
        deck_t * lineDeck = hand_from_string(line, fc);
        if (lineDeck == NULL)
            continue;
        decks[idx] = lineDeck;
        idx++;
    }
    free(line);
    *n_hands = idx;
    return decks;
}
