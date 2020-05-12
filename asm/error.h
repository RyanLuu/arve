#ifndef ERROR_H
#define ERROR_H

#define ERROR_EFFECT(s) "\x1b[91;1m" s "\x1b[0m"
#define BOLD(s) "\x1b[1m" s "\x1b[0m"

extern void print_lineerror(char *, ...);
extern void print_error(char *, ...);

#endif /* ERROR_H */
