#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int card_ptr_comp(const void * vp1, const void * vp2) {
  const card_t * const * cp1 = vp1;
  const card_t * const * cp2 = vp2;
  if((**cp1).value > (**cp2).value)
    return -1;
  else if((**cp1).value < (**cp2).value)
    return 1;
  else
  {
    if ((**cp1).suit > (**cp2).suit)
      return -1;
    else
      return 1;
  }
  return 0;
}

suit_t flush_suit(deck_t * hand) {
  /*
 - suit_t flush_suit(deck_t * hand);
   This function looks at the hand and determines
   if a flush (at least 5 cards of one suit) exists.
   If so, it returns the suit of the cards comprising
   the flush.  If not, it returns NUM_SUITS.
   For example:
   Given Ks Qs 0s 9h 8s 7s, it would return SPADES.
   Given Kd Qd 0s 9h 8c 7c, it would return NUM_SUITS.
*/
 
  size_t s=0;
  size_t c=0;
  size_t h=0;
  size_t d=0;
  card_t current_card;
  
  for (size_t i =0; i<(hand->n_cards); i++)
  {
    current_card = **(hand->cards+i);
    switch(current_card.suit)
    {
      case SPADES:
	s++;
	break;
      case HEARTS:
        h++;
        break;
      case DIAMONDS:
        d++;
        break;
      case CLUBS:
        c++;
        break;
      default:
	break;
    }
  }
  if(s>=5)
    return SPADES;
  else if(c>=5)
    return CLUBS;
  else if(h>=5)
    return HEARTS;
  else if(d>=5)
    return DIAMONDS;
  else
  {
    return NUM_SUITS;
  }
}

unsigned get_largest_element(unsigned * arr, size_t n) {
  /*- unsigned get_largest_element(unsigned * arr, size_t n);
   This function returns the largest element in an array
   of unsigned integers.  This should be familiar
   from the videos you watched.*/
  unsigned max = arr[0];
  
  for(unsigned i = 1; i<n; i++)
  {
    if (arr[i]>max)
      max = arr[i];
  }
  return max;
}

size_t get_match_index(unsigned * match_counts, size_t n,unsigned n_of_akind)
{
  size_t index=-1;
  for (size_t i = 0; i<n; i++)
  {
    if (match_counts[i] == n_of_akind)
    {
      index = i;
      break;
    }
  }
  assert(index!=-1);
  return index;
}

ssize_t  find_secondary_pair(deck_t * hand,
			     unsigned * match_counts,
			     size_t match_idx) {
  
  card_t current_card;
  
  ssize_t index = -1;
  unsigned match_idx_value = (**(hand->cards + match_idx)).value;
  for(size_t i = 0; i<hand->n_cards; i++)
  {
    current_card = **(hand->cards+i);
    //check its match index
    if(match_counts[i] > 1 && current_card.value != match_idx_value)
    {  index = i;
      return index;}
  }
  
  return -1;
}
//As Ah Qc Jc 4c


int is_ace_low_straight_at(deck_t * hand, size_t index, suit_t fs)
{
  card_t current_card;
  unsigned counter = 1;
  //ACE is the highest value so it'd be upfornt
  if((**(hand->cards+index)).value != VALUE_ACE)
    return 0;
  //we should look for a straight with no specific suit in mind
  if(fs == NUM_SUITS)
  {
    //since an ace low straight goes like : As 5h 4c 3d 2s, so 5 is the next element, but ace value is 14 and 5's value is 5
    //so we will loop until we find 5 then perform normal iteration
    size_t new_index= 0;
    size_t size = hand->n_cards;
    for(size_t i = index+1; i<size; i++)
    {
      if((**(hand->cards+i)).value == 5)
      {
	new_index = i;
	counter++;
	break;
      }
    }
    //the 5 should be in an index where there is enough place for 4,3,2
    //so it should not be in index 0, because then it means that there is no A, it shouldn't be above index 3
    if(new_index==0 || new_index>3)
      return 0;
    unsigned straight_last_value = (**(hand->cards + new_index)).value;
    //now we will iterate the normal way
    for(size_t j = new_index; j<size ; j++)
    {
      current_card = (**(hand->cards+j));
      if(current_card.value == straight_last_value-1){
	counter++;
	straight_last_value = current_card.value;
      }
      else
	continue;
    }
    if (counter >=5)
      return 1;
    else
      return 0;
  }
  else
  {
    size_t new_index= 0;
    size_t size = hand->n_cards;
    if ((**(hand->cards+index)).suit != fs){return 0;}
    for(size_t i = index; i<size; i++)
    {
      if((**(hand->cards+i)).value == 5 && (**(hand->cards+i)).suit == fs)
      {
        new_index = i;
	counter++;
        break;
      }
    }
  
    if(new_index==0 || new_index>3)
      return 0;
    unsigned straight_last_value = (**(hand->cards + new_index)).value;
 
    for(size_t j = new_index+1; j<size ; j++)
    {
      current_card = (**(hand->cards+j));
      if(current_card.value == straight_last_value-1 && current_card.suit == fs){
        counter++;
        straight_last_value = current_card.value;
      }
      else
        continue;
    }
    if (counter >=5)
      return 1;
    else
      return 0;
  }
  
  
}

