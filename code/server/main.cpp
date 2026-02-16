/*======================================================================
   File: main.cpp
   Project: lair
   Author: ksiric <email@example.com>
   Created: 2026-02-16 01:52:19
   Last Modified by: ksiric
   Last Modified: 2026-02-16 11:55:29
   ---------------------------------------------------------------------
   Description:
       
   ---------------------------------------------------------------------
   License: 
   Company: 
   Version: 1.00a
 ======================================================================
                                                                       */
                                                                       
/* main.cpp - Lair server entry point */

#include "l_shared.h"
#include "lcommon.h"
#include "platform.h"
#include "server.h"

int main(int argc, char **argv)
{
       
    Com_Init();
    SV_Init();
     
    while ( 1 )
    {
        SV_Frame();
    }
        
    return ( 0 );
    
}
