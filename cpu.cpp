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
			// INC
			case 0xe6:
			case 0xf6:
			case 0xee:
			case 0xff:
			{
				byte data{ getDataForINC(opcode) };
				setZeroAndNegativeFlags(data);
				break;
			}

			// INX
			case 0xe8:
			{
				byte data{ ++reg_x };
				setZeroAndNegativeFlags(data);
				break;
			}

			// INY
			case 0xc8:
			{
				byte data{ ++reg_y };
				setZeroAndNegativeFlags(data);
				break;
			}
			
			// JMP
			case 0x4c:
			case 0x6c:
			{
				program_counter = getDataForJMP(opcode);
				break;
			}

			// LDA
			case 0xa9:
			case 0xa5:
			case 0xb5:
			case 0xad:
			case 0xbd:
			case 0xb9:
			case 0xa1:
			case 0xb1:
			{
				byte data{ getDataForLDA(opcode) };
				reg_accumulator = data;
				setZeroAndNegativeFlags(data);

				break;
			}

		}
	}
}

word CPU::getDataForJMP(const byte opcode)
{
	switch (opcode)
	{
		case 0x4c: return getAddr_Absolute();
		case 0x6c: return getAddr_Indirect();
	}
}

byte CPU::getDataForINC(const byte opcode)
{
	switch (opcode)
	{
		case 0xe6: return ++memory[getAddr_ZeroPage()];
		case 0xf6: return ++memory[getAddr_ZeroPageX()];
		case 0xee: return ++memory[getAddr_Absolute()];
		case 0xff: return ++memory[getAddr_AbsoluteX()];
	}
}


byte CPU::getDataForLDA(const byte opcode)
{
	switch (opcode)
	{
		case 0xa9: return getAddr_Immediate();
		case 0xa5: return memory[getAddr_ZeroPage()];
		case 0xb5: return memory[getAddr_ZeroPageX()];
		case 0xad: return memory[getAddr_Absolute()];
		case 0xbd: return memory[getAddr_AbsoluteX()];
		case 0xb9: return memory[getAddr_AbsoluteY()];
		case 0xa1: return memory[getAddr_IndirectX()];
		case 0xb1: return memory[getAddr_IndirectY()];
	}
}


byte CPU::getAddr_Immediate()
{
	return memory[program_counter++];
}


byte CPU::getAddr_ZeroPage()
{
	return memory[program_counter++];
}


byte CPU::getAddr_ZeroPageX()
{
	return getSumAsByte(memory[program_counter++], reg_x, false);
}


word CPU::getAddr_ZeroPageY()
{
	return getSumAsWord(memory[program_counter++], reg_y);
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
	return getSumAsWord(address_before_offset, reg_x);
}


word CPU::getAddr_AbsoluteY()
{
	byte lo = memory[program_counter++];
	byte hi = memory[program_counter++];

	word address_before_offset = (hi << 8) | lo;
	return getSumAsWord(address_before_offset, reg_y);
}


word CPU::getAddr_IndirectX()
{
	byte table_address{ memory[program_counter++] };
	return getSumAsByte(table_address, reg_x, false);
}


word CPU::getAddr_IndirectY()
{
	byte lo_idx{ memory[program_counter++] };
	byte lo{ memory[lo_idx] };

	return getSumAsWord(reg_y, lo);
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
	word incorrect_indirect_address = (hi << 8) | getSumAsByte(lo, 0b1, false);
	byte i_hi = memory[incorrect_indirect_address];
#endif

	return (i_hi << 8) | i_lo;
}

/*
	Binary arithmetic
*/

byte CPU::getSumAsByte(byte a, byte b, bool use_carry_flag)
{
	byte carry{ (use_carry_flag ? static_cast<byte>(processor_status & (1 << C)) : static_cast<byte>(0)) };
	byte result{ 0x00 };

	for (size_t i{ 0 }; i < 8; ++i)
	{
		byte x{ static_cast<byte>((a & (0b01 << i)) >> i) };
		byte y{ static_cast<byte>((b & (0b01 << i)) >> i) };

		// Full Adder.
		byte sum = carry ^ (x ^ y);
		carry = (x & y) + (y & carry) + (x & carry);
		result |= (sum << i);

		// Set overflow.
		if (i == 7 && carry)
		{
			setFlag(V, true);
		}
	}

	// Set carry flag.
	if (use_carry_flag)
	{
		setFlag(C, carry > 0);
	}

	std::cout << "Add bytes: (bits) " << std::bitset<8>(result) << " | (hex) " << std::hex << (int)result << std::endl;

	return result;
}


word CPU::getSumAsWord(word a, word b)
{
	byte carry{ 0 };
	word result{ 0x00 };

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

word CPU::getSumAsWord(word a, byte b) {	return getSumAsWord(a, static_cast<word>(b)); }
word CPU::getSumAsWord(byte a, word b) {	return getSumAsWord(static_cast<word>(a), b); }
word CPU::getSumAsWord(byte a, byte b) {	return getSumAsWord(static_cast<word>(a), static_cast<word>(b)); }


void CPU::setZeroAndNegativeFlags(byte data)
{
	setFlag(Z, data == 0);
	setFlag(N, data & 0b1000'0000);
}

void CPU::setFlag(byte flag_idx, bool value)
{
	processor_status = (value ? processor_status | (1 << flag_idx) : processor_status & ~(1 << flag_idx));
}