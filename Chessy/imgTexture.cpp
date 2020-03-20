#include "workGL.h"
#include <stdio.h>
#include "./ImTexture.h"

int ImgTexture::LoadFromFile(const wchar_t* fname){
	if(Loaded)delete bmp;
	bmp=new Bitmap(fname);
	Width=bmp->GetWidth();
	Height=bmp->GetHeight();
	Loaded=1;
	return 0;
}

void ImgTexture::DrawImage(int x, int y){
	HDC hdc = GetDC(hwnd);
	Graphics graphics(hdc);
	graphics.DrawImage(bmp, x, y, Width, Height);
	ReleaseDC(hwnd, hdc);
}

void ImgTexture::Print(float x, float y){
	DrawImage(x-Width/2, y-Height/2);
}
