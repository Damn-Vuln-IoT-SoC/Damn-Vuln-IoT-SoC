#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <system.h>
#include <string.h>
#include <irq.h>

#include "readline.h"
#include "helpers.h"
#include "command.h"

#include <generated/csr.h>
#include <generated/soc.h>
#include <generated/mem.h>
#include <generated/git.h>

#include <libbase/console.h>
#include <libbase/uart.h>

__attribute__((__used__)) int main(int i, char **c)
{
	char buffer[CMD_LINE_BUFFER_SIZE];
	char *params[MAX_PARAM];
	char *command;
	struct command_struct *cmd;
	int nb_params;

	//Enable count every tick of the clock
	counter_control_write(1);

#ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);
#endif
#ifdef CSR_UART_BASE
	uart_init();
#endif

	while (1)
	{
		if(keypad_status_read() == 0)
		{
			printf("Console is locked !\r");
			busy_wait(500);
			continue;
		}
		break;
	}
	

    printf("\n");
	printf("\e[1m    ____                           _    __      __         ____    ______   _____       ______\e[0m\n");
	printf("\e[1m   / __ \\____ _____ ___  ____     | |  / /_  __/ /___     /  _/___/_  __/  / ___/____  / ____/\e[0m\n");
	printf("\e[1m  / / / / __ `/ __ `__ \\/ __ \\    | | / / / / / / __ \\    / // __ \\/ /     \\__ \\/ __ \\/ /     \e[0m\n");
	printf("\e[1m / /_/ / /_/ / / / / / / / / /    | |/ / /_/ / / / / /  _/ // /_/ / /     ___/ / /_/ / /___   \e[0m\n");
	printf("\e[1m/_____/\\__,_/_/ /_/ /_/_/ /_/     |___/\\__,_/_/_/ /_/  /___/\\____/_/     /____/\\____/\\____/   \e[0m\n");
	printf("\n");
  printf("\e[1m                             Secure Lock Administration Panel\e[0m\n");
	printf("\n");

    init_dispatcher();

    printf("--======================================= \e[1mConsole\e[0m ==========================================--\n");
    printf("DVS{@dmIniStr4t10N-p@ne1-UNlOcK3d}\n");
    hist_init();
    printf("\n%s", PROMPT);
  while(1) 
	{
		readline(buffer, CMD_LINE_BUFFER_SIZE);
		if (buffer[0] != 0) {
			printf("\n");
			nb_params = get_param(buffer, &command, params);
			cmd = command_dispatcher(command, nb_params, params);
			if (!cmd)
				printf("Command not found");
		}
		printf("\n%s", PROMPT);
	}
}
