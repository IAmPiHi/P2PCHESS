#include "Renderer.hpp"
#include <iostream>


Renderer::Renderer() : uiText(font) { 
    // 初始化 TILE_SIZE
    TILE_SIZE = static_cast<float>(WINDOW_SIZE) / BOARD_DIMENSION;

    // 建立視窗
    window.create(sf::VideoMode({WINDOW_SIZE, WINDOW_SIZE}), "LAN Chess");
    window.setFramerateLimit(60);

    
    
    // 載入字型
    if (!font.openFromFile("resources/arial.ttf")) {
        std::cerr << "Error: Could not load font from resources/arial.ttf" << std::endl;
    }
    

    // 載入圖片
    loadTexture("board", "board.png");

    std::string colors[] = {"w", "b"};
    std::string types[] = {"pawn", "rook", "knight", "bishop", "queen", "king"};
    for (const auto& c : colors) {
        for (const auto& t : types) {
            std::string key = c + "_" + t;
            loadTexture(key, key + ".png");
        }
    }
}

Renderer::~Renderer() {
    if (window.isOpen()) window.close();
}

void Renderer::loadTexture(const std::string& name, const std::string& filename) {
    sf::Texture texture;
    std::string fullPath = RESOURCE_PATH + filename;
    if (!texture.loadFromFile(fullPath)) {
        std::cerr << "Error: " << fullPath << std::endl;
    } else {
        texture.setSmooth(true);
        textureMap[name] = std::move(texture);
    }
}

bool Renderer::isOpen() { return window.isOpen(); }

void Renderer::pollEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void Renderer::clear() { window.clear(sf::Color::Black); }
void Renderer::display() { window.display(); }

void Renderer::drawBoard() {
    if (textureMap.count("board")) {
        sf::Sprite boardSprite(textureMap["board"]);
        sf::Vector2u texSize = textureMap["board"].getSize();
        
        float scaleX = static_cast<float>(WINDOW_SIZE) / texSize.x;
        float scaleY = static_cast<float>(WINDOW_SIZE) / texSize.y;
        
        boardSprite.setScale({scaleX, scaleY});
        window.draw(boardSprite);
    }
}

void Renderer::drawPiece(PieceType type, PieceColor color, int gridX, int gridY) {
    if (type == PieceType::NONE || color == PieceColor::NONE) return;

    std::string colorPrefix = (color == PieceColor::WHITE) ? "w" : "b";
    std::string typeName;
    switch (type) {
        case PieceType::PAWN:   typeName = "pawn"; break;
        case PieceType::ROOK:   typeName = "rook"; break;
        case PieceType::KNIGHT: typeName = "knight"; break;
        case PieceType::BISHOP: typeName = "bishop"; break;
        case PieceType::QUEEN:  typeName = "queen"; break;
        case PieceType::KING:   typeName = "king"; break;
        default: return;
    }

    std::string key = colorPrefix + "_" + typeName;
    if (textureMap.count(key)) {
        sf::Sprite pieceSprite(textureMap[key]);
        float texWidth = static_cast<float>(textureMap[key].getSize().x);
        float scale = TILE_SIZE / texWidth * 0.82f; // 留點邊距

        pieceSprite.setScale({scale, scale});
        
        float pixelX = static_cast<float>(gridX) * TILE_SIZE + (TILE_SIZE * scale * 0.05f); 
        float pixelY = static_cast<float>(gridY) * TILE_SIZE + (TILE_SIZE * scale * 0.05f);
        pieceSprite.setPosition({pixelX, pixelY});
        
        window.draw(pieceSprite);
    }
}

// --- 大廳UI ---

sf::Vector2i Renderer::getMousePosition() {
    return sf::Mouse::getPosition(window);
}

bool Renderer::isMouseLeftClicked() {
    return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}

bool Renderer::drawButton(const std::string& label, float x, float y, float width, float height, sf::Vector2i mousePos, bool isClicking) {
    sf::RectangleShape rect({width, height});
    rect.setPosition({x, y});

    sf::FloatRect bounds = rect.getGlobalBounds();
    bool isHovered = bounds.contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)});
    
    if (isHovered) {
        rect.setFillColor(isClicking ? sf::Color(100, 100, 100) : sf::Color(150, 150, 150));
    } else {
        rect.setFillColor(sf::Color(50, 50, 50));
    }
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(2);

    window.draw(rect);

    uiText.setString(label);
    uiText.setCharacterSize(24);
    uiText.setFillColor(sf::Color::White);
    
    sf::FloatRect textBounds = uiText.getLocalBounds();
    uiText.setOrigin({textBounds.position.x + textBounds.size.x / 2.0f,
                      textBounds.position.y + textBounds.size.y / 2.0f});
    uiText.setPosition({x + width / 2.0f, y + height / 2.0f});

    window.draw(uiText);
    return isHovered && isClicking;
}

