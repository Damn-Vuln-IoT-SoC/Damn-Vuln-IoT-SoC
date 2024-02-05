#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void admin_panel(void);
void read_top_sec(unsigned int *addr, unsigned int length);
void read_csr(unsigned int *addr, unsigned int length);
void mem_write(void *addr, unsigned int value, unsigned int count, unsigned int size);
