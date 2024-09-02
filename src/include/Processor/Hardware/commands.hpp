#pragma once

#include "definitions.hpp"

#include "Memory/registers.hpp"
#include "Memory/heap.hpp"
#include "Memory/stack.hpp"

#include "tools.hpp"
#include <bitset>

namespace qasm{
    

    int __mov(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,
        
        argutype args
    ){
        REG_TYPE typea = __getregtypefromstr(args[0]), typeb = __getregtypefromstr(args[1]);
        if(__grandtype(typea) != __grandtype(typeb))
            return 1;

        switch(__grandtype(typea)){
            case __64BITS: {
                int64_t tmp = regs.__bytype(typea);
                regs.__bytype(typea) = regs.__bytype(typeb);
                regs.__bytype(typeb) = tmp;

                return 0;
            }
            case __32BITS: {
                int32_t tmp = regs.__32lbit(
                    regs.__bytype(typea)
                );

                regs.__rs32lbit(typea, regs.__32lbit(
                    regs.__bytype(typeb)
                ));
                regs.__rs32lbit(typeb, tmp);
                
                return 0;
            }
            case __16BITS: {
                int16_t tmp = regs.__16lbit(
                    regs.__32lbit(
                        regs.__bytype(typea)
                    )
                );
                regs.__rs16lbit(typea, 
                    regs.__16lbit(
                        regs.__32lbit(
                            regs.__bytype(typeb)
                        )
                    )
                );
                regs.__rs16lbit(typeb, tmp);

                return 0;
            }
            case __8BITS: {
                auto btypea = __bytetype(typea);
                auto btypeb = __bytetype(typeb);

                int8_t tmp = regs.__8bit(
                    btypea,
                    regs.__16lbit(
                        regs.__32lbit(
                            regs.__bytype(typea)
                        )
                    )
                );

                regs.__rs8bit(typea,
                    btypea,
                    regs.__8bit(
                        btypeb,
                        regs.__16lbit(
                            regs.__32lbit(
                                regs.__bytype(typeb)
                            )
                        )
                    )
                );

                regs.__rs8bit(typeb,
                    btypeb,
                    regs.__8bit(
                        btypea,
                        regs.__16lbit(
                            regs.__32lbit(
                                regs.__bytype(typea)
                            )
                        )
                    )
                );

                return 0;
            }
        }

        return -1;
    }

    int __reg(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,
        
        argutype args

    ){
        auto finfo = __getregisterinfo(__getregtypefromstr(args[0]));
        REG_TYPE rtype = finfo.type;
        REG_GRANDTYPE gtype = finfo.grandtype;
        BYTE_TYPE btype = finfo.bytetype;

        int64_t value = connectBytes64b(extractValue(args[1], regs, heap));

        // std::cout << "registration: " << rtype << ' ' << gtype << ' ' << value << ' ' << btype << std::endl;

        regs.__rsbits( rtype, gtype, value, btype );
        return 0;
    }

    int __push(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        stack.push(connectBytes64b(extractValue(args[0], regs, heap)));
        regs.__rs8lbit(RSP, stack.top());
        return 0;
    }

    int __pop(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        bool isempty = stack.size() == 0;
        stack.pop();
        regs.__rs8lbit(RSP, stack.top());
        return !isempty ? 0: -1;
    }

    int __peek(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        bool isempty = stack.size() == 0;
        if(!isempty){
            regs.__rs8lbit(R8, stack.top());
        }

        return !isempty ? 0: -1;
    }

    int __eql(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        if(extractValue(args[0], regs, heap) == extractValue(args[1], regs, heap)){
            regs.__rs8lbit(RDX, 1, 1);
            return 0;
        } else {
            regs.__rs8lbit(RDX, 0, 1);
            return 1;
        }
        return -1;
    }

    int __less(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        if(extractValue(args[0], regs, heap) < extractValue(args[1], regs, heap)){
            regs.__rs8lbit(RDX, 1, 1);
            return 0;
        } else {
            regs.__rs8lbit(RDX, 0, 1);
            return 1;
        }
        return -1;
    }

    int __more(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        if(extractValue(args[0], regs, heap) > extractValue(args[1], regs, heap)){
            regs.__rs8lbit(RDX, 1, 1);
            return 0;
        } else {
            regs.__rs8lbit(RDX, 0, 1);
            return 1;
        }
        return -1;
    }

    int __neql(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        if(extractValue(args[0], regs, heap) != extractValue(args[1], regs, heap)){
            regs.__rs8lbit(RDX, 1, 1);
            return 0;
        } else {
            regs.__rs8lbit(RDX, 0, 1);
            return 1;
        }
        return -1;
    }

    int __jmp(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args,
        std::map<std::string, int> blocksinfo,
        int &count,
        int &local_count,
        bool &change_index,
        std::string &change_to
    ){
        regs.rbp = blocksinfo[args[0]];

        change_index = true;
        count = blocksinfo[args[0]];
        change_to = args[0];
        local_count = 0;

        return 0;
    }

