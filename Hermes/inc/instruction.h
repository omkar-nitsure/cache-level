#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <string>
#include <sstream>
#include <iomanip>
#include "defs.h"
#include "set.h"
#include <cstdint>
class input_instr {
  public:

    // instruction pointer or PC (Program Counter)
    uint64_t ip;

    // branch info
    uint8_t is_branch;
    uint8_t branch_taken;

    uint8_t destination_registers[NUM_INSTR_DESTINATIONS]; // output registers
    uint8_t source_registers[NUM_INSTR_SOURCES]; // input registers

    uint64_t destination_memory[NUM_INSTR_DESTINATIONS]; // output memory
    uint64_t source_memory[NUM_INSTR_SOURCES]; // input memory

    input_instr() {
        ip = 0;
        is_branch = 0;
        branch_taken = 0;

        for (uint32_t i=0; i<NUM_INSTR_SOURCES; i++) {
            source_registers[i] = 0;
            source_memory[i] = 0;
        }

        for (uint32_t i=0; i<NUM_INSTR_DESTINATIONS; i++) {
            destination_registers[i] = 0;
            destination_memory[i] = 0;
        }
    };
};

class cloudsuite_instr {
  public:

    // instruction pointer or PC (Program Counter)
    uint64_t ip;

    // branch info
    uint8_t is_branch;
    uint8_t branch_taken;

    uint8_t destination_registers[NUM_INSTR_DESTINATIONS_SPARC]; // output registers
    uint8_t source_registers[NUM_INSTR_SOURCES]; // input registers

    uint64_t destination_memory[NUM_INSTR_DESTINATIONS_SPARC]; // output memory
    uint64_t source_memory[NUM_INSTR_SOURCES]; // input memory

    uint8_t asid[2];

    cloudsuite_instr() {
        ip = 0;
        is_branch = 0;
        branch_taken = 0;

        for (uint32_t i=0; i<NUM_INSTR_SOURCES; i++) {
            source_registers[i] = 0;
            source_memory[i] = 0;
        }

        for (uint32_t i=0; i<NUM_INSTR_DESTINATIONS_SPARC; i++) {
            destination_registers[i] = 0;
            destination_memory[i] = 0;
        }

        asid[0] = UINT8_MAX;
        asid[1] = UINT8_MAX;
    };
};

class ooo_model_instr {
  public:
    uint64_t instr_id,
             ip,
             fetch_producer,
             producer_id,
             translated_cycle,
             fetched_cycle,
             execute_begin_cycle,
             retired_cycle,
             event_cycle,
             rob_head_cycle; // when the instruction became ROB head

    uint8_t is_branch,
            is_memory,
            is_load,
            is_store,
            branch_taken,
            branch_mispredicted,
            branch_prediction_made,
            translated,
            data_translated,
            source_added[NUM_INSTR_SOURCES],
            destination_added[NUM_INSTR_DESTINATIONS_SPARC],
            is_producer,
            is_consumer,
            reg_RAW_producer,
            reg_ready,
            mem_ready,
            asid[2],
            reg_RAW_checked[NUM_INSTR_SOURCES];

    uint8_t branch_type;
    uint64_t branch_target;

    uint32_t fetched, scheduled;
    int num_reg_ops, num_mem_ops, num_mem_src, num_mem_dest, num_reg_dependent;

    // executed bit is set after all dependencies are eliminated and this instr is chosen on a cycle, according to EXEC_WIDTH
    int executed;

    uint8_t destination_registers[NUM_INSTR_DESTINATIONS_SPARC]; // output registers

    uint8_t source_registers[NUM_INSTR_SOURCES]; // input registers 

    // these are instruction ids of other instructions in the window
    //int64_t registers_instrs_i_depend_on[NUM_INSTR_SOURCES];
    // these are indices of instructions in the window that depend on me
    //uint8_t registers_instrs_depend_on_me[ROB_SIZE], registers_index_depend_on_me[ROB_SIZE][NUM_INSTR_SOURCES];
    fastset registers_instrs_depend_on_me, registers_index_depend_on_me[NUM_INSTR_SOURCES];


    // memory addresses that may cause dependencies between instructions
    uint64_t instruction_pa, data_pa, virtual_address, physical_address;
    uint64_t destination_memory[NUM_INSTR_DESTINATIONS_SPARC]; // output memory
    uint64_t source_memory[NUM_INSTR_SOURCES]; // input memory
    //int source_memory_outstanding[NUM_INSTR_SOURCES];  // a value of 2 here means the load hasn't been issued yet, 1 means it has been issued, but not returned yet, and 0 means it has returned

    // keep around a record of what the original virtual addresses were
    uint64_t destination_virtual_address[NUM_INSTR_DESTINATIONS_SPARC];
    uint64_t source_virtual_address[NUM_INSTR_SOURCES];

