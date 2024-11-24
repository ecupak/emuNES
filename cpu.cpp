#include "cpu.hpp"


CPU::CPU()
{
	memory.resize(0xFFFF);
}


void CPU::reset()
{
	program_counter = 0x00;
	stack_pointer = 0x00;
	reg_accumulator = 0x00;
	reg_x  = 0x00;
	reg_y  = 0x00;
	processor_status = 0b0000'0000;
}


void CPU::loadROM(std::vector<byte>& rom)
{
	std::copy(rom.begin(), rom.end(), memory.begin() + 0x8000);
}


void CPU::run()
{
	while (program_counter < memory.size())
	{
		// fetch();
		byte opcode = memory[program_counter];
		++program_counter;


		// decode();
		switch (opcode)
		{
			// ADC - Add with Carry
			case 0x69: doADC(get_Immediate());		break;
			case 0x65: doADC(get_ZeroPage());		break;
			case 0x75: doADC(get_ZeroPageX());		break;
			case 0x6d: doADC(get_Absolute());		break;
			case 0x7d: doADC(get_AbsoluteX());		break;
			case 0x79: doADC(get_AbsoluteY());		break;
			case 0x61: doADC(get_IndirectX());		break;
			case 0x71: doADC(get_IndirectY());		break;

			// AAND - Logical AND
			case 0x29: doAND(get_Immediate());		break;
			case 0x25: doAND(get_ZeroPage());		break;
			case 0x35: doAND(get_ZeroPageX());		break;
			case 0x2d: doAND(get_Absolute());		break;
			case 0x3d: doAND(get_AbsoluteX());		break;
			case 0x39: doAND(get_AbsoluteY());		break;
			case 0x21: doAND(get_IndirectX());		break;
			case 0x31: doAND(get_IndirectY());		break;

			// ASL - Arithmetic Shift Left
			case 0x0a: doAND(get_Accumulator());	break;
			case 0x06: doAND(get_ZeroPage());		break;
			case 0x16: doAND(get_ZeroPageX());		break;
			case 0x0e: doAND(get_Absolute());		break;
			case 0x1e: doAND(get_AbsoluteX());		break;

			// (Branches) BCC, BCS, BEQ, BMI, BNE, BVC, BVS
			case 0x90: doBCC(get_Immediate());		break;
			case 0xb0: doBCS(get_Immediate());		break;
			case 0xf0: doBEQ(get_Immediate());		break;
			case 0x30: doBMI(get_Immediate());		break;
			case 0xd0: doBNE(get_Immediate());		break;
			case 0x10: doBPL(get_Immediate());		break;
			case 0x50: doBVC(get_Immediate());		break;
			case 0x70: doBVS(get_Immediate());		break;

			// BIT - Bit Test
			case 0x24: doBIT(get_ZeroPage());		break;
			case 0x2c: doBIT(get_Absolute());		break;

			// BRK - Force Interrupt
			case 0x00: doBRK();						break;

			// (Clears) CLC, CLD, CLI, CLV 
			case 0x18: doCLC();						break;
			case 0xd8: doCLD();						break;
			case 0x58: doCLI();						break;
			case 0xb8: doCLV();						break;			

			// CMP - Compare
			case 0xc9: doCMP(get_Immediate());		break;
			case 0xc5: doCMP(get_ZeroPage());		break;
			case 0xd5: doCMP(get_ZeroPageX());		break;
			case 0xcd: doCMP(get_Absolute());		break;
			case 0xdd: doCMP(get_AbsoluteX());		break;
			case 0xd9: doCMP(get_AbsoluteY());		break;
			case 0xc1: doCMP(get_IndirectX());		break;
			case 0xd1: doCMP(get_IndirectY());		break;

			// CPX - Compare X Register
			case 0xe0: doCMX(get_Immediate());		break;
			case 0xe4: doCMX(get_ZeroPage());		break;
			case 0xec: doCMX(get_Absolute());		break;

			// CPY - Compare Y Register
			case 0xc0: doCMY(get_Immediate());		break;
			case 0xc4: doCMY(get_ZeroPage());		break;
			case 0xcc: doCMY(get_Absolute());		break;

			// (Decrements) DEC, DEX, DEY
			case 0xc6: doDEC(get_ZeroPage());		break;
			case 0xd6: doDEC(get_ZeroPageX());		break;
			case 0xce: doDEC(get_Absolute());		break;
			case 0xde: doDEC(get_AbsoluteX());		break;
			case 0xca: doDEX();						break;
			case 0x88: doDEY();						break;

			// EOR - Exclusive OR
			case 0x49: doEOR(get_Immediate());		break;
			case 0x45: doEOR(get_ZeroPage());		break;
			case 0x55: doEOR(get_ZeroPageX());		break;
			case 0x4d: doEOR(get_Absolute());		break;
			case 0x5d: doEOR(get_AbsoluteX());		break;
			case 0x59: doEOR(get_AbsoluteY());		break;
			case 0x41: doEOR(get_IndirectX());		break;
			case 0x51: doEOR(get_IndirectY());		break;

			// (Increments) INC, INX, INY
			case 0xe6: doINC(get_ZeroPage());		break;
			case 0xf6: doINC(get_ZeroPageX());		break;
			case 0xee: doINC(get_Absolute());		break;
			case 0xfe: doINC(get_AbsoluteX());		break;
			case 0xe8: doINX();						break;
			case 0xc8: doINY();						break;
			
			// JMP - Jump
			case 0x4c: doJMP(getAddr_Absolute());	break;
			case 0x6c: doJMP(getAddr_Indirect());	break;

			// JSR - skip for now

			// LDA - Load Accumulator
			case 0xa9: doLDA(get_Immediate());		break;
			case 0xa5: doLDA(get_ZeroPage());		break;
			case 0xb5: doLDA(get_ZeroPageX());		break;
			case 0xad: doLDA(get_Absolute());		break;
			case 0xbd: doLDA(get_AbsoluteX());		break;
			case 0xb9: doLDA(get_AbsoluteY());		break;
			case 0xa1: doLDA(get_IndirectX());		break;
			case 0xb1: doLDA(get_IndirectY());		break;

			// LDX - Load X Register
			case 0xa2: doLDX(get_Immediate());		break;
			case 0xa6: doLDX(get_ZeroPage());		break;
			case 0xb6: doLDX(get_ZeroPageY());		break;
			case 0xae: doLDX(get_Absolute());		break;			
			case 0xbe: doLDX(get_AbsoluteY());		break;

			// LDY - Load Y Register
			case 0xa0: doLDY(get_Immediate());		break;
			case 0xa4: doLDY(get_ZeroPage());		break;
			case 0xb4: doLDY(get_ZeroPageX());		break;
			case 0xac: doLDY(get_Absolute());		break;
			case 0xbc: doLDY(get_AbsoluteX());		break;
			

		}
	}
}


