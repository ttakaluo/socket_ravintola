#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "print_error.h"

int spawn_logger(void) {
	pid_t pid;
	pid = fork();

	if (pid < 0)
		error("on fork while daemonizing");

	if (pid > 0) 
		return 0;						//return to main

	if (setsid() < 0)						//create new session
		error("on session creation while daemonizing");

	if (chdir("/") < 0) 					//change to root
		error("on root dir change while daemonizing");
	
	umask(0);							//reset umask

	close(STDIN_FILENO);					//close normal file descs
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	return 0;
}
