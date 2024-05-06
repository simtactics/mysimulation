# Compiling

<p align="center">
    <b>WARNING: This is a work in progress</b>
</p>

## Client

Compiling the client requires Zig 0.11 or later.

To simply build the client:

```bash
zig build
```

If you want to run and test the client:

```bash
zig build run
```

## Libaries & Tools

*Note: The setup process is outdated, and currently planned to be rewritten.*

### Zig Backend (Optional)

While My Simulation uses NioTSO's C headers directly, you can still compile tools that make use of them. In this case, it is [recommend](https://zig.news/kristoff/make-zig-your-c-c-build-system-28g5) to use Zig's C/C++ compiler as a backend.

```bash
CC="zig cc" CXX="zig c++" cmake -B build
cd build && make -j8
```

On Unix-based systems, you can add it to your ``.bashrc`` or ``.zshrc`` file.

```bash
# C/C++ Linker
export CC="zig cc"
export CXX="zig c++"
```

Afterwards, you can pass the same commands without.

```bash
cmake -B build && cd build && make -j8
```

### Building

At bare minium, My Simulation's C/C++ libaries require make and cmake. Generate the build files:

```bash
cd library # Or cd tools
cmake
```

Afterwards, comile the software:

```bash
make
```

## Server

<p align="center">
    <b>Coming soon!</b>
</p>