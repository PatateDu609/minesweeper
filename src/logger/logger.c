#include "logger/logger.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

#define DATE_SIZE 1024

static void print_date(char *date)
{
	const time_t t = time(NULL);
	const struct tm *current_date = localtime(&t);
	size_t len = strftime(date, DATE_SIZE, "%D %r", current_date);

	for (size_t i = 0; i < len; i++)
		date[i] = tolower(date[i]);
}

static void internal_log(char *bold, char *regular, char *msg)
{
	char date[DATE_SIZE];

	print_date(date);
	printf("%s[%s]%s - %d - %s%s%s\n", bold, date, NO_STYLE, getpid(), regular, msg, NO_STYLE);
}

void consoleinfo(char *msg)
{
	internal_log(BBLUE, BLUE, msg);
}

void consolelog(char *msg)
{
	internal_log(BGREEN, GREEN, msg);
}

void consolewarn(char *msg)
{
	internal_log(BYELLOW, YELLOW, msg);
}

void consoleerror(char *msg)
{
	internal_log(BRED, RED, msg);
}
