#ifndef OPCODE_HH
#define OPCODE_HH


#include <string>
#include <cstdint>
#include <map>

enum AddressingMode : int {
    Immediate,
    None,
    ZeroPage,
    ZeroPage_X,
    ZeroPage_Y,
    Absolute,
    Absolute_X,
    Absolute_Y,
    Indirect_Y,
    Indirect_X,
    Indirect,
    Accumulator,
    Relative,
    Implied,
};

class Opcode {
public:
    uint8_t opcode;
    std::string mnemonic;
    uint8_t len_bytes;
    uint8_t cycles; 
    AddressingMode mode;
};



 const std::map<uint8_t, Opcode> opcodes = {
        //Lookup, opcode, mnemonic, bytes, cycles, addressing mode
        {0x69, {0x69, "ADC", 2, 2, AddressingMode::Immediate}},
        {0x65, {0x65, "ADC", 2, 3, AddressingMode::ZeroPage}},
        {0x75, {0x75, "ADC", 2, 4, AddressingMode::ZeroPage_X}},
        {0x6D, {0x6D, "ADC", 3, 4, AddressingMode::Absolute}},
        {0x7D, {0x7D, "ADC", 3, 4, AddressingMode::Absolute_X}}, //+1 if page cross
        {0x79, {0x79, "ADC", 3, 4, AddressingMode::Absolute_Y}}, //+1 if page cross
        {0x61, {0x61, "ADC", 2, 6, AddressingMode::Indirect_X}},
        {0x71, {0x71, "ADC", 2, 5, AddressingMode::Indirect_Y}}, //+1 if page cross

        /* AND */
        {0x29, {0x29, "AND", 2, 2, AddressingMode::Immediate}},
        {0x25, {0x25, "AND", 2, 3, AddressingMode::ZeroPage}},
        {0x35, {0x35, "AND", 2, 4, AddressingMode::ZeroPage_X}},
        {0x2D, {0x2D, "AND", 3, 4, AddressingMode::Absolute}},
        {0x3D, {0x3D, "AND", 3, 4, AddressingMode::Absolute_X}}, //+1 if page cross
        {0x39, {0x39, "AND", 3, 4, AddressingMode::Absolute_Y}}, //+1 if page cross
        {0x21, {0x21, "AND", 2, 6, AddressingMode::Indirect_X}},
        {0x31, {0x31, "AND", 2, 5, AddressingMode::Indirect_Y}}, //+1 if page cross

        /*ASL */
        {0x0A, {0x0A, "ASL", 1, 2, AddressingMode::Accumulator}},
        {0x06, {0x06, "ASL", 2, 5, AddressingMode::ZeroPage}},
        {0x16, {0x16, "ASL", 2, 6, AddressingMode::ZeroPage_X}},
        {0x0E, {0x0E, "ASL", 3, 6, AddressingMode::Absolute}},
        {0x1E, {0x1E, "ASL", 3, 7, AddressingMode::Absolute_X}}, //+1 if page cross

        /* BCC */
        {0x90, {0x90, "BCC", 2, 2, AddressingMode::Relative}}, //+1 if branch succeeds +2 if to a new page

        /* BCS */
        {0xB0, {0xB0, "BCS", 2, 2, AddressingMode::Relative}}, //+1 if branch succeeds +2 if to a new page

        /*BEQ */
        {0xF0, {0xF0, "BEQ", 2, 2, AddressingMode::Relative}}, //+1 if branch succeeds +2 if to a new page

        /*BIT*/
        {0x24, {0x24, "BIT", 2, 3, AddressingMode::ZeroPage}}, 
        {0x2C, {0x2C, "BIT", 3, 4, AddressingMode::Absolute}}, 

        /*BMI*/
        {0x30, {0x30, "BMI", 2, 2, AddressingMode::Relative}},   //+1 if branch succeeds +2 if to a new page

        /* BNE */
        {0xD0, {0xD0, "BNE", 2, 2, AddressingMode::Relative}},   //+1 if branch succeeds +2 if to a new page


        /* BPL */
        {0x10, {0x10, "BPL", 2, 2, AddressingMode::Relative}}, 

        /*BRK*/
        {0x00, {0x00, "BRK", 1, 7, AddressingMode::Implied}}, 

        /* BVC */
        {0x50, {0x50, "BVC", 2, 2, AddressingMode::Relative}},   //+1 if branch succeeds +2 if to a new page

        /* BVS */
        {0x70, {0x70, "BVS", 2, 2, AddressingMode::Relative}},   //+1 if branch succeeds +2 if to a new page

        /*CLC */
        {0x18, {0x18, "CLC", 1, 2, AddressingMode::Implied}},

        /*CLD */
        {0xD8, {0xD8, "CLD", 1, 2, AddressingMode::Implied}},

        /* CLI */
         {0x58, {0x58, "CLI", 1, 2, AddressingMode::Implied}},

        /* CLV */
         {0xB8, {0xB8, "CLV", 1, 2, AddressingMode::Implied}},

         /* CMP*/
        {0xC9, {0xC9, "CMP", 2, 2, AddressingMode::Immediate}},
        {0xC5, {0xC5, "CMP", 2, 3, AddressingMode::ZeroPage}},
        {0xD5, {0xD5, "CMP", 2, 4, AddressingMode::ZeroPage_X}},
        {0xCD, {0xCD, "CMP", 3, 4, AddressingMode::Absolute}},
        {0xDD, {0xDD, "CMP", 3, 4, AddressingMode::Absolute_X}}, //+1 if page cross
        {0xD9, {0xD9, "CMP", 3, 4, AddressingMode::Absolute_Y}}, //+1 if page cross
        {0xC1, {0xC1, "CMP", 2, 6, AddressingMode::Indirect_X}},
        {0xD1, {0xD1, "CMP", 2, 5, AddressingMode::Indirect_Y}}, //+1 if page cross

        /*CPX*/
        {0xE0, {0xE0, "CPX", 2, 2, AddressingMode::Immediate}},
        {0xE4, {0xE4, "CPX", 2, 3, AddressingMode::ZeroPage}},
        {0xEC, {0xEC, "CPX", 3, 4, AddressingMode::Absolute}},

        /*CPY*/
        {0xC0, {0xC0, "CPX", 2, 2, AddressingMode::Immediate}},
        {0xC4, {0xC4, "CPX", 2, 3, AddressingMode::ZeroPage}},
        {0xCC, {0xCC, "CPX", 3, 4, AddressingMode::Absolute}},
    
        /*DEC*/
        {0xC6, {0xC6, "DEC", 2, 2, AddressingMode::ZeroPage}},
        {0xD6, {0xD6, "DEC", 2, 3, AddressingMode::ZeroPage_X}},
        {0xCE, {0xCE, "DEC", 2, 4, AddressingMode::Absolute}},
        {0xDE, {0xDE, "DEC", 3, 4, AddressingMode::Absolute_X}},

        /*DEY */
        {0x88, {0x88, "DEY", 1, 2, AddressingMode::Implied}},

        /* DEX */
        {0xCA,{0XCA, "DEX", 1,2, AddressingMode::Implied}},

        /* EOR */
        {0x49, {0x49, "EOR", 2, 2, AddressingMode::Immediate}},
        {0x45, {0x45, "EOR", 2, 3, AddressingMode::ZeroPage}},
        {0x55, {0x55, "EOR", 2, 4, AddressingMode::ZeroPage_X}},
        {0x4D, {0x4D, "EOR", 3, 4, AddressingMode::Absolute}},
        {0x5D, {0x5D, "EOR", 3, 4, AddressingMode::Absolute_X}}, //+1 if page cross
        {0x59, {0x59, "EOR", 3, 4, AddressingMode::Absolute_Y}}, //+1 if page cross
        {0x41, {0x41, "EOR", 2, 6, AddressingMode::Indirect_X}},
        {0x51, {0x51, "EOR", 2, 5, AddressingMode::Indirect_Y}}, //+1 if page cross

        /*INC */
        {0xE6, {0xE6, "INC", 2, 5, AddressingMode::ZeroPage}},
        {0xF6, {0xF6, "INC", 2, 6, AddressingMode::ZeroPage_X}},
        {0xEE, {0xEE, "INC", 3, 6, AddressingMode::Absolute}},
        {0xFE, {0xFE, "INC", 3, 7, AddressingMode::Absolute_X}}, //+1 if page cross

        /* INX */
        {0xE8, {0xE8, "INX", 1, 2, AddressingMode::Implied}},
        /* INY */
        {0xC8, {0xC8, "INY", 1, 2, AddressingMode::Implied}},


        /* JMP */
        {0x4C, {0x4C, "JMP", 3, 3, AddressingMode::Absolute}},
        {0x6C, {0x6C, "JMP", 3, 5, AddressingMode::Indirect}},


        /*JSR*/
        {0x20, {0x20, "JSR", 3, 6, AddressingMode::Absolute}},

        /* LDA */
        {0xA9, {0xA9, "LDA", 2, 2, AddressingMode::Immediate}},
        {0xA5, {0xA5, "LDA", 2, 3, AddressingMode::ZeroPage}},
        {0xB5, {0xB5, "LDA", 2, 4, AddressingMode::ZeroPage_X}},
        {0xAD, {0xAD, "LDA", 3, 4, AddressingMode::Absolute}},
        {0xBD, {0xBD, "LDA", 3, 4, AddressingMode::Absolute_X}}, //+1 if page cross
        {0xB9, {0xB9, "LDA", 3, 4, AddressingMode::Absolute_Y}}, //+1 if page cross
        {0xA1, {0xA1, "LDA", 2, 6, AddressingMode::Indirect_X}},
        {0xB1, {0xB1, "LDA", 2, 5, AddressingMode::Indirect_Y}}, //+1 if page cross

        /* LDX */
        {0xA2, {0xA2, "LDX", 2, 2, AddressingMode::Immediate}},
        {0xA6, {0xA6, "LDX", 2, 3, AddressingMode::ZeroPage}},
        {0xB6, {0xB6, "LDX", 2, 4, AddressingMode::ZeroPage_Y}},
        {0xAE, {0xAE, "LDX", 3, 4, AddressingMode::Absolute}},
        {0xBE, {0xBE, "LDX", 3, 4, AddressingMode::Absolute_Y}}, //+1 if page cross
   
        /* LDY */
        {0xA0, {0xA0, "LDY", 2, 2, AddressingMode::Immediate}},
        {0xA4, {0xA4, "LDY", 2, 3, AddressingMode::ZeroPage}},
        {0xB4, {0xB4, "LDY", 2, 4, AddressingMode::ZeroPage_X}},
        {0xAC, {0xAC, "LDY", 3, 4, AddressingMode::Absolute}},
        {0xBC, {0xBC, "LDY", 3, 4, AddressingMode::Absolute_X}}, //+1 if page cross

        /*LSR*/
        {0x4A, {0x4A, "LSR", 1, 2, AddressingMode::Accumulator}},
        {0x46, {0x46, "LSR", 2, 3, AddressingMode::ZeroPage}},
        {0x56, {0x56, "LSR", 2, 4, AddressingMode::ZeroPage_X}},
        {0x4E, {0x4E, "LSR", 3, 4, AddressingMode::Absolute}},
        {0x5E, {0x5E, "LSR", 3, 4, AddressingMode::Absolute_X}}, //+1 if page cross


        /*NOP*/
        {0xEA, {0xEA, "NOP", 1, 2, AddressingMode::Implied}},



        /* ORA */
        {0x09, {0x09, "ORA", 2, 2, AddressingMode::Immediate}},
        {0x05, {0x05, "ORA", 2, 3, AddressingMode::ZeroPage}},
        {0x15, {0x15, "ORA", 2, 4, AddressingMode::ZeroPage_X}},
        {0x0D, {0x0D, "ORA", 3, 4, AddressingMode::Absolute}},
        {0x1D, {0x1D, "ORA", 3, 4, AddressingMode::Absolute_X}}, //+1 if page cross
        {0x19, {0x19, "ORA", 3, 4, AddressingMode::Absolute_Y}}, //+1 if page cross
        {0x01, {0x01, "ORA", 2, 6, AddressingMode::Indirect_X}},
        {0x11, {0x11, "ORA", 2, 5, AddressingMode::Indirect_Y}}, //+1 if page cross

        /*PHA*/
        {0x48, {0x48, "PHA", 1, 3, AddressingMode::Implied}},

        /*PHP*/
        {0x08, {0x08, "PHP", 1, 3, AddressingMode::Implied}},

        /*PLA*/
        {0x68, {0x68, "PlA", 1, 4, AddressingMode::Implied}},
        
        /*PLP */
        {0x28, {0x28, "PLP", 1, 4, AddressingMode::Implied}},

        /*ROL*/
        {0x2A, {0x2A, "ROL", 1, 2, AddressingMode::Accumulator}},
        {0x26, {0x26, "ROL", 2, 5, AddressingMode::ZeroPage}},
        {0x36, {0x36, "ROL", 2, 6, AddressingMode::ZeroPage_X}},
        {0x2E, {0x2E, "ROL", 3, 6, AddressingMode::Absolute}},
        {0x3E, {0x3E, "ROL", 3, 7, AddressingMode::Absolute_X}}, //+1 if page cross


        /*ROR*/
        {0x6A, {0x6A, "ROR", 1, 2, AddressingMode::Accumulator}},
        {0x66, {0x66, "ROR", 2, 5, AddressingMode::ZeroPage}},
        {0x76, {0x76, "ROR", 2, 6, AddressingMode::ZeroPage_X}},
        {0x6E, {0x6E, "ROR", 3, 6, AddressingMode::Absolute}},
        {0x7E, {0x7E, "ROR", 3, 7, AddressingMode::Absolute_X}}, //+1 if page cross

        /*RTI */
        {0x40, {0x40, "RTI", 1, 6, AddressingMode::Implied}},

        /*RTS */
        {0x60, {0x60, "RTS", 1, 6, AddressingMode::Implied}},

        /* SBC */
        {0xE9, {0xE9, "SBC", 2, 2, AddressingMode::Immediate}},
        {0xE5, {0xE5, "SBC", 2, 3, AddressingMode::ZeroPage}},
        {0xF5, {0xF5, "SBC", 2, 4, AddressingMode::ZeroPage_X}},
        {0xED, {0xED, "SBC", 3, 4, AddressingMode::Absolute}},
        {0xFD, {0xFD, "SBC", 3, 4, AddressingMode::Absolute_X}}, //+1 if page cross
        {0xF9, {0xF9, "SBC", 3, 4, AddressingMode::Absolute_Y}}, //+1 if page cross
        {0xE1, {0xE1, "SBC", 2, 6, AddressingMode::Indirect_X}},
        {0xF1, {0xF1, "SBC", 2, 5, AddressingMode::Indirect_Y}}, //+1 if page cross

        /*SEC*/
        {0x38, {0x38, "SEC", 1, 2, AddressingMode::Implied}},

        /*SED*/
        {0xF8, {0xF8, "SED", 1, 2, AddressingMode::Implied}},

        /*SEI*/
        {0x78, {0x78, "SEI", 1, 2, AddressingMode::Implied}},

        /* STA */
        {0x85, {0x85, "STA", 2, 3, AddressingMode::ZeroPage}},
        {0x95, {0x95, "STA", 2, 4, AddressingMode::ZeroPage_X}},
        {0x8D, {0x8D, "STA", 3, 4, AddressingMode::Absolute}},
        {0x9D, {0x9D, "STA", 3, 5, AddressingMode::Absolute_X}},
        {0x99, {0x99, "STA", 3, 5, AddressingMode::Absolute_Y}}, 
        {0x81, {0x81, "STA", 2, 6, AddressingMode::Indirect_X}},
        {0x91, {0x91, "STA", 2, 6, AddressingMode::Indirect_Y}}, 


        /* STX */
        {0x86, {0x86, "STX", 2, 3, AddressingMode::ZeroPage}},
        {0x96, {0x96, "STX", 2, 4, AddressingMode::ZeroPage_Y}},
        {0x8E, {0x8E, "STX", 3, 4, AddressingMode::Absolute}},

        /* STX */
        {0x84, {0x84, "STY", 2, 3, AddressingMode::ZeroPage}},
        {0x94, {0x94, "STY", 2, 4, AddressingMode::ZeroPage_X}},
        {0x8C, {0x8C, "STY", 3, 4, AddressingMode::Absolute}},

        /*TAX*/
        {0xAA, {0xAA, "TAX", 1, 2, AddressingMode::Implied}},

        /*TAY*/
        {0xA8, {0xA8, "TAY", 1, 2, AddressingMode::Implied}},

        /*TSX*/
        {0xBA, {0xBA, "TSX", 1, 2, AddressingMode::Implied}},

        /*TXA*/
        {0x8A, {0x8A, "TXA", 1, 2, AddressingMode::Implied}},

         /*TXS*/
        {0x9A, {0x9A, "TXS", 1, 2, AddressingMode::Implied}},   

        /*TYA*/
        {0x98, {0x98, "TYA", 1, 2, AddressingMode::Implied}},  



        /* ========================== EXTRA / ILLEGAL OPCODES ==================*/

        /*ASO*/
        {0x0F, {0x0F, "*SLO", 3, 6, AddressingMode::Absolute}}, 
        {0x1F, {0x1F, "*SLO", 3, 7, AddressingMode::Absolute_X}},  
        {0x1B, {0x1B, "*SLO", 3, 7, AddressingMode::Absolute_Y}},  
        {0x07, {0x07, "*SLO", 2, 5, AddressingMode::ZeroPage}},  
        {0x17, {0x17, "*SLO", 2, 6, AddressingMode::ZeroPage_X}},  
        {0x03, {0x03, "*SLO", 2, 8, AddressingMode::Indirect_X}},   
        {0x13, {0x13, "*SLO", 2, 8, AddressingMode::Indirect_Y}}, 


        /*RLA*/
        {0x2F, {0x2F, "RLA", 3, 6, AddressingMode::Absolute}}, 
        {0x3F, {0x3F, "RLA", 3, 7, AddressingMode::Absolute_X}},  
        {0x3B, {0x3B, "RLA", 3, 7, AddressingMode::Absolute_Y}},  
        {0x27, {0x27, "RLA", 2, 5, AddressingMode::ZeroPage}},  
        {0x37, {0x37, "RLA", 2, 6, AddressingMode::ZeroPage_X}},  
        {0x23, {0x23, "RLA", 2, 8, AddressingMode::Indirect_X}},   
        {0x33, {0x33, "RLA", 2, 8, AddressingMode::Indirect_Y}}, 



        /*LSE*/
        {0x4F, {0x4F, "*SRE", 3, 6, AddressingMode::Absolute}}, 
        {0x5F, {0x5F, "*SRE", 3, 7, AddressingMode::Absolute_X}},  
        {0x5B, {0x5B, "*SRE", 3, 7, AddressingMode::Absolute_Y}},  
        {0x47, {0x47, "*SRE", 2, 5, AddressingMode::ZeroPage}},  
        {0x57, {0x57, "*SRE", 2, 6, AddressingMode::ZeroPage_X}},  
        {0x43, {0x43, "*SRE", 2, 8, AddressingMode::Indirect_X}},   
        {0x53, {0x53, "*SRE", 2, 8, AddressingMode::Indirect_Y}}, 


        /*RRA*/
        {0x6F, {0x6F, "RRA", 3, 6, AddressingMode::Absolute}}, 
        {0x7F, {0x7F, "RRA", 3, 7, AddressingMode::Absolute_X}},  
        {0x7B, {0x7B, "RRA", 3, 7, AddressingMode::Absolute_Y}},  
        {0x67, {0x67, "RRA", 2, 5, AddressingMode::ZeroPage}},  
        {0x77, {0x77, "RRA", 2, 6, AddressingMode::ZeroPage_X}},  
        {0x63, {0x63, "RRA", 2, 8, AddressingMode::Indirect_X}},   
        {0x73, {0x73, "RRA", 2, 8, AddressingMode::Indirect_Y}}, 


        /*AXS*/
        {0x8F, {0x8F, "*SAX", 3, 4, AddressingMode::Absolute}}, 
        {0x87, {0x87, "*SAX", 2, 3, AddressingMode::ZeroPage}},  
        {0x97, {0x97, "*SAX", 2, 4, AddressingMode::ZeroPage_Y}}, 
        {0x83, {0x83, "*SAX", 2, 6, AddressingMode::Indirect_X}}, 



        /*LAX*/
        {0xAF, {0xAF, "LAX", 3, 4, AddressingMode::Absolute}}, 
        {0xBF, {0xBF, "LAX", 3, 4, AddressingMode::Absolute_Y}}, //+1 if page  
        {0xA7, {0xA7, "LAX", 2, 3, AddressingMode::ZeroPage}},  
        {0xB7, {0xB7, "LAX", 2, 4, AddressingMode::ZeroPage_Y}}, 
        {0xA3, {0xA3, "LAX", 2, 6, AddressingMode::Indirect_X}}, 
        {0xB3, {0xB3, "LAX", 2, 5, AddressingMode::Indirect_Y}},  //+1 if page cross

        /*DCM*/
        {0xCF, {0xCF, "*DCP", 3, 6, AddressingMode::Absolute}}, 
        {0xDF, {0xDF, "*DCP", 3, 7, AddressingMode::Absolute_X}},  
        {0xDB, {0xDB, "*DCP", 3, 7, AddressingMode::Absolute_Y}},  
        {0xC7, {0xC7, "*DCP", 2, 5, AddressingMode::ZeroPage}},  
        {0xD7, {0xD7, "*DCP", 2, 6, AddressingMode::ZeroPage_X}},  
        {0xC3, {0xC3, "*DCP", 2, 8, AddressingMode::Indirect_X}},   
        {0xD3, {0xD3, "*DCP", 2, 8, AddressingMode::Indirect_Y}}, 

        /*INS*/
        {0xEF, {0xEF, "*ISB", 3, 6, AddressingMode::Absolute}}, 
        {0xFF, {0xFF, "*ISB", 3, 7, AddressingMode::Absolute_X}},  
        {0xFB, {0xFB, "*ISB", 3, 7, AddressingMode::Absolute_Y}},  
        {0xE7, {0xE7, "*ISB", 2, 5, AddressingMode::ZeroPage}},  
        {0xF7, {0xF7, "*ISB", 2, 6, AddressingMode::ZeroPage_X}},  
        {0xE3, {0xE3, "*ISB", 2, 8, AddressingMode::Indirect_X}},   
        {0xF3, {0xF3, "*ISB", 2, 8, AddressingMode::Indirect_Y}},

        /*ALR*/
        {0x4B, {0x4B, "ALR", 2, 2, AddressingMode::Immediate}}, 
  
        /*ARR*/
        {0x6B, {0x6B, "ARR", 2, 2, AddressingMode::Immediate}}, 

        /*XAA*/
        {0x8B, {0x8B, "XAA", 2, 2, AddressingMode::Immediate}}, 


        /*XAA*/
        {0xAB, {0xAB, "OAL", 2, 2, AddressingMode::Immediate}}, 

        /*SAX*/
        {0xCB, {0xCB, "SAX", 2, 2, AddressingMode::Immediate}}, 

        /*NOP*/
        {0x1A, {0x1A, "NOP", 1, 2, AddressingMode::Implied}},
        {0x3A, {0x3A, "NOP", 1, 2, AddressingMode::Implied}},
        {0x5A, {0x5A, "NOP", 1, 2, AddressingMode::Implied}},
        {0x7A, {0x7A, "NOP", 1, 2, AddressingMode::Implied}},
        {0xDA, {0xDA, "NOP", 1, 2, AddressingMode::Implied}},
        {0xFA, {0xFA, "NOP", 1, 2, AddressingMode::Implied}},
        

        /*SKB*/
        {0x80, {0x80, "*NOP", 1, 2, AddressingMode::Implied}},
        {0x82, {0x82, "*NOP", 1, 2, AddressingMode::Implied}},
        {0xC2, {0xC2, "*NOP", 1, 2, AddressingMode::Implied}},
        {0xE2, {0xE2, "*NOP", 1, 2, AddressingMode::Implied}},
        {0x04, {0x04, "*NOP", 1, 2, AddressingMode::Implied}},
        {0x14, {0x14, "*NOP", 1, 2, AddressingMode::Implied}},
        {0x34, {0x34, "*NOP", 1, 2, AddressingMode::Implied}},
        {0x44, {0x44, "*NOP", 1, 2, AddressingMode::Implied}},
        {0x54, {0x54, "*NOP", 1, 2, AddressingMode::Implied}},
        {0x64, {0x64, "*NOP", 1, 2, AddressingMode::Implied}},
        {0x74, {0x74, "*NOP", 1, 2, AddressingMode::Implied}},
        {0xD4, {0xD4, "*NOP", 1, 2, AddressingMode::Implied}},
        {0xF4, {0xF4, "*NOP", 1, 2, AddressingMode::Implied}},
    

        /*SKW*/
        {0x0C, {0x0C, "SKW", 3, 4, AddressingMode::Absolute}},
        {0x1C, {0x1C, "SKW", 3, 4, AddressingMode::Absolute_X}},
        {0x3C, {0x3C, "SKW", 3, 4, AddressingMode::Absolute_X}},
        {0x5C, {0x5C, "SKW", 3, 4, AddressingMode::Absolute_X}},
        {0x7C, {0x7C, "SKW", 3, 4, AddressingMode::Absolute_X}},
        {0xDC, {0xDC, "SKW", 3, 4, AddressingMode::Absolute_X}},
        {0xFC, {0xFC, "SKW", 3, 4, AddressingMode::Absolute_X}},


        /*HLT*/
        {0x02, {0x02, "HLT", 1, 4, AddressingMode::Implied}},
        {0x12, {0x12, "HLT", 1, 4, AddressingMode::Implied}},
        {0x22, {0x22, "HLT", 1, 4, AddressingMode::Implied}},
        {0x32, {0x32, "HLT", 1, 4, AddressingMode::Implied}},
        {0x42, {0x42, "HLT", 1, 4, AddressingMode::Implied}},
        {0x52, {0x52, "HLT", 1, 4, AddressingMode::Implied}},
        {0x62, {0x62, "HLT", 1, 4, AddressingMode::Implied}},
        {0x72, {0x72, "HLT", 1, 4, AddressingMode::Implied}},
        {0x92, {0x92, "HLT", 1, 4, AddressingMode::Implied}},
        {0xB2, {0xB2, "HLT", 1, 4, AddressingMode::Implied}},
        {0xD2, {0xD2, "HLT", 1, 4, AddressingMode::Implied}},
        {0xF2, {0xF2, "HLT", 1, 4, AddressingMode::Implied}},



        /*TAS*/
        {0x9B, {0x9B, "TAS", 3, 5, AddressingMode::Absolute_Y}}, 

        /*TAS*/
        {0x9C, {0x9C, "SAY", 3, 5, AddressingMode::Absolute_X}}, 

        /*XAS*/
        {0x9E, {0x9E, "XAS", 3, 5, AddressingMode::Absolute_X}}, 


        /*AXA*/
        {0x9F, {0x9F, "AXA", 3, 5, AddressingMode::Absolute_X}}, 
        {0x93, {0x93, "AXA", 2, 6, AddressingMode::Indirect_Y}}, 


        {0xEB, {0xEB, "*SBC", 2, 6, AddressingMode::Immediate}}, 

    };




#endif