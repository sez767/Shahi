#pragma once
struct ImgTexture {
    int Width;  // минимальная ширина символа
    int Height; // минимальная высота символа
    int Loaded;
    Bitmap* bmp;
    void DrawImage(int x, int y);
    int LoadFromFile(const wchar_t* fname);
    void Print(float x, float y);
};

extern HFONT font;
extern HFONT fontBIG;
extern HFONT fontKor;

struct TMemo10 {
    int x, y, w, h; // лев, верх, ширина, высота
    int dy = 20;
    int ind = 0; // хвост
    int tp = 0; // top string
    int offset = 0; // смещение по клавишах
    int N; // количество отображаемых строк
    char string[100][100]; // 100 строк по 100 символов
    //Color Fon; // пока все это с MyText(...
    //Color Ton;
    //HFONT fnt;
    TMemo10(int ix, int iy, int iw, int ih) {
        x = ix, y = iy, w = iw, h = ih;
        N = 4; // количество строк по умолчанию
            //Fon = WinFon; // по умолчанию
            //Ton = Black;
            //fnt = font;
    }
    void Clear();
    void ClearBar();
    void print(const char* Format, ...);
    void Draw();

};

extern TMemo10 memo;