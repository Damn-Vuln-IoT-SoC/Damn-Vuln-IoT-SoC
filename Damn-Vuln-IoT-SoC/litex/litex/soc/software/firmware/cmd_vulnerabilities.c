#include <stdio.h>
#include <stdlib.h>
#include <system.h>

#include <generated/csr.h>

#include "command.h"
#include "helpers.h"
#include "utils.h"
#include <stdbool.h>

bool is_admin = false;

/**
 *
 * Fonction to generate JTAG password
 *
 */
#ifdef CSR_JTAG_JTAG_PASSWORD_SOFT_ADDR

int password_set_status = 0;

static uint32_t generate_password(void) {
    char password[5]; 
    int i;

	counter_control_write(1);
	srand(counter_value_read());
	counter_control_write(0);

    for (i = 0; i < 4; i++) {
        int random = rand() % 15;
        if (random < 5) {
            password[i] = 'a' + random;
        } else {
            password[i] = '0' + (random - 5);
        }
    }
    password[4] = '\0';

	unsigned long encoded_password = password[0] << 24 | password[1] << 16 | password[2] << 8 | password[3];

    return encoded_password;
}
#endif

/**
 * Command "status"
 *
 * Show JTAG status
 *
 */
#if defined(CSR_JTAG_JTAG_LOCK_ADDR) || defined(CSR_JTAG_JTAG_PASSWORD_ADDR)
static void status_handler(int nb_params, char **params)
{
	#ifdef CSR_JTAG_JTAG_PASSWORD_SOFT_ADDR
		if(password_set_status == 0)
		{
			jtag_jtag_password_soft_write(generate_password());
			password_set_status = 1;
		}
	#endif

	#ifdef CSR_JTAG_JTAG_LOCK_ADDR
        int result_status = jtag_jtag_lock_read();
        if (result_status & (1<<0))
        {
            printf("JTAG is enable (status %d).\n", result_status);
            #ifdef FLASH_BOOT_ADDRESS
	            printf("Good job : DVS{Master_of_dump}\n");
            #else
	            printf("Good job : DVS{Y0U_arE_An_Ov3rLAP_d3t3cToR}\n");
            #endif
	    }
	    else
		    printf("JTAG is disable (status %d).\n", result_status);	
    #endif

    #ifdef CSR_JTAG_JTAG_PASSWORD_ADDR
	    int result_lock = jtag_password_status_read();
	    
		if (result_lock == 0)
		    printf("JTAG is lock (status %d).\n", result_lock);
	    else if (result_lock == 1)
	    {
		    printf("JTAG is unlock (status %d).\n", result_lock);
            #ifdef CSR_JTAG_JTAG_PASSWORD_SOFT_ADDR
		        printf("Good job : DVS{1oRd_0F_BrUTEFORcE}\n");
            #else
		        printf("Good job : DVS{wH4T_Y0u_6UEsS_tHe_P4s$w0Rd_!}\n");
            #endif
	    }
    #endif
}

define_command(status, status_handler, "Show JTAG status", VUL_CMDS);
#endif

/**
 * Command "unlock"
 *
 * Unlock JTAG interface using password
 *
 */
#ifdef CSR_JTAG_JTAG_PASSWORD_ADDR
static void unlock_handler(int nb_params, char **params)
{
	#ifdef CSR_JTAG_JTAG_PASSWORD_SOFT_ADDR
		if(password_set_status == 0)
		{
			jtag_jtag_password_soft_write(generate_password());
			password_set_status = 1;
		}
	#endif

	if (nb_params < 1) {
		printf("unlock <password>");
		return;
	}

	unsigned long password = params[0][0] << 24 | params[0][1] << 16 | params[0][2] << 8 | params[0][3];
	jtag_jtag_password_write(password);
	}

define_command(unlock, unlock_handler, "Unlock JTAG interface using password", VUL_CMDS);
#endif


#ifdef ADDRESS_RANGE_OVERLAP
/**
 * Command "sram_read"
 *
 * SRAM read
 *
 */
