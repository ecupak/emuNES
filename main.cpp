#include <iostream>

#include <vector>
#include <array>
#include <bitset>

using byte = uint8_t;
using word = uint16_t;

struct CPU
{
public:
	CPU()
	{
		memory.resize(0xFFFF);
	}

	word program_counter{ 0x00 };
	byte stack_pointer{ 0x00 };
	byte reg_accumulator{ 0x00 };
	byte reg_x{ 0x00 };
	byte reg_y{ 0x00 };
	byte processor_status{ 0b0000'0000 };

	// RAM.
	std::vector<byte> memory;
	
	void loadROM(std::vector<byte>&rom)
	{
		std::copy(rom.begin(), rom.end(), memory.begin() + 0x8000);
	}

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

	void setZeroAndNegativeFlags(byte data)
	{
		setFlag(Z, data == 0);
		setFlag(N, data & 0b1000'0000);
	}
	
	// Bitwise addition with carry.
	byte addGetByte(byte a, byte b, bool use_carry)
	{		
		byte carry{ (use_carry ? static_cast<byte>(processor_status & (1 << C)) : static_cast<byte>(0)) };
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
		setFlag(C, carry > 0);

		std::cout << "Add bytes: (bits) " << std::bitset<8>(result) << " | (hex) " << std::hex << (int)result <<std::endl;

		return result;
	}

	word addGetWord(word a, byte b)
	{
		word c{ static_cast<word>(b) };

		byte carry{ 0 };
		word result{ 0x00 };

		for (size_t i{ 0 }; i < 16; ++i)
		{
			byte x{ static_cast<byte>((a & (0b01 << i)) >> i) };
			byte y{ static_cast<byte>((c & (0b01 << i)) >> i) };

			// Full Adder.
			byte sum = carry ^ (x ^ y);
			carry = (x & y) + (y & carry) + (x & carry);
			result |= (sum << i);
		}

		std::cout << "Add word/byte: (bits) " << std::bitset<16>(result) << " | (hex) " << std::hex << (int)result << std::endl;

		return result;
	}

	word addGetWord(byte a, byte b)
	{
		word c{ static_cast<word>(a) };
		word d{ static_cast<word>(b) };

		byte carry{ 0 };
		word result{ 0x00 };

		for (size_t i{ 0 }; i < 16; ++i)
		{
			byte x{ static_cast<byte>((c & (0b01 << i)) >> i) };
			byte y{ static_cast<byte>((d & (0b01 << i)) >> i) };

			// Full Adder.
			byte sum = carry ^ (x ^ y);
			carry = (x & y) + (y & carry) + (x & carry);
			result |= (sum << i);
		}

		std::cout << "Add words: (bits) " << std::bitset<16>(result) << " | (hex) " << std::hex << (int)result << std::endl;

		return result;
	}

private:
	void setFlag(byte flag, bool value)
	{
		processor_status = (value ? processor_status | (1 << flag) : processor_status & ~(1 << flag));
	}
};

int main()
{
	// Create virtual hardware.
	CPU cpu;	

	// Load rom.
	std::vector<byte> rom{
		0xb1, 0x0C, 0x00
	};

	cpu.loadROM(rom);
	
	// Emulate!
	while (cpu.program_counter < rom.size())
	{
		// fetch();
		byte opcode = rom[cpu.program_counter];
		++cpu.program_counter;


		// decode();
		switch (opcode)
		{
			// ACD - Add with Carry.
			case 0x69:
			{

			}

#pragma region LDA
			// LDA - Load data to Register A.
			case 0xa9: 
			{
				// Immediate.
				byte data = rom[cpu.program_counter];
				++cpu.program_counter;

				cpu.reg_accumulator = data;
				cpu.setZeroAndNegativeFlags(data);

				break;
			}

			case 0xa5:
			{
				// Zero Page.
				byte data = cpu.memory[rom[cpu.program_counter]];
				++cpu.program_counter;

				cpu.reg_accumulator = data;
				cpu.setZeroAndNegativeFlags(data);

				break;
			}

			case 0xb5:
			{
				// Zero Page, X.
				byte data = cpu.memory[cpu.addGetByte(rom[cpu.program_counter], cpu.reg_x, false)];
				++cpu.program_counter;

				cpu.reg_accumulator = data;
				cpu.setZeroAndNegativeFlags(data);

				break;
			}

			case 0xad:
			{
				// Absolute.
				byte lo = rom[cpu.program_counter];
				++cpu.program_counter;

				byte hi = rom[cpu.program_counter];
				++cpu.program_counter;

				word address = (hi << 8) | lo;

				byte data = cpu.memory[address];

				cpu.reg_accumulator = data;
				cpu.setZeroAndNegativeFlags(data);

				break;
			}

			case 0xbd:
			{
				std::cout << "LDA: Absolute, X" << std::endl;
				// Absolute, X.
				byte lo = rom[cpu.program_counter];
				++cpu.program_counter;

				byte hi = rom[cpu.program_counter];
				++cpu.program_counter;

				word address = (hi << 8) | lo;
				byte offset_address = cpu.memory[cpu.addGetWord(address, cpu.reg_x)];

				byte data = cpu.memory[offset_address];

				cpu.reg_accumulator = data;
				cpu.setZeroAndNegativeFlags(data);

				break;
			}

			case 0xb9:
			{
				std::cout << "LDA: Absolute, Y" << std::endl;
				// Absolute, X.
				byte lo = rom[cpu.program_counter];
				++cpu.program_counter;

				byte hi = rom[cpu.program_counter];
				++cpu.program_counter;

				word address = (hi << 8) | lo;
				byte offset_address = cpu.memory[cpu.addGetWord(address, cpu.reg_y)];

				byte data = cpu.memory[offset_address];

				cpu.reg_accumulator = data;
				cpu.setZeroAndNegativeFlags(data);

				break;
			}

			case 0xa1:
			{
				std::cout << "LDA: Indirect, X" << std::endl;
				// Indirect, X.
				byte table_idx = rom[cpu.program_counter];
				++cpu.program_counter;
				
				byte address = cpu.addGetByte(table_idx, cpu.reg_x, false);

				byte data = cpu.memory[address];

				cpu.reg_accumulator = data;
				cpu.setZeroAndNegativeFlags(data);

				break;
			}

			case 0xb1:
			{
				std::cout << "LDA: Indirect, Y" << std::endl;
				// Indirect, Y.
				byte lo_idx = rom[cpu.program_counter];
				++cpu.program_counter;

				byte lo = cpu.memory[lo_idx];

				word address = cpu.addGetWord(cpu.reg_y, lo);

				byte data = cpu.memory[address];

				cpu.reg_accumulator = data;
				cpu.setZeroAndNegativeFlags(data);

				break;
			}
#pragma endregion LDA
#pragma region LDX
			// LDX - Load data to Register X.
			case 0xa2:
			{
				std::cout << "LDX: Immediate" << std::endl;
				// Immediate.
				byte data = rom[cpu.program_counter];
				++cpu.program_counter;

				cpu.reg_x = data;
				cpu.setZeroAndNegativeFlags(data);

				break;
			}

#pragma endregion LDX
#pragma region BRK
			case 0x00:
			{
				std::cout << "Terminated" << std::endl;
				break;
			}

		}
#pragma endregion BRK


		// execute();

	}// end while loop.
}