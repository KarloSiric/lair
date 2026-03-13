#ifndef __TUI_H__
#define __TUI_H__

#include "l_shared.h"
#include <ncurses.h>

/*
==============================================================
WINDOW DIMENSIONS
==============================================================
*/

#define CONNECT_WIN_HEIGHT      14
#define CONNECT_WIN_WIDTH       44
#define NAME_WIN_HEIGHT         10
#define NAME_WIN_WIDTH          40
#define QUIT_WIN_HEIGHT         7
#define QUIT_WIN_WIDTH          30
#define SERVERIP_FIELD_WIDTH    24
#define SERVERPORT_FIELD_WIDTH  8
#define MAX_CHAT_MESSAGES       256

/*
==============================================================
COLOR PAIRS
==============================================================
*/

typedef enum {
    COL_DEFAULT = 0,
    COL_STATUS_BAR,         // White on blue - status bar
    COL_CHAT_SYSTEM,        // Green - system messages
    COL_CHAT_SELF,          // Cyan - your own messages
    COL_CHAT_OTHER,         // White - other users
    COL_CHAT_TIMESTAMP,     // Dim/gray - timestamps
    COL_INPUT_LINE,         // Yellow - input prompt
    COL_DIALOG_BORDER,      // Cyan - dialog borders
    COL_DIALOG_TITLE,       // White bold - dialog titles
    COL_BUTTON_NORMAL,      // White - normal buttons
    COL_BUTTON_SELECTED,    // Black on cyan - selected
    COL_FIELD_ACTIVE,       // Black on cyan - active field
    COL_ERROR,              // Red - errors
    COL_COUNT
} tuicolor_t;

/*
==============================================================
DOUBLE-LINE BOX CHARACTERS (Unicode)
==============================================================
*/

#define BOX_CORNER_TL   "\u2554"  // ╔

#define BOX_CORNER_TR   "\u2557"  // ╗

#define BOX_CORNER_BL   "\u255A"  // ╚

#define BOX_CORNER_BR   "\u255D"  // ╝

#define BOX_HORIZ       "\u2550"  // ═

#define BOX_VERT        "\u2551"  // ║

#define BOX_TEE_LEFT    "\u2560"  // ╠

#define BOX_TEE_RIGHT   "\u2563"  // ╣

#define BOX_TEE_TOP     "\u2566"  // ╦

#define BOX_TEE_BOTTOM  "\u2569"  // ╩

#define BOX_CROSS       "\u256C"  // ╬

/*
==============================================================
TUI STATE
==============================================================
*/

typedef enum {
    STATE_CONNECT,
    STATE_NAME,
    STATE_CHAT
} tuistate_t;

typedef enum {
    DISPLAY_CHAT,
    DISPLAY_SYSTEM,
    DISPLAY_ERROR
} displaytype_t;

typedef struct {
    displaytype_t type;
    char sender[MAX_USERNAME];
    char text[MAX_STRING_CHARS];
    char timestamp[16];
} chatmsg_t;

void TUI_Init( void );
void TUI_Shutdown( void );
void TUI_HandleResize( void );
void TUI_InitColors( void );
void TUI_DrawDoubleBox( WINDOW *win, int starty, int startx, int height, int width );

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
