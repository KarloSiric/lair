#ifndef __TUI_H__
#define __TUI_H__ 

#include "l_shared.h"

#define CONNECT_WIN_HEIGHT      14
#define CONNECT_WIN_WIDTH       44

#define NAME_WIN_HEIGHT         10
#define NAME_WIN_WIDTH          40

#define QUIT_WIN_HEIGHT         5
#define QUIT_WIN_WIDTH          26

#define NAME_WIN_HEIGHT         10
#define NAME_WIN_WIDTH          40

#define SERVERIP_FIELD_WIDTH    24
#define SERVERPORT_FIELD_WIDTH  8


typedef enum {
    STATE_CONNECT,
    STATE_NAME,
    STATE_CHAT
} tuistate_t;

void TUI_Init();
void TUI_Shutdown();

// Drawing the chat main window
void TUI_DrawStatusBar();
void TUI_DrawInputLine();
void TUI_DrawChatWindow();

// Drawing the connect main screen
void TUI_DrawConnectScreen();
void TUI_HandleConnectScreenInput();

// Drawing of the quit screen
void TUI_DrawPromptQuitScreen();
void TUI_HandleQuitInput();

// Drawing of the name screen
void TUI_DrawNameScreen();
void TUI_HandleNameInput();
lboolean TUI_HandleNameValidation();

lboolean TUI_Frame();

lboolean TUI_HandleInput();
#endif          /* __TUI_H__ END */
