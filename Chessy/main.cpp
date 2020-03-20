#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include "workGL.h"
#include <tchar.h>
#include <stdio.h>
#include "Chess.h"


void MyText(int x, int y, const char* txt);

int  width = 1300; //размер окна
int  height = 800;
int xPos, yPos;

/* консоль для ОТЛАДКИ

void ShowConsoleWindow() {
    AllocConsole();
    freopen("CONOUT$", "wb", stdout);
    freopen("CONIN$", "rb", stdin);
    // Заголовок консоли совпадает с полным именем exe-шника
    TCHAR fullName[MAX_PATH];
    GetModuleFileName(NULL, fullName, MAX_PATH);
    HWND hWnd = FindWindow(NULL, fullName);
    SetConsoleTitle(_T("Attached to workGL.exe"));
    RECT rect;
    GetWindowRect(hWnd, &rect);
    //Shell_TrayWnd
    RECT desktopArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &desktopArea, 0);

    SetWindowPos(hWnd, HWND_TOP, 0, desktopArea.bottom - rect.bottom + rect.top, 0, 0, SWP_NOSIZE);
}
*/
//************* тут великий кусок стандартного коду для пыдключення GDI*****/////

HDC hDC;
//ссылка на OpenGL
HGLRC hGLRC;
//ссылка на окно в котором будет происходить рисование
HWND hwnd;

// фоны, их потом перенесу
HFONT font = CreateFont(20, 0, 0, 0,
    FW_BOLD, FALSE, FALSE, FALSE,
    ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	DEFAULT_PITCH | FF_ROMAN,
    "Arial");

HFONT fontBIG = CreateFont(60, 0, 0, 0,
    FW_BOLD, FALSE, FALSE, FALSE,
    ANSI_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_ROMAN,
    "Arial");
HFONT fontKor = CreateFont(16, 16, 0, 0,
    FW_BOLD, FALSE, FALSE, FALSE,
    ANSI_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_ROMAN,
    "Arial");

