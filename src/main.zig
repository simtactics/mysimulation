const std = @import("std");
const rl = @import("raylib");

const MAX_INPUT_CHARS = 9;

const GameScreen = enum {
    login,
    cas,
    world,
};

// Still in the proof of concept phase, don't midn the mess
pub fn main() anyerror!void {
    const screen_width = 800;
    const screen_height = 600;

    // var timePlayed: f32 = 0.0;

    rl.initWindow(screen_width, screen_height, "My Simulation");
    defer rl.closeWindow();

    var current_screen: GameScreen = .login;
    var frame_counter: i32 = 0;

    var camera = rl.Camera3D{
        .position = rl.Vector3.init(5.0, 4.0, 5.0),
        .target = rl.Vector3.init(0, 2.0, 0),
        .up = rl.Vector3.init(0, 1.0, 0),
        .fovy = 60,
        .projection = rl.CameraProjection.camera_perspective,
    };

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
            // Skip straight to CAS (Create-A-Sim) until city server is complete
            // TODO: Create login window with username, password, and server option
            .login => {
                frame_counter += 1;

                if (frame_counter > 120) current_screen = .world;
            },
            //
            .cas => {},
            .world => {
                camera.update(rl.CameraMode.camera_third_person);
            },
        }
        // ------------------

        // Draw
        rl.beginDrawing();
        defer rl.endDrawing();

        switch (current_screen) {
            .login => {
                // Splash screen
                rl.drawTexture(splash, 0, 0, rl.Color.white);
                rl.drawTexture(logo, screen_width / 2.0 - 240, 30, rl.Color.white);

                // Loading text
                rl.drawText("Reticulating splines...", 20, screen_height - 30, 20, rl.Color.white);
            },
            .cas => {},
            //
            .world => {
                rl.clearBackground(rl.Color.ray_white);

                camera.begin();
                defer camera.end();
                rl.drawGrid(64, 1.0);
            },
        }

        rl.drawFPS(10, 10);
    }
}
