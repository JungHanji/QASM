#pragma once

#include "../definitions.hpp"
#include "../../Hardware/commands.hpp"

#include "tools.hpp"

namespace qasm{
    int __executebyname(
        std::stack<__STACK_MAIN_TYPE> &stack,
        registers &regs,
        newheap &heap,

        std::map<std::string, int> blocksinfo, 
        std::string name, 
        argutype args,

        int &counter,
        int &local_counter,
        bool &changeblock,
        std::string &changeto
    ){
        if(name == "mov")     {return __mov    (stack, regs, heap, args);}
        if(name == "reg")     {return __reg    (stack, regs, heap, args);}
        if(name == "push")    {return __push   (stack, regs, heap, args);}
        if(name == "pop")     {return __pop    (stack, regs, heap, args);}
        if(name == "peek")    {return __peek   (stack, regs, heap, args);}
        if(name == "eql")     {return __eql    (stack, regs, heap, args);}
        if(name == "less")    {return __less   (stack, regs, heap, args);}
        if(name == "neql")    {return __neql   (stack, regs, heap, args);}
        if(name == "jmp")     {return __jmp    (stack, regs, heap, args, blocksinfo, counter, local_counter, changeblock, changeto);}
        if(name == "jep")     {return __jep    (stack, regs, heap, args, blocksinfo, counter, local_counter, changeblock, changeto);}
        if(name == "jnp")     {return __jnp    (stack, regs, heap, args, blocksinfo, counter, local_counter, changeblock, changeto);}
        if(name == "sum")     {return __sum    (stack, regs, heap, args);}
        if(name == "sub")     {return __sub    (stack, regs, heap, args);}
        if(name == "div")     {return __div    (stack, regs, heap, args);}
        if(name == "mlt")     {return __mlt    (stack, regs, heap, args);}
        if(name == "pwr")     {return __pwr    (stack, regs, heap, args);}
        if(name == "memalc")  {return __memalc (stack, regs, heap, args);}
        if(name == "memfr")   {return __memfr  (stack, regs, heap, args);}
        if(name == "memwr")   {return __memwr  (stack, regs, heap, args);}
        if(name == "gt")      {return __memgt  (stack, regs, heap, args);}
        if(name == "syscall") {return __syscall(stack, regs, heap, args);}
        if(name == "clean")   {return __clean  (stack, regs, heap, args);}
        if(name == "setch")   {return __setch  (stack, regs, heap, args);}
        if(name == "rsh")     {return __rshift (stack, regs, heap, args);}
        if(name == "lsh")     {return __lshift (stack, regs, heap, args);}
        if(name == "and")     {return __and    (stack, regs, heap, args);}
        if(name == "not")     {return __not    (stack, regs, heap, args);}
        if(name == "or")      {return __or     (stack, regs, heap, args);}
        if(name == "xor")     {return __xor    (stack, regs, heap, args);}
        if(name == "land")    {return __land   (stack, regs, heap, args);}
        if(name == "lor")     {return __lor    (stack, regs, heap, args);}
        if(name == "lnot")    {return __lnot   (stack, regs, heap, args);}
        if(name == "write")   {return __write  (stack, regs, heap, args);}
        if(name == "__debug_reg"){

            auto info = __getregisterinfo(__getregtypefromstr(args[0]));
            auto data = __getanyregister(info.type, regs);
            int size = 64;

            switch(info.grandtype){
                case __8BITS: {
                    size = 8;
                    break;
                }
                case __16BITS: {
                    size = 16;
                    break;
                }
                case __32BITS: {
                    size = 32;
                    break;
                }
                case __64BITS: {
                    size = 64;
                    break;
                }
            }

            std::cout << "\n__debug: register <" << args[0] << ">:"
                << "\n\tbin: 0b" << std::bitset<64>(data).to_string().substr(64 - size)
                << "\n\tdec: " << data
                << "\n\thex: 0x" << std::hex << data << std::dec
            << std::endl;
            return 0;
        }
        if(name == "__debug_heap"){
            auto data = connectBytes64b(extractValue(args[0], regs, heap));
            int howmanybits = 8;

            if(args.size() > 1){
                howmanybits = std::stoi(args[1]);
            }

            std::cout << "\n__debug: heap <" << args[0].substr(1) << ">:"
                << "\n\tbin: 0b" << std::bitset<64>(data).to_string().substr(64 - howmanybits)
                << "\n\tdec: " << data
                << "\n\thex: 0x" << std::hex << data << std::dec
            << std::endl;
            return 0;
        }
        if(name == "__debug_stack"){
            std::cout << "\n__debug: stack:" << std::endl;
            std::cout << "\t" << stack.top() << std::endl;
            return 0;
        }
        if(name == "__debug_stop"){
            std::cout << "\n__debug: stopping execution..." << std::endl;
            getch();
        }
        throw std::invalid_argument("Unexcepted command: " + name);
        return -299;
    }
}