    int __jep(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args,
        std::map<std::string, int> blocksinfo,
        int &count,
        int &local_count,
        bool &change_index,
        std::string &change_to
    ){
        // std::cout << "JEP: start\n";
        if(!__peek(stack, regs, heap, args)){
            // std::cout << "JEP: peek\n";
            if(regs.__8lbit(regs.r8) == 1){
                // std::cout << "JEP: jmp\n";
                __jmp(stack, regs, heap, args, blocksinfo, count, local_count, change_index, change_to);
                return 0;
            } else {
                // std::cout << "JEP: NOT: jmp\n";
                return 1;
            }
        }

        // std::cout << "JEP: EXIT\n";
        return -1;
    }

    int __jnp(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args,
        std::map<std::string, int> blocksinfo,
        int &count,
        int &local_count,
        bool &change_index,
        std::string &change_to
    ){
        if(!__peek(stack, regs, heap, args)){
            if(regs.__8lbit(regs.r8) == 0){
                __jmp(stack, regs, heap, args, blocksinfo, count, local_count, change_index, change_to);
                return 0;
            } else {
                return 1;
            }
        }
        return -1;
    }

    int __sum(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        regs.rax = connectBytes64b(extractValue(args[0], regs, heap)) + connectBytes64b(extractValue(args[1], regs, heap));
        return 0;
    }

    int __sub(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        regs.rax = connectBytes64b(extractValue(args[0], regs, heap)) - connectBytes64b(extractValue(args[1], regs, heap));
        return 0;
    }

    int __div(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        regs.rax = connectBytes64b(extractValue(args[0], regs, heap)) / connectBytes64b(extractValue(args[1], regs, heap));
        return 0;
    }

    int __mlt(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        regs.rax = connectBytes64b(extractValue(args[0], regs, heap)) * connectBytes64b(extractValue(args[1], regs, heap));
        return 0;
    }

    int __pwr(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        regs.rax = std::pow(connectBytes64b(extractValue(args[0], regs, heap)), connectBytes64b(extractValue(args[1], regs, heap)));
        return 0;
    }

    int __memalc(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        
        return heap.__alloc(
            connectBytes64b(extractValue(args[0], regs, heap)), // size
            &regs.rdi  // addr
        );
    }

    int __memfr(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        return heap.__free(
            connectBytes64b(extractValue(args[0], regs, heap)), // addr
            connectBytes64b(extractValue(args[1], regs, heap))  // size
        );
    }

    int __memgt(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        return heap.__get(
            connectBytes64b(extractValue(args[0], regs, heap)),
            &regs.rdi,
            &regs.rdx
        );
    }

    int __memwr(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){

        auto addr = connectBytes64b(extractValue(args[0], regs, heap));
        auto content = extractValue(args[1], regs, heap);

        for(int i = 0; i < content.size(); i++){
            heap.__write(
                addr + i,
                (int64_t)(content[i])
            );
        }

        stack.push(addr+content.size());
        regs.__rs8lbit(RSP, stack.top());

        return 0;
    }

    int __rshift(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){

        regs.rax = connectBytes64b(extractValue(args[0], regs, heap)) >> connectBytes64b(extractValue(args[1], regs, heap));

        return 0;
    }
    
    int __lshift(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){

        regs.rax = connectBytes64b(extractValue(args[0], regs, heap)) << connectBytes64b(extractValue(args[1], regs, heap));

        return 0;
    }
    
    int __and(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){

        regs.rax = connectBytes64b(extractValue(args[0], regs, heap)) & connectBytes64b(extractValue(args[1], regs, heap));

        return 0;
    }
    
    int __not(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){

        regs.rax = ~connectBytes64b(extractValue(args[0], regs, heap));

        return 0;
    }
    
    int __or(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){

        regs.rax = connectBytes64b(extractValue(args[0], regs, heap)) | connectBytes64b(extractValue(args[1], regs, heap));

        return 0;
    }
    
    int __xor(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){

        regs.rax = connectBytes64b(extractValue(args[0], regs, heap)) ^ connectBytes64b(extractValue(args[1], regs, heap));

        return 0;
    }
    
    int __land(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        
        regs.rax = connectBytes64b(extractValue(args[0], regs, heap)) && connectBytes64b(extractValue(args[1], regs, heap));

        return 0;
    }
    
    int __lor(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        regs.rax = connectBytes64b(extractValue(args[0], regs, heap)) || connectBytes64b(extractValue(args[1], regs, heap));

        return 0;
    }
    
    int __lnot(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){

        regs.rax = !connectBytes64b(extractValue(args[0], regs, heap));

        return 0;
    }

    int __syscall(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        return system(args[0].c_str());
    }

    int __clean(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        regs.rsi = 0;
        regs.rcx = 0;

        return 0;
    }

    int __setch(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        //regs.rsi |= (int8_t)extractValue(args[0], regs, heap) << (regs.rcx*8);
        regs.rsi = (regs.rsi << 8) + connectBytes8b(extractValue(args[0], regs, heap));
        if (regs.rcx < 8) regs.rcx += 1;

        return 0;
    }

    int __write(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        argutype args
    ){
        // std::cout << "write debug\n";
        if(regs.rsi == 0)
            return 1;

        std::string out;
        int64_t mask = 255;
        for(int64_t i = regs.rcx - 1; i >= 0; i--){
            // std::cout << i << ' ' << ((regs.rsi >> (i*8)) & mask) << std::endl;
            out += (char)((regs.rsi >> (i*8)) & mask);
        }
        std::cout << out;

        return 0;
    }
}
