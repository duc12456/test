#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <cmath>

using namespace std;

// Enum cho các trạng thái game
enum GameScreen { MENU = 0, PLAYING, INSTRUCTIONS };

Color green = {173, 204, 96, 255};
Color darkgreen = {43, 51, 24, 255};

int kichThuocO = 25; //cellsize
int SoOtrongMoiHangCot = 20; //cellcount
int offset = 62.5;

double lastUpdateTime = 0;

// Biến toàn cục cho background và UI
Texture2D backgroundTexture;
Texture2D startButtonTexture;
Texture2D exitButtonTexture;
Texture2D instructionsTexture;
Texture2D upTexture, downTexture, leftTexture, rightTexture;

// Texture cho rắn và đồ ăn
Texture2D snakeHeadTexture;
Texture2D snakeBodyTexture;
Texture2D foodTextures[5]; // Array cho 5 loại đồ ăn
int totalFoodTypes = 5;

// Âm thanh
Music backgroundMusic;
bool musicLoaded = false;

// Trạng thái game và hiệu ứng
GameScreen currentScreen = MENU;
float transitionAlpha = 0.0f;
bool isTransitioning = false;
GameScreen nextScreen = MENU;

// Hiệu ứng nền động
float animationTime = 0.0f;
Vector2 floatingOffset = {0, 0};

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for(unsigned int i = 0; i < deque.size(); i++)
    {
        if(Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

// Hàm kiểm tra va chạm với nút
bool IsButtonClicked(Rectangle button, Vector2 mousePos, bool mousePressed)
{
    return CheckCollisionPointRec(mousePos, button) && mousePressed;
}

// Hàm vẽ nền động
void DrawAnimatedBackground()
{
    animationTime += GetFrameTime();
    
    // Tính toán hiệu ứng floating
    floatingOffset.x = sin(animationTime * 2.0f) * 10.0f;
    floatingOffset.y = cos(animationTime * 1.5f) * 8.0f;
    
    // Vẽ background với hiệu ứng floating
    int windowWidth = 2 * offset + kichThuocO * SoOtrongMoiHangCot;
    int windowHeight = 2 * offset + kichThuocO * SoOtrongMoiHangCot;
    float scaleX = (float)windowWidth / backgroundTexture.width;
    float scaleY = (float)windowHeight / backgroundTexture.height;
    
    DrawTextureEx(backgroundTexture, Vector2{floatingOffset.x * 0.2f, floatingOffset.y * 0.2f}, 
                  sin(animationTime * 0.5f) * 2.0f, scaleX, WHITE);
    
    // Thêm hiệu ứng overlay gradient
    Color overlayColor = {0, 0, 0, (unsigned char)(30 + sin(animationTime) * 15)};
    DrawRectangle(0, 0, windowWidth, windowHeight, overlayColor);
}

// Hàm bắt đầu chuyển cảnh
void StartTransition(GameScreen target)
{
    if (!isTransitioning)
    {
        isTransitioning = true;
        nextScreen = target;
        transitionAlpha = 0.0f;
    }
}

// Hàm cập nhật chuyển cảnh
void UpdateTransition()
{
    if (isTransitioning)
    {
        transitionAlpha += GetFrameTime() * 4.0f; // Tốc độ chuyển cảnh
        
        if (transitionAlpha >= 1.0f)
        {
            currentScreen = nextScreen;
            transitionAlpha = 1.0f;
        }
        
        if (transitionAlpha >= 2.0f)
        {
            isTransitioning = false;
            transitionAlpha = 0.0f;
        }
    }
}

// Hàm vẽ hiệu ứng chuyển cảnh
void DrawTransition()
{
    if (isTransitioning)
    {
        float alpha = transitionAlpha;
        if (alpha > 1.0f) alpha = 2.0f - alpha;
        
        Color transitionColor = {0, 0, 0, (unsigned char)(alpha * 255)};
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), transitionColor);
    }
}

class Snake
{
public:
    deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction = {1, 0};
    bool addSegment = false;

