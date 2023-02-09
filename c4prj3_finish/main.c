#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "cards.h"
#include "deck.h"
#include "eval.h"
#include "future.h"
#include "input.h"

void monteCarloRound (deck_t ** hands, size_t n_hands,  unsigned * count_arr)
{
  int res = 0;
  int curridx = 0;
  size_t tie_flag = 0;
  for (int j = 1; j<n_hands; j++)
  {

      res = compare_hands(hands[curridx], hands[j]);
      if (res == 0)
      {
        tie_flag = 1;
      }
      else if(res == -1)
      {
        tie_flag = 0;
        curridx = j;
      }
  }
  if (tie_flag == 1)
    count_arr[n_hands]++;
  else
    count_arr[curridx]++;
}



void freeAll(future_cards_t * fc, deck_t ** decks_read, deck_t * remaining_deck, unsigned * count_arr, size_t n_hands)
{
  for (size_t i = 0; i<fc->n_decks; i++)
  {
    free(fc->decks[i].cards);
  }
  free(fc->decks);
  free(fc);


  for(size_t i =0; i<(n_hands); i++)
  {
    free_deck(decks_read[i]);
  }
  free(decks_read);

  free_deck(remaining_deck);

  free(count_arr);
}


int main(int argc, char ** argv) {
  if(argc == 1)
  {
    perror("Not enough arguments!\n");
    return EXIT_FAILURE;
  }
 
  else if(argc > 3)
  {
    perror("Too many arguments!\n");
    return EXIT_FAILURE;
  }


  int num_trials=0;
  if(argc == 2)
    num_trials = 10000;
  else if (argc == 3)
    num_trials = atoi(argv[2]);

  FILE * f = fopen(argv[1], "r");
  size_t n_hands;
  n_hands = 0;
  future_cards_t * fc =(future_cards_t *) malloc(sizeof(*fc));
  fc->decks = NULL;
  fc->n_decks = 0;
  deck_t ** decks_read = read_input(f, &n_hands, fc); 
  deck_t * remaining_deck = build_remaining_deck(decks_read, n_hands);
  
  unsigned * count_arr = malloc((n_hands + 1)* sizeof(*count_arr));
  for (size_t i = 0; i<(n_hands)+1; i++)
    count_arr[i] = 0;
  
  for (size_t i = 0; i<num_trials; i++)
  {
    shuffle(remaining_deck);
    future_cards_from_deck(remaining_deck, fc);
    monteCarloRound(decks_read, n_hands, count_arr);
  }

  //       "Hand %zu won %u / %u times (%.2f%%)\n"
  for(size_t i =0; i<n_hands; i++)
  {
    printf("Hand %zu won %u / %u times (%.2f%%)\n", i, count_arr[i], num_trials, 100*(float)count_arr[i]/(float)num_trials);
  }

  printf("And there were %u ties\n", count_arr[n_hands]);


  freeAll(fc, decks_read, remaining_deck, count_arr, n_hands);
  if(fclose(f)!=0)
  {
    perror("Error closing the file!\n");
    return EXIT_FAILURE;
  }


  return EXIT_SUCCESS;
}