/*

	INSTRUCTIONS

 ______________________________________________________
|  __________________________________________________  |
| |                 |_______________|                | |
| |       ___       ,---------------.                | |
| |      |   |      |_______________|     emuNES     | |
| |   ___|   |___   ,---------------.                | |
| |  |    ,-.    |  |SELECT    START|   ___B  ___A   | |
| |  |___ `-' ___|  :===============:  |,-.| |,-.|   | |
| |      |   |      |  ===     ===  |  |._.| |._.|   | |
| |      |___|      :===============:  '---' '---'   | |
| |_________________|_______________|________________| |
|______________________________________________________|

*/
#pragma region INSTRUCTIONS

/*	
	ADC - Add with Carry
 
	This instruction adds the contents of a memory location to the accumulator together with the carry bit. 
	If overflow occurs the carry bit is set, this enables multiple byte addition to be performed.
*/
void CPU::doADC(byte& data)
{
	reg_accumulator = addBytes(data, reg_accumulator, true);
	setZeroAndNegativeFlags(reg_accumulator);	
}


/*	
	AND - Logical AND
 
	A logical AND is performed, bit by bit, on the accumulator contents using the contents of a byte of memory.
*/
void CPU::doAND(byte& data)
{
	reg_accumulator &= data;
	setZeroAndNegativeFlags(reg_accumulator);
}


