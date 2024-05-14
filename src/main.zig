/// Still in the proof of concept phase, don't mind the mess
const std = @import("std");
const rl = @import("raylib");
const world = @import("world.zig");
const nso = @import("niotso.zig");
const clap = @import("clap");
const view = @import("view.zig");

const dbg = std.debug;

const GameScreen = enum {
    login,
    cas, // Create-A-Sim
    map, // city screen
    lot, // world view
};

//We start that NorthWest so it is easy to determine the flip
const CardinalDirection = enum {
    NorthWest, // 0, sprite 1
    NorthEast, // 1, sprite 1 flip
    SouthEast, // 2, sprite 2
    SouthWest, // 3, sprite 2 flip
};

const Rotations = enum {
    left, // 0
    right, // 1
};

const RotationManager = struct {
    Direction: CardinalDirection,

    pub fn init(self: RotationManager) RotationManager {
        return self;
    }

    pub fn rotate(self: *RotationManager, rotation: Rotations) void {
        //rotate the direction by 90 degrees
        var direction_index = @as(i8, @intFromEnum(self.Direction));
        switch (rotation) {
            .left => {
                direction_index = direction_index - 1;
            },
            .right => {
                direction_index = direction_index + 1;
            },
        }
        // Circle around if out of bounds
        if (direction_index < 0) {
            self.Direction = CardinalDirection.SouthWest;
        } else if (direction_index > 3) {
            self.Direction = CardinalDirection.NorthWest;
        } else {
            self.Direction = @as(CardinalDirection, @enumFromInt(direction_index));
        }
        //Result
        dbg.print("Orientation: {any}\n", .{self.Direction});
    }
};

pub fn main() anyerror!void {
    const screen_width = 800;
    const screen_height = 600;

    view.init(true, screen_width, screen_height, "My Simulation");

    var current_screen: GameScreen = .login;
    var frame_counter: i32 = 0;

    // NOTE: jip
    // I don't think we can get away with using the built-in camera
    // We need pixel perfect isometric camera
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
    const itemStaticSize = rl.Vector2.init(2.0, 2.0);

    var rotation_manager = RotationManager.init(.{
        .Direction = CardinalDirection.SouthEast,
    });

    const logo = rl.Texture.init("resources/logo.png");
    const splash = rl.Texture.init("resources/tsosplash.png");
    const chair1 = rl.Texture.init("resources/items/dorm/chair/chair_1.png");
    const chair1_rect = rl.Rectangle.init(0, 0, @as(f32, @floatFromInt(-chair1.width)), @as(f32, @floatFromInt(chair1.height)));
    const chair2 = rl.Texture.init("resources/items/dorm/chair/chair_2.png");
    const chair2_rect = rl.Rectangle.init(0, 0, @as(f32, @floatFromInt(-chair2.width)), @as(f32, @floatFromInt(chair2.height)));
    const city = rl.loadImage("resources/cities/city_0100/elevation.png");
    // const city_texture = rl.Texture.init("resources/cities/city_0100/vertexcolor.png");
    // TODO: figure out a better way to unload all images and textures.
    defer {
        rl.unloadTexture(splash);
        rl.unloadTexture(logo);
        rl.unloadTexture(chair1);
        rl.unloadTexture(chair2);
        rl.unloadImage(city);
    }

    const mesh = rl.genMeshHeightmap(city, rl.Vector3.init(16, 8, 16));
    const model = rl.loadModelFromMesh(mesh);

    // model.materials[0].maps[rl.MATERIAL_MAP_DIFFUSE].texture = city_texture;

    while (!rl.windowShouldClose()) {

        // Update
        // ------------------
        switch (current_screen) {
            // Skip straight to lot view until city server is complete
            .login => {
                frame_counter += 1;

                if (frame_counter > 120) current_screen = .lot;
            },
            // TODO: Write CAS (Create-A-Sim) screen
            .cas => {},
            .map => {},
            .lot => {
                const zoom_increment = 5;
                // rotate with keyboard
                if (rl.isKeyPressed(rl.KeyboardKey.key_s)) {
                    if (lot_camera.fovy == 10) {
                        lot_camera.fovy += zoom_increment;
                    }

                    dbg.print("Zoom level: {d}\n", .{
                        lot_camera.fovy,
                    });
                } else if (rl.isKeyPressed(rl.KeyboardKey.key_w)) {
                    if (lot_camera.fovy == 15) {
                        lot_camera.fovy -= zoom_increment;
                    }

                    dbg.print("Zoom level: {d}\n", .{
                        lot_camera.fovy,
                    });
                }
                // roate with scrollwheel
                const mouse_wheel_y = std.math.clamp(-rl.getMouseWheelMove(), -1, 1) * zoom_increment;
                if (mouse_wheel_y != 0) {
                    const zoom_min = 10;
                    const zoom_max = 25;
                    lot_camera.fovy = std.math.clamp(lot_camera.fovy + mouse_wheel_y, zoom_min, zoom_max);
                    dbg.print("Zoom level: {d}\n", .{
                        lot_camera.fovy,
                    });
                }
                if (rl.isKeyPressed(rl.KeyboardKey.key_a)) {
                    lot_camera.position = rl.Vector3.init(-90.0, 20.0, 90.0);
                    rotation_manager.rotate(Rotations.left);
                    dbg.print("rotate left\n", .{});
                } else if (rl.isKeyPressed(rl.KeyboardKey.key_d)) {
                    lot_camera.position = rl.Vector3.init(90.0, 20.0, 90.0);
                    rotation_manager.rotate(Rotations.right);
                    dbg.print("rotate right\n", .{});
                }

                // camera.update(rl.CameraMode.camera_custom);
            },
        }
        // ------------------

        // Draw
        view.render();

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
                switch (rotation_manager.Direction) {
                    .NorthWest => {
                        rl.drawBillboardRec(lot_camera, chair1, chair1_rect, itemStatic, itemStaticSize, rl.Color.white);
                    },
                    .NorthEast => {
                        rl.drawBillboard(lot_camera, chair1, itemStatic, 2.0, rl.Color.white);
                    },
                    .SouthEast => {
                        rl.drawBillboard(lot_camera, chair2, itemStatic, 2.0, rl.Color.white);
                    },
                    .SouthWest => {
                        rl.drawBillboardRec(lot_camera, chair2, chair2_rect, itemStatic, itemStaticSize, rl.Color.white);
                    },
                }

                rl.drawGrid(64, 1.0);
            },
        }
    }
}
