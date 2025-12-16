#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <optional> 
#include <PieceType.hpp>

// 新增：大廳按鈕的回傳動作狀態
enum class LobbyAction {
    NONE,
    CREATE_ROOM,
    REFRESH,
    JOIN_SELECTED
};

class Renderer {
private:
    // ---視窗設定 ---
    sf::RenderWindow window;
    const unsigned int WINDOW_SIZE = 780; 
    const float BOARD_DIMENSION = 8.0f;
    float TILE_SIZE; 

    // --- 圖片與字型 ---
    const std::string RESOURCE_PATH = "resources/textures/";
    std::map<std::string, sf::Texture> textureMap;
    
    // 需要的字型與文字物件
    sf::Font font;
    sf::Text uiText;

    // 內部輔助函式
    void loadTexture(const std::string& name, const std::string& filename);
    
    //繪製按鈕的通用函式 (回傳 true 代表被點擊)
    bool drawButton(const std::string& label, float x, float y, float width, float height, sf::Vector2i mousePos, bool isClicking);

public:
    Renderer();
    ~Renderer();

    //視窗基礎控制 
    bool isOpen();
    void pollEvents();
    void clear();
    void display();

    //遊戲畫面繪製 
    void drawBoard();
    void drawPiece(PieceType type, PieceColor color, int gridX, int gridY);

    // --- UI 與大廳功能 ---
    
    // 取得滑鼠狀態
    sf::Vector2i getMousePosition();
    bool isMouseLeftClicked();

    /**
     * 繪製大廳畫面
     * @param serverList 搜尋到的伺服器列表
     * @param selectedIndex 目前選中的伺服器索引
     * @return LobbyAction 回傳使用者的操作 (建立、重整、加入)
     */
    LobbyAction drawLobby(const std::vector<std::string>& serverList, int& selectedIndex);

    /**
     * 繪製 "等待玩家中" 的畫面
     * @param ipAddress 本機 IP 顯示用
     * @return true 如果使用者點擊了 "Cancel/返回"
     */
    bool drawWaitingScreen(const std::string& ipAddress);
};

#endif // RENDERER_HPP