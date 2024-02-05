// SPDX-License-Identifier: BSD-Source-Code

#include <stdio.h>
#include <stdlib.h>
#include <system.h>

#include <generated/csr.h>

#include "../command.h"
#include "../helpers.h"

/**
 * Command "help"
 *
 * Print a list of available commands with their help text
 *
 */
static void help_handler(int nb_params, char **params)
{
	struct command_struct * const *cmd;
	int i, not_empty;
	char title[128] = "";

	#ifdef CSR_LOCK_HANDLER_LOCK_ERROR_ADDR
		if (lock_handler_lock_error_read()){
			sprintf(title, "\nLock Admin Tool (\e[31mRecovery Mode\e[0m), available commands:\n");
		}
		else {
			sprintf(title, "\nLock Admin Tool (\e[32mNormal Mode\e[0m), available commands:\n");
		}
	#else
		sprintf(title, "\nLock Admin Tool, available commands:\n");
	#endif

	puts(title);

	for (i = 0; i < NB_OF_GROUPS; i++) {
		not_empty = 0;
		for (cmd = __bios_cmd_start; cmd != __bios_cmd_end; cmd++) {
			if ((*cmd)->group == i) {
				printf("%-24s - %s\n", (*cmd)->name, (*cmd)->help ? (*cmd)->help : "-");
				not_empty = 1;
			}
		}
		if (not_empty)
			printf("\n");
	}
}

define_command(help, help_handler, "Print this help", SYSTEM_CMDS);

/**
 * Command "leds"
 *
 * Set Leds value
 *
 */
#ifdef CSR_LEDS_BASE
static void leds_handler(int nb_params, char **params)
{
	char *c;
	unsigned int value;

	if (nb_params < 1) {
		printf("leds <value>");
		return;
	}

	value = strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect value");
		return;
	}

	printf("Settings Leds to 0x%x", value);
	leds_out_write(value);
}

define_command(leds, leds_handler, "Set Leds value", SYSTEM_CMDS);
#endif

/**
 * Command "reboot"
 *
 * Reset the system
 *
 */
static void reboot_handler(int nb_params, char **params)
{
	ctrl_reset_write(1);
}

define_command(reboot, reboot_handler, "Reboot the system", SYSTEM_CMDS);