#include "sudoku_core.h"
#include <cstring>
#include <random>
#include <algorithm>

// 位运算适配与 C++ <random> 随机库提升性能
void SudokuCore::shuffleArray(uint8_t arr[], uint8_t size) {
    static std::mt19937 rng(std::random_device{}());
    std::shuffle(arr, arr + size, rng);
}

// ... isValidPlacement 保持原样 (把 board[r][c].value 替换即可) ...
bool SudokuCore::isValidPlacement(int8_t row, int8_t col, uint8_t num) const {
    if (num == 0) return true;
    for (uint8_t i = 0; i < 9; ++i) {
        if (i != col && board[row][i].value == num) return false;
        if (i != row && board[i][col].value == num) return false;
    }
    int8_t box_row = (row / 3) * 3, box_col = (col / 3) * 3;
    for (int8_t i = box_row; i < box_row + 3; ++i)
        for (int8_t j = box_col; j < box_col + 3; ++j)
            if (i != row && j != col && board[i][j].value == num) return false;
    return true;
}

bool SudokuCore::solve(int8_t row, int8_t col) {
    if (row == 9) return true;
    if (col == 9) return solve(row + 1, 0);
    if (board[row][col].value != 0) return solve(row, col + 1);

    uint8_t nums[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    shuffleArray(nums, 9);

    for (uint8_t i = 0; i < 9; ++i) {
        if (isValidPlacement(row, col, nums[i])) {
            board[row][col].value = nums[i];
            if (solve(row, col + 1)) return true;
            board[row][col].value = 0;
        }
    }
    return false;
}

void SudokuCore::generateCompleteBoard() {
    memset(board, 0, sizeof(board));
    solve(0, 0);
    memcpy(solution, board, sizeof(board));
}

void SudokuCore::removeNumbers(uint8_t count) {
    uint8_t removed = 0;
    while (removed < count) {
        uint8_t row = rand() % 9, col = rand() % 9;
        if (board[row][col].value != 0) {
            board[row][col].value = 0;
            board[row][col].setFlag(Cell::GIVEN, false);
            removed++;
        }
    }
    for (uint8_t i = 0; i < 9; ++i)
        for (uint8_t j = 0; j < 9; ++j)
            if (board[i][j].value != 0)
                board[i][j].setFlag(Cell::GIVEN, true);
}

void SudokuCore::loadPuzzle(Difficulty difficulty) {
    generateCompleteBoard();
    removeNumbers(static_cast<uint8_t>(difficulty));
    clearSelection();
}

void SudokuCore::reset() {
    for (uint8_t i = 0; i < 9; ++i) {
        for (uint8_t j = 0; j < 9; ++j) {
            if (!board[i][j].hasFlag(Cell::GIVEN)) {
                board[i][j].value = 0;
                board[i][j].notes = 0;
                board[i][j].setFlag(Cell::ERR, false);
            }
            board[i][j].setFlag(Cell::SELECTED, false);
            board[i][j].setFlag(Cell::HIGHLIGHTED, false);
        }
    }
    clearSelection();
}

uint8_t SudokuCore::getValue(int8_t row, int8_t col) const { return board[row][col].value; }

bool SudokuCore::setValue(int8_t row, int8_t col, uint8_t value) {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return false;
    if (board[row][col].hasFlag(Cell::GIVEN)) return false;
    board[row][col].value = value;
    board[row][col].notes = 0;
    checkConflicts();
    return true;
}

bool SudokuCore::eraseValue(int8_t row, int8_t col) { return setValue(row, col, 0); }

bool SudokuCore::toggleNote(int8_t row, int8_t col, uint8_t note) {
    if (row < 0 || row >= 9 || col < 0 || col >= 9 || note < 1 || note > 9) return false;
    if (board[row][col].hasFlag(Cell::GIVEN) || board[row][col].value != 0) return false;
    board[row][col].notes ^= (1 << (note - 1)); // XOR 翻转该位
    return true;
}

bool SudokuCore::clearNotes(int8_t row, int8_t col) {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return false;
    board[row][col].notes = 0;
    return true;
}

void SudokuCore::selectCell(int8_t row, int8_t col) {
    clearSelection();
    if (row >= 0 && row < 9 && col >= 0 && col < 9) {
        selected_row = row; selected_col = col;
        board[row][col].setFlag(Cell::SELECTED, true);
        uint8_t val = board[row][col].value;

        for (uint8_t i = 0; i < 9; ++i) {
            board[row][i].setFlag(Cell::HIGHLIGHTED, true);
            board[i][col].setFlag(Cell::HIGHLIGHTED, true);
        }
        int8_t box_row = (row / 3) * 3, box_col = (col / 3) * 3;
        for (int8_t i = box_row; i < box_row + 3; ++i)
            for (int8_t j = box_col; j < box_col + 3; ++j)
                board[i][j].setFlag(Cell::HIGHLIGHTED, true);

        if (val != 0) {
            for (uint8_t i = 0; i < 9; ++i)
                for (uint8_t j = 0; j < 9; ++j)
                    if (board[i][j].value == val)
                        board[i][j].setFlag(Cell::HIGHLIGHTED, true);
        }
    }
}

void SudokuCore::clearSelection() {
    selected_row = -1; selected_col = -1;
    for (uint8_t i = 0; i < 9; ++i) {
        for (uint8_t j = 0; j < 9; ++j) {
            board[i][j].setFlag(Cell::SELECTED, false);
            board[i][j].setFlag(Cell::HIGHLIGHTED, false);
        }
    }
}

bool SudokuCore::isGiven(int8_t row, int8_t col) const { return board[row][col].hasFlag(Cell::GIVEN); }
bool SudokuCore::isSelected(int8_t row, int8_t col) const { return board[row][col].hasFlag(Cell::SELECTED); }
bool SudokuCore::isHighlighted(int8_t row, int8_t col) const { return board[row][col].hasFlag(Cell::HIGHLIGHTED); }
bool SudokuCore::isError(int8_t row, int8_t col) const { return board[row][col].hasFlag(Cell::ERR); }
uint16_t SudokuCore::getNotes(int8_t row, int8_t col) const { return board[row][col].notes; }

bool SudokuCore::isComplete() const {
    for (uint8_t i = 0; i < 9; ++i)
        for (uint8_t j = 0; j < 9; ++j)
            if (board[i][j].value == 0) return false;
    return !const_cast<SudokuCore*>(this)->checkConflicts();
}

bool SudokuCore::checkConflicts() {
    bool has_conflict = false;
    for (uint8_t i = 0; i < 9; ++i)
        for (uint8_t j = 0; j < 9; ++j)
            board[i][j].setFlag(Cell::ERR, false);

    for (int8_t row = 0; row < 9; ++row) {
        for (int8_t col = 0; col < 9; ++col) {
            uint8_t val = board[row][col].value;
            if (val == 0) continue;

            for (uint8_t i = 0; i < 9; ++i) {
                if (i != col && board[row][i].value == val) {
                    board[row][col].setFlag(Cell::ERR, true);
                    board[row][i].setFlag(Cell::ERR, true);
                    has_conflict = true;
                }
                if (i != row && board[i][col].value == val) {
                    board[row][col].setFlag(Cell::ERR, true);
                    board[i][col].setFlag(Cell::ERR, true);
                    has_conflict = true;
                }
            }
            int8_t box_row = (row / 3) * 3, box_col = (col / 3) * 3;
            for (int8_t i = box_row; i < box_row + 3; ++i) {
                for (int8_t j = box_col; j < box_col + 3; ++j) {
                    if ((i != row || j != col) && board[i][j].value == val) {
                        board[row][col].setFlag(Cell::ERR, true);
                        board[i][j].setFlag(Cell::ERR, true);
                        has_conflict = true;
                    }
                }
            }
        }
    }
    return has_conflict;
}