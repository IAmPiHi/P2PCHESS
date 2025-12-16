#include <PieceType.hpp>
int abs(int a)
{
    return a<0?-a:a;
}
class Position
{
    int x;
    int y;
public:
    Position(int a=0,int b=0):x(a),y(b){}
    int getX(){return x;}
    int getY(){return y;}
    void setX(int a){x=a;}
    void setY(int b){y=b;}
        // 1. 重載 == (可直接判斷)
    bool operator==(const Position& other) const {
            return x == other.x && y == other.y;
        }

        // 2. 重載 != 
    bool operator!=(const Position& other) const {
            return !(*this == other);
        }
        
        
        // 判斷是否在對角線上 (給主教或皇后用)
    bool onDiagonal(const Position& other) const {
            return abs(x - other.x) == abs(y - other.y);
    }
    bool onSameLine(const Position& other) const {
            return x==other.x || y==other.y;
    }
};

class Piece
{
public:
    PieceType type;
    PieceColor color;
    Position position;
    bool isAlive=true;

    Piece(PieceType t = PieceType::NONE, PieceColor c = PieceColor::NONE, int x=0, int y=0) 
        : type(t), color(c), position(x,y) {}

    void moveTo(int x, int y){
        position.setX(x);
        position.setY(y);
    
    }

    void capture(){
        isAlive = false;  //you can delete the piece in main process or just mark it as not alive
    }

    int* getPosition(){
        return new int[2]{position.getX(), position.getY()};  //return position as an array 1x 2y
    }
};