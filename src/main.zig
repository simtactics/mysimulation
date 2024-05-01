const std = @import("std");
const rl = @import("raylib");

const MAX_INPUT_CHARS = 9;

// Still in the proof of concept phase, don't midn the mess
pub fn main() anyerror!void {
    const screenWidth = 800;
    const screenHeight = 600;

    // var timePlayed: f32 = 0.0;

    rl.initWindow(screenWidth, screenHeight, "My Simulation");
    defer rl.closeWindow();

    // var textBox = rl.Rectangle.init(screenWidth / 2.0 - 100, 180, 50);
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

        // Load music
        // ------------------
        // rl.updateMusicStream(music);
        // rl.playMusicStream(music);

        // timePlayed = rl.getMusicTimePlayed(music) / rl.getMusicTimeLength(music);
        // if (timePlayed > 1.0) timePlayed = 1.0;
        // ------------------

        // Draw
        rl.beginDrawing();
        defer rl.endDrawing();

        // Splash screen
        rl.drawTexture(splash, 0, 0, rl.Color.white);
        rl.drawTexture(logo, screenWidth / 2.0 - 240, 30, rl.Color.white);

        // Loading text
        rl.drawText("Reticulating splines...", 20, screenHeight - 30, 20, rl.Color.white);
    }
}
