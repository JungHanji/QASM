#pragma once

#include <stack>
#include <cmath>
#include <cctype>

namespace qasm{
    #ifndef __STACK_MAIN_TYPE
        #define __STACK_MAIN_TYPE int64_t
    #endif

    #ifndef __HEAP_MAIN_TYPE
        #define __HEAP_MAIN_TYPE int64_t
    #endif

    #ifndef __HEAP_MAX_SIZE
        #define __HEAP_MAX_SIZE 128
    #endif

    #ifndef __HEAP_BLOCK_SIZE
        #define __HEAP_BLOCK_SIZE 64
    #endif

    using argutype = std::vector<std::string>;

    enum REG_TYPE{
        RAX,
        RBX,
        RCX,
        RDX,
        RSI,
        RDI,
        RBP,
        RSP,
        R8,
        R9,
        R10,
        R11,
        R12,
        R13,
        R14,
        R15,
        EAX, 
        EBX, 
        ECX, 
        EDX, 
        ESI, 
        EDI, 
        EBP, 
        ESP, 
        R8D, 
        R9D, 
        R10D, 
        R11D, 
        R12D, 
        R13D, 
        R14D, 
        R15D,
        AX, 
        BX, 
        CX, 
        DX, 
        SI, 
        DI, 
        BP, 
        SP, 
        R8W, 
        R9W, 
        R10W, 
        R11W, 
        R12W, 
        R13W, 
        R14W, 
        R15W, 
        AL, 
        AH, 
        BL, 
        BH, 
        CL, 
        CH, 
        DL, 
        DH, 
        R8B, 
        R9B, 
        R10B, 
        R11B, 
        R12B, 
        R13B, 
        R14B, 
        R15B,
        NOTAREG
    };

    enum REG_GRANDTYPE{
        __64BITS,
        __32BITS,
        __16BITS,
        __8BITS,
        __NTOBITS
    };

    enum BYTE_TYPE{
        __LOWEST,
        __HIGHEST
    };

    bool __isit64bitreg(REG_TYPE type){
        return type >= RAX && type <= R15; 
    }

    bool __isit32bitreg(REG_TYPE type){
        return type >= EAX && type <= R15D;
    }

    bool __isit16bitreg(REG_TYPE type){
        return type >= AX && type <= R15W;
    }

    bool __isit8bitreg(REG_TYPE type){
        return type >= AL && type <= R15B;
    }

    REG_GRANDTYPE __grandtype(REG_TYPE type){
        if(__isit64bitreg(type)) return __64BITS;
        if(__isit32bitreg(type)) return __32BITS;
        if(__isit16bitreg(type)) return __16BITS;
        if(__isit8bitreg(type))  return __8BITS;
        return __NTOBITS;
    }

    BYTE_TYPE __bytetype(REG_TYPE type){
        bool ishighest = type == AH ||
                         type == BH ||
                         type == CH ||
                         type == DH;

        return (ishighest ?
            __HIGHEST :
            __LOWEST
        );
    }
}
