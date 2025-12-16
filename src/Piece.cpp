#include "Piece.hpp"
#include <cmath> // std::abs

// --- Position 實作 ---

Position::Position(int a, int b) : x(a), y(b) {}

bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}

bool Position::onDiagonal(const Position& other) const {
    // 使用 std::abs
    return std::abs(x - other.x) == std::abs(y - other.y);
}

bool Position::onSameLine(const Position& other) const {
    return x == other.x || y == other.y;
}

// --- Piece 實作 ---

Piece::Piece(PieceType t, PieceColor c, int x, int y) 
    : type(t), color(c), position(x, y), isAlive(true) {}

void Piece::moveTo(int x, int y) {
    position.setX(x);
    position.setY(y);
}

void Piece::capture() {
    isAlive = false;
}

Position Piece::getPosition() const {
    return position; // 直接回傳物件，安全又簡單
}

// --- 子類別實作 (例如 Pawn) ---

Pawn::Pawn(PieceColor c, int x, int y) 
    : Piece(PieceType::PAWN, c, x, y) {}

bool Pawn::isValidMove(Position newPos, const std::vector<Piece*>& boardState) {
    // 這裡實作兵的具體走法規則
    // 1. 判斷方向 (白兵向上 y+1, 黑兵向下 y-1)
    // 2. 判斷是否第一步 (可以走兩格)
    // 3. 判斷是否吃子 (斜向移動)
    
    int direction = (this->color == PieceColor::WHITE) ? 1 : -1;
    // ... (其他實作) ...
    return true; // 暫時回傳
}

King::King(PieceColor c, int x, int y) 
    : Piece(PieceType::KING, c, x, y) {}

bool King::isValidMove(Position newPos, const std::vector<Piece*>& boardState) {
    // 國王周圍 1 格邏輯
    int dx = std::abs(newPos.getX() - position.getX());
    int dy = std::abs(newPos.getY() - position.getY());
    return (dx <= 1 && dy <= 1);
}