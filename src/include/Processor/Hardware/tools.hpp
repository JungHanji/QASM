#pragma once

#include "definitions.hpp"

#include "Memory/registers.hpp"
#include "Memory/heap.hpp"
#include "Memory/stack.hpp"

#include <cstring>
#include <string>
#include <tuple>

namespace qasm{
    REG_TYPE __getregtypefromstr(std::string name){
        if (name == "rax")  return RAX;
        if (name == "rbx")  return RBX;
        if (name == "rcx")  return RCX;
        if (name == "rdx")  return RDX;
        if (name == "rsi")  return RSI;
        if (name == "rdi")  return RDI;
        if (name == "rbp")  return RBP;
        if (name == "rsp")  return RSP;
        if (name == "r8")   return R8;
        if (name == "r9")   return R9;
        if (name == "r10")  return R10;
        if (name == "r11")  return R11;
        if (name == "r12")  return R12;
        if (name == "r13")  return R13;
        if (name == "r14")  return R14;
        if (name == "r15")  return R15;
        if (name == "eax")  return EAX;
        if (name == "ebx")  return EBX;
        if (name == "ecx")  return ECX;
        if (name == "edx")  return EDX;
        if (name == "esi")  return ESI;
        if (name == "edi")  return EDI;
        if (name == "ebp")  return EBP;
        if (name == "esp")  return ESP;
        if (name == "r8d")  return R8D;
        if (name == "r9d")  return R9D;
        if (name == "r10d") return R10D;
        if (name == "r11d") return R11D;
        if (name == "r12d") return R12D;
        if (name == "r13d") return R13D;
        if (name == "r14d") return R14D;
        if (name == "r15d") return R15D;
        if (name == "ax")   return AX;
        if (name == "bx")   return BX;
        if (name == "cx")   return CX;
        if (name == "dx")   return DX;
        if (name == "si")   return SI;
        if (name == "di")   return DI;
        if (name == "bp")   return BP;
        if (name == "sp")   return SP;
        if (name == "r8w")  return R8W;
        if (name == "r9w")  return R9W;
        if (name == "r10w") return R10W;
        if (name == "r11w") return R11W;
        if (name == "r12w") return R12W;
        if (name == "r13w") return R13W;
        if (name == "r14w") return R14W;
        if (name == "r15w") return R15W;
        if (name == "al")   return AL;
        if (name == "ah")   return AH;
        if (name == "bl")   return BL;
        if (name == "bh")   return BH;
        if (name == "cl")   return CL;
        if (name == "ch")   return CH;
        if (name == "dl")   return DL;
        if (name == "dh")   return DH;
        if (name == "r8b")  return R8B;
        if (name == "r9b")  return R9B;
        if (name == "r10b") return R10B;
        if (name == "r11b") return R11B;
        if (name == "r12b") return R12B;
        if (name == "r13b") return R13B;
        if (name == "r14b") return R14B;
        if (name == "r15b") return R15B;
        return NOTAREG;
    }

    REG_TYPE __getbase64reg(REG_TYPE type) {
        switch(type) {
            case RAX: case EAX: case AX: case AH: case AL: return RAX;
            case RBX: case EBX: case BX: case BH: case BL: return RBX;
            case RCX: case ECX: case CX: case CH: case CL: return RCX;
            case RDX: case EDX: case DX: case DH: case DL: return RDX;
            case RSI: case ESI: case SI: return RSI;
            case RDI: case EDI: case DI: return RDI;
            case RBP: case EBP: case BP: return RBP;
            case RSP: case ESP: case SP: return RSP;
            case R8:  case R8D:  case R8W:  case R8B:  return R8;
            case R9:  case R9D:  case R9W:  case R9B:  return R9;
            case R10: case R10D: case R10W: case R10B: return R10;
            case R11: case R11D: case R11W: case R11B: return R11;
            case R12: case R12D: case R12W: case R12B: return R12;
            case R13: case R13D: case R13W: case R13B: return R13;
            case R14: case R14D: case R14W: case R14B: return R14;
            case R15: case R15D: case R15W: case R15B: return R15;
            default: return NOTAREG;
        }
    }