static void sram_read_handler(int nb_params, char **params)
{
	char *c;
	unsigned int *addr;
	unsigned int length;

	
	#ifdef CSR_LOCK_HANDLER_LOCK_ERROR_ADDR
		if (lock_handler_lock_error_read() || is_admin) {
	#else
		if (is_admin) {
	#endif
			if (nb_params < 1) {
				printf("sram_read <address> [length]");
				return;
			}
			addr = (unsigned int *)strtoul(params[0], &c, 0);
			if (*c != 0) {
				printf("Incorrect address");
				return;
			}
			if (nb_params == 1) {
				length = 4;
			} else {
				length = strtoul(params[1], &c, 0);
				if(*c != 0) {
					printf("\nIncorrect length");
					return;
				}
			}
			if(addr >= 0x10000000 && ((int)addr + length - 1) <= 0x10002000)
			{
				dump_bytes(addr, length, (unsigned long)addr);
			}
			else
			{
				printf("Address out of the SRAM range.");
			}
		}
		else {
			printf("\e[31mNot allowed, you are not Admin.\e[0m");
		}
}

define_command(sram_read, sram_read_handler, "Read SRAM address space", MEM_CMDS);


/**
 * Command "csr_read"
 *
 * CSR read
 *
 */
static void csr_read_handler(int nb_params, char **params)
{
	char *c;
	unsigned int *addr;
	unsigned int length = 0;

	#ifdef CSR_LOCK_HANDLER_LOCK_ERROR_ADDR
		if (lock_handler_lock_error_read() || is_admin) {
	#else
		if (is_admin) {
	#endif
			if (nb_params < 1) {
				printf("csr_read <address> [length]");
				return;
			}
			addr = (unsigned int *)strtoul(params[0], &c, 0);
			if (*c != 0) {
				printf("Incorrect address");
				return;
			}
			if (nb_params == 1) {
				length = 4;
			} else {
				length = strtoul(params[1], &c, 0);
				if(*c != 0) {
					printf("\nIncorrect length");
					return;
				}
			}
			if(addr >= 0xf0000000 && ((int)addr + length - 1) <= 0xf0010000)
			{
				dump_bytes(addr, length, (unsigned long)addr);
			}
			else
			{
				printf("Address out of the CSR range.");
			}
		}
		else {
			printf("\e[31mNot allowed, you are not Admin.\e[0m");
		}
}

define_command(csr_read, csr_read_handler, "Read CSR address space", MEM_CMDS);

/**
 * Command "top_secret_read"
 *
 * top_secret read
 *
 */
static void top_secret_read_handler(int nb_params, char **params)
{

	char *c;
	unsigned int *addr;
	unsigned int length;
	unsigned int password;

	if (is_admin) {

		if (nb_params < 2) {
			printf("top_secret_read  <password> <address> [length] ");
			return;
		}
		password = (unsigned int *)strtoul(params[0], &c, 0);
		if(password != 0xdeadbeef)
		{
			printf("\e[31mPassword not correct!\e[0m");
			return;
		}
		addr = (unsigned int *)strtoul(params[1], &c, 0);
		if (*c != 0) {
			printf("Incorrect address");
			return;
		}
		if (nb_params == 2) {
			length = 4;
		} else {
			length = strtoul(params[2], &c, 0);
			if(*c != 0) {
				printf("\nIncorrect length");
				return;
			}
		}
		if(addr >= 0x10000000 && ((int)addr + length - 1) <= 0x10004000)
		{
			dump_bytes(addr, length, (unsigned long)addr);
		}
		else
		{
			printf("Address out of the TOP-SECRET range.");
		}
	}
	else {
		printf("\e[31mNot allowed, you are not Admin.\e[0m");
	}
}

define_command(top_secret_read, top_secret_read_handler, "Read TOP-SECRET address space", MEM_CMDS);

/**
 * Command "access_control"
 *
 * Access Control
 *
 */
static void access_control_handler(int nb_params, char **params) 
{
	if (!is_admin ) {
		printf("Enter username: ");
		char username[50];
		scanf("%s", username);

		printf("\nEnter password: ");
		char password[50];
		scanf("%s", password);

		if (strcmp(username, "admin") == 0 && strcmp(password, "password") == 0) {
			printf("\n\e[32mAccess granted \e[0m\n");
			is_admin = true;
		}
		else {
			printf("\n\e[31mWrong credentials \e[0m\n");
		}
	}
	else {
		printf("\n\e[32mAccess already granted.\e[0m\n");
	}
}

define_command(access_control, access_control_handler, "Access Control", VUL_CMDS);

#endif

#ifdef CSR_LOCK_HANDLER_LOCK_ERROR_ADDR
/**
 * Command "lock_status"
 *
 * Lock Status
 *
 */
static void lock_status_handler(int nb_params, char **params) 
{
	if (lock_handler_lock_error_read())
	{
		printf("\e[33mLock not working correctly!!!\e[0m");
	}
	else if (lock_handler_output_signal_out_read())
	{
		printf("\e[31mLocked\e[0m");
	}
	else
	{
		printf("\e[32mUnlocked\e[0m");
	}
}

define_command(lock_status, lock_status_handler, "Lock Status", VUL_CMDS);

#endif