
extern unsigned short *videoBuffer;


void drawImage3(int r, int c, int width, int height, const u16* image);
void setPixel(int row, int col, unsigned short color);
void setBG(volatile u16 bg);
void placePlayer(Player p, const u16* image);
void movePlayer();
void updateMenu();
void gameplayScene();
void gameOverScene();

void drawRectangle(int row, int col, int height, int width, volatile unsigned short color);
