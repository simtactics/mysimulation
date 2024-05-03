const std = @import("std");
const rl = @import("raylib");

const MAX_INPUT_CHARS = 9;

const GameScreen = enum {
    logo,
    gameplay,
};

// Still in the proof of concept phase, don't midn the mess
pub fn main() anyerror!void {
    const screen_width = 800;
    const screen_height = 600;

    // var timePlayed: f32 = 0.0;

    rl.initWindow(screen_width, screen_height, "My Simulation");
    defer rl.closeWindow();

    var current_screen: GameScreen = .logo;
    var frame_counter: i32 = 0;

    // var textBox = rl.Rectangle.init(screen_width / 2.0 - 100, 180, 50);
    // var mouseOnText = false;
    // var letterCount = 0;

    rl.setTargetFPS(60);

    // Media must be loaded after window init
    // rl.initAudioDevice();
    // const music = rl.loadMusicStream("resources/LeeRosevere-ThreeThingsYouNeedtoKnowAboutToday.mp3");
    // defer rl.closeAudioDevice();
    const logo = rl.Texture.init("resources/logo.png");
    const splash = rl.Texture.init("resources/tsosplash.png");
    defer rl.unloadTexture(splash);
    defer rl.unloadTexture(logo);

    while (!rl.windowShouldClose()) {

        // Update
        // ------------------
        switch (current_screen) {
            .logo => {
                frame_counter += 1;

                if (frame_counter > 120) current_screen = .gameplay;
            },
            .gameplay => {},
        }
        // ------------------

        // Draw
        rl.beginDrawing();
        defer rl.endDrawing();

        switch (current_screen) {
            .logo => {
                // Splash screen
                rl.drawTexture(splash, 0, 0, rl.Color.white);
                rl.drawTexture(logo, screen_width / 2.0 - 240, 30, rl.Color.white);

                // Loading text
                rl.drawText("Reticulating splines...", 20, screen_height - 30, 20, rl.Color.white);
            },
            .gameplay => {
                rl.clearBackground(rl.Color.black);
            },
        }
    }
}
