# Motivation


Although an established reimplementation project already exists, it has excess legacy baggage due to it originally being based on .NET Framework 3.5 and XNA, causing plenty of performance issues and accessibility remains limited in its current form even after they've moved to MonoGame and .NET Framework 4.0. Meanwhile, NioTSO's foundation remains solid, even though it was never finished. The client perfectly simulates a loading screen, plus it can render Sims and objects flawlessly.

The problem is C is, well, C. ¯\\\_(ツ)\_/¯ Accessing NioTSO's code in safer languages like Go or Rust has been historically difficult. Both comprise the language's promised safety in order to access vital APIs that make up the foundation of modern computing. While Zig is still in its infancy, the language's FFI is solid as well, and fully capable of directly accessing C APIs. Even when wrappers are used, it's a pretty thin layer that allow for idiomatic usage. So even though the language hasn't hit 1.0, as of this writing, you can still access plenty of mature APIs and make use of existing documentation without fear of breakage.

Will this succeed? *I have no idea*. I'm not much of a game developer, but that hasn't stopped me from dreaming. Your help is most welcome. My goal is to at least create a successful lot server for endless P2P fun.