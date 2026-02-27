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
#define MAX_CHAT_MESSAGES       256

typedef enum {
    STATE_CONNECT,
    STATE_NAME,
    STATE_CHAT
} tuistate_t;

typedef struct {
    char sender[MAX_USERNAME];
    char text[MAX_STRING_CHARS];
    char timestamp[16];
} chatmsg_t;

void TUI_Init();
void TUI_Shutdown();

void TUI_DrawStatusBar();
void TUI_DrawInputLine();
void TUI_DrawChatWindow();

void TUI_DrawConnectScreen();
void TUI_HandleConnectScreenInput();

void TUI_DrawPromptQuitScreen();
void TUI_HandleQuitInput();

void TUI_DrawNameScreen();
void TUI_HandleNameInput();
lboolean TUI_HandleNameValidation();
lboolean TUI_Frame();
lboolean TUI_HandleInput();

void TUI_AddChatMessage( const char *sender, const char *text );

#endif          /* __TUI_H__ END */
