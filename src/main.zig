const std = @import("std");
const rl = @import("raylib");
const world = @import("world.zig");
const core = @cImport({
    @cInclude("./core.h");
});

const GameScreen = enum {
    login,
    cas,
    map,
    lot,
};

// Still in the proof of concept phase, don't mind the mess
pub fn main() anyerror!void {
    const screen_width = 800;
    const screen_height = 600;

    rl.initWindow(screen_width, screen_height, "My Simulation");
    defer rl.closeWindow();

    var current_screen: GameScreen = .login;
    var frame_counter: i32 = 0;

    var camera = rl.Camera3D{
        .position = rl.Vector3.init(0.0, 20.0, 90.0),
        .target = rl.Vector3.init(0, 0.0, 0),
        .up = rl.Vector3.init(0, 1.0, 0),
        .fovy = 10,
        .projection = rl.CameraProjection.camera_orthographic,
    };

    const floorLevel = rl.Vector3.init(0.0, 0.0, 0.0);
    const itemStatic = rl.Vector3.init(0.0, 2.0, 0.0);

    // const planePosition = rl.Vector3.init(0.0, 0.0, 0.0);

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
    const table4 = rl.Texture.init("resources/items/dorms/table_4.png");
    defer rl.unloadTexture(splash);
    defer rl.unloadTexture(logo);
    defer rl.unloadTexture(table4);

    while (!rl.windowShouldClose()) {

        // Update
        // ------------------
        switch (current_screen) {
            // Skip straight to CAS (Create-A-Sim) until city server is complete
            // TODO: Create login window with username, password, and server option
            .login => {
                frame_counter += 1;

                if (frame_counter > 120) current_screen = .lot;
            },
            //
            .cas => {},
            .map => {},
            .lot => {
                // camera.update(rl.CameraMode.camera_custom);
            },
        }
        // ------------------

        // Draw
        rl.beginDrawing();
        defer rl.endDrawing();

        switch (current_screen) {
            // Mockup loading screen, skips straight to world
            .login => {
                // Splash screen
                rl.drawTexture(splash, 0, 0, rl.Color.white);
                rl.drawTexture(logo, screen_width / 2.0 - 240, 30, rl.Color.white);

                // Loading text
                rl.drawText("Reticulating splines...", 20, screen_height - 30, 20, rl.Color.white);
            },
            // Skip this for now
            .cas => {},
            .map => {},
            // "World" (i.e. lot view)
            .lot => {
                rl.clearBackground(rl.Color.sky_blue);

                camera.begin();
                defer camera.end();

                rl.drawPlane(floorLevel, rl.Vector2.init(64, 64), rl.Color.dark_green);
                rl.drawBillboard(camera, table4, itemStatic, 2.0, rl.Color.white);
                // rl.drawGrid(64, 1.0);
            },
        }

        rl.drawFPS(10, 10);
    }
}
