/**
 * Author: K_gadenflm
 * Description: header for Server Manager.
 */

#ifndef _SERVER_MANAGER_H
#define _SERVER_MANAGER_H
#include <stdio.h>

void show_info() {
    putchar('\n'); putchar('\n'); putchar('\n');
    printf(" _______________________________________________________________ \n"
           "|                     Server Manager V0.0                       |\n"
           "|                                      Author:K_gadenflm        |\n"
           "|_______________________________________________________________|\n\n"
           "-------------------------- Help Menu --------------------------- \n"
           "Test the SDK------------------------------------------------( 1 ) \n"
           "Help -------------------------------------------------------( h ) \n"
           "Configure Options-------------------------------------------( o ) \n"
           "Quit--------------------------------------------------------( q ) \n");
}

#endif
