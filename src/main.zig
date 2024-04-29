const std = @import("std");
const rl = @import("raylib");

pub fn main() !void {
    const screenWidth = 800;
    const screenHeight = 450;

    rl.initWindow(screenWidth, screenHeight, "Basic Window");
    defer rl.closeWindow();

    // var ballPos = rl.Vector2.init(screenWidth / 2, screenHeight / 2);

    rl.setTargetFPS(60);

    while (!rl.windowShouldClose()) {

        // Update
        // if (rl.isKeyDown(rl.KeyboardKey.key_right)) {
        //     ballPos.x += 2.0;
        // }

        // if (rl.isKeyDown(rl.KeyboardKey.key_left)) {
        //     ballPos.x -= 2.0;
        // }

        // if (rl.isKeyDown(rl.KeyboardKey.key_up)) {
        //     ballPos.y -= 2.0;
        // }

        // if (rl.isKeyDown(rl.KeyboardKey.key_down)) {
        //     ballPos.y -= 2.0;
        // }

        // Draw
        rl.beginDrawing();
        defer rl.endDrawing();

        rl.clearBackground(rl.Color.dark_gray);

        rl.drawText("Hello, zTSO!", 190, 200, 20, rl.Color.sky_blue);

        // rl.drawCircle(ballPos, 50, rl.Color.maroon);
    }
}