/******************************************************************************/

int is_straight_at(deck_t * hand, size_t index, suit_t fs) {

  //  card_t current_card;
  unsigned counter = 1;
  //  unsigned counter_ACE = 0;
  if (is_ace_low_straight_at(hand,index,fs)==1){return -1;}
  if (fs == NUM_SUITS)
  {
    // we should look for any straight
    unsigned straight_last_value = (**(hand->cards + index)).value;
    //    if (straight_last_value == VALUE_ACE)
    //      counter_ACE=1;
    for(size_t i = index+1; i<hand->n_cards; i++)
    {
      if( (**(hand->cards+i)).value == straight_last_value-1)
      {
	straight_last_value = (**(hand->cards+i)).value;
	counter++;
      }
      else
	continue;
    }
    if (counter>=5)
      return 1;
    else
      return 0;
  }
  else
  {
    // we should look for a straight flush of a 'fs' suit
    
    unsigned straight_last_value = (**(hand->cards + index)).value;
    if((**(hand->cards + index)).suit != fs) return 0;
    //  if(straight_last_value== VALUE_ACE)
    //  counter_ACE=1;
    
    for(size_t i = index+1; i<hand->n_cards; i++)
    {
      if((**(hand->cards + i)).value == straight_last_value-1 && (**(hand->cards + i)).suit == fs)
      {
        straight_last_value = (**(hand->cards + i)).value;
        counter++;
      }
      else
        continue;
    }
    if(counter >= 5)
      return 1;
    else
      return 0;
  }
}

/*****************************************************************************************/

hand_eval_t build_hand_from_match(deck_t * hand,
				  unsigned n,
				  hand_ranking_t what,
				  size_t idx) {

  hand_eval_t ans;
  ans.ranking = what;
  unsigned counter=n;
  for (unsigned i = 0; i<n; i++)
  {
    ans.cards[i] = *(hand->cards+idx+i);
  }
  for(unsigned j=0; j<idx; j++)
  {
    ans.cards[j+n] = *(hand->cards+j);
    counter++;
    if(counter == 5)
      break;
  }
  if(counter<5)
  {
    for(int k = idx+n; k<hand->n_cards; k++)
    {
      ans.cards[counter] = *(hand->cards+k);
      counter++;
      if(counter == 5)
	break;
    }
  }
    
  return ans;
}


int compare_hands(deck_t * hand1, deck_t * hand2) {
  qsort(hand1->cards, hand1->n_cards, sizeof(card_t), card_ptr_comp);
  qsort(hand2->cards, hand2->n_cards, sizeof(card_t), card_ptr_comp);
  hand_eval_t hand1_evaluated= evaluate_hand(hand1);
  hand_eval_t hand2_evaluated= evaluate_hand(hand2);
  if (hand1_evaluated.ranking < hand2_evaluated.ranking)
    return 1;
  else if (hand1_evaluated.ranking > hand2_evaluated.ranking)
    return -1;
  else
  {
    for(size_t i = 0; i<5; i++)
    {
      card_t* card_1 = hand1_evaluated.cards[i];
      card_t* card_2 = hand2_evaluated.cards[i];
      if (card_1->value > card_2->value)
	return 1;
      else if (card_1->value < card_2->value)
        return -1;
      else
	continue;
    }
  }
  return 0;
}



//You will write this function in Course 4.
//For now, we leave a prototype (and provide our
//implementation in eval-c4.o) so that the
//other functions we have provided can make
//use of get_match_counts.
unsigned * get_match_counts(deck_t * hand) ;

// We provide the below functions.  You do NOT need to modify them
// In fact, you should not modify them!


//This function copies a straight starting at index "ind" from deck "from".
//This copies "count" cards (typically 5).
//into the card array "to"
//if "fs" is NUM_SUITS, then suits are ignored.
//if "fs" is any other value, a straight flush (of that suit) is copied.
void copy_straight(card_t ** to, deck_t *from, size_t ind, suit_t fs, size_t count) {
  assert(fs == NUM_SUITS || from->cards[ind]->suit == fs);
  unsigned nextv = from->cards[ind]->value;
  size_t to_ind = 0;
  while (count > 0) {
    assert(ind < from->n_cards);
    assert(nextv >= 2);
    assert(to_ind <5);
    if (from->cards[ind]->value == nextv &&
	(fs == NUM_SUITS || from->cards[ind]->suit == fs)){
      to[to_ind] = from->cards[ind];
      to_ind++;
      count--;
      nextv--;
    }
    ind++;
  }
}


