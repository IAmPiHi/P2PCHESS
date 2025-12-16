#pragma once  
#include "PieceType.hpp" 
#include <vector>
#include <cmath> 


class Position {
private:
    int x;
    int y;
public:
    Position(int a = 0, int b = 0);
    
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int a) { x = a; }
    void setY(int b) { y = b; }

    // 運算子重載宣告
    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;

    // 判斷邏輯宣告
    bool onDiagonal(const Position& other) const;
    bool onSameLine(const Position& other) const;
};

class Piece {
protected: 
    PieceType type;
    PieceColor color;
    Position position;
    bool isAlive;

public:
    Piece(PieceType t, PieceColor c, int x, int y);
    
    virtual ~Piece() = default;

    void moveTo(int x, int y);
    void capture();
    
    bool getAlive() const { return isAlive; }
    PieceColor getColor() const { return color; }
    PieceType getType() const { return type; }
    
    Position getPosition() const; 

    //強迫子類別必須實作
    virtual bool isValidMove(Position newPos, const std::vector<Piece*>& boardState) = 0;
};

// --- 子類別宣告 ---

class Pawn : public Piece {
public:
    Pawn(PieceColor c, int x, int y);
    
    bool isValidMove(Position newPos, const std::vector<Piece*>& boardState) override;
};

class King : public Piece {
public:
    King(PieceColor c, int x, int y);
    bool isValidMove(Position newPos, const std::vector<Piece*>& boardState) override;
};

//其他棋子類別依此模式繼續添加