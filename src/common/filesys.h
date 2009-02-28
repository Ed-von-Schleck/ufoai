/**
 * @file filesys.h
 * @brief Filesystem header file.
 */

/*
All original materal Copyright (C) 2002-2007 UFO: Alien Invasion team.

Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/


#ifndef _COMMON_FILESYS_H
#define _COMMON_FILESYS_H

#include <stdio.h>

/* Maximum maps constant */
#define	MAX_MAPS 400

/* max length of the ufo virtual filesystem */
#define MAX_QPATH           64

/* max length of a filesystem pathname
 * windows + linux 256, macosx 32 */
#define	MAX_OSPATH          256

/* max files in a directory */
#define	MAX_FILES           512

/*
==============================================================
FILESYSTEM
==============================================================
*/
typedef struct qFILE_s {
	void *z; /**< in case of the file being a zip archive */
	FILE *f; /**< in case the file being part of a pak or the actual file */
	char name[MAX_OSPATH];
	unsigned long filepos;
	unsigned long size;
} qFILE;

typedef enum {
	FS_READ,
	FS_WRITE,
	FS_APPEND,
	FS_APPEND_SYNC
} fsMode_t;

typedef enum {
	FS_SEEK_CUR,
	FS_SEEK_END,
	FS_SEEK_SET
} fsOrigin_t;

/** @brief Links one file onto another - like a symlink */
typedef struct filelink_s {
	struct filelink_s *next;
	char *from;
	int fromlength;
	char *to;
} filelink_t;

typedef struct {
	char name[MAX_QPATH];
	unsigned long filepos;
	unsigned long filelen;
} packfile_t;

typedef struct pack_s {
	char filename[MAX_OSPATH];
	qFILE handle;
	int numfiles;
	packfile_t *files;
} pack_t;

typedef struct searchpath_s {
	char filename[MAX_OSPATH];
	pack_t *pack;				/* only one of filename / pack will be used */
	struct searchpath_s *next;
} searchpath_t;

/*
==============================================================
FILESYSTEM
==============================================================
*/
extern char *fs_maps[MAX_MAPS];
extern int fs_numInstalledMaps;

int FS_FileLength(qFILE * f);
void FS_OpenFileWrite(const char *filename, qFILE * f);
int FS_Seek(qFILE * f, long offset, int origin);
int FS_WriteFile(const void *buffer, size_t len, const char *filename);
int FS_Write(const void *buffer, int len, qFILE * f);
void FS_InitFilesystem(void);
void FS_RestartFilesystem(void);
const char *FS_Gamedir(void);
const char *FS_NextPath(const char *prevpath);
void FS_ExecAutoexec(void);
const char *FS_GetCwd(void);
void FS_NormPath(char *path);
qboolean FS_FileExists(const char *filename);
void FS_SkipBlock(const char **text);

void FS_GetMaps(qboolean reset);

int FS_OpenFile(const char *filename, qFILE * file);
void FS_CloseFile(qFILE * f);

qboolean FS_RenameFile(const char *from, const char *to, qboolean relative);
void FS_RemoveFile(const char *osPath);
void FS_CopyFile(const char *fromOSPath, const char *toOSPath);

/* note: this can't be called from another DLL, due to MS libc issues */

int FS_LoadFile(const char *path, byte **buffer);

/* a null buffer will just return the file length without loading */
/* a -1 length is not present */

#ifdef DEBUG
#define FS_Read(buffer, len, f) FS_ReadDebug(buffer, len, f, __FILE__, __LINE__)
int FS_ReadDebug(void *buffer, int len, qFILE * f, const char* file, int line);
#else
int FS_Read(void *buffer, int len, qFILE * f);
#endif
/* properly handles partial reads */

void FS_FreeFile(void *buffer);

int FS_CheckFile(const char *path);

int FS_BuildFileList(const char *files);
const char* FS_NextFileFromFileList(const char *files);
char *FS_NextScriptHeader(const char *files, const char **name, const char **text);
void FS_CreatePath(const char *path);

/* Make sure we have this available */
char **FS_ListFiles(const char *findname, int *numfiles, unsigned musthave, unsigned canthave);

const char *FS_GetFileData(const char *files);

/**
 * @brief cleanup function
 */
void FS_Shutdown(void);

#endif
