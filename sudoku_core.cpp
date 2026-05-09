#include "sudoku_core.h"
#include <cmath>
#include <cstring>

bool SudokuCore::isValidPlacement(int8_t row, int8_t col, uint8_t num) const {
    if (num == 0) return true;
    
    for (uint8_t i = 0; i < 9; ++i) {
        if (i != col && board[row][i].value == num) return false;
        if (i != row && board[i][col].value == num) return false;
    }
    
    int8_t box_row = (row / 3) * 3;
    int8_t box_col = (col / 3) * 3;
    for (int8_t i = box_row; i < box_row + 3; ++i) {
        for (int8_t j = box_col; j < box_col + 3; ++j) {
            if (i != row && j != col && board[i][j].value == num) return false;
        }
    }
    
    return true;
}

bool SudokuCore::solve(int8_t row, int8_t col) {
    if (row == 9) return true;
    if (col == 9) return solve(row + 1, 0);
    if (board[row][col].value != 0) return solve(row, col + 1);
    
    uint8_t nums[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
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

void SudokuCore::shuffleArray(uint8_t arr[], uint8_t size) {
    for (uint8_t i = size - 1; i > 0; --i) {
        uint8_t j = rand() % (i + 1);
        uint8_t temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void SudokuCore::generateCompleteBoard() {
    memset(board, 0, sizeof(board));
    solve(0, 0);
    memcpy(solution, board, sizeof(board));
}

void SudokuCore::removeNumbers(uint8_t count) {
    uint8_t removed = 0;
    while (removed < count) {
        uint8_t row = rand() % 9;
        uint8_t col = rand() % 9;
        if (board[row][col].value != 0) {
            board[row][col].value = 0;
            board[row][col].is_given = 0;
            removed++;
        }
    }
    
    for (uint8_t i = 0; i < 9; ++i) {
        for (uint8_t j = 0; j < 9; ++j) {
            if (board[i][j].value != 0) {
                board[i][j].is_given = 1;
            }
        }
    }
}

void SudokuCore::loadPuzzle(Difficulty difficulty) {
    generateCompleteBoard();
    removeNumbers(static_cast<uint8_t>(difficulty));
    clearSelection();
}

void SudokuCore::reset() {
    for (uint8_t i = 0; i < 9; ++i) {
        for (uint8_t j = 0; j < 9; ++j) {
            if (!board[i][j].is_given) {
                board[i][j].value = 0;
                memset(board[i][j].notes, 0, sizeof(board[i][j].notes));
                board[i][j].is_error = 0;
            }
            board[i][j].is_selected = 0;
            board[i][j].is_highlighted = 0;
        }
    }
    clearSelection();
}

uint8_t SudokuCore::getValue(int8_t row, int8_t col) const {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return 0;
    return board[row][col].value;
}

bool SudokuCore::setValue(int8_t row, int8_t col, uint8_t value) {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return false;
    if (board[row][col].is_given) return false;
    
    board[row][col].value = value;
    memset(board[row][col].notes, 0, sizeof(board[row][col].notes));
    
    checkConflicts();
    return true;
}

bool SudokuCore::eraseValue(int8_t row, int8_t col) {
    return setValue(row, col, 0);
}

bool SudokuCore::toggleNote(int8_t row, int8_t col, uint8_t note) {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return false;
    if (note < 1 || note > 9) return false;
    if (board[row][col].is_given) return false;
    if (board[row][col].value != 0) return false;
    
    board[row][col].notes[note - 1] = !board[row][col].notes[note - 1];
    return true;
}

bool SudokuCore::clearNotes(int8_t row, int8_t col) {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return false;
    memset(board[row][col].notes, 0, sizeof(board[row][col].notes));
    return true;
}

void SudokuCore::selectCell(int8_t row, int8_t col) {
    for (uint8_t i = 0; i < 9; ++i) {
        for (uint8_t j = 0; j < 9; ++j) {
            board[i][j].is_selected = 0;
            board[i][j].is_highlighted = 0;
        }
    }
    
    if (row >= 0 && row < 9 && col >= 0 && col < 9) {
        selected_row = row;
        selected_col = col;
        board[row][col].is_selected = 1;
        
        uint8_t selected_value = board[row][col].value;
        
        for (uint8_t i = 0; i < 9; ++i) {
            board[row][i].is_highlighted = 1;
            board[i][col].is_highlighted = 1;
        }
        
        int8_t box_row = (row / 3) * 3;
        int8_t box_col = (col / 3) * 3;
        for (int8_t i = box_row; i < box_row + 3; ++i) {
            for (int8_t j = box_col; j < box_col + 3; ++j) {
                board[i][j].is_highlighted = 1;
            }
        }
        
        if (selected_value != 0) {
            for (uint8_t i = 0; i < 9; ++i) {
                for (uint8_t j = 0; j < 9; ++j) {
                    if (board[i][j].value == selected_value) {
                        board[i][j].is_highlighted = 1;
                    }
                }
            }
        }
    } else {
        clearSelection();
    }
}

void SudokuCore::clearSelection() {
    selected_row = -1;
    selected_col = -1;
    for (uint8_t i = 0; i < 9; ++i) {
        for (uint8_t j = 0; j < 9; ++j) {
            board[i][j].is_selected = 0;
            board[i][j].is_highlighted = 0;
        }
    }
}

int8_t SudokuCore::getSelectedRow() const {
    return selected_row;
}

int8_t SudokuCore::getSelectedCol() const {
    return selected_col;
}

bool SudokuCore::isGiven(int8_t row, int8_t col) const {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return false;
    return board[row][col].is_given;
}

bool SudokuCore::isSelected(int8_t row, int8_t col) const {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return false;
    return board[row][col].is_selected;
}

bool SudokuCore::isHighlighted(int8_t row, int8_t col) const {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return false;
    return board[row][col].is_highlighted;
}

bool SudokuCore::isError(int8_t row, int8_t col) const {
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return false;
    return board[row][col].is_error;
}

const uint8_t* SudokuCore::getNotes(int8_t row, int8_t col) const {
    static uint8_t empty_notes[9] = {0};
    if (row < 0 || row >= 9 || col < 0 || col >= 9) return empty_notes;
    return board[row][col].notes;
}

bool SudokuCore::isComplete() const {
    for (uint8_t i = 0; i < 9; ++i) {
        for (uint8_t j = 0; j < 9; ++j) {
            if (board[i][j].value == 0) return false;
        }
    }
    return !const_cast<SudokuCore*>(this)->checkConflicts();
}

bool SudokuCore::checkConflicts() {
    bool has_conflict = false;
    
    for (uint8_t i = 0; i < 9; ++i) {
        for (uint8_t j = 0; j < 9; ++j) {
            board[i][j].is_error = 0;
        }
    }
    
    for (int8_t row = 0; row < 9; ++row) {
        for (int8_t col = 0; col < 9; ++col) {
            uint8_t val = board[row][col].value;
            if (val == 0) continue;
            
            for (uint8_t i = 0; i < 9; ++i) {
                if (i != col && board[row][i].value == val) {
                    board[row][col].is_error = 1;
                    board[row][i].is_error = 1;
                    has_conflict = true;
                }
                if (i != row && board[i][col].value == val) {
                    board[row][col].is_error = 1;
                    board[i][col].is_error = 1;
                    has_conflict = true;
                }
            }
            
            int8_t box_row = (row / 3) * 3;
            int8_t box_col = (col / 3) * 3;
            for (int8_t i = box_row; i < box_row + 3; ++i) {
                for (int8_t j = box_col; j < box_col + 3; ++j) {
                    if ((i != row || j != col) && board[i][j].value == val) {
                        board[row][col].is_error = 1;
                        board[i][j].is_error = 1;
                        has_conflict = true;
                    }
                }
            }
        }
    }
    
    return has_conflict;
}