LobbyAction Renderer::drawLobby(const std::vector<std::string>& serverList, int& selectedIndex) {
    sf::Vector2i mousePos = getMousePosition();
    bool isClick = isMouseLeftClicked();
    LobbyAction action = LobbyAction::NONE;

    // 標題
    uiText.setString("LANCHESS Lobby");
    uiText.setCharacterSize(40);
    uiText.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = uiText.getLocalBounds();
    uiText.setOrigin({titleBounds.position.x + titleBounds.size.x / 2.0f, 0});
    uiText.setPosition({WINDOW_SIZE / 2.0f, 50});
    window.draw(uiText);

    // 列表背景
    float listX = 100, listY = 150, listW = WINDOW_SIZE - 200, listH = 400;
    sf::RectangleShape listBg({listW, listH});
    listBg.setPosition({listX, listY});
    listBg.setFillColor(sf::Color(30, 30, 30));
    listBg.setOutlineColor(sf::Color::White);
    listBg.setOutlineThickness(1);
    window.draw(listBg);

    // 列表項目
    float itemHeight = 40;
    for (size_t i = 0; i < serverList.size(); ++i) {
        float itemY = listY + i * itemHeight;
        if (itemY + itemHeight > listY + listH) break;

        sf::RectangleShape itemRect({listW, itemHeight});
        itemRect.setPosition({listX, itemY});
        
        bool itemHover = itemRect.getGlobalBounds().contains({(float)mousePos.x, (float)mousePos.y});
        
        if (selectedIndex == (int)i) itemRect.setFillColor(sf::Color(100, 100, 200));
        else if (itemHover) itemRect.setFillColor(sf::Color(60, 60, 60));
        else itemRect.setFillColor(sf::Color::Transparent);

        window.draw(itemRect);

        uiText.setString(serverList[i]);
        uiText.setCharacterSize(20);
        uiText.setFillColor(sf::Color::White);
        uiText.setOrigin({0, 0});
        uiText.setPosition({listX + 10, itemY + 8});
        window.draw(uiText);

        if (itemHover && isClick) selectedIndex = (int)i;
    }

    // 按鈕
    float btnW = 200, btnH = 50, btnY = listY + listH + 50;
    if (drawButton("Create Room", listX-30, btnY, btnW, btnH, mousePos, isClick)) action = LobbyAction::CREATE_ROOM;
    if (drawButton("Refresh", listX + 190, btnY, btnW, btnH, mousePos, isClick)) action = LobbyAction::REFRESH;
    if (selectedIndex >= 0 && selectedIndex < (int)serverList.size()) {
        if (drawButton("Join Game", listX + 410, btnY, btnW, btnH, mousePos, isClick)) action = LobbyAction::JOIN_SELECTED;
    }

    return action;
}

bool Renderer::drawWaitingScreen(const std::string& ipAddress) {
    sf::Vector2i mousePos = getMousePosition();
    bool isClick = isMouseLeftClicked();

    uiText.setString("Waiting for Opponent...");
    uiText.setCharacterSize(36);
    uiText.setFillColor(sf::Color::White);
    sf::FloatRect b = uiText.getLocalBounds();
    uiText.setOrigin({b.position.x + b.size.x/2, b.position.y + b.size.y/2});
    uiText.setPosition({WINDOW_SIZE/2.0f, 200});
    window.draw(uiText);

    uiText.setString("Your IP: " + ipAddress);
    uiText.setCharacterSize(24);
    uiText.setFillColor(sf::Color::Green);
    b = uiText.getLocalBounds();
    uiText.setOrigin({b.position.x + b.size.x/2, 0});
    uiText.setPosition({WINDOW_SIZE/2.0f, 260});
    window.draw(uiText);

    if (drawButton("Cancel", WINDOW_SIZE/2.0f - 100, 500, 200, 50, mousePos, isClick)) return true;
    return false;
}