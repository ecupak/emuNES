#include "cpu.hpp"

int main()
{
	// Create virtual hardware.
	CPU cpu;	

	// Load rom.
	{
		std::vector<byte> rom{
			0xb1, 0x0C, 0x00
		};

		cpu.loadROM(rom);
	}
	
	// Emulate!
	cpu.run();
}