//This looks for a straight (or straight flush if "fs" is not NUM_SUITS)
// in "hand".  It calls the student's is_straight_at for each possible
// index to do the work of detecting the straight.
// If one is found, copy_straight is used to copy the cards into
// "ans".
int find_straight(deck_t * hand, suit_t fs, hand_eval_t * ans) {
  if (hand->n_cards < 5){
    return 0;
  }
  for(size_t i = 0; i <= hand->n_cards -5; i++) {
    int x = is_straight_at(hand, i, fs);
    if (x != 0){
      if (x < 0) { //ace low straight
	assert(hand->cards[i]->value == VALUE_ACE &&
	       (fs == NUM_SUITS || hand->cards[i]->suit == fs));
	ans->cards[4] = hand->cards[i];
	size_t cpind = i+1;
	while(hand->cards[cpind]->value != 5 ||
	      !(fs==NUM_SUITS || hand->cards[cpind]->suit ==fs)){
	  cpind++;
	  assert(cpind < hand->n_cards);
	}
	copy_straight(ans->cards, hand, cpind, fs,4) ;
      }
      else {
	copy_straight(ans->cards, hand, i, fs,5);
      }
      return 1;
    }
  }
  return 0;
}


//This function puts all the hand evaluation logic together.
//This function is longer than we generally like to make functions,
//and is thus not so great for readability :(
hand_eval_t evaluate_hand(deck_t * hand) {
  suit_t fs = flush_suit(hand);
  hand_eval_t ans;
  if (fs != NUM_SUITS) {
    if(find_straight(hand, fs, &ans)) {
      ans.ranking = STRAIGHT_FLUSH;
      return ans;
    }
  }
  unsigned * match_counts = get_match_counts(hand);
  unsigned n_of_a_kind = get_largest_element(match_counts, hand->n_cards);
  assert(n_of_a_kind <= 4);
  size_t match_idx = get_match_index(match_counts, hand->n_cards, n_of_a_kind);
  ssize_t other_pair_idx = find_secondary_pair(hand, match_counts, match_idx);
  free(match_counts);
  if (n_of_a_kind == 4) { //4 of a kind
    return build_hand_from_match(hand, 4, FOUR_OF_A_KIND, match_idx);
  }
  else if (n_of_a_kind == 3 && other_pair_idx >= 0) {     //full house
    ans = build_hand_from_match(hand, 3, FULL_HOUSE, match_idx);
    ans.cards[3] = hand->cards[other_pair_idx];
    ans.cards[4] = hand->cards[other_pair_idx+1];
    return ans;
  }
  else if(fs != NUM_SUITS) { //flush
    ans.ranking = FLUSH;
    size_t copy_idx = 0;
    for(size_t i = 0; i < hand->n_cards;i++) {
      if (hand->cards[i]->suit == fs){
	ans.cards[copy_idx] = hand->cards[i];
	copy_idx++;
	if (copy_idx >=5){
	  break;
	}
      }
    }
    return ans;
  }
  else if(find_straight(hand,NUM_SUITS, &ans)) {     //straight
    ans.ranking = STRAIGHT;
    return ans;
  }
  else if (n_of_a_kind == 3) { //3 of a kind
    return build_hand_from_match(hand, 3, THREE_OF_A_KIND, match_idx);
  }
  else if (other_pair_idx >=0) {     //two pair
    assert(n_of_a_kind ==2);
    ans = build_hand_from_match(hand, 2, TWO_PAIR, match_idx);
    ans.cards[2] = hand->cards[other_pair_idx];
    ans.cards[3] = hand->cards[other_pair_idx + 1];
    if (match_idx > 0) {
      ans.cards[4] = hand->cards[0];
    }
    else if (other_pair_idx > 2) {  //if match_idx is 0, first pair is in 01
      //if other_pair_idx > 2, then, e.g. A A K Q Q
      ans.cards[4] = hand->cards[2];
    }
    else {       //e.g., A A K K Q
      ans.cards[4] = hand->cards[4];
    }
    return ans;
  }
  else if (n_of_a_kind == 2) {
    return build_hand_from_match(hand, 2, PAIR, match_idx);
  }
  return build_hand_from_match(hand, 0, NOTHING, 0);
}
