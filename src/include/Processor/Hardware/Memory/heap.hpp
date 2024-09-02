#pragma once
#include "../definitions.hpp"

namespace qasm{

    struct memcell{
        std::byte data;
        bool isFree = true;
    };

    struct newheap{
        memcell __memory[__HEAP_MAX_SIZE]{};

        int __alloc(
            int64_t size_to_alloc,
            int64_t *allc_addr
        ){
            // std::cout << "newheap: alloc: " << size_to_alloc << std::endl;
            if(size_to_alloc < 0 || (size_to_alloc >= __HEAP_MAX_SIZE)){
                return -1;
            }

            int64_t cells_to_alloc = size_to_alloc;

            // std::cout << "newheap alloc: " << cells_to_alloc << " from " << size_to_alloc << std::endl;

            int start_address = 0;

            for(int64_t i = 0; i < __HEAP_MAX_SIZE; i++){
                memcell &cell = __memory[i];
                if(cell.isFree){
                    start_address = i;
                    *allc_addr = i;
                    break;
                }
            }

            for(int64_t i = start_address; i < cells_to_alloc; i++){
                __memory[i].isFree = false;
            }

            return 0;
        }

        int __get(
            int64_t           memaddr,
            int64_t        *memdata,
            int64_t          *written_size
        ){
            // std::cout << "newheap: get: " << memaddr << std::endl;
            if(memaddr < 0 || memaddr >= __HEAP_MAX_SIZE){
                return -1;
            }

            // std::cout << "newheap get: " << __memory[memaddr].data << std::endl;
            *memdata = (int64_t)__memory[memaddr].data;

            return 0;
        }

        int __free(
            int64_t memaddr,
            int64_t size_of_allcmem
        ){
            // std::cout << "newheap: free: " << memaddr << ' ' << size_of_allcmem << std::endl;
            if(memaddr < 0 || memaddr >= __HEAP_MAX_SIZE){
                return -1;
            }

            int64_t cells_to_alloc = size_of_allcmem;// / (__HEAP_MAX_SIZE);
            // cells_to_alloc += (cells_to_alloc == 0) ? 1: 0;

            for(int64_t i = memaddr; i < memaddr + cells_to_alloc; i++){
                __memory[i].isFree = true;
            }

            return 0;
        }

        int __write(
            int64_t memaddr,
            int64_t data
        ){
            // std::cout << "newheap: write: " << memaddr << ' ' << data << std::endl;
            if(memaddr < 0 || memaddr >= __HEAP_MAX_SIZE){
                return -1;
            }

            if(__memory[memaddr].isFree){
                return -2;
            }

            // std::cout << "newheap write: " << memaddr << " with " << data << std::endl;
            __memory[memaddr].data = (std::byte)data;

            return 0;
        }
    };
}