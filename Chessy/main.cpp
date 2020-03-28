#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <stdio.h>
#include "workGL.h"

#include "TBoard.h"

//***** тут большой кусок стандартного коду для подключения GDI+ и окон виндовс/////

HDC hDC; //ссылка на OpenGL
HGLRC hGLRC; //ссылка на окно в котором будет происходить рисование
HWND hwnd;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                     LPSTR lpszArgument, int nCmdShow){

    //HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_HREDRAW | CS_VREDRAW|CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH)4;////COLOR_BACKGROUND

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("СhessSolver"),   /* имя */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,      /* Windows decides the position */// 
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           width,                 /* The programs width */
           height,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    
    init_img();
    Shess_init();
    //ShowConsoleWindow();

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0)){
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

	GdiplusShutdown(gdiplusToken);
	//deinit_img();
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

HFONT hFont;
void IniFonts(HDC hdc) {
    hFont = CreateFont(30, 15,
        0, 0, // углы
        400,//FW_DONTCARE, 
        FALSE, // bItalic
        FALSE,  // bUnderline
        FALSE, // bStrikeOut
        DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
    SelectObject(hdc, hFont);
}

HBRUSH hBrush=CreateSolidBrush(RGB(0,255,0)); //задаём сплошную кисть, цветом RGB
HPEN pen= CreatePen(PS_NULL,0,0);
int xPos, yPos;
int matFlg = 0;

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message,
    WPARAM wParam, LPARAM lParam){
    int wmId, wmEvent;
	PAINTSTRUCT ps;
    static int MS_start=0;
    static int q=0;
    char txt[30];

    TKing *pk;
    Position5 p5;
    int by = 120; // кнопки по высоте
    int hy = 50; // смещение справки по высоте
    int hx = 7; // смещение справки
    
 switch (message){         /* handle the messages */
    // кнопки рисуем
    case WM_CREATE: 
        CreateWindow("button", "Move", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
            770, 570 + by, 110, 40, hwnd, (HMENU)3, NULL, NULL);
        CreateWindow("button", "Random", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
            895, 570 + by, 110, 40, hwnd, (HMENU)4, NULL, NULL);
        CreateWindow("button","Quick calculate",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
            1020,570+by,110,40,hwnd,(HMENU)1,NULL,NULL);
        CreateWindow("button","Reset",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
            1145,570+by,110,40,hwnd,(HMENU)2,NULL,NULL);
        break;

    case WM_COMMAND: // кнопки нажимаем
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        
        switch (wmId){
            case 1: // нажата кнопка Quick
                if (matFlg != 1 && Board.Fig[1]->Enbl == 1 && Board.Fig[0]->Enbl == 1) {
                    Make500Move();
                    Draw();
                    SetFocus(hwnd);
                }
                break;
            case 2: // нажата кнопка Reset
                Board.OnStart();
                PlaySound("src/reset.wav", NULL, SND_ASYNC | SND_FILENAME);
                //DrawFonRect();
                Pmcnt = 0;
                count = 1;
                matFlg = 0;
                Draw();
                SetFocus(hwnd);
                break;
            case 3: // нажата кнопка Move
                if (matFlg != 1 && Board.Fig[1]->Enbl == 1 && Board.Fig[0]->Enbl == 1) {
                    MakeNextMove();
                    PlaySound("src/move.wav", NULL, SND_ASYNC | SND_FILENAME);
                    Draw();
                    SetFocus(hwnd);
                }
                break;
            case 4: // нажата кнопка Рандом
                RandPosition();
                tt_cnt = 0;
                Pmcnt = 0;
                count = 1;
                Draw();
                matFlg = 0;
                PlaySound("src/move.wav", NULL, SND_ASYNC | SND_FILENAME);
                SetFocus(hwnd);
                 break;
        }
        break;
     
	case WM_KEYDOWN: //гарячие клавиши 
		switch(wParam) {
            case 27: Board.OnStart();
                PlaySound("src/reset.wav", NULL, SND_ASYNC | SND_FILENAME);
                tt_cnt = 0; Pmcnt = 0; matFlg = 0; Draw(); break;//ESC
            case 0x31: Board.SetPosition(&p5); WKing.CalcRP(&p5); WKing.KingCorrect(&p5);
                  Draw(); WKing.DrawRP(); WKing.DrawCF(); break;
            case 0x32: Board.SetPosition(&p5);BKing.CalcRP(&p5); BKing.KingCorrect(&p5);
                  Draw(); BKing.DrawRP(); BKing.DrawCF();break;
            case 0x33: Board.SetPosition(&p5); BSlon.CalcRP(&p5); BSlon.KingCorrect(&p5);
                Draw(); BSlon.DrawRP(); BSlon.DrawCF();break;
            case 0x34: Board.SetPosition(&p5); BTura.CalcRP(&p5); BTura.KingCorrect(&p5);
                Draw();BTura.DrawRP(); BTura.DrawCF();break;
            case 0x35: Board.SetPosition(&p5); BKonj.CalcRP(&p5); BKonj.KingCorrect(&p5);
                Draw();BKonj.DrawRP(); BKonj.DrawCF();break;
            case 69: Evaluate(&Board);  break; // E
            case 82: RandPosition(); 
                PlaySound("src/move.wav", NULL, SND_ASYNC | SND_FILENAME);
                tt_cnt = 0; Pmcnt = 0; Draw();count = 1; matFlg = 0; break; // R
            case 72:  H *= -1;
                if (H == 1) { Bar(800, 20, 830, 50, Black); }
                else { Bar(800, 20, 830, 50, White); } break; // H
            case 0x20: if (matFlg != 1 && Board.Fig[1]->Enbl ==1 && Board.Fig[0]->Enbl == 1) {
                MakeNextMove();PlaySound("src/move.wav", NULL, SND_ASYNC | SND_FILENAME);
                Draw();}break; // space
            case 81:if (matFlg != 1 && Board.Fig[1]->Enbl == 1 && Board.Fig[0]->Enbl == 1) {
                Make500Move();Draw();}break; // Q
            case 114: TestShess100_950();Draw(); break; // F3
            case 115:  BackMove(); Draw(); Pmcnt--; break; //F4
            case 38: memo.offset++; memo.Draw(); break; // вверх
            case 40: memo.offset--; memo.Draw(); break; // вниз
		}
		break;

    case WM_LBUTTONDOWN: //клик мышки
        MS_start=1;
        xPos = LOWORD(lParam);  // horizontal position of cursor 
        yPos = HIWORD(lParam);  // vertical position of cursor 
        q=Board.FindFigOnMouse(xPos, yPos);//тип фигуры при наж кнопки
        break;

    case WM_LBUTTONUP:
        xPos = LOWORD(lParam);  // horizontal position of cursor 
        yPos = HIWORD(lParam);  // vertical position of cursor 
        MS_start=0;
        if(q>=0 && q<5 && Board.InBoardRect(xPos,yPos))
            Board.SetFigCoord(xPos, yPos, q);//координати доски при видпускании кнопки
        PlaySound("src/put.wav", NULL, SND_ASYNC | SND_FILENAME);
        Draw();
        break;

     case WM_PAINT: // рисуем
		BeginPaint(hwnd, &ps);
        Draw();
        Bar(890+hx, 30+hy, 1250+hx, 600+hy, ClrRmkFon);
        RectAng(889 + hx, 30 + hy, 1250 + hx, 440 + hy, SwKorr); 
        PrintHelpInWindow(910, 10+hy);
        MyTextBIG(910, 8, "INTITA  CHESS");
        MyTextBl(1005, 58, "TASKS SOLVER");
        MyTextKor(50, 735, "A        B        C        D         E         F         G        H");
        PrintRow(737, 45);
        
        EndPaint(hwnd, &ps);
		break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
