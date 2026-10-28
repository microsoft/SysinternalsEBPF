// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static int testOpen(const char *path, int flags, ...);
static ssize_t testWrite(int fd, const void *data, size_t size);
static int testFchmod(int fd, mode_t mode);
static int testClose(int fd);

#define open testOpen
#define write testWrite
#define fchmod testFchmod
#define close testClose
#include "../installer.c"
#undef open
#undef write
#undef fchmod
#undef close

static bool raceCreate;
static bool raceChmod;
static bool failChmod;
static bool failClose;
static int writeError;
static bool zeroWrite;
static bool shortWrite;
static const char payload[] = "new payload";

static int testOpen(const char *path, int flags, ...)
{
    mode_t mode = 0;
    if (flags & O_CREAT) {
        va_list args;
        va_start(args, flags);
        mode = va_arg(args, int);
        va_end(args);
        if (raceCreate) {
            raceCreate = false;
            assert(symlink("sentinel", path) == 0);
        }
    }
    return open(path, flags, mode);
}

static ssize_t testWrite(int fd, const void *data, size_t size)
{
    if (writeError) {
        errno = writeError;
        writeError = 0;
        return -1;
    }
    if (zeroWrite) {
        zeroWrite = false;
        return 0;
    }
    return write(fd, data, shortWrite && size > 2 ? 2 : size);
}

static int testFchmod(int fd, mode_t mode)
{
    if (raceChmod) {
        raceChmod = false;
        assert(rename("output", "original") == 0);
        assert(symlink("sentinel", "output") == 0);
    }
    if (failChmod) {
        failChmod = false;
        errno = EPERM;
        return -1;
    }
    return fchmod(fd, mode);
}

static int testClose(int fd)
{
    int ret = close(fd);
    if (failClose) {
        failClose = false;
        errno = EIO;
        return -1;
    }
    return ret;
}

static void checkFile(const char *path, const char *contents, mode_t mode)
{
    struct stat st;
    char data[64] = {0};
    int fd = open(path, O_RDONLY | O_NOFOLLOW);
    assert(fd >= 0);
    assert(fstat(fd, &st) == 0);
    assert(S_ISREG(st.st_mode));
    assert((st.st_mode & 0777) == mode);
    assert(read(fd, data, sizeof(data)) == (ssize_t)strlen(contents));
    assert(strcmp(data, contents) == 0);
    assert(close(fd) == 0);
}

static bool install(bool force)
{
    return dropFile("output", payload, payload + strlen(payload), force, 0644);
}

int main(void)
{
    char scratch[] = "/tmp/sysinternalsebpf-installer-test.XXXXXX";
    int cwd = open(".", O_RDONLY | O_DIRECTORY);
    assert(cwd >= 0);
    assert(mkdtemp(scratch) != NULL);
    assert(chdir(scratch) == 0);
    umask(0);

    assert(dropFile("sentinel", "untouched", "untouched" + 9, true, 0600));
    assert(install(false));
    checkFile("output", payload, 0644);
    assert(dropFile("output", "ignored", "ignored" + 7, false, 0600));
    checkFile("output", payload, 0600);

    // Forced replacement must not truncate a hard-linked or mapped old inode.
    assert(link("output", "original") == 0);
    assert(dropFile("output", "replacement", "replacement" + 11, true, 0644));
    checkFile("original", payload, 0600);
    checkFile("output", "replacement", 0644);
    assert(unlink("original") == 0);
    assert(unlink("output") == 0);

    assert(symlink("sentinel", "output") == 0);
    assert(!install(false));
    checkFile("sentinel", "untouched", 0600);
    assert(install(true));
    checkFile("sentinel", "untouched", 0600);
    checkFile("output", payload, 0644);
    assert(unlink("output") == 0);

    assert(symlink("missing", "output") == 0);
    assert(!install(false));
    assert(access("missing", F_OK) < 0 && errno == ENOENT);
    assert(install(true));
    assert(unlink("output") == 0);

    assert(mkdir("output", 0700) == 0);
    assert(!install(false));
    assert(!install(true));
    assert(rmdir("output") == 0);
    assert(mkfifo("output", 0600) == 0);
    assert(!install(false));
    assert(unlink("output") == 0);

    for (int force = 0; force < 2; force++) {
        if (force)
            assert(install(true));
        raceCreate = true;
        assert(!install(force));
        checkFile("sentinel", "untouched", 0600);
        assert(unlink("output") == 0);
    }

    assert(install(true));
    raceChmod = true;
    assert(dropFile("output", payload, payload + strlen(payload), false, 0600));
    checkFile("original", payload, 0600);
    checkFile("sentinel", "untouched", 0600);
    assert(unlink("original") == 0);
    assert(unlink("output") == 0);

    writeError = EINTR;
    shortWrite = true;
    assert(install(true));
    checkFile("output", payload, 0644);
    shortWrite = false;
    writeError = ENOSPC;
    assert(!install(true));
    zeroWrite = true;
    assert(!install(true));
    failChmod = true;
    assert(!install(false));
    failClose = true;
    assert(!install(true));
    failClose = true;
    assert(!install(false));
    assert(dropFile("output", payload, payload, true, 0644));
    checkFile("output", "", 0644);

    assert(unlink("output") == 0);
    assert(unlink("sentinel") == 0);
    assert(fchdir(cwd) == 0);
    assert(close(cwd) == 0);
    assert(rmdir(scratch) == 0);
    puts("installer regression tests passed");
    return 0;
}
