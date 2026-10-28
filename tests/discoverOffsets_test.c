// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char *testMkdtemp(char *path);
static int testOpen(const char *path, int flags, ...);
static int testFchmod(int fd, mode_t mode);
static int testFchown(int fd, uid_t uid, gid_t gid);

#define mkdtemp testMkdtemp
#define open testOpen
#define fchmod testFchmod
#define fchown testFchown
#include "../discoverOffsets.c"
#undef mkdtemp
#undef open
#undef fchmod
#undef fchown

enum failure { NONE, CREATE, OPEN, MODE, OWNER, SYMLINK };
static enum failure failure;
static char sentinel[] = "/tmp/sysinternalsebpf-discovery-test.XXXXXX";
static int ownershipCalls;

void logMessage(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

static char *testMkdtemp(char *path)
{
    assert(strcmp(path, TMPDIR_TEMPLATE) == 0);
    if (failure == CREATE) {
        errno = EACCES;
        return NULL;
    }
    char *result = mkdtemp(path);
    if (result && failure == SYMLINK) {
        assert(rmdir(path) == 0);
        assert(symlink(sentinel, path) == 0);
    }
    return result;
}

static int testOpen(const char *path, int flags, ...)
{
    if (failure == OPEN) {
        errno = EMFILE;
        return -1;
    }
    return open(path, flags);
}

static int testFchmod(int fd, mode_t mode)
{
    if (failure == MODE) {
        errno = EPERM;
        return -1;
    }
    return fchmod(fd, mode);
}

static int testFchown(int fd, uid_t uid, gid_t gid)
{
    struct stat st;
    assert(fstat(fd, &st) == 0);
    assert(S_ISDIR(st.st_mode));
    assert((st.st_mode & 0777) == TEMPDIR_MODE);
    assert(uid == TEMPUID && gid == TEMPGID);
    ownershipCalls++;
    if (failure == OWNER) {
        errno = EPERM;
        return -1;
    }
    // Do not require root or give another UID control over test directories.
    return 0;
}

int main(void)
{
    struct stat before, after, current;
    char previous[sizeof(tmpdir)] = {0};
    int cwd = open(".", O_RDONLY | O_DIRECTORY);
    assert(cwd >= 0);
    assert(mkdtemp(sentinel) != NULL);
    assert(stat(sentinel, &before) == 0);

    for (int i = 0; i < 3; i++) {
        mode_t mask = umask(0077);
        int fd = createDiscoveryDirectory();
        umask(mask);
        assert(fd >= 0);
        assert(strcmp(previous, tmpdir) != 0);
        strcpy(previous, tmpdir);
        assert(strcmp(sysinternalsEBPFtmp, strrchr(tmpdir, '/') + 1) == 0);
        // The dentry name must still fit Linux's inline d_iname storage.
        assert(strlen(sysinternalsEBPFtmp) < 32);
        assert(fstat(fd, &after) == 0);
        assert((after.st_mode & 0777) == TEMPDIR_MODE);
        assert(fchdir(fd) == 0);
        assert(stat(".", &current) == 0);
        assert(current.st_ino == after.st_ino && current.st_dev == after.st_dev);
        assert(fchdir(cwd) == 0);
        assert(close(fd) == 0);
        assert(rmdir(tmpdir) == 0);
    }
    assert(ownershipCalls == 3);

    for (failure = CREATE; failure <= SYMLINK; failure++) {
        int callsBefore = ownershipCalls;
        assert(createDiscoveryDirectory() == -1);
        if (failure == SYMLINK) {
            assert(lstat(tmpdir, &after) == 0 && S_ISLNK(after.st_mode));
            assert(ownershipCalls == callsBefore);
            assert(unlink(tmpdir) == 0);
        } else {
            assert(lstat(tmpdir, &after) < 0 && errno == ENOENT);
        }
    }
    assert(stat(sentinel, &after) == 0);
    assert(before.st_uid == after.st_uid && before.st_gid == after.st_gid);
    assert(before.st_mode == after.st_mode && before.st_ino == after.st_ino);
    assert(close(cwd) == 0);
    assert(rmdir(sentinel) == 0);
    puts("discovery directory regression tests passed");
    return 0;
}
