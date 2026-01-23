#ifndef _SYSCALLS_H
#define _SYSCALLS_H


extern int _close(int );
__attribute__((weak)) extern int _write(int , char *, int );
__attribute__((weak)) extern int _read(int , char *, int );


#endif
