# zTSO

<p align="center">
<img title="" src="SimTactics.png" width="250" alt=""">
</p>

zTSO is intended to be an experimental reimplementation of The Sims Online written in Zig that's based on RayLab and NioTSO's C libraries. It uses FreeSO's API server for handling backend.

## Motivation

Although an established reimplementation project already exists, it has excess legacy baggage due to it originally being based on .NET Framework 3.5 and XNA, causing plenty of performance issues and accessibility remains limited in its current form. (Trust me, I've tried). Meanwhile, NioTSO's foundation remains solid, even though it was never finished. The client perfectly simulates a loading screen, plus it can render Sims and objects flawlessly.

The problem is C is, well, C. ¯\\\_(ツ)\_/¯ Accessing NioTSO's code in safer languages like Go or Rust has been historically difficult. While Zig is still in its infancy, the language's FFI is perfectly capable of directly accessing C headers without jumping through hoops. Making it well worth giving it a try. Although Zig itself has not yet hit 1.0, the RayLib game engine is mature.

Will this succeed? *I have no idea*. I'm not much of a game developer, but that hasn't stopped me from dreaming. Your help is most welcome. My goal is to at least create a successful lot server for endless P2P fun.

## To do

- [ ] Rewrite header files

- [ ] Communicate with FreeSO-based API server

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