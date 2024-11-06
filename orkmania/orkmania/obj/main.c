#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include <locale.h>

typedef struct {
    Vector2 position;
    Texture2D texture;
    float scale;
    int health;
    int maxHealth;
    const char* name;
     int birim_sayisi;   // Birim adı
} Mizrakci;

typedef struct {
    Vector2 position;
    Texture2D texture;
    float scale;
    int health;
    int maxHealth;
    const char* name;
     int birim_sayisi;   // Birim adı
} KusatmaMakinesi;

typedef struct {
    Vector2 position;
    Texture2D texture;
    float scale;
    int health;
    int maxHealth;
    const char* name;
     int birim_sayisi;   // Birim adı
} Suvari;
typedef struct {
    Vector2 position;
    Texture2D texture;
    float scale;
    int health;
    int maxHealth;
    const char* name;
    int birim_sayisi;  // Birim adı
} Okcu;
typedef struct {
    Vector2 position;
    Texture2D texture;
    float scale;
    int health;
    int maxHealth;
    const char* name;
     int birim_sayisi;   // Birim adı
} Trol;

typedef struct {
    Vector2 position;
    Texture2D texture;
    float scale;
    int health;
    int maxHealth;
    const char* name;
     int birim_sayisi;   // Birim adı
} Varg_Binicisi;

typedef struct {
    Vector2 position;
    Texture2D texture;
    float scale;
    int health;
    int maxHealth;
    const char* name;
     int birim_sayisi;   // Birim adı
} Piyade;

typedef struct {
    Vector2 position;
    Texture2D texture;
    float scale;
    int health;
    int maxHealth;
    const char* name;
     int birim_sayisi;   // Birim adı
} Ork_Dovuscusu;

int screenWidth = 1500;
int screenHeight = 1024;
int gridSize = 1024;
int squareSize = 40;  // Kare boyutu 40x40
float scaleFactor = 1.0f;

// Can barını çizen fonksiyon
void DrawHealthBar(Vector2 position, int health, int maxHealth, float scaleFactor) {
    float barWidth = 40 * scaleFactor;  // Can barının genişliği
    float barHeight = 8 * scaleFactor;  // Can barının yüksekliği
    float healthPercentage = (float)health / (float)maxHealth;  // Can yüzdesi
    float filledWidth = barWidth * healthPercentage;  // Dolu olan kısmın genişliği

    // Can barının arkaplanını çiz (kırmızı)
    DrawRectangle(position.x, position.y - 10 * scaleFactor, barWidth, barHeight, RED);

    // Can barının dolu kısmını çiz (yeşil)
    DrawRectangle(position.x, position.y - 10 * scaleFactor, filledWidth, barHeight, GREEN);

    // Can barının etrafına sınır çiz
    DrawRectangleLines(position.x, position.y - 10 * scaleFactor, barWidth, barHeight, BLACK);
}

