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

private:
	/// <summary>
	/// Bitwise addition returning a byte. Overflow does not create a word!
	/// (i.e., 0xFF + 0x02 = 0x01)
	/// </summary>
	/// <param name="a">First addend.</param>
	/// <param name="b">Second addend.</param>
	/// <param name="use_carry">
	/// Should method use flags? 
	/// If true, the carry flag will be used to get and store the C-IN and C-OUT values; 
	/// and the overflow flag will be set at the end of calculation if overflow occurred.
	/// </param>
	/// <returns>Sum of binary addition.</returns>
	byte addBytes(byte a, byte b, bool use_carry_flag);

	/// <summary>
	/// Bitwise addtion returning a word.
	/// </summary>
	/// <param name="a">First addend.</param>
	/// <param name="b">Second addend.</param>
	/// <returns>Sum of binary addition.</returns>
	word addWords(word a, word b);
	word addWords(word a, byte b);
	word addWords(byte a, word b);
	word addWords(byte a, byte b);

	/// <summary>
	/// Bitwise subtraction returning a byte.
	/// </summary>
	/// <param name="a">Minuend.</param>
	/// <param name="b">Subtrahend.</param>
	/// <param name="use_carry">
	/// Should method use flags?
	/// </param>
	/// <returns>Difference of binary subtraction.</returns>
	byte subtractBytes(byte a, byte b, bool use_carry_flag);

	/// <summary>
	/// Bitwise subtraction returning a word.
	/// </summary>
	/// <param name="a">Minuend.</param>
	/// <param name="b">Subtrahend.</param>
	/// <returns>Difference of binary subtraction.</returns>
	word subtractWords(word a, word b);
	word subtractWords(word a, byte b);
	word subtractWords(byte a, word b);
	word subtractWords(byte a, byte b);

	// Processor status bit indices.
	enum : byte {
		C = 0,
		Z = 1,
		I = 2,
		D = 3,
		B = 4,
		// skip
		V = 6,
		N = 7,
	};

	/// <summary>
	/// Toggles given flag index on/off according to value.
	/// </summary>
	/// <param name="flag">Bitflag index to toggle.</param>
	/// <param name="value">Should flag be turned on or off?</param>
	void setFlag(byte flag_idx, bool value);
	void setZeroAndNegativeFlags(byte data);
	bool isFlagSet(byte flag_idx) const;


	/// <summary>
	/// All instructions.
	/// </summary>
	/// <param name="data">
	/// The data from memory to act upon.
	/// Parameter is omitted if instruction is implied.
	/// </param>
	void doADC(byte& data);
	void doAND(byte& data);
	void doASL(byte& data);
	void doBCC(byte& data);
	void doBCS(byte& data);
	void doBEQ(byte& data);
	void doBIT(byte& data);
	void doBMI(byte& data);
	void doBNE(byte& data);
	void doBPL(byte& data);
	void doBRK();
	void doBVC(byte& data);
	void doBVS(byte& data);
	void doCLC();
	void doCLD();
	void doCLI();
	void doCLV();
	void doCMP(byte& data);
	void doCMX(byte& data);
	void doCMY(byte& data);
	void doDEC(byte& data);
	void doDEX();
	void doDEY();
	void doEOR(byte& data);
	void doINC(byte& data);
	void doINX();
	void doINY();
	void doJMP(word address);
	void doLDA(byte& data);
	void doLDX(byte& data);
	void doLDY(byte& data);
	void doLSR(byte& data);
	void doORA(byte& data);
	// TODO: PHA
	// TODO: PHP
	// TODO: PLA
	// TODO: PLP	
	void doROL(byte& data);
	void doROR(byte& data);
	// TODO: RTI
	// TODO: RTS
	void doSBC(byte& data);





	/// <summary>
	/// Addressing Modes. Locates the address of where the wanted data is.
	/// </summary>	
	/// <returns>Address of data in memory.</returns>
	word getAddr_Immediate();
	word getAddr_ZeroPage();
	word getAddr_ZeroPageX();
	word getAddr_ZeroPageY();
	word getAddr_Absolute();
	word getAddr_AbsoluteX();
	word getAddr_AbsoluteY();
	word getAddr_IndirectX();
	word getAddr_IndirectY();
	word getAddr_Indirect();


	/// <summary>
	/// Addressing Mode helpers. 
	/// Fetches the data from memory to simplify instuction code.
	/// </summary>
	/// <returns>Reference to the data in memory.</returns>
	byte& get_Immediate();
	byte& get_ZeroPage();
	byte& get_ZeroPageX();
	byte& get_ZeroPageY();
	byte& get_Absolute();
	byte& get_AbsoluteX();
	byte& get_AbsoluteY();
	byte& get_IndirectX();
	byte& get_IndirectY();
	byte& get_Indirect();
	byte& get_Accumulator();


	// Properties.
	word program_counter{ 0x00 };
	byte stack_pointer{ 0x00 };
	byte reg_accumulator{ 0x00 };
	byte reg_x{ 0x00 };
	byte reg_y{ 0x00 };
	byte processor_status{ 0b0000'0000 };

	std::vector<byte> memory;
};