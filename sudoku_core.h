#pragma once
#include <cstdint>

// 状态压缩：每个 Cell 从 14 字节压缩到严格的 4 字节
struct Cell {
    uint16_t notes = 0;     // 前9位代表1-9的笔记
    uint8_t value = 0;      // 填入的值 (0-9)
    uint8_t flags = 0;      // 状态位掩码

    enum Flags {
        GIVEN = 1 << 0,
        SELECTED = 1 << 1,
        HIGHLIGHTED = 1 << 2,
        ERR = 1 << 3
    };

    inline void setFlag(Flags f, bool v) { if (v) flags |= f; else flags &= ~f; }
    inline bool hasFlag(Flags f) const { return (flags & f) != 0; }
};

enum class Difficulty : uint8_t {
    EASY = 36,
    NORMAL = 30,
    HARD = 24,
    EXPERT = 17
};

class SudokuCore {
private:
    Cell board[9][9];
    Cell solution[9][9];
    int8_t selected_row = -1;
    int8_t selected_col = -1;

    bool isValidPlacement(int8_t row, int8_t col, uint8_t num) const;
    bool solve(int8_t row, int8_t col);
    void shuffleArray(uint8_t arr[], uint8_t size);
    void generateCompleteBoard();
    void removeNumbers(uint8_t count);

public:
    SudokuCore() = default;

    void loadPuzzle(Difficulty difficulty);
    void reset();

    uint8_t getValue(int8_t row, int8_t col) const;
    bool setValue(int8_t row, int8_t col, uint8_t value);
    bool eraseValue(int8_t row, int8_t col);

    bool toggleNote(int8_t row, int8_t col, uint8_t note);
    bool clearNotes(int8_t row, int8_t col);

    void selectCell(int8_t row, int8_t col);
    void clearSelection();
    int8_t getSelectedRow() const { return selected_row; }
    int8_t getSelectedCol() const { return selected_col; }

    bool isGiven(int8_t row, int8_t col) const;
    bool isSelected(int8_t row, int8_t col) const;
    bool isHighlighted(int8_t row, int8_t col) const;
    bool isError(int8_t row, int8_t col) const;
    uint16_t getNotes(int8_t row, int8_t col) const; // 返回掩码

    bool isComplete() const;
    bool checkConflicts();
};