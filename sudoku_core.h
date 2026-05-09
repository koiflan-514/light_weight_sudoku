#pragma once
#include <cstdint>
struct Cell {
    uint8_t value;      
    uint8_t notes[9];   
    uint8_t is_given;      
    uint8_t is_selected;   
    uint8_t is_highlighted;
    uint8_t is_error;      
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
    ~SudokuCore() = default;
    
    void loadPuzzle(Difficulty difficulty);
    void reset();
    
    uint8_t getValue(int8_t row, int8_t col) const;
    bool setValue(int8_t row, int8_t col, uint8_t value);
    bool eraseValue(int8_t row, int8_t col);
    
    bool toggleNote(int8_t row, int8_t col, uint8_t note);
    bool clearNotes(int8_t row, int8_t col);
    
    void selectCell(int8_t row, int8_t col);
    void clearSelection();
    int8_t getSelectedRow() const;
    int8_t getSelectedCol() const;
    
    bool isGiven(int8_t row, int8_t col) const;
    bool isSelected(int8_t row, int8_t col) const;
    bool isHighlighted(int8_t row, int8_t col) const;
    bool isError(int8_t row, int8_t col) const;
    const uint8_t* getNotes(int8_t row, int8_t col) const;
    
    bool isComplete() const;
    bool checkConflicts();
};
