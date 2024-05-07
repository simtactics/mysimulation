#include <d20.h>

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define MAX_UINT64_T 18446744073709551615ul;

/*---------------------------------------------------------------------------*/

/* This is xoshiro256** 1.0, one of our all-purpose, rock-solid
   generators. It has excellent (sub-ns) speed, a state (256 bits) that is
   large enough for any parallel application, and it passes all tests we
   are aware of.

   For generating just floating-point numbers, xoshiro256+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

static uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

uint64_t s[4];

uint64_t next(void) {
	const uint64_t result = rotl(s[1] * 5, 7) * 9;

	const uint64_t t = s[1] << 17;

	s[2] ^= s[0];
	s[3] ^= s[1];
	s[1] ^= s[2];
	s[0] ^= s[3];

	s[2] ^= t;

	s[3] = rotl(s[3], 45);

	return result;
}

/*---------------------------------------------------------------------------*/

int roll_from_char_array(const char *ca){
    int dice = 0;
    int sides = 0;
    int modifier = 0;

    int d_token = 0;
    int m_token = 0;

    char *d_tok = (char *)"d";
    char *D_tok = (char *)"D";
    char *mi_tok = (char *)"-";
    char *pl_tok = (char *)"+";

    while(*ca != '\0'){

        if(*ca == *d_tok || *ca == *D_tok){
            d_token = 1;
        } else if(*ca == *mi_tok) {
            m_token = -1;
        } else if (*ca == *pl_tok) {
            m_token = 1;
        } else {
            if(d_token == 0)
            {
                dice = dice * 10;
                dice += *ca - '0';
            } else if(m_token == 0){
                sides = sides * 10;
                sides += *ca - '0';
            } else {
                modifier = modifier * 10;
                modifier += *ca - '0';
            }
        }
        ++ca;
    }

    modifier = modifier * m_token;

    if(dice == 0){
        return ROLL_FROM_CHAR_ARRAY_PARSE_FAILURE_NO_NUMBER_DICE;
    } else if (sides == 0){
        return ROLL_FROM_CHAR_ARRAY_PARSE_FAILURE_NO_NUMBER_OF_SIDES;
    }

    return roll(dice, sides, modifier);
}

int roll(int dice, int sides, int modifier){
    uint64_t one = rand();
    uint64_t two = rand();
    uint64_t three = rand();
    uint64_t four = rand();

    s[0] = one;
    s[1] = two;
    s[2] = three;
    s[3] = four;

	unsigned mask;
	mask = (1 << 12) - 1;
    //uint8_t i=0,parts[8]={0};
	
	int a = 0;
	int result = 0;
	
	do {
		result += (int) floor(((float) (next() & mask) / (float) 4096) * (float) sides) + 1;
		++a;
	} while (a < dice);
	result += modifier;

    return result;
}