# Build
Please see the history of this file for instructions for older, unsupported versions.

## Prerequisites
- clang/llvm v10+

### Azure Linux
```
sudo dnf install gcc gcc-c++ make cmake llvm clang elfutils-libelf-devel rpm-build json-glib-devel python3
```

### Ubuntu
```
sudo apt update
sudo apt -y install build-essential gcc g++ make cmake libelf-dev llvm clang libzstd1 git libjson-glib-dev
```

### Rocky Linux
```
sudo yum install gcc gcc-c++ make cmake llvm clang elfutils-libelf-devel rpm-build json-glib-devel python3
```

### Fedora
```
sudo yum install gcc gcc-c++ make cmake llvm clang elfutils-libelf-devel rpm-build json-glib-devel python3
```

### Debian
```
sudo apt -y install build-essential gcc g++ make cmake libelf-dev llvm clang libzstd1 git libjson-glib-dev
```

## Build
```
cd
git clone https://github.com/Microsoft/SysinternalsEBPF.git
cd SysinternalsEBPF
mkdir build
cd build
cmake ..
make
```

## Filesystem safety regressions

After installing the build prerequisites, run from the repository root:

```
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
```

Both tests run without root or eBPF privileges and use disposable, randomly
named directories. Expected output is two passing tests. Intentional failure
cases print diagnostics; use `ctest --test-dir build -V` to see them.
`installer_file_safety` checks normal creation, non-forced preservation,
forced replacement without truncating the old inode, existing/dangling
symlinks, deterministic symlink insertion races, non-regular files, partial
and interrupted writes, and write/chmod/close failures. Sentinel contents
and permissions must remain unchanged.
`discovery_directory_safety` checks unique directory names, the actual
basename used in dentry discovery, mode 0700, descriptor-based ownership
arguments and directory entry, cleanup on setup failure, and rejection of
a substituted symlink. Ownership changes are stubbed, so the test does not
give another UID access to a directory or require root.

For an end-to-end manual check, use a disposable Linux VM with a kernel
that requires offset autodiscovery and supports this library. Rebuild and
install the library and eBPF objects, then start a consuming application
through its normal offset-discovery path. A BTF/CO:RE-only startup does not
exercise this code. Trace that startup with
`strace -f -e trace=mkdir,openat,fchmod,fchown,fchdir,rmdir`.
Expect a fresh `/tmp/sysinternalsEBPFtmp.XXXXXX` name (with a generated
suffix), mode 0700, ownership 12345:67890 set through its descriptor, a
successful discovery, cwd restoration, and removal of the temporary
directory. Repeat startup to check that a new name is used and no directory
is left behind. Only in that disposable VM, pre-create the old
`/tmp/sysinternalsEBPFtmp` name as a symlink to a disposable sentinel
directory and repeat: its ownership and permissions must remain unchanged.
Do not run this setup against shared or production `/tmp` paths.

`dropFile` requires trusted parent directories; it does not secure an
attacker-controlled directory hierarchy. Non-forced installs now reject
leaf symlinks, while forced installs replace the link itself without
modifying its target. Consumers must rebuild/deploy the corrected shared
library (and rebuild statically linked or embedded copies). Updating an
application alone does not update its installed SysinternalsEBPF dependency.

## Cross-building for aarch64 (on x86_64)

### Prerequisites
Install cross-compilation toolchain and target-architecture libraries:
```
sudo apt -y install crossbuild-essential-arm64 binutils-aarch64-linux-gnu \
    libc6-dev-arm64-cross linux-libc-dev-arm64-cross \
    libelf-dev:arm64 libjson-glib-dev:arm64 zlib1g-dev:arm64 libzstd-dev:arm64
```

You may need to enable the arm64 architecture first:
```
sudo dpkg --add-architecture arm64
sudo apt update
```

### Cross-build
```
cd SysinternalsEBPF
mkdir build-arm64
cd build-arm64
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE=../cmake/aarch64-linux-gnu.cmake
cmake --build . --parallel
```

### Install (staged)
```
DESTDIR=$(pwd)/staging cmake --install .
```

## (Build from Sysmon ADO internally)
*This is only required when cloning from the Sysmon ADO. Most users can ignore
this.*
```
cd
git clone <Sysmon ADO>
cd Sysmon/sysinternalsEBPF
mkdir build
cd build
cmake ..
make
```

## Install
SysinternalsEBPF can be installed in two different ways, either directly via
CMake (if just built) or by running the libsysinternalsEBPFinstaller binary.
The installer binary is portable and contains all the necessary files to
install sysinternalsEBPF onto a system.

Either:
```
sudo ./libsysinternalsEBPFinstaller -i
```
Or:
```
sudo make install
sudo ldconfig
```
The shared library will be installed to the appropriate multiarch library
directory (e.g. /lib/x86\_64-linux-gnu on Debian x86\_64,
/lib/aarch64-linux-gnu on Debian arm64, or /lib64 on Fedora); the header to
/usr/include; the offsets database and EBPF objects to /opt/sysinternalsEBPF.
The libsysinternalsEBPFinstaller binary will also be installed in
/opt/sysinternalsEBPF (which can be copied to another system and run to install
sysinternalsEBPF there). *Note:* 'sudo make install' will use the binary,
include, and lib directories that cmake prefers or you have overridden, whereas
the installer and the packages (see below) use the paths specified above.

## Make Packages
Packages can be generated with:
```
make deb
```
or
```
make rpm
```
The directories build/deb and build/rpm will be populated with the required
files. If dpkg-deb is available, the build/deb directory will be used to create
a deb package. Similarly if rpmbuild is available, the build/rpm directory will
be used to create an rpm package. Package architecture is set automatically
based on the target platform.
