# My Simulation

<p align="center">
<img title="" src="resources/logo.png" width="150" alt=""">
</p>
<p align="center">
<img alt="GitHub License" src="https://img.shields.io/github/license/tonytins/mysimulation"> <img alt="GitHub Pull Requests" src="https://img.shields.io/github/issues-pr/tonytins/mysimulation"> <img alt="GitHub Issues" src="https://img.shields.io/github/issues/tonytins/mysimulation">

</p>

My Simulation is an **experimental** open source multiplayer game based upon The Sims Online. It attempts to mimic the original game as closely as possible while extending it with new features.

The client is written in Zig and uses Raylib game engine with NioTSO's libraries for interacting with The Sims' proprietary formats. FreeSO's API server is used for the backend.

## Motivation

See [motivation.md](./docs/motivation.md) for motivation and vision.

## Requirements

### Prerequisites

- The Sims Online
- Client
  - [Zig](https://ziglang.org/) 0.11+
- API Server
  - [.NET](https://dotnet.microsoft.com/en-us/) 8.0+
  - [Mockoon](https://mockoon.com/) for mock APIs (Optional)
- Libraries & Tools
  - TBA

### System Requirements

| Platform | Version | Architectures   |
| -------- | ------- | --------------- |
| Windows  | 10+     | x86_64, aarch64 |
| Linux    | 3.16+   | x86_64          |
| macOS    | 11.0+   | x86_64, aarch64 |

## Getting Started

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/tonytins/mysimulation.git
   git submodule update --init --recursive
   ```

2. **Navigate to the Repository**:

   ```bash
   cd mysimulation
   ```

3. **Run the Client**:

   ```bash
   zig build run
   ```

Further details on building can be found at [compiling.md](./docs/compiling.md).

## Contributing

They're many ways you can contribute to My Simulation such as trying things out, filing bugs, and joining in the discussion!

- How to Contribute (coming soon)
- [Pull Requests](<[https://github.com/tonytins/mysimulatione/pulls](https://github.com/tonytins/mysimulation/pulls)>): [Open](https://github.com/tonytins/mysimulation/pulls)/[Closed](https://github.com/tonytins/mysimulation/pulls?q=is%3Apr+is%3Aclosed)

## License

I license this project under either the GPL-3.0 or the EUPL-1.2 license – your choice. See [LICENSE-GPL](LICENSE-GPL) or [LICENSE-EUPL](LICENSE-EUPL) for details.

## Disclaimer

*My Simulation is not in any way affiliated with Electronic Arts, Maxis or The Sims, it is an unofficial community run project.*
