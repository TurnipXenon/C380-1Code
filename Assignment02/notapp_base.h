#ifndef _NOTAPP_BASE_H_
#define _NOTAPP_BASE_H_

#include <setjmp.h>
#include <stdlib.h> // for malloc
#include <signal.h>

/**
 * @brief Stores context during sigsetjumps in user client and observer client
 */
sigjmp_buf env;

void handle_signal(int sig);


#endif /* _NOTAPP_BASE_H_ */