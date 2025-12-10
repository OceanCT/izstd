// #ifndef FSE_O1_H
// #define FSE_O1_H

#define FSE_O1_VERSION_MAJOR    0
#define FSE_O1_VERSION_MINOR    9
#define FSE_O1_VERSION_RELEASE  0

#define FSE_O1_LIB_VERSION FSE_O1_VERSION_MAJOR.FSE_O1_VERSION_MINOR.FSE_O1_VERSION_RELEASE

#define FSE_O1_VERSION_NUMBER  (FSE_O1_VERSION_MAJOR *100*100 + FSE_O1_VERSION_MINOR *100 + FSE_O1_VERSION_RELEASE)

unsigned FSE_O1_versionNumber(void);   /**< library version number; to be used when checking dll version */

typedef struct {
    
} FSEO1_TABLE;


// MEM_STATIC void FSEO1_initCState(FSEO1_CState_t* statePtr, const FSEO1_CTable* ct)
// {
//     const void* ptr = ct;
//     const U16* u16ptr = (const U16*) ptr;
//     const U32 tableLog = MEM_read16(ptr);
//     statePtr->value = (ptrdiff_t)1<<tableLog;
//     statePtr->stateTable = u16ptr+2;
//     statePtr->symbolTT = ct + 1 + (tableLog ? (1<<(tableLog-1)) : 1);
//     statePtr->stateLog = tableLog;
// }

// MEM_STATIC void FSEO1_initCState2(FSEO1_CState_t* statePtr, const FSEO1_CTable* ct, U32 symbol)
// {
//     FSE_initCState(statePtr, ct);
//     {   const FSE_symbolCompressionTransform symbolTT = ((const FSE_symbolCompressionTransform*)(statePtr->symbolTT))[symbol];
//         const U16* stateTable = (const U16*)(statePtr->stateTable);
//         U32 nbBitsOut  = (U32)((symbolTT.deltaNbBits + (1<<15)) >> 16);
//         statePtr->value = (nbBitsOut << 16) - symbolTT.deltaNbBits;
//         statePtr->value = stateTable[(statePtr->value >> nbBitsOut) + symbolTT.deltaFindState];
//     }
// }

// MEM_STATIC void FSEO1_encodeSymbol(BIT_CStream_t* bitC, FSEO1_CState_t* statePtr, unsigned symbol)
// {
//     FSE_symbolCompressionTransform const symbolTT = ((const FSE_symbolCompressionTransform*)(statePtr->symbolTT))[symbol];
//     const U16* const stateTable = (const U16*)(statePtr->stateTable);
//     U32 const nbBitsOut  = (U32)((statePtr->value + symbolTT.deltaNbBits) >> 16);
//     BIT_addBits(bitC, (BitContainerType)statePtr->value, nbBitsOut);
//     statePtr->value = stateTable[ (statePtr->value >> nbBitsOut) + symbolTT.deltaFindState];
// }

// MEM_STATIC void FSEO1_flushCState(BIT_CStream_t* bitC, const FSEO1_CState_t* statePtr)
// {
//     BIT_addBits(bitC, (BitContainerType)statePtr->value, statePtr->stateLog);
//     BIT_flushBits(bitC);
// }