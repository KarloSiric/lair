#ifndef __TUI_H__
#define __TUI_H__ 

#include "l_shared.h"

typedef enum {
    STATE_CONNECT,
    STATE_NAME,
    STATE_CHAT
} tuistate_t;

void TUI_Init();
void TUI_Shutdown();
void TUI_DrawStatusBar();
void TUI_DrawInputLine();
void TUI_DrawChatWindow();
lboolean TUI_HandleInput();

#endif          /* __TUI_H__ END */