    void Draw()
    {
        for(unsigned int i = 0; i < body.size(); i++)
        {
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offset + x * kichThuocO, offset + y * kichThuocO, (float)kichThuocO, (float)kichThuocO};
            
            if(i == 0) // Đầu rắn
            {
                // Tính góc xoay dựa trên hướng di chuyển
                float rotation = 0.0f;
                if(direction.x == 1) rotation = 0.0f;      // Phải
                else if(direction.x == -1) rotation = 180.0f; // Trái
                else if(direction.y == -1) rotation = 270.0f; // Lên
                else if(direction.y == 1) rotation = 90.0f;   // Xuống
                
                // Vẽ đầu rắn với scale phù hợp (28x28 -> 25x25)
                float headScale = (float)kichThuocO / 28.0f;
                Vector2 origin = {14.0f * headScale, 14.0f * headScale}; // Center của texture đã scale
                Vector2 position = {segment.x + kichThuocO/2.0f, segment.y + kichThuocO/2.0f};
                DrawTexturePro(snakeHeadTexture, 
                              Rectangle{0, 0, 28.0f, 28.0f},
                              Rectangle{position.x, position.y, (float)kichThuocO, (float)kichThuocO},
                              origin, rotation, WHITE);
            }
            else // Thân rắn
            {
                // Vẽ thân rắn với scale phù hợp (28x28 -> 25x25)
                float bodyScale = (float)kichThuocO / 28.0f;
                DrawTextureEx(snakeBodyTexture, Vector2{segment.x, segment.y}, 0.0f, bodyScale, WHITE);
            }
        }
    }

    void Update()
    {
        body.push_front(Vector2Add(body[0], direction));
        if(addSegment == true)
        {
            addSegment = false;
        }
        else
        {  
            body.pop_back();
        }
    }

    void Reset()
    {
        body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1, 0};
    }
};

class Food
{
public:
    Vector2 position;
    int foodType; // Loại đồ ăn (0-4)

    Food(deque<Vector2> snakeBody)
    {
        position = GenerateRandomPos(snakeBody);
        foodType = GetRandomValue(0, totalFoodTypes - 1); // Random chọn loại đồ ăn
    }

    void Draw()
    {
        // Vẽ đồ ăn với scale phù hợp (28x28 -> 25x25)
        float scale = (float)kichThuocO / 28.0f;
        Vector2 pos = {offset + position.x * kichThuocO, offset + position.y * kichThuocO};
        DrawTextureEx(foodTextures[foodType], pos, 0.0f, scale, WHITE);
    }

    Vector2 GenerateRandomCell()
    {
        float x = GetRandomValue(0, SoOtrongMoiHangCot - 1);
        float y = GetRandomValue(0, SoOtrongMoiHangCot - 1);
        return Vector2{x, y};
    }

    Vector2 GenerateRandomPos(deque<Vector2>snakeBody)
    {
        Vector2 position = GenerateRandomCell();
        while(ElementInDeque(position, snakeBody))
        {
            position = GenerateRandomCell();
        }
        return position;
    }
    
    void Regenerate(deque<Vector2> snakeBody)
    {
        position = GenerateRandomPos(snakeBody);
        foodType = GetRandomValue(0, totalFoodTypes - 1); // Random loại mới
    }
};

class Game
{
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;
    Sound eatSound;
    Sound wallSound;

    Game()
    {
        InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
        wallSound = LoadSound("Sounds/wall.mp3");
    }

