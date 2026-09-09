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
