#ifndef PAK_H
#define PAK_H

#include <stdint.h>

struct s_PakFile;
typedef struct s_PakFile PakFile;

PakFile *readPakFile(const char *filename);
void *readFile(PakFile *pf, const char *filename, int *size);
void freePak(PakFile *pf);

#endif