    ~Game()
    {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Draw()
    {
        food.Draw();
        snake.Draw();
    }

    void Update()
    {
        if(running)
        {
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood()
    {
        if(Vector2Equals(snake.body[0], food.position))
        {
            food.Regenerate(snake.body); // Tạo đồ ăn mới với loại ngẫu nhiên
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);
        }
    }

    void CheckCollisionWithEdges()
    {
        if(snake.body[0].x == SoOtrongMoiHangCot || snake.body[0].x == -1)
        {
            GameOver();
        }
        if(snake.body[0].y == SoOtrongMoiHangCot || snake.body[0].y == -1)
        {
            GameOver();
        }
    }

    void GameOver()
    {
        snake.Reset();
        food.Regenerate(snake.body); // Tạo đồ ăn mới khi game over
        running = false;
        score = 0;
        PlaySound(wallSound);
    }

    void CheckCollisionWithTail()
    {
        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if(ElementInDeque(snake.body[0], headlessBody))
        {
            GameOver();
        }
    }
};

// Hàm vẽ màn hình menu
void DrawMenuScreen()
{
    DrawAnimatedBackground();
    
    int windowWidth = 2 * offset + kichThuocO * SoOtrongMoiHangCot;
    int windowHeight = 2 * offset + kichThuocO * SoOtrongMoiHangCot;
    
    // Vẽ tiêu đề với hiệu ứng
    float titleFloat = sin(animationTime * 2.0f) * 5.0f;
    DrawText("SNAKE GAME", windowWidth/2 - 150, 100 + titleFloat, 50, WHITE);
    DrawText("SNAKE GAME", windowWidth/2 - 152, 98 + titleFloat, 50, darkgreen);
    
    // Vẽ các nút với kích thước phù hợp
    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    
    // Scale cho các nút (29x20 -> 150x60)
    float buttonScaleX = 150.0f / 29.0f;
    float buttonScaleY = 60.0f / 20.0f;
    
    // Nút Start
    Rectangle startBtn = {windowWidth/2 - 75, 250, 150, 60};
    Color startColor = CheckCollisionPointRec(mousePos, startBtn) ? Color{200, 255, 200, 255} : Color{255, 255, 255, 200};
    DrawRectangleRec(startBtn, startColor);
    DrawRectangleLinesEx(startBtn, 2, darkgreen);
    
    // Vẽ icon start ở giữa nút
    Vector2 startIconPos = {startBtn.x + (startBtn.width - 29*2)/2, startBtn.y + (startBtn.height - 20*2)/2};
    DrawTextureEx(startButtonTexture, startIconPos, 0.0f, 2.0f, WHITE);
    
    if (IsButtonClicked(startBtn, mousePos, mousePressed))
    {
        StartTransition(PLAYING);
    }
    
    // Nút Instructions
    Rectangle instrBtn = {windowWidth/2 - 75, 330, 150, 60};
    Color instrColor = CheckCollisionPointRec(mousePos, instrBtn) ? Color{200, 255, 200, 255} : Color{255, 255, 255, 200};
    DrawRectangleRec(instrBtn, instrColor);
    DrawRectangleLinesEx(instrBtn, 2, darkgreen);
    
    Vector2 instrIconPos = {instrBtn.x + (instrBtn.width - 29*2)/2, instrBtn.y + (instrBtn.height - 20*2)/2};
    DrawTextureEx(instructionsTexture, instrIconPos, 0.0f, 2.0f, WHITE);
    
    if (IsButtonClicked(instrBtn, mousePos, mousePressed))
    {
        StartTransition(INSTRUCTIONS);
    }
    
    // Nút Exit
    Rectangle exitBtn = {windowWidth/2 - 75, 410, 150, 60};
    Color exitColor = CheckCollisionPointRec(mousePos, exitBtn) ? Color{255, 200, 200, 255} : Color{255, 255, 255, 200};
    DrawRectangleRec(exitBtn, exitColor);
    DrawRectangleLinesEx(exitBtn, 2, darkgreen);
    
    Vector2 exitIconPos = {exitBtn.x + (exitBtn.width - 29*2)/2, exitBtn.y + (exitBtn.height - 20*2)/2};
    DrawTextureEx(exitButtonTexture, exitIconPos, 0.0f, 2.0f, WHITE);
    
    if (IsButtonClicked(exitBtn, mousePos, mousePressed))
    {
        exit(0);
    }
}

// Hàm vẽ màn hình hướng dẫn
void DrawInstructionsScreen()
{
    DrawAnimatedBackground();
    
    int windowWidth = 2 * offset + kichThuocO * SoOtrongMoiHangCot;
    
    DrawText("HUONG DAN CHOI", windowWidth/2 - 120, 50, 30, WHITE);
    
    // Vẽ các phím điều khiển
    DrawText("DIEU KHIEN:", 50, 120, 25, WHITE);
    DrawTexture(upTexture, 100, 160, WHITE);
    DrawText("LEN", 150, 175, 20, WHITE);
    
    DrawTexture(downTexture, 100, 200, WHITE);
    DrawText("XUONG", 150, 215, 20, WHITE);
    
    DrawTexture(leftTexture, 100, 240, WHITE);
    DrawText("TRAI", 150, 255, 20, WHITE);
    
    DrawTexture(rightTexture, 100, 280, WHITE);
    DrawText("PHAI", 150, 295, 20, WHITE);
    
    DrawText("AN THUC AN DE TANG DIEM!", 50, 350, 20, WHITE);
    DrawText("TRANH VA CHAM TUONG VA THAN!", 50, 380, 20, WHITE);
    
    // Nút back
    Rectangle backBtn = {windowWidth/2 - 50, 450, 100, 40};
    DrawRectangleRec(backBtn, GREEN);
    DrawText("BACK", backBtn.x + 25, backBtn.y + 10, 20, WHITE);
    
    Vector2 mousePos = GetMousePosition();
    if (IsButtonClicked(backBtn, mousePos, IsMouseButtonPressed(MOUSE_BUTTON_LEFT)))
    {
        StartTransition(MENU);
    }
}

int main () 
{
    cout << "Starting the game..." <<endl;
    InitWindow(2 * offset +kichThuocO * SoOtrongMoiHangCot,2 * offset + kichThuocO * SoOtrongMoiHangCot, "Snake Game");
    SetTargetFPS(60);

    // Tải ảnh background
    Image backgroundImage = LoadImage("bia.png");
    backgroundTexture = LoadTextureFromImage(backgroundImage);
    UnloadImage(backgroundImage);
    
    // Tải texture cho UI
    Image startImage = LoadImage("Graphics/Start.png");
    startButtonTexture = LoadTextureFromImage(startImage);
    UnloadImage(startImage);
    
    Image exitImage = LoadImage("Graphics/Exit.png");
    exitButtonTexture = LoadTextureFromImage(exitImage);
    UnloadImage(exitImage);
    
    Image instrImage = LoadImage("Graphics/HuongDan.png");
    instructionsTexture = LoadTextureFromImage(instrImage);
    UnloadImage(instrImage);
    
    Image upImage = LoadImage("Graphics/Up.png");
    upTexture = LoadTextureFromImage(upImage);
    UnloadImage(upImage);
    
    Image downImage = LoadImage("Graphics/Down.png");
    downTexture = LoadTextureFromImage(downImage);
    UnloadImage(downImage);
    
    Image leftImage = LoadImage("Graphics/Left.png");
    leftTexture = LoadTextureFromImage(leftImage);
    UnloadImage(leftImage);
    
    Image rightImage = LoadImage("Graphics/Right.png");
    rightTexture = LoadTextureFromImage(rightImage);
    UnloadImage(rightImage);
    
    // Tải texture cho rắn
    Image headImage = LoadImage("Graphics/dau.png");
    snakeHeadTexture = LoadTextureFromImage(headImage);
    UnloadImage(headImage);
    
    Image bodyImage = LoadImage("Graphics/than.png");
    snakeBodyTexture = LoadTextureFromImage(bodyImage);
    UnloadImage(bodyImage);
    
    // Tải texture cho đồ ăn (5 loại)
    for(int i = 0; i < totalFoodTypes; i++)
    {
        string foodPath = "Graphics/Food" + to_string(i + 1) + ".png";
        Image foodImage = LoadImage(foodPath.c_str());
        foodTextures[i] = LoadTextureFromImage(foodImage);
        UnloadImage(foodImage);
    }
    
    // Tải nhạc nền
    InitAudioDevice();
    backgroundMusic = LoadMusicStream("Sounds/Nen.mp3");
    musicLoaded = true;
    PlayMusicStream(backgroundMusic);
    SetMusicVolume(backgroundMusic, 0.5f);

    Game game = Game();

    // Vòng lặp game chính
    while(WindowShouldClose() == false)
    {
        // Cập nhật nhạc nền
        if (musicLoaded)
        {
            UpdateMusicStream(backgroundMusic);
        }
        
        // Cập nhật chuyển cảnh
        UpdateTransition();

        BeginDrawing();

        if (currentScreen == MENU)
        {
            DrawMenuScreen();
        }
        else if (currentScreen == INSTRUCTIONS)
        {
            DrawInstructionsScreen();
        }
        else if (currentScreen == PLAYING)
        {
            // Logic game gốc
            if(eventTriggered(0.2))
            {
                game.Update();
            }

            if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
            {
                game.snake.direction = {0, -1};
                game.running = true;
            }
            
            if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
            {
                game.snake.direction = {0, 1};
                game.running = true;
            }

            if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
            {
                game.snake.direction = {-1, 0};
                game.running = true;
            }

            if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
            {
                game.snake.direction = {1, 0};
                game.running = true;
            }
            
            // Nút ESC để về menu
            if(IsKeyPressed(KEY_ESCAPE))
            {
                StartTransition(MENU);
            }

            // Vẽ game
            DrawAnimatedBackground();
            DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)SoOtrongMoiHangCot * kichThuocO + 10, (float)SoOtrongMoiHangCot * kichThuocO + 10}, 5, darkgreen);
            DrawText("Snake Game", offset - 5, 20, 40, darkgreen);
            DrawText(TextFormat("%i", game.score), offset - 5, offset + 5 + SoOtrongMoiHangCot * kichThuocO, 40, darkgreen);
            game.Draw();
        }
        
        // Vẽ hiệu ứng chuyển cảnh
        DrawTransition();

        EndDrawing();
    }

    // Giải phóng bộ nhớ
    if (musicLoaded)
    {
        UnloadMusicStream(backgroundMusic);
    }
    
    UnloadTexture(backgroundTexture);
    UnloadTexture(startButtonTexture);
    UnloadTexture(exitButtonTexture);
    UnloadTexture(instructionsTexture);
    UnloadTexture(upTexture);
    UnloadTexture(downTexture);
    UnloadTexture(leftTexture);
    UnloadTexture(rightTexture);
    UnloadTexture(snakeHeadTexture);
    UnloadTexture(snakeBodyTexture);
    
    // Giải phóng texture đồ ăn
    for(int i = 0; i < totalFoodTypes; i++)
    {
        UnloadTexture(foodTextures[i]);
    }
    
    CloseAudioDevice();
    CloseWindow();
    return 0;
}