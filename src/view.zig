const rl = @import("raylib");

var is_debug: bool = false;

pub fn init(dbg: bool, width: i32, height: i32, title: [:0]const u8) void {
    if (dbg == true) {
        is_debug = dbg;
    }

    rl.initWindow(width, height, title);
    defer rl.closeWindow();

    rl.setTargetFPS(60);
}

/// Setup basic rendering
pub fn render() void {
    rl.beginDrawing();
    defer rl.endDrawing();

    if (is_debug == true) {
        rl.drawFPS(10, 10);
    }
}