/*	
	ASL - Arithmetic Shift Left
 
	This operation shifts all the bits of the accumulator or memory contents one bit left. 
	Bit 0 is set to 0 and bit 7 is placed in the carry flag. 
	The effect of this operation is to multiply the memory contents by 2 (ignoring 2's complement considerations), 
	setting the carry if the result will not fit in 8 bits.
*/
void CPU::doASL(byte& data)
{
	setFlag(C, (data & 0b1000'0000) != 0);
	data <<= 1;
	setZeroAndNegativeFlags(data);
}


/*	
	BCC - Branch if Carry Clear

	If the carry flag is clear then add the relative displacement to the program counter 
	to cause a branch to a new location.
*/
void CPU::doBCC(byte& data)
{
	if (!isFlagSet(C))
	{
		program_counter = addWords(program_counter, data);
	}
}


/*	
	BCS - Branch if Carry Set

	If the carry flag is set then add the relative displacement to the program counter
	to cause a branch to a new location.
*/
void CPU::doBCS(byte& data)
{
	if (isFlagSet(C))
	{
		program_counter = addWords(program_counter, data);
	}
}


/*	
	BEQ - Branch if Equal

	If the zero flag is set then add the relative displacement to the program counter
	to cause a branch to a new location.
*/
void CPU::doBEQ(byte& data)
{
	if (isFlagSet(Z))
	{
		program_counter = addWords(program_counter, data);
	}
}


/*	
	BIT - Bit Test

	This instructions is used to test if one or more bits are set in a target memory location. 
	The mask pattern in A is ANDed with the value in memory to set or clear the zero flag, 
	but the result is not kept. Bits 7 and 6 of the value from memory are copied into the N and V flags.
*/
void CPU::doBIT(byte& data)
{
	byte result{ reg_accumulator & data };

	if (result == 0)
	{
		setFlag(Z, true);
	}
	
	setFlag(N, (data & 0b1000'0000) != 0);
	setFlag(V, (data & 0b0100'0000) != 0);
}


/*	
	BMI - Branch if Minus

	If the negative flag is set then add the relative displacement to the program counter 
	to cause a branch to a new location.
*/
void CPU::doBMI(byte& data)
{
	if (isFlagSet(N))
	{
		program_counter = addWords(program_counter, data);
	}
}


/*	
	BNE - Branch if Not Equal

	If the zero flag is clear then add the relative displacement to the program counter 
	to cause a branch to a new location.
*/
void CPU::doBNE(byte& data)
{
	if (!isFlagSet(Z))
	{
		program_counter = addWords(program_counter, data);
	}
}


/*	
	BPL - Branch if Positive

	If the negative flag is clear then add the relative displacement to the program counter
	to cause a branch to a new location.
*/
void CPU::doBPL(byte& data)
{
	if (!isFlagSet(N))
	{
		program_counter = addWords(program_counter, data);
	}
}


/*	
	BRK - Force Interrupt

	The BRK instruction forces the generation of an interrupt request. 
	The program counter and processor status are pushed on the stack 
	then the IRQ interrupt vector at $FFFE/F is loaded into the PC and the break flag in the status set to one.
*/
void CPU::doBRK()
{
	// Push program_counter and processor_status onto stack.
	// Load word at $FFFE/F into program_counter.

	setFlag(B, true);
}


/*	
	BVC - Branch if Overflow Clear

	If the overflow flag is clear then add the relative displacement to the program counter
	to cause a branch to a new location.
*/
void CPU::doBVC(byte& data)
{
	if (!isFlagSet(V))
	{
		program_counter = addWords(program_counter, data);
	}
}


/*	
	BVS - Branch if Overflow Set

	If the overflow flag is set then add the relative displacement to the program counter
	to cause a branch to a new location.
*/
void CPU::doBVS(byte& data)
{
	if (isFlagSet(V))
	{
		program_counter = addWords(program_counter, data);
	}
}


/*	
	CLC - Clear Carry Flag

	Set the carry flag to zero.
*/
void CPU::doCLC()
{
	setFlag(C, false);
}


/*	
	CLD - Clear Decimal Mode

	Set the decimal mode flag to zero.

	NB: The state of the decimal flag is uncertain when the CPU is powered up and it is not reset 
	when an interrupt is generated. In both cases you should include an explicit CLD to ensure that 
	the flag is cleared before performing addition or subtraction.
*/
void CPU::doCLD()
{
	setFlag(D, false);
}


/*	
	CLI - Clear Interrupt Disable

	Clears the interrupt disable flag allowing normal interrupt requests to be serviced.
*/
void CPU::doCLI()
{
	setFlag(I, false);
}


/*	
	CLV - Clear Overflow Flag

	Clears the overflow flag.
*/
void CPU::doCLV()
{
	setFlag(V, false);
}


/*	
	CMP - Compare

	This instruction compares the contents of the accumulator with another 
	memory held value and sets the zero and carry flags as appropriate.
*/
void CPU::doCMP(byte& data)
{
	if (reg_accumulator >= data)
	{
		setFlag(C, true);
	}

	if (reg_accumulator == data)
	{
		setFlag(Z, true);
	}
	
	byte result = subtractBytes(reg_accumulator, data, false);
	if ((result & 0b1000'0000) != 0)
	{
		setFlag(N, true);
	}
}


/*	
	CPX - Compare X Register

	This instruction compares the contents of the X register with another
	memory held value and sets the zero and carry flags as appropriate.
*/
void CPU::doCMX(byte& data)
{
	if (reg_x >= data)
	{
		setFlag(C, true);
	}

	if (reg_x == data)
	{
		setFlag(Z, true);
	}

	byte result = subtractBytes(reg_x, data, false);
	if ((result & 0b1000'0000) != 0)
	{
		setFlag(N, true);
	}
}


/*	
	CPY - Compare Y Register

	This instruction compares the contents of the Y register with another
	memory held value and sets the zero and carry flags as appropriate.
*/
void CPU::doCMY(byte& data)
{
	if (reg_y >= data)
	{
		setFlag(C, true);
	}

	if (reg_y == data)
	{
		setFlag(Z, true);
	}

	byte result = subtractBytes(reg_y, data, false);
	if ((result & 0b1000'0000) != 0)
	{
		setFlag(N, true);
	}
}


/*	
	DEC - Decrement Memory

	Subtracts one from the value held at a specified memory location,
	setting the zero and negative flags as appropriate.
*/
void CPU::doDEC(byte& data)
{
	data = subtractBytes(data, 0b0000'0001, false);
	setZeroAndNegativeFlags(data);
}


/*	
	DEX - Decrement Memory

	Subtracts one from the X register,
	setting the zero and negative flags as appropriate.
*/
void CPU::doDEX()
{
	reg_x = subtractBytes(reg_x, 0b0000'0001, false);
	setZeroAndNegativeFlags(reg_x);
}


/*	
	DEY - Decrement Memory

	Subtracts one from the Y register,
	setting the zero and negative flags as appropriate.
*/
void CPU::doDEY()
{
	reg_y = subtractBytes(reg_y, 0b0000'0001, false);
	setZeroAndNegativeFlags(reg_y);
}


/*	
	EOR - Exclusive OR

	An exclusive OR is performed, bit by bit, on the 
	accumulator contents using the contents of a byte of memory.
*/
void CPU::doEOR(byte& data)
{
	reg_accumulator = reg_accumulator ^ data;
	setZeroAndNegativeFlags(reg_accumulator);
}


/*	
	INC - Increment Memory

	Adds one to the value held at a specified memory location,
	setting the zero and negative flags as appropriate.
*/
void CPU::doINC(byte& data)
{
	data = addBytes(data, 0b0000'0001, false);
	setZeroAndNegativeFlags(data);
}


/*	
	INX - Increment X Register

	Adds one to the X register,
	setting the zero and negative flags as appropriate.
*/
void CPU::doINX()
{
	reg_x = addBytes(reg_x, 0b0000'0001, false);
	setZeroAndNegativeFlags(reg_x);
}


/*	
	INC - Increment Memory

	Adds one to the Y register,
	setting the zero and negative flags as appropriate.
*/
void CPU::doINY()
{
	reg_y = addBytes(reg_y, 0b0000'0001, false);
	setZeroAndNegativeFlags(reg_y);
}


/*
	JMP - Jump

	Sets the program counter to the address specified by the operand.
*/
void CPU::doJMP(word address)
{
	program_counter = address;
}


/*
	LDA - Load Accumulator

	Loads a byte of memory into the accumulator,
	setting the zero and negative flags as appropriate.
*/
void CPU::doLDA(byte& data)
{
	reg_accumulator = data;
	setZeroAndNegativeFlags(data);
}


/*
	LDX - Load X Register

	Loads a byte of memory into the X register,
	setting the zero and negative flags as appropriate.
*/
void CPU::doLDX(byte& data)
{
	reg_x = data;
	setZeroAndNegativeFlags(data);
}


/*
	LDY - Load Y Register

	Loads a byte of memory into the Y register,
	setting the zero and negative flags as appropriate.
*/
void CPU::doLDY(byte& data)
{
	reg_y = data;
	setZeroAndNegativeFlags(data);
}



#pragma endregion


/*

	GET DATA FROM MEMORY

 ______________________________________________________
|  __________________________________________________  |
| |                 |_______________|                | |
| |       ___       ,---------------.                | |
| |      |   |      |_______________|     emuNES     | |
| |   ___|   |___   ,---------------.                | |
| |  |    ,-.    |  |SELECT    START|   ___B  ___A   | |
| |  |___ `-' ___|  :===============:  |,-.| |,-.|   | |
| |      |   |      |  ===     ===  |  |._,| |._,|   | |
| |      |___|      :===============:  '---' '---'   | |
| |_________________|_______________|________________| |
|______________________________________________________|


*/
#pragma region GET_DATA


byte& CPU::get_Immediate()
{
	return memory[getAddr_Immediate()];
}


byte& CPU::get_ZeroPage()
{
	return memory[getAddr_ZeroPage()];
}


byte& CPU::get_ZeroPageX()
{
	return memory[getAddr_ZeroPageX()];
}


byte& CPU::get_ZeroPageY()
{
	return memory[getAddr_ZeroPageY()];
}


byte& CPU::get_Absolute()
{
	return memory[getAddr_Absolute()];
}


byte& CPU::get_AbsoluteX()
{
	return memory[getAddr_AbsoluteX()];
}


byte& CPU::get_AbsoluteY()
{
	return memory[getAddr_AbsoluteY()];
}


byte& CPU::get_IndirectX()
{
	return memory[getAddr_IndirectX()];
}


byte& CPU::get_IndirectY()
{
	return memory[getAddr_IndirectY()];
}


byte& CPU::get_Indirect()
{
	return memory[getAddr_Indirect()];
}

byte& CPU::get_Accumulator()
{
	return reg_accumulator;
}
#pragma endregion


/*

	GET ADDRESSES

 ______________________________________________________
|  __________________________________________________  |
| |                 |_______________|                | |
| |       ___       ,---------------.                | |
| |      |   |      |_______________|     emuNES     | |
| |   ___|   |___   ,---------------.                | |
| |  |    ,-.    |  |SELECT    START|   ___B  ___A   | |
| |  |___ `-' ___|  :===============:  |,-.| |,-.|   | |
| |      |   |      |  ===     ===  |  |._,| |._,|   | |
| |      |___|      :===============:  '---' '---'   | |
| |_________________|_______________|________________| |
|______________________________________________________|

*/
#pragma region GET_ADDRESS


word CPU::getAddr_Immediate()
{
	return program_counter++;
}


word CPU::getAddr_ZeroPage()
{
	return memory[program_counter++];
}


word CPU::getAddr_ZeroPageX()
{
	return addBytes(memory[program_counter++], reg_x, false);
}


word CPU::getAddr_ZeroPageY()
{
	//return addWords(memory[program_counter++], reg_y); not sure if the NB applies to this as well.
	return addBytes(memory[program_counter++], reg_y, false);
}


word CPU::getAddr_Absolute()
{
	byte lo = memory[program_counter++];
	byte hi = memory[program_counter++];

	return (hi << 8) | lo;
}


word CPU::getAddr_AbsoluteX()
{
	byte lo = memory[program_counter++];
	byte hi = memory[program_counter++];

	word address_before_offset = (hi << 8) | lo;
	return addWords(address_before_offset, reg_x);
}


word CPU::getAddr_AbsoluteY()
{
	byte lo = memory[program_counter++];
	byte hi = memory[program_counter++];

	word address_before_offset = (hi << 8) | lo;
	return addWords(address_before_offset, reg_y);
}


word CPU::getAddr_IndirectX()
{
	byte table_address{ memory[program_counter++] };
	return addBytes(table_address, reg_x, false);
}


word CPU::getAddr_IndirectY()
{
	byte lo_idx{ memory[program_counter++] };
	byte lo{ memory[lo_idx] };

	return addWords(reg_y, lo);
}


word CPU::getAddr_Indirect()
{
	byte lo = memory[program_counter++];
	byte hi = memory[program_counter++];

	word indirect_address = (hi << 8) | lo;

	byte i_lo = memory[indirect_address++];

	// Per nesdev.org (https://www.nesdev.org/obelisk-6502-guide/reference.html#JMP):
	/*
		An original 6502 has does not correctly fetch the target address 
		if the indirect vector falls on a page boundary (e.g. $xxFF where xx 
		is any value from $00 to $FF). In this case fetches the LSB from $xxFF 
		as expected but takes the MSB from $xx00. This is fixed in some later 
		chips like the 65SC02 so for compatibility always ensure the indirect 
		vector is not at the end of the page.
	*/
#ifdef 65SC02
	// Correctly fetches MSB.
	byte i_hi = memory[indirect_address]; 
#else
	// Increments only the lo without regard to the whole indirect address value.
	// Thus an overflow does not carry to the hi.
	// (e.g., hi = 0x32, lo = 0xFF; lo + 1 = 0x00; (hi << 8) | lo = 0x3200 (!0x3300))
	word incorrect_indirect_address = (hi << 8) | addBytes(lo, 0b1, false);
	byte i_hi = memory[incorrect_indirect_address];
#endif

	return (i_hi << 8) | i_lo;
}
#pragma endregion


/*

	BINARY ARITHMETIC

 ______________________________________________________
|  __________________________________________________  |
| |                 |_______________|                | |
| |       ___       ,---------------.                | |
| |      |   |      |_______________|     emuNES     | |
| |   ___|   |___   ,---------------.                | |
| |  |    ,-.    |  |SELECT    START|   ___B  ___A   | |
| |  |___ `-' ___|  :===============:  |,-.| |,-.|   | |
| |      |   |      |  ===     ===  |  |._,| |._,|   | |
| |      |___|      :===============:  '---' '---'   | |
| |_________________|_______________|________________| |
|______________________________________________________|

*/
#pragma region BINARY_MATH


byte CPU::addBytes(byte a, byte b, bool use_flags)
{
	byte carry{ (use_flags ? static_cast<byte>(processor_status & (1 << C)) : static_cast<byte>(0)) };
	byte result{ 0x00 };

	for (size_t i{ 0 }; i < 8; ++i)
	{
		byte x{ static_cast<byte>((a & (0b01 << i)) >> i) };
		byte y{ static_cast<byte>((b & (0b01 << i)) >> i) };

		// Full Adder.
		byte sum = carry ^ (x ^ y);
		carry = (x & y) | (y & carry) | (x & carry);
		result |= (sum << i);
	}

	std::cout << "\n\nAdd bytes:\n";
	std::cout << std::bitset<8>(a) << '\n';
	std::cout << std::bitset<8>(b) << '\n';
	std::cout << std::bitset<8>(result) << '\n';

	// Set flags.
	if (use_flags)
	{
		// Carry flag - set to the carry bit.
		setFlag(C, carry > 0);
	
		// Overflow flag - checks if MSB of starting values is different from result.
		bool had_overflow{ (a ^ result) & (b ^ result) & 0b1000'000 };
		setFlag(V, had_overflow);
		
		std::cout << "Carry: " << carry << " | Overflow? " << had_overflow << std::endl;
	}

	return result;
}


word CPU::addWords(word a, word b)
{
	byte carry{ 0x00 };
	word result{ 0x0000 };

	for (size_t i{ 0 }; i < 16; ++i)
	{
		byte x{ static_cast<byte>((a & (0b01 << i)) >> i) };
		byte y{ static_cast<byte>((b & (0b01 << i)) >> i) };

		// Full Adder.
		byte sum = carry ^ (x ^ y);
		carry = (x & y) | (y & carry) | (x & carry);
		result |= (sum << i);
	}

	std::cout << "Add word/word: (bits) " << std::bitset<16>(result) << " | (hex) " << std::hex << (int)result << std::endl;

	return result;
}


word CPU::addWords(word a, byte b) { return addWords(a, static_cast<word>(b)); }
word CPU::addWords(byte a, word b) { return addWords(static_cast<word>(a), b); }
word CPU::addWords(byte a, byte b) { return addWords(static_cast<word>(a), static_cast<word>(b)); }


byte CPU::subtractBytes(byte minuend, byte subtrahend, bool use_flags)
{
	byte initial_subtrahend{ subtrahend };

	// Flip sign of subtrahend so that binary addition can be used.
	// (e.g., minuend - subtrahend => minuend + -subtrahend).
	// (e.g., minuend - -subtrahend => minuend + subtrahend).
	// Find 2's complement if negative; Restore to positive if negative.
	if (subtrahend & 0b1000'0000)
	{
		// Is negative. Make positive (undo 2's complement).
		// Subtract 1 and get 1's complement.		
		subtrahend = ~(subtrahend - 0b1);
	}
	else
	{
		// Is positive. Make negative (use 2's complement).
		// Get 1's complement and add 1.
		subtrahend = (~subtrahend) + 0b1;
	}

	byte carry{ (use_flags ? static_cast<byte>(processor_status & (1 << C)) : static_cast<byte>(0x00)) };
	byte result{ 0x0000 };

	for (size_t i{ 0 }; i < 8; ++i)
	{
		byte x{ static_cast<byte>((minuend & (0b01 << i)) >> i) };
		byte y{ static_cast<byte>((subtrahend & (0b01 << i)) >> i) };

		// Full Adder.
		byte sum = carry ^ (x ^ y);
		carry = (x & y) | (y & carry) | (x & carry);
		result |= (sum << i);
	}

	std::cout << "\n\nSubtract bytes:\n";
	std::cout << std::bitset<8>(minuend) << '\n';
	std::cout << std::bitset<8>(initial_subtrahend) << " => " << std::bitset<8>(subtrahend) << '\n';
	std::cout << std::bitset<8>(result) << '\n';

	// Set flags.
	if (use_flags)
	{
		// Carry flag - set to the carry bit.
		setFlag(C, carry > 0);

		// Overflow flag - checks if MSB of starting values is different from result.
		//bool had_overflow{ (a ^ result) & (b ^ result) & 0b1000'000 };
		//setFlag(V, had_overflow);

		//std::cout << "Carry: " << carry << " | Overflow? " << had_overflow << std::endl;
	}

	return result;
}


word CPU::subtractWords(word minuend, word subtrahend)
{
	// Flip subtrahend so that (minuend - subtrahend) => (minuend + (-subtrahend)).
	// Find 2's complement if negative; Restore to positive if negative.
	if (subtrahend & 0b1000'0000)
	{
		// Is negative. Make positive (undo 2's complement).
		// Subtract 1 and get 1's complement.		
		subtrahend = ~(subtrahend - 0b1);
	}
	else
	{
		// Is positive. Make negative (use 2's complement).
		// Get 1's complement and add 1.
		subtrahend = (~subtrahend) + 0b1;		
	}

	byte carry{ 0x00 };
	word result{ 0x0000 };

	for (size_t i{ 0 }; i < 16; ++i)
	{
		byte x{ static_cast<byte>((minuend & (0b01 << i)) >> i) };
		byte y{ static_cast<byte>((subtrahend & (0b01 << i)) >> i) };

		// Full Adder.
		byte sum = carry ^ (x ^ y);
		carry = (x & y) | (y & carry) | (x & carry);
		result |= (sum << i);
	}

	std::cout << "Subtract word/word: (bits) " << std::bitset<16>(result) << " | (hex) " << std::hex << (int)result << std::endl;

	return result;
}


word CPU::subtractWords(word a, byte b) { return subtractWords(a, static_cast<word>(b)); }
word CPU::subtractWords(byte a, word b) { return subtractWords(static_cast<word>(a), b); }
word CPU::subtractWords(byte a, byte b) { return subtractWords(static_cast<word>(a), static_cast<word>(b)); }
#pragma endregion


/*

	PROCESSOR STATUS (FLAGS) 

 ______________________________________________________
|  __________________________________________________  |
| |                 |_______________|                | |
| |       ___       ,---------------.                | |
| |      |   |      |_______________|     emuNES     | |
| |   ___|   |___   ,---------------.                | |
| |  |    ,-.    |  |SELECT    START|   ___B  ___A   | |
| |  |___ `-' ___|  :===============:  |,-.| |,-.|   | |
| |      |   |      |  ===     ===  |  |._,| |._,|   | |
| |      |___|      :===============:  '---' '---'   | |
| |_________________|_______________|________________| |
|______________________________________________________|

*/
#pragma region PROCESSOR_FLAGS


void CPU::setZeroAndNegativeFlags(byte data)
{
	//setFlag(Z, data == 0);
	//setFlag(N, data & 0b1000'0000);

	if (data == 0) 
	{
		setFlag(Z, true);
	}

	if ((data & 0b1000'0000) != 0) 
	{
		setFlag(N, true);
	}
}


void CPU::setFlag(byte flag_idx, bool value)
{
	processor_status = (value ? processor_status | (1 << flag_idx) : processor_status & ~(1 << flag_idx));
}


bool CPU::isFlagSet(byte flag_idx) const
{
	return ((processor_status & (1 << flag_idx)) != 0);
}
#pragma endregion