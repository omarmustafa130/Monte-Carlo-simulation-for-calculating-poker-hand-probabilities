#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "deck.h"
/*
   struct card_tag
   {
   unsigned value;
   suit_t suit;
   };
   typedef struct card_tag card_t;

   void print_card(card_t c);
   
   struct deck_tag 
   {
     card_t ** cards;
     size_t n_cards;
   };
   typedef struct deck_tag deck_t;

*/

/*
 - void print_hand(deck_t * hand);
   This should print out the contents of a hand.
   It should print each card (recall that
   you wrote print_card in Course 2), and
   a space after each card.  Do not put
   a newline after the hand, as this
   function gets called to print a hand
   in the middle of a line of output.
*/
void print_hand(deck_t * hand){
  for(int i=0; i<(hand->n_cards); i++)
  {
    print_card((**(hand->cards+i)));
    printf(" ");
  }
}


/*
 - int deck_contains(deck_t * d, card_t c);
   This function should check if a deck
   contains a particular card or not.  If
   the deck does contain the card, this
   function should return 1.  Otherwise,
   it should return 0.
   (You will use this later to
   build the deck of remaining cards
   which are not in any player's hand).
*/
int deck_contains(deck_t * d, card_t c) {
  for(int i=0; i<(d->n_cards); i++)
  {
    if((*((d->cards)+i))->value == c.value && (*((d->cards)+i))->suit == c.suit)
      return 1;
  }
  return 0;
  
}
/*
 - void shuffle(deck_t * d);
   This function takes in a deck an shuffles
   it, randomly permuting the order of the cards.
   There are MANY valid ways to shuffle a deck
   of cards---we'll leave the specifics
   of the algorithm design up to you.  However,
   you will want to use random() to generate
   pseudo-random numbers.  (Pseudo-random
   numbers are quite sufficient here,
   since they are not used for security
   purposes). Note that you should not need to
   use the 'srand' function.
*/
void shuffle(deck_t * d){
  
  card_t ** card_temp = d->cards;
  unsigned int random_num;
  card_t* temp;
  for (int i =(d->n_cards)-1; i>0; i--)
  {
    random_num=rand()%(i+1);
    temp = card_temp[random_num];
    card_temp[random_num] = card_temp[i];
    card_temp[i] = temp;
    }
}


/*
- void assert_full_deck(deck_t * d);
  This function should check that
  the passed in deck contains ever
  valid card exactly once.  If
  the deck has any problems, this function
  should fail an assert.  This will
  be used to help you test your deck
  shuffling: we will shuffle
  a full deck, then call assert_full_deck,
  so that you can identfiy problems with
  the deck.  You can print
  any error messages you want
  if there is a problem.
  Hint: you already wrote deck_contains.
*/
void assert_full_deck(deck_t * d) {
  size_t assert_check=1;
  for (int i = 0; i<(d->n_cards); i++)
    for(int k = i+1; k< (d->n_cards); k++)
      if((*((d->cards)+i))->value == (*((d->cards)+k))->value && (*((d->cards)+i))->suit == (*((d->cards)+k))->suit){
	assert_check = 0;
	break;
      }
  assert(assert_check);
}

/*
 - void add_card_to(deck_t * deck, card_t c);
   Add the particular card to the given deck (which will
   involve reallocing the array of cards in that deck).
*/
void add_card_to(deck_t * deck, card_t c)
{
    deck->cards = realloc(deck->cards, (deck->n_cards + 1)*sizeof(*(deck->cards)));
    card_t * one_card = malloc(sizeof(one_card));
    one_card->value = c.value;
    one_card->suit = c.suit;
    deck->cards[deck->n_cards]= one_card;
    deck->n_cards++;  
}

/*
 - card_t * add_empty_card(deck_t * deck);
   Add a card whose value and suit are both 0, and return a pointer
   to it in the deck.
   This will add an invalid card to use as a placeholder
   for an unknown card.
*/

card_t * add_empty_card(deck_t * deck)
{
    card_t empty_card;
    empty_card.value = 0;
    empty_card.suit = 0;
    add_card_to(deck, empty_card);
    return deck->cards[deck->n_cards - 1];
}

/*
 - deck_t * make_deck_exclude(deck_t * excluded_cards);
   Create a deck that is full EXCEPT for all the cards
   that appear in excluded_cards.  For example,
   if excluded_cards has Kh and Qs, you would create
   a deck with 50 cards---all of them except Kh and Qs.
   You will need to use malloc to allocate this deck.
   (You will want this for the next function).
   Don't forget you wrote card_t card_from_num(unsigned c)
   in Course 2 and int deck_contains(deck_t * d, card_t c)
   in Course 3!  They might be useful here.

   */

deck_t * make_deck_exclude(deck_t * excluded_cards)
{
    deck_t * full_deck_exc = malloc(sizeof(*full_deck_exc));
    full_deck_exc->n_cards = 0;
    full_deck_exc->cards = NULL;
    for (unsigned i = 0; i<52; i++)
    {         
        card_t c = card_from_num(i);
        if(deck_contains(excluded_cards, c) ==0)
        {
            add_card_to(full_deck_exc, c);
        }
        else
        {
            continue;
        }
    }

    return full_deck_exc;
}


/*
 - deck_t * build_remaining_deck(deck_t ** hands, size_t n_hands) ;
   This function takes an array of hands (remember
   that we use deck_t to represent a hand).  It then builds
   the deck of cards that remain after those cards have
   been removed from a full deck.  For example, if we have
   two hands:
      Kh Qs ?0 ?1 ?2 ?3 ?4
      As Ac ?0 ?1 ?2 ?3 ?4
   then this function should build a deck with 48
   cards (all but As Ac Kh Qs).  You can just build
   one deck with all the cards from all the hands
   (remember you just wrote add_card_to),
   and then pass it to make_deck_exclude.
*/
deck_t * build_remaining_deck(deck_t ** hands, size_t n_hands)
{
    deck_t * excluded_cards = malloc(sizeof(*excluded_cards));
    excluded_cards->cards = NULL;
    excluded_cards->n_cards = 0;
    for (size_t i = 0; i<n_hands; i++)
    {
        deck_t * deck_temp = hands[i];
        for(int k = 0; k<deck_temp->n_cards; k++)
        {
            card_t c;
            c.suit = deck_temp->cards[k]->suit;
            c.value = deck_temp->cards[k]->value;
            add_card_to(excluded_cards, c);
        }
    }
    deck_t * deck_final = make_deck_exclude(excluded_cards);
    return deck_final;
}

/*
 - void free_deck(deck_t * deck) ;
   Free the memory allocated to a deck of cards.
   For example, if you do
     deck_t * d = make_excluded_deck(something);
     free_deck(d);
   it should free all the memory allocated by make_excluded_deck.
   Once you have written it, add calls to free_deck anywhere you
   need to to avoid memory leaks.
*/

void free_deck(deck_t * deck)
{
    for (size_t i =0; i<deck->n_cards; i++)
    {
        free(deck->cards[i]);
    }
    free(deck->cards);
    free(deck);
}
