#ifndef LOGGER_H
#define LOGGER_H

#define NO_STYLE "\033[0m"

#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"
#define BLUE "\033[0;34m"

#define BGREEN "\033[1;32m"
#define BYELLOW "\033[1;33m"
#define BRED "\033[1;31m"
#define BBLUE "\033[1;34m"

void consoleinfo(char *);
void consolewarn(char *);
void consolelog(char *);
void consoleerror(char *);

#endif
