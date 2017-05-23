#include "nixieDriver.h"

#define TUBE_CNT     6
#define DIGIT_CNT   10

#define SHIFT_BYTES  8

static uint8_t nixieBitmap[TUBE_CNT][DIGIT_CNT] =
{
    { 22, 23, 24, 25,  6,  7,  8,  9, 10, 21 },
    { 17, 18, 19, 20, 11, 12, 13, 14, 15, 16 },

    { 30, 31, 32,  1,  2,  3,  4,  5, 28, 29 },
    { 36, 35, 34, 33, 60, 61, 62, 63, 64, 37 },

    { 45, 46, 47, 48, 49, 50, 51, 52, 43, 44 },
    { 40, 41, 42, 53, 54, 55, 56, 57, 38, 39 }
};

static void setbit(void* addr, unsigned int cnt);

static void setbit(void* addr, unsigned int cnt)
{
    uint8_t* byte = ((uint8_t*)addr) + (cnt / 8);
    unsigned int bit = cnt % 8;

    *byte |= 1 << bit;

    return;
}

void nixieDriver_set(int* vals)
{
    int index;
    uint8_t bitmask[SHIFT_BYTES] = {0};
    int tensDigit;
    int onesDigit;

    for(index = 0; index < NUM_CNT; index++)
    {
        tensDigit = vals[index] / DIGIT_CNT;
        onesDigit = vals[index] % DIGIT_CNT;

        if(tensDigit >= DIGIT_CNT)
        {
            tensDigit = DIGIT_CNT-1;
        }

        setbit(bitmask, nixieBitmap[index*2][tensDigit] - 1);
        setbit(bitmask, nixieBitmap[index*2 + 1][onesDigit] - 1);
    }

    return;
}