    struct registerFullInfo{
        REG_TYPE      type;
        REG_GRANDTYPE grandtype;
        BYTE_TYPE     bytetype;
    };

    registerFullInfo __getregisterinfo(REG_TYPE type){
        registerFullInfo info;
        info.type      = type;
        info.grandtype = __grandtype(type);
        info.bytetype = __bytetype(type);
        return info;
    }
    
    int64_t __getanyregister(
        REG_TYPE type,
        registers &reg
    ){
        auto reginfo = __getregisterinfo(type);
        REG_TYPE baseType = __getbase64reg(type);
        int64_t fullValue = reg.__bytype(baseType);
        switch(reginfo.grandtype){
            case __64BITS:
                return fullValue;
            case __32BITS:
                return reg.__32lbit(fullValue);
            case __16BITS:
                return reg.__16lbit(reg.__32lbit(fullValue));
            case __8BITS:
                if (reginfo.bytetype == __HIGHEST)
                    return reg.__8hbit(reg.__16lbit(reg.__32lbit(fullValue)));
                else
                    return reg.__8lbit(reg.__16lbit(reg.__32lbit(fullValue)));
        }
        return 0; // Возврат по умолчанию
    }

    int64_t connectBytes64b(std::vector<std::byte> bytearray){
        int64_t result = 0;
        for(int i = 0; i < 8; ++i){
            result |= static_cast<int64_t>(bytearray[i]) << (i * 8);
        }

        // std::cout << "connect64b: " << result << std::endl;
        return result;
    }

    int32_t connectBytes32b(std::vector<std::byte> bytearray){
        int32_t result = 0;
        for(int i = 0; i < 4; ++i){
            result |= static_cast<int32_t>(bytearray[i]) << (i * 8);
        }

        // std::cout << "connect32b: " << result << std::endl;
        return result;
    }

    int16_t connectBytes16b(std::vector<std::byte> bytearray){
        int16_t result = 0;
        for(int i = 0; i < 2; ++i){
            result |= static_cast<int16_t>(bytearray[i]) << (i * 8);
        }

        // std::cout << "connect16b: " << result << std::endl;
        return result;
    }

    int8_t connectBytes8b(std::vector<std::byte> bytearray){
        // std::cout << "connect8b: " << static_cast<int8_t>(bytearray[0]) << std::endl;  // Выводим первый байт в виде числа
        return static_cast<int8_t>(bytearray[0]);
    }

    std::vector<std::byte> extractValue(
        std::string val,
        registers &reg,
        newheap &heap
    ){
        if(val[0] == '['){
            int64_t intValue = std::stoll(val.substr(1, val.size() - 2), nullptr, 0);
            std::vector<std::byte> byteArray(sizeof(int64_t));
            std::memcpy(byteArray.data(), &intValue, sizeof(int64_t));
            return byteArray;
        } else if(val[0] == '\'') {
            std::vector<std::byte> byteArray(1);
            byteArray[0] = static_cast<std::byte>(val.substr(1, val.size() - 2)[0]);
            return byteArray;
        } else if(val[0] == '"') {
            std::vector<std::byte> byteArray;
            for(std::size_t i = 1; i < val.size() - 1; ++i){
                byteArray.push_back(static_cast<std::byte>(val[i]));
            }
            return byteArray;
        } else if(val[0] == '$'){

            int64_t memdata;
            int64_t written;

            int code = heap.__get(
                connectBytes64b(extractValue(val.substr(1), reg, heap)), 
                &memdata,
                &written
            );

            std::vector<std::byte> byteArray(sizeof(int64_t));
            std::memcpy(byteArray.data(), &memdata, sizeof(int64_t));
            return byteArray;
            
        } else {
            auto intvalue = __getanyregister(__getregtypefromstr(val), reg);

            std::vector<std::byte> byteArray(sizeof(int64_t));
            std::memcpy(byteArray.data(), &intvalue, sizeof(int64_t));

            return byteArray;
        }
    }
}