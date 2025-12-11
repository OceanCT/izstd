#include "hist.h"
#include "../common/o1debug.h"
/*
    TODO: parallelize it and add some simd function to it
*/
size_t HIST_count_o1ando0(unsigned* o0count, unsigned* o1count, unsigned* maxSymbolValuePtr, const void *src, size_t srcSize) {
    // o1printf("|HIST_count_o1ando0| maxSymbolValue=%u, srcSize=%zu \n", maxSymbolValue, srcSize);
    // set o0 count and o1 count to 0
    ZSTD_memset(o0count, 0, ((*maxSymbolValuePtr) + 1) * sizeof(unsigned));
    ZSTD_memset(o1count, 0, ((*maxSymbolValuePtr) + 1) * ((*maxSymbolValuePtr) + 1) * sizeof(unsigned));
    unsigned maxSymbolValue = *maxSymbolValuePtr;
    int i = 0;
    unsigned lastsymbol = 0;
    unsigned currentsymbol;
    const unsigned char* srcbytes = src;
    for(; i < srcSize; i++) {
        currentsymbol = srcbytes[i];
        o0count[currentsymbol]++;
        o1count[lastsymbol * (maxSymbolValue + 1) + currentsymbol]++;
        lastsymbol = currentsymbol;
    }
    // find the max occurred symbol 
    i = 1;
    lastsymbol = 0;
    for(; i <= maxSymbolValue; i++) {
        if(o0count[i] > o0count[lastsymbol]) {
            lastsymbol = i;
        }
    }
    for(; o0count[maxSymbolValue] == 0 && maxSymbolValue > 0; maxSymbolValue--);
    *maxSymbolValuePtr = maxSymbolValue;
    // o1printf o0 count 
    o1printf("------------------\n");
    for(unsigned i = 0; i <= maxSymbolValue; i++) {
        o1printf("o0count[%u]=%u ", i, o0count[i]);
    }
    o1printf("\n------------------\n");
    // o1printf o1 count 
    for(unsigned i = 0; i <= maxSymbolValue; i++) {
        o1printf("o1count for lastsymbol=%u: ", i);
        for(unsigned j = 0; j <= maxSymbolValue; j++) {
            o1printf("%u ", o1count[i * (maxSymbolValue + 1) + j]);
        }
        o1printf("\n");
    }
    return lastsymbol;
}
