#pragma once
#include "../definitions.hpp"
#include <bitset>

namespace qasm{
    struct registers {
        int64_t nanreg = -__INT_MAX__;
        int64_t rax = 0; // Arithmetics register
        int64_t rbx = 0; // Adressess register
        int64_t rcx = 0; // Shifts and counter
        int64_t rdx = 0; // Shifts and counter
        int64_t rsi = 0; // I/O and mlt/div
        int64_t rdi = 0; // Source index for arrays and strings
        int64_t rbp = 0; // Basic stack pointer for local variables
        int64_t rsp = 0; // Pointer to the top of the stack 
        int64_t r8  = 0; // General purposes
        int64_t r9  = 0; // General purposes
        int64_t r10 = 0; // General purposes
        int64_t r11 = 0; // General purposes
        int64_t r12 = 0; // General purposes
        int64_t r13 = 0; // General purposes
        int64_t r14 = 0; // General purposes
        int64_t r15 = 0; // General purposes

        int64_t &__bytype(REG_TYPE type){
            switch(type){
                case RAX: return rax;
                case RBX: return rbx;
                case RCX: return rcx;
                case RDX: return rdx;
                case RSI: return rsi;
                case RDI: return rdi;
                case RBP: return rbp;
                case RSP: return rsp;
                case R8:  return r8;
                case R9:  return r9;
                case R10: return r10;
                case R11: return r11;
                case R12: return r12;
                case R13: return r13;
                case R14: return r14;
                case R15: return r15;
            }
            return nanreg;
        }

        // Lowest 32 bits
        int32_t __32lbit(int64_t reg){
            return reg & 4294967295;
        }

        // Lowest 16 bits
        int16_t __16lbit(int32_t reg){
            return reg & 65535;
        }

        // Lowest 8 bits from 16lbits
        int8_t __8lbit(int16_t reg){
            return reg & 255;
        }

        // Highest 8 bits from 16lbits
        int8_t __8hbit(int16_t reg){
            return (reg & 65280) >> 8;
        }

        int8_t __8bit(BYTE_TYPE btype, int16_t reg){
            if(btype == __HIGHEST) 
                return __8hbit(reg);
            return __8lbit(reg);
        }

        // Reset lowest 32 bits from register
        void __rs32lbit(REG_TYPE type, int32_t reg, int64_t mask = -1){
            if(mask == -1)
                mask = 4294967295;
            __bytype(type) &= ~mask;
            __bytype(type) |= (mask & reg);
        }

        // Reset lowest 16 bits from 32lbits
        void __rs16lbit(REG_TYPE type, int16_t reg, int64_t mask = -1){
            if(mask == -1)
                mask = 65535;
            __bytype(type) &= ~mask;
            __bytype(type) |= (mask & reg);
        }

        // Reset lowest 8 bits from 16lbits
        void __rs8lbit (REG_TYPE type, int8_t  reg, int64_t mask = -1){
            if(mask == -1)
                mask = 255;
            __bytype(type) &= ~mask;
            __bytype(type) |= (mask & reg);
        }

        // Reset highest 8 bits from 16lbits
        void __rs8hbit (REG_TYPE type, int8_t  reg, int64_t mask = -1){
            if(mask == -1)
                mask = 255 << 8;
            else
                mask <<= 8;
            __bytype(type) &= ~mask;
            __bytype(type) |= (mask & reg);
        }

        void __rsbits (REG_TYPE type, REG_GRANDTYPE gtype, int64_t reg, BYTE_TYPE btype = __LOWEST){
            switch(gtype){
                case __64BITS: {
                    __bytype(type) = reg;
                    break;
                }
                case __32BITS: {
                    __rs32lbit(type, (int32_t)reg);
                    break;
                }
                case __16BITS: {
                    __rs16lbit(type, (int16_t)reg);
                    break;
                }
                case __8BITS: {
                    __rs8bit(type, btype, (int8_t)reg);
                    break;
                }
            }
        }

        void __rs8bit (REG_TYPE type, BYTE_TYPE btype, int8_t reg){
            if(btype == __HIGHEST)
                __rs8hbit(type, reg);
            else
                __rs8lbit(type, reg);
        }
    };
}