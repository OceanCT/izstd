/* ******************************************************************
 * FSEo1: adding order-1 tans support to FSE
 
****************************************************************** */
/* **************************************************************
*  Includes
****************************************************************/
#include "../common/compiler.h"
#include "../common/mem.h"        /* U32, U16, etc. */
#include "../common/debug.h"      /* assert, DEBUGLOG */
#include "hist.h"       /* HIST_count_wksp */
#include "../common/bitstream.h"
#define FSE_STATIC_LINKING_ONLY
#include "../common/fseo1.h"
#include "../common/error_private.h"
#define ZSTD_DEPS_NEED_MALLOC
#define ZSTD_DEPS_NEED_MATH64
#include "../common/zstd_deps.h"  /* ZSTD_memset */
#include "../common/bits.h" /* ZSTD_highbit32 */
/* **************************************************************
*  Templates
****************************************************************/
/*
  designed to be included
  for type-specific functions (template emulation in C)
  Objective is to write these functions only once, for improved maintenance
*/

/* safety checks */
// #ifndef FSE_FUNCTION_EXTENSION
// #  error "FSE_FUNCTION_EXTENSION must be defined"
// #endif
// #ifndef FSE_FUNCTION_TYPE
// #  error "FSE_FUNCTION_TYPE must be defined"
// #endif

// static size_t FSE_countO1Total(unsigned symbol, unsigned maxSymbolValue, const U32* counto1)
// {
//     size_t total = 0;
//     const U32* startPtr = counto1 + symbol * (maxSymbolValue + 1);
//     unsigned next;
//     for (next = 0; next <= maxSymbolValue; next++) {
//         total += startPtr[next];
//     }
//     return total;
// }

// size_t FSE_normalizeCountO1 (short* normalo0, short* normalo1, short* useo1_flag,
//                       unsigned maxSymbolValue, unsigned tableLog,
//                       const U32* counto0, const U32* counto1,
//                       size_t total, int useLowProbCount)
// {
//     o1printf("|FSE_normalizeCountO1| maxSymbolValue=%u, tableLog=%u, total=%zu \n", maxSymbolValue, tableLog, total);
//     /* Sanity checks */
//     if (tableLog==0) tableLog = FSE_DEFAULT_TABLELOG;
//     if (tableLog < FSE_MIN_TABLELOG) return ERROR(GENERIC);   /* Unsupported size */
//     if (tableLog > FSE_MAX_TABLELOG) return ERROR(tableLog_tooLarge);   /* Unsupported size */
//     if (tableLog < FSE_minTableLog(total, maxSymbolValue)) return ERROR(GENERIC);   /* Too small tableLog, compression potentially impossible */

//     {   static U32 const rtbTable[] = {     0, 473195, 504333, 520860, 550000, 700000, 750000, 830000 };
//         short const lowProbCount = useLowProbCount ? -1 : 1;
//         U64 const scale = 62 - tableLog;
//         U64 const step = ZSTD_div64((U64)1<<62, (U32)total);   /* <== here, one division ! */
//         U64 const vStep = 1ULL<<(scale-20);
//         int stillToDistribute = 1<<tableLog;
//         unsigned s;
//         unsigned largest=0;
//         short largestP=0;
//         U32 lowThreshold = (U32)(total >> tableLog);

//         for (s=0; s<=maxSymbolValue; s++) {
//             if (count[s] == total) return 0;   /* rle special case */
//             if (count[s] == 0) { normalizedCounter[s]=0; continue; }
//             if (count[s] <= lowThreshold) {
//                 normalizedCounter[s] = lowProbCount;
//                 stillToDistribute--;
//             } else {
//                 short proba = (short)((count[s]*step) >> scale);
//                 if (proba<8) {
//                     U64 restToBeat = vStep * rtbTable[proba];
//                     proba += (count[s]*step) - ((U64)proba<<scale) > restToBeat;
//                 }
//                 if (proba > largestP) { largestP=proba; largest=s; }
//                 normalizedCounter[s] = proba;
//                 stillToDistribute -= proba;
//         }   }
//         normalizedCounter[largest] += (short)stillToDistribute;
//     }
//     return tableLog;
// }

// size_t FSE_buildCTableO1()