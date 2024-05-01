# zTSO

zTSO (working title) is intended to be an experimental reimplementation of The Sims Online written in Zig. It directly accesses NioTSO's C-based libraries and uses RayLib for the game engine.

## Motivation

Although an established reimplementation project already exists, it has excess legacy baggage due to it originally being based on .NET Framework 3.5 and XNA, causing plenty of performance issues in its current form. Meanwhile, NioTSO's foundation remains solid, even though it was never finished. The client perfectly simulates a loading screen, plus it can render Sims and objects flawlessly.

The problem is C is, well, C. ¯\\\_(ツ)\_/¯ Accessing NioTSO's code in safer languages like Go or Rust has been historically difficult. While Zig is still in its infancy, the language's FFI is perfectly capable of directly accessing C headers without jumping through hoops. Finally, although Zig itself has not yet hit 1.0, the RayLib game engine is mature. Making it well worth giving it a try.

Will this succeed? *I have no idea*. I'm not much of a game developer, but that hasn't stopped me from dreaming. Your help is most welcome.

## To do

- [ ] Rewrite header files

- [ ] Write Vitaboy renderer in Zig

## Requirements

- The Sims Online
- [Zig](https://ziglang.org/) 0.11 or newer

## Getting Started

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/tonytins/ztso.git
   ```

2. **Navigate to the Repository**:

   ```bash
   cd ztso
   ```

3. **Run the Examples**: Execute the code examples using the Zig compiler. For instance:

   ```bash
   zig build run
   ```

4. **Experiment and Learn**: Modify the code, try out different constructs, and see how Zig behaves.

## License

I license this project under the GPL-3.0 license - see [LICENSE](LICENSE) for details.