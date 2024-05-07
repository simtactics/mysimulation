const std = @import("std");
const rl = @import("raylib");
const world = @import("world.zig");

const dbg = std.debug;

const GameScreen = enum {
    login,
    cas,
    map,
    lot,
};

const Rotations = enum {
    left,
    right,

    pub fn init(self: Rotations) Rotations {
        return self;
    }
};

// Still in the proof of concept phase, don't mind the mess
pub fn main() anyerror!void {
    const screen_width = 800;
    const screen_height = 600;

    rl.initWindow(screen_width, screen_height, "My Simulation");
    defer rl.closeWindow();

    var current_screen: GameScreen = .login;
    var frame_counter: i32 = 0;

    var lot_camera = rl.Camera3D{
        .position = rl.Vector3.init(-90.0, 20.0, 90.0),
        .target = rl.Vector3.init(0, 0.0, 0),
        .up = rl.Vector3.init(0, 1.0, 0),
        .fovy = 10,
        .projection = rl.CameraProjection.camera_orthographic,
    };

    var city_camera = rl.Camera3D{
        .position = rl.Vector3.init(18.0, 21.0, 18),
        .target = rl.Vector3.init(0, 0, 0),
        .up = rl.Vector3.init(0, 1.0, 0),
        .fovy = 45,
        .projection = rl.CameraProjection.camera_perspective,
    };

    const floorLevel = rl.Vector3.init(0.0, 0.0, 0.0);
    const itemStatic = rl.Vector3.init(0.0, 1.0, 0.0);

    var rotation_manager = Rotations.init(Rotations.left);

    rl.setTargetFPS(60);

    const logo = rl.Texture.init("resources/logo.png");
    const splash = rl.Texture.init("resources/tsosplash.png");
    const table3 = rl.Texture.init("resources/items/dorms/table_3.png");
    const table4 = rl.Texture.init("resources/items/dorms/table_4.png");
    const city = rl.loadImage("resources/cities/city_0100/elevation.png");
    // const city_texture = rl.Texture.init("resources/cities/city_0100/vertexcolor.png");
    defer rl.unloadTexture(splash);
    defer rl.unloadTexture(logo);
    defer rl.unloadTexture(table4);
    defer rl.unloadTexture(table3);
    defer rl.unloadImage(city);

    const mesh = rl.genMeshHeightmap(city, rl.Vector3.init(16, 8, 16));
    const model = rl.loadModelFromMesh(mesh);

    // model.materials[0].maps[rl.MATERIAL_MAP_DIFFUSE].texture = city_texture;

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
                const zoom_increment = 5;

                if (rl.isKeyPressed(rl.KeyboardKey.key_s)) {
                    if (lot_camera.fovy == 10) {
                        lot_camera.fovy += zoom_increment;
                    }

                    dbg.print("Zoom level: {any}\n", .{
                        lot_camera.fovy,
                    });
                } else if (rl.isKeyPressed(rl.KeyboardKey.key_w)) {
                    if (lot_camera.fovy == 15) {
                        lot_camera.fovy -= zoom_increment;
                    }

                    dbg.print("Zoom level: {any}\n", .{
                        lot_camera.fovy,
                    });
                }

                if (rl.isKeyPressed(rl.KeyboardKey.key_a)) {
                    lot_camera.position = rl.Vector3.init(-90.0, 20.0, 90.0);
                    rotation_manager = Rotations.init(Rotations.left);
                    dbg.print("Rotate right\n", .{});
                } else if (rl.isKeyPressed(rl.KeyboardKey.key_d)) {
                    lot_camera.position = rl.Vector3.init(90.0, 20.0, 90.0);
                    rotation_manager = Rotations.init(Rotations.right);
                    dbg.print("Rotate left\n", .{});
                }

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
            .map => {
                rl.clearBackground(rl.Color.sky_blue);

                city_camera.begin();
                defer city_camera.end();

                rl.drawModel(model, floorLevel, 1.0, rl.Color.green);
            },
            // Low view (i.e. world)
            .lot => {
                rl.clearBackground(rl.Color.sky_blue);

                lot_camera.begin();
                defer lot_camera.end();

                rl.drawPlane(floorLevel, rl.Vector2.init(64, 64), rl.Color.dark_green);
                switch (rotation_manager) {
                    .right => rl.drawBillboard(lot_camera, table4, itemStatic, 2.0, rl.Color.white),
                    .left => rl.drawBillboard(lot_camera, table3, itemStatic, 2.0, rl.Color.white),
                }

                // rl.drawGrid(64, 1.0);
            },
        }

        rl.drawFPS(10, 10);
    }
}
