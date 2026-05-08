#pragma once
#include <array>
#include <map>

struct block {
	uint8_t select, preselect;
	bool variability;
};

enum DIFFICULTY {EASY, NORMAL, HARD, EXPERT};

class sudoku_core
{
private:
	std::array<std::array<uint8_t, 9>, 9> solution;
	std::array<std::array<block, 9>, 9> board;
	bool is_vaild(uint8_t row, uint8_t col) const;
public:
	void load_puzzle(DIFFICULTY difficulty);
	uint8_t get_number(uint8_t row, uint8_t col) const;
	bool set_number(uint8_t row, uint8_t col, uint8_t number, bool is_preselection);
	bool erase_number(uint8_t row, uint8_t col, bool is_preselection);
	void reset();
	bool is_complete() const;
};
