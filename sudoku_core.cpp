#include "sudoku_core.h"

void sudoku_core::load_puzzle(DIFFICULTY difficulty) {

}

bool sudoku_core::is_vaild(uint8_t row, uint8_t col) const {
	if (row > 9 || col > 9) {
		return false;
	}
	else {
		return true;
	}
}

uint8_t sudoku_core::get_number(uint8_t row, uint8_t col) const {
	if (!is_vaild(row, col)) {
		return 0;
	}
	else {
		return board[row][col].select;
	}
}

bool sudoku_core::set_number(uint8_t row, uint8_t col, uint8_t number, bool is_preselection) {
	if (!is_vaild(row, col) || !board[row][col].variability) {
		return false;
	}
	else {
		if (is_preselection) {
			board[row][col].preselect = number;
		}
		else {
			board[row][col].select = number;
		}
		return true;
	}
}

bool sudoku_core::erase_number(uint8_t row, uint8_t col, bool is_preselection) {
	return set_number(row, col, 0, is_preselection);
}

void sudoku_core::reset() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			erase_number(i, j, true);
			erase_number(i, j, false);
		}
	}
}

bool sudoku_core::is_complete() const {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (board[i][j].variability) {
				if (solution[i][j] != board[i][j].select) {
					return false;
				}
			}
		}
	}
	return true;
}