//"Times New Roman");

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
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
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("СhessSolver"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
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
   

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    //ShowConsoleWindow();//для отладки

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
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

HBRUSH hBrush=CreateSolidBrush(RGB(0,255,0)); //задаём сплошную кисть, закрашенную цветом RGB
HPEN pen= CreatePen(PS_NULL,0,0);

Color ClrRamka(255,100,100,255);
Color ClrRmkFon(255,240,240,240);

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
	PAINTSTRUCT ps;
    //int xPos, yPos;
    static int MS_start=0;
    static int q=0;
    char txt[30];

    TKing *pk;
    Position5 p5;

    int by = 120; // кнопки по высоте
    int hy = 50; // смещение справки по высоте
    int hx = 7; // смещение справки

    switch (message)                  /* handle the messages */
    {
    // кнопки
    case WM_CREATE: 
        CreateWindow("button", "Move", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
            780, 560 + by, 140, 40, hwnd, (HMENU)3, NULL, NULL);
        CreateWindow("button","Quick calculate",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
            950,560+by,140,40,hwnd,(HMENU)1,NULL,NULL);
        CreateWindow("button","Reset",WS_CHILD|BS_PUSHBUTTON|WS_VISIBLE,
            1120,560+by,140,40,hwnd,(HMENU)2,NULL,NULL);
        break;

    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
       
        switch (wmId)
        {
            case 1: // нажата кнопка Quick
                Make500Move();
                Draw();
                SetFocus(hwnd);
                break;
            case 2: // нажата кнопка Reset
                Board.OnStart();
                PlaySound("reset.wav", NULL, SND_ASYNC | SND_FILENAME);
                DrawFonRect();
                Pmcnt = 0;
                Draw();
                SetFocus(hwnd);
                break;
            case 3: // нажата кнопка Move
                 
                MakeNextMove();
                PlaySound("move.wav", NULL, SND_ASYNC | SND_FILENAME);
                Draw();
                SetFocus(hwnd);
                

        }
        break;
//гарячие клавиши      
	case WM_KEYDOWN: //printf("Dn=%X\n",wParam);
		switch(wParam) {
        case 27: Board.OnStart();
            PlaySound("reset.wav", NULL, SND_ASYNC | SND_FILENAME);
            DrawFonRect(); tt_cnt = 0; Pmcnt = 0; Draw(); break;//esc
        case 0x31: Board.SetPosition(&p5); WKing.CalcRP(&p5); WKing.KingCorrect(&p5);
        //case 0x31: Board.SetPosition(&p5); WKing.CalcRP(&p5); BKing.CalcRP(&p5); WKing.KingCorrectM();
            Draw(); WKing.DrawRP(); WKing.DrawCF(); break;
        case 0x32: Board.SetPosition(&p5);BKing.CalcRP(&p5); BKing.KingCorrect(&p5);
        //case 0x32: Board.SetPosition(&p5);BKing.CalcRP(&p5); WKing.CalcRP(&p5); BKing.KingCorrectM();
            Draw(); BKing.DrawRP(); BKing.DrawCF();break;
        case 0x33: Board.SetPosition(&p5); BSlon.CalcRP(&p5); BSlon.KingCorrect(&p5);
            Draw(); BSlon.DrawRP(); BSlon.DrawCF();break;
        case 0x34: Board.SetPosition(&p5); BTura.CalcRP(&p5); BTura.KingCorrect(&p5);
            Draw();BTura.DrawRP(); BTura.DrawCF();break;
        case 0x35: Board.SetPosition(&p5); BKonj.CalcRP(&p5); BKonj.KingCorrect(&p5);
            Draw();BKonj.DrawRP(); BKonj.DrawCF();break;


        case 69: Evaluate(&Board);  break; // E
        case 82: RandPosition(); DrawFonRect(); tt_cnt = 0; Pmcnt = 0; Draw(); break; // R
        
           
        case 72:  H *= -1; break; // h
           
        case 112: PrintHelp();  break; // F1 - help
        case 0x20: MakeNextMove();
            PlaySound("move.wav", NULL, SND_ASYNC | SND_FILENAME);
            Draw(); break; // space
        case 120:  Make500Move();Draw(); break; // F9
        case 0x37: TestShess100_950();Draw(); break; // 7
        case 115:  BackMove(); Draw(); break; //F4
        case 114:  FirstMove(); Draw(); break; //F3
        case 38: memo.offset++; memo.Draw(); break; // вверх
        case 40: memo.offset--; memo.Draw(); break; // вниз

        //case 83:  MyText(100, 300, "AAAAAAAAAAAAAAAAAAAAAA"); break;
            //BestMove2();  break; // t test();
		}
		break;

    case WM_LBUTTONDOWN: 
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
        PlaySound("put.wav", NULL, SND_ASYNC | SND_FILENAME);
        Draw();
        break;
        case WM_PAINT:
		BeginPaint(hwnd, &ps);

		Draw();

        Bar(890+hx, 10+hy, 1250+hx, 455+hy, ClrRmkFon);
        RectAng(890 + hx, 10 + hy, 1250 + hx, 455 + hy, White); //ClrRamka);
        PrintHelpInWindow(905, -10+hy);
        MyTextBIG(910, 8, "Chess solver");
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

HFONT hFont;
void IniFonts(HDC hdc)
{
    hFont = CreateFont(30,15,
        0,0, // углы
        400,//FW_DONTCARE, 
        FALSE, // bItalic
        FALSE,  // bUnderline
        FALSE, // bStrikeOut
        DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS,
        CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY, VARIABLE_PITCH,TEXT("Arial"));
    SelectObject(hdc,hFont);
}


void MyText(int x, int y, const char* txt)
{
    HDC hDC = GetDC(hwnd);
    SelectObject(hDC, font);
    SetTextColor(hDC,RGB(0,0,0)); //цвет текста
    //SetBkColor(hDC,RGB(0,255,255));// фон, если не использовать SetBkMode !
    SetBkMode(hDC,TRANSPARENT);
    TextOutA(hDC, x, y, txt, strlen(txt));
    ReleaseDC(hwnd, hDC); // Освобождаем контекст устройства

}

void MyTextBIG(int x, int y, const char* txt)
{
    HDC hDC = GetDC(hwnd);
    SelectObject(hDC, fontBIG);
    SetTextColor(hDC,RGB(0,0,0));// цвет текста
                                 //SetBkColor(hDC,RGB(0,255,255));// фон, если не использовать SetBkMode !
    SetBkMode(hDC,TRANSPARENT);
    TextOutA(hDC, x, y, txt, strlen(txt));
    ReleaseDC(hwnd, hDC); // Освобождаем контекст устройства

}
void MyTextKor(int x, int y, const char* txt)
{
    HDC hDC = GetDC(hwnd);
    SelectObject(hDC, fontKor);
    SetTextColor(hDC, RGB(153, 102, 0));//цвет текста
                                 //SetBkColor(hDC,RGB(0,255,255));// фон, если не использовать SetBkMode !
    SetBkMode(hDC, TRANSPARENT);
    TextOutA(hDC, x, y, txt, strlen(txt));
    ReleaseDC(hwnd, hDC); // Освобождаем контекст устройства

}
void PrintRow(int x, int y) {
    int dy = 91;
    MyTextKor(x, y , "1");
    MyTextKor(x, y += dy, "2");
    MyTextKor(x, y += dy, "3");
    MyTextKor(x, y += dy, "4");
    MyTextKor(x, y += dy, "5");
    MyTextKor(x, y += dy, "6");
    MyTextKor(x, y += dy, "7");
    MyTextKor(x, y += dy, "8");


}