int main() {
    InitWindow(screenWidth, screenHeight, "Selam");
    SetTargetFPS(60);

    srand(time(NULL));

    // Arka plan resmini yükle
    Texture2D backgroundTexture = LoadTexture("background.png");

    // Texture'ları yükle
    Texture2D mizrakciTexture = LoadTexture("mizrakci.png");
    Texture2D kusatmaTexture = LoadTexture("kusatma_makinesi.png");
    Texture2D suvariTexture = LoadTexture("suvari.png");
    Texture2D okcuTexture = LoadTexture("okcu.png");
    Texture2D trolTexture = LoadTexture("trol.png");
    Texture2D vargBinicisiTexture = LoadTexture("varg_binicisi.png");
    Texture2D piyadeTexture = LoadTexture("piyade.png");
    Texture2D orkDovuscusuTexture = LoadTexture("ork_dovuscusu.png");

    // Mizrakcı birimini mavi bölümde tanımla
    Mizrakci mizrakci = { (Vector2){ (rand() % 10 + 10) * squareSize, (rand() % 25) * squareSize }, mizrakciTexture, 0.8f, 100, 100, "Mizrakcı",100 };

    // Kuşatma makinesini tanımla
    KusatmaMakinesi kusatma = { (Vector2){ (rand() % 10) * squareSize, (rand() % 25) * squareSize }, kusatmaTexture, 0.8f, 200, 200, "Kuşatma Makinesi",100};

    Suvari suvari =  { (Vector2){ (rand() % 10) * squareSize, (rand() % 25) * squareSize }, suvariTexture, 0.8f, 150, 150, "Süvari",100 };
    Okcu okcu = { (Vector2){ (rand() % 10) * squareSize, (rand() % 25) * squareSize }, okcuTexture, 0.8f, 120, 120, "Okçu",100 };
    // Trol birimini tanımla (sadece mavi bölümde olacak)
    Trol trol = { (Vector2){ (rand() % 10 + 10) * squareSize, (rand() % 25) * squareSize }, trolTexture, 0.7f, 180, 180, "Trol",100 };

    // Varg_Binicisi birimini mavi bölgeye taşımak için
    Varg_Binicisi vargBinicisi = { (Vector2){ (rand() % 10 + 10) * squareSize, (rand() % 25) * squareSize }, vargBinicisiTexture, 0.8f, 150, 150, "Varg Binicisi",100 };

    // Piyade birimini kırmızı bölgeye taşımak için
    Piyade piyade = { (Vector2){ (rand() % 10) * squareSize, (rand() % 25) * squareSize }, piyadeTexture, 0.8f, 120, 120, "Piyade",100 };

    // Ork dövüşçüsü birimini mavi bölgeye yerleştirmek için
    Ork_Dovuscusu orkDovuscusu = { (Vector2){ (rand() % 10 + 10) * squareSize, (rand() % 25) * squareSize }, orkDovuscusuTexture, 0.8f, 130, 130, "Ork Dovuscusu",100};

    while (!WindowShouldClose()) {
        int scaledSquareSize = squareSize * scaleFactor;

        // Izgarayı tam ortaya yerleştir
        int gridOffsetX = (screenWidth - (scaledSquareSize * 20)) / 2;
        int gridOffsetY = (screenHeight - (scaledSquareSize * 25)) / 2;

        BeginDrawing();
        ClearBackground(RAYWHITE);  // Arka plan rengi beyaz yapılır

        DrawTextureEx(backgroundTexture, (Vector2){ 0, 0 }, 0.0f, (float)screenWidth / backgroundTexture.width, WHITE);

        // Izgarayı çiz
        for (int row = 0; row < 25; row++) {
            for (int col = 0; col < 20; col++) {
                int x = gridOffsetX + col * scaledSquareSize;
                int y = gridOffsetY + row * scaledSquareSize;

                Color fillColor = (col < 10) ? RED : BLUE;  // İlk 10 sütun kırmızı, sonraki 10 sütun mavi

                DrawRectangle(x, y, scaledSquareSize, scaledSquareSize, fillColor);
                DrawRectangleLines(x, y, scaledSquareSize, scaledSquareSize, BLACK);
            }
        }

       // Mizrakcı'yı çiz
Vector2 mizrakciPosition = (Vector2){
    gridOffsetX + mizrakci.position.x * scaleFactor,
    gridOffsetY + mizrakci.position.y * scaleFactor
};
DrawTextureEx(mizrakci.texture, mizrakciPosition, 0.0f, mizrakci.scale * scaleFactor, WHITE);
DrawHealthBar(mizrakciPosition, mizrakci.health, mizrakci.maxHealth, mizrakci.scale * scaleFactor);
DrawText(mizrakci.name, mizrakciPosition.x, mizrakciPosition.y + 40 * scaleFactor, 10 * scaleFactor, BLACK);
DrawText(TextFormat("Birim Sayısı: %d", mizrakci.birim_sayisi), mizrakciPosition.x, mizrakciPosition.y + 50 * scaleFactor, 10 * scaleFactor, BLACK);

// Kuşatma makinesini çiz
Vector2 kusatmaPosition = (Vector2){
    gridOffsetX + kusatma.position.x * scaleFactor,
    gridOffsetY + kusatma.position.y * scaleFactor
};
DrawTextureEx(kusatma.texture, kusatmaPosition, 0.0f, kusatma.scale * scaleFactor, WHITE);
DrawHealthBar(kusatmaPosition, kusatma.health, kusatma.maxHealth, kusatma.scale * scaleFactor);
DrawText(kusatma.name, kusatmaPosition.x, kusatmaPosition.y + 40 * scaleFactor, 10 * scaleFactor, BLACK);
DrawText(TextFormat("Birim Sayısı: %d", kusatma.birim_sayisi), kusatmaPosition.x, kusatmaPosition.y + 50 * scaleFactor, 10 * scaleFactor, BLACK);

// Trol'ü çiz
Vector2 trolPosition = (Vector2){
    gridOffsetX + trol.position.x * scaleFactor,
    gridOffsetY + trol.position.y * scaleFactor
};
DrawTextureEx(trol.texture, trolPosition, 0.0f, trol.scale * scaleFactor, WHITE);
DrawHealthBar(trolPosition, trol.health, trol.maxHealth, trol.scale * scaleFactor);
DrawText(trol.name, trolPosition.x, trolPosition.y + 40 * scaleFactor, 10 * scaleFactor, BLACK);
DrawText(TextFormat("Birim Sayısı: %d", trol.birim_sayisi), trolPosition.x, trolPosition.y + 50 * scaleFactor, 10 * scaleFactor, BLACK);

// Varg Binicisi'ni çiz
Vector2 vargPosition = (Vector2){
    gridOffsetX + vargBinicisi.position.x * scaleFactor,
    gridOffsetY + vargBinicisi.position.y * scaleFactor
};
DrawTextureEx(vargBinicisi.texture, vargPosition, 0.0f, vargBinicisi.scale * scaleFactor, WHITE);
DrawHealthBar(vargPosition, vargBinicisi.health, vargBinicisi.maxHealth, vargBinicisi.scale * scaleFactor);
DrawText(vargBinicisi.name, vargPosition.x, vargPosition.y + 40 * scaleFactor, 10 * scaleFactor, BLACK);
DrawText(TextFormat("Birim Sayısı: %d", vargBinicisi.birim_sayisi), vargPosition.x, vargPosition.y + 50 * scaleFactor, 10 * scaleFactor, BLACK);

// Süvari'yi çiz
Vector2 suvariPosition = (Vector2){
    gridOffsetX + suvari.position.x * scaleFactor,
    gridOffsetY + suvari.position.y * scaleFactor
};
DrawTextureEx(suvari.texture, suvariPosition, 0.0f, suvari.scale * scaleFactor, WHITE);
DrawHealthBar(suvariPosition, suvari.health, suvari.maxHealth, suvari.scale * scaleFactor);
DrawText(suvari.name, suvariPosition.x, suvariPosition.y + 40 * scaleFactor, 10 * scaleFactor, BLACK);
DrawText(TextFormat("Birim Sayısı: %d", suvari.birim_sayisi), suvariPosition.x, suvariPosition.y + 50 * scaleFactor, 10 * scaleFactor, BLACK);

// Okçu'yu çiz
Vector2 okcuPosition = (Vector2){
    gridOffsetX + okcu.position.x * scaleFactor,
    gridOffsetY + okcu.position.y * scaleFactor
};
DrawTextureEx(okcu.texture, okcuPosition, 0.0f, okcu.scale * scaleFactor, WHITE);
DrawHealthBar(okcuPosition, okcu.health, okcu.maxHealth, okcu.scale * scaleFactor);
DrawText(okcu.name, okcuPosition.x, okcuPosition.y + 40 * scaleFactor, 10 * scaleFactor, BLACK);
DrawText(TextFormat("Birim Sayısı: %d", okcu.birim_sayisi), okcuPosition.x, okcuPosition.y + 50 * scaleFactor, 10 * scaleFactor, BLACK);

// Piyade'yi çiz
Vector2 piyadePosition = (Vector2){
    gridOffsetX + piyade.position.x * scaleFactor,
    gridOffsetY + piyade.position.y * scaleFactor
};
DrawTextureEx(piyade.texture, piyadePosition, 0.0f, piyade.scale * scaleFactor, WHITE);
DrawHealthBar(piyadePosition, piyade.health, piyade.maxHealth, piyade.scale * scaleFactor);
DrawText(piyade.name, piyadePosition.x, piyadePosition.y + 40 * scaleFactor, 10 * scaleFactor, BLACK);
DrawText(TextFormat("Birim Sayısı: %d", piyade.birim_sayisi), piyadePosition.x, piyadePosition.y + 50 * scaleFactor, 10 * scaleFactor, BLACK);

// Ork Dövüşçüsü'nü çiz
Vector2 orkPosition = (Vector2){
    gridOffsetX + orkDovuscusu.position.x * scaleFactor,
    gridOffsetY + orkDovuscusu.position.y * scaleFactor
};
DrawTextureEx(orkDovuscusu.texture, orkPosition, 0.0f, orkDovuscusu.scale * scaleFactor, WHITE);
DrawHealthBar(orkPosition, orkDovuscusu.health, orkDovuscusu.maxHealth, orkDovuscusu.scale * scaleFactor);
DrawText(orkDovuscusu.name, orkPosition.x, orkPosition.y + 40 * scaleFactor, 10 * scaleFactor, BLACK);
DrawText(TextFormat("Birim Sayısı: %d", orkDovuscusu.birim_sayisi), orkPosition.x, orkPosition.y + 50 * scaleFactor, 10 * scaleFactor, BLACK);


        EndDrawing();
    }

    // Texture'ları ve pencereyi kapat
    UnloadTexture(backgroundTexture);
    UnloadTexture(mizrakciTexture);
    UnloadTexture(kusatmaTexture);
    UnloadTexture(suvariTexture);
    UnloadTexture(okcuTexture);
    UnloadTexture(trolTexture);
    UnloadTexture(vargBinicisiTexture);
    UnloadTexture(piyadeTexture);
    UnloadTexture(orkDovuscusuTexture);
    CloseWindow();

    return 0;
}
