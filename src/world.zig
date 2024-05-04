const std = @import("std");
const rl = @import("raylib");
const json = std.json;

pub const Floor = struct {
    level: u8,
    x: u8,
    y: u8,
    value: u8,
};

pub const Wall = struct {
    level: u8,
    x: u8,
    y: u8,
    value: u8,
    tls: u8,
    trs: u8,
    tlp: u8,
    trp: u8,
    blp: u8,
    brp: u8,
};

pub const World = struct {
    floors: []Floor,
    walls: []Wall,
};

pub const Item = struct {
    guid: u8,
    level: u8,
    x: u8,
    y: u8,
    dir: u8,
    group: u8,
};

pub const House = struct {
    version: f32,
    size: u8,
    category: u8,
    world: World,
    items: []Item,
};

/// Draws floors from JSON Blueprint files
pub fn draw_floors(json_file: [:0]const u8) void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    const floorLevel = rl.Vector3.init(0.0, 0.0, 0.0);

    const parsed = try json.parseFromSlice(House, allocator, json_file, .{});
    defer parsed.deinit();

    const blueprint = parsed.value;

    for (blueprint.world.floors) |flr| {
        rl.drawPlane(floorLevel, rl.Vector2.init(flr.x, flr.y), rl.Color.green);
    }
}
