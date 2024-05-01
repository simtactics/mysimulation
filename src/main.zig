const std = @import("std");
const rl = @import("raylib");

pub fn main() anyerror!void {
    const screenWidth = 800;
    const screenHeight = 600;

    // var timePlayed: f32 = 0.0;

    rl.initWindow(screenWidth, screenHeight, "My Simulation");
    defer rl.closeWindow();

    rl.setTargetFPS(60);

    // Media must be loaded after window init
    // rl.initAudioDevice();
    // const music = rl.loadMusicStream("resources/LeeRosevere-ThreeThingsYouNeedtoKnowAboutToday.mp3");
    // defer rl.closeAudioDevice();

    const splash: rl.Texture = rl.Texture.init("resources/tsosplash.png");
    defer rl.unloadTexture(splash);

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

        // Loading text
        rl.drawText("Reticulating splines...", 20, screenHeight - 30, 20, rl.Color.white);
    }
}
