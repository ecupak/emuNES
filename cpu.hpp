#pragma once

#include <iostream>

#include <vector>
#include <array>
#include <bitset>

using byte = uint8_t;
using word = uint16_t;


class CPU
{
public:
	CPU();

	void reset();	

	void loadROM(std::vector<byte>& rom);	

	void run();

	void setZeroAndNegativeFlags(byte data);

	/// <summary>
	/// Bitwise addition returning a byte. Overflow does not create a word!
	/// (i.e., 0xFF + 0x02 = 0x01)
	/// </summary>
	/// <param name="a">First addend.</param>
	/// <param name="b">Second addend.</param>
	/// <param name="use_carry">Should method use and/or alter the carry flag?</param>
	/// <returns>Sum of binary addition.</returns>
	byte getSumAsByte(byte a, byte b, bool use_carry_flag);

	/// <summary>
	/// Bitwise addtion returning a word.
	/// </summary>
	/// <param name="a">First addend.</param>
	/// <param name="b">Second addend.</param>
	/// <returns>Sum of binary addition.</returns>
	word getSumAsWord(word a, word b);
	word getSumAsWord(word a, byte b);
	word getSumAsWord(byte a, word b);
	word getSumAsWord(byte a, byte b);

private:
	// Processor status bit indices.
	enum {
		C = 0,
		Z = 1,
		I = 2,
		B = 3,
		// skip
		V = 5,
		N = 6,
	};

	/// <summary>
	/// Toggles given flag index on/off according to value.
	/// </summary>
	/// <param name="flag">Bitflag index to toggle.</param>
	/// <param name="value">Should flag be turned on or off?</param>
	void setFlag(byte flag_idx, bool value);	
	
	word getDataForJMP(const byte opcode);
	byte getDataForINC(const byte opcode);
	byte getDataForLDA(const byte opcode);


	/// <summary>
	/// Addressing Modes.
	/// </summary>
	/// <param name="cpu"></param>
	/// <param name="register_destination"></param>
	byte getAddr_Immediate();
	byte getAddr_ZeroPage();
	byte getAddr_ZeroPageX();
	word getAddr_ZeroPageY();
	word getAddr_Absolute();
	word getAddr_AbsoluteX();
	word getAddr_AbsoluteY();
	word getAddr_IndirectX();
	word getAddr_IndirectY();
	word getAddr_Indirect();

	// Properties.
	word program_counter{ 0x00 };
	byte stack_pointer{ 0x00 };
	byte reg_accumulator{ 0x00 };
	byte reg_x{ 0x00 };
	byte reg_y{ 0x00 };
	byte processor_status{ 0b0000'0000 };

	std::vector<byte> memory;
};