    // these are instruction ids of other instructions in the window
    //uint32_t memory_instrs_i_depend_on[NUM_INSTR_SOURCES];

    // these are indices of instructions in the ROB that depend on me
    //uint8_t memory_instrs_depend_on_me[ROB_SIZE];
    fastset memory_instrs_depend_on_me;

    uint32_t lq_index[NUM_INSTR_SOURCES],
             sq_index[NUM_INSTR_DESTINATIONS_SPARC],
             forwarding_index[NUM_INSTR_DESTINATIONS_SPARC];

    uint8_t mem_source_went_offchip[NUM_INSTR_SOURCES]; // RBERA: to track which memory sources went offchip

    ooo_model_instr() {
        instr_id = 0;
        ip = 0;
        fetch_producer = 0;
        producer_id = 0;
        translated_cycle = 0;
        fetched_cycle = 0;
        execute_begin_cycle = 0;
        retired_cycle = 0;
        event_cycle = 0;
        rob_head_cycle = 0;

        is_branch = 0;
        is_memory = 0;
        is_load = 0;
        is_store = 0;
        branch_taken = 0;
        branch_mispredicted = 0;
	    branch_prediction_made = 0;
        translated = 0;
        data_translated = 0;
        is_producer = 0;
        is_consumer = 0;
        reg_RAW_producer = 0;
        fetched = 0;
        scheduled = 0;
        executed = 0;
        reg_ready = 0;
        mem_ready = 0;
        asid[0] = UINT8_MAX;
        asid[1] = UINT8_MAX;

	    branch_type = NOT_BRANCH;
	    branch_target = 0;

        instruction_pa = 0;
        data_pa = 0;
        virtual_address = 0;
        physical_address = 0;

        num_reg_ops = 0;
        num_mem_ops = 0;
        num_mem_src = 0;
        num_mem_dest = 0;
        num_reg_dependent = 0;

        for (uint32_t i=0; i<NUM_INSTR_SOURCES; i++) {
            source_registers[i] = 0;
            source_memory[i] = 0;
            source_virtual_address[i] = 0;
            source_added[i] = 0;
            lq_index[i] = UINT32_MAX;
            reg_RAW_checked[i] = 0;
            mem_source_went_offchip[i] = 0;
        }

        for (uint32_t i=0; i<NUM_INSTR_DESTINATIONS_SPARC; i++) {
            destination_memory[i] = 0;
            destination_registers[i] = 0;
            destination_virtual_address[i] = 0;
            destination_added[i] = 0;
            sq_index[i] = UINT32_MAX;
            forwarding_index[i] = 0;
        }

#if 0
        for (uint32_t i=0; i<ROB_SIZE; i++) {
            registers_instrs_depend_on_me[i] = 0;
            memory_instrs_depend_on_me[i] = 0;

            for (uint32_t j=0; j<NUM_INSTR_SOURCES; j++)
                registers_index_depend_on_me[i][j] = 0;
        }
#endif
    };

    string to_string()
    {
        stringstream ss;

        ss << "|"
            << hex << std::setw(8) << instr_id << dec << "|"
            << hex << std::setw(8) << (uint64_t)ip << dec << "|"
            << "<" << (uint32_t)is_branch << "-" << (uint32_t)branch_mispredicted << "-" << (uint32_t)branch_taken << ">"
            << "<" << (uint32_t)is_producer << "-" << (uint32_t)is_consumer << "-" << (uint32_t)reg_RAW_producer << ">"
            << "<" << (uint64_t)is_memory << "-" << (uint32_t)is_load << "-" << (uint32_t)is_store << ">"
            << "|";

        for(uint32_t i=0; i<NUM_INSTR_SOURCES; i++)
        {
            if(i) ss << ",";
            ss << std::setw(2) << (uint32_t)source_registers[i];
        }
        ss << "|";
        
        for(uint32_t i=0; i<NUM_INSTR_SOURCES; i++)
        {
            if(i) ss << ",";
            ss << hex << std::setw(8) << (uint32_t)source_memory[i] << dec;
        }
        ss << "|";
        
        for(uint32_t i=0; i<NUM_INSTR_DESTINATIONS; i++)
        {
            if(i) ss << ",";
            ss << std::setw(2) << (uint32_t)destination_registers[i];
        }
        ss << "|";
        
        for(uint32_t i=0; i<NUM_INSTR_DESTINATIONS; i++)
        {
            if(i) ss << ",";
            ss << hex << std::setw(8) << (uint32_t)destination_memory[i] << dec;
        }
        ss << "|";

        ss << hex << std::setw(8) << producer_id << dec;
        ss << "|";

        ITERATE_SET(i, registers_instrs_depend_on_me, ROB_SIZE)
        {
            if(i) ss << ",";
            ss << std::setw(3) << i;
        }
        ss << "|";
        
        return ss.str();
    }
};

#endif