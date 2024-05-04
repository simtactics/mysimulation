const std = @import("std");
const rl = @import("raylib");
const Allocator = std.mem.Allocator;
const json = std.json;
const dbg = std.debug;
const fmt = std.fmt;
const fs = std.fs;

pub const Floor = struct {
    level: i32,
    x: f32,
    y: f32,
    value: i32,
};

pub const Wall = struct {
    level: i32,
    x: f32,
    y: f32,
    value: i32,
    tls: i32,
    trs: i32,
    tlp: i32,
    trp: i32,
    blp: i32,
    brp: i32,
};

pub const World = struct {
    floors: []Floor,
    walls: ?[]Wall,
};

pub const Item = struct {
    guid: i32,
    level: i32,
    x: i32,
    y: i32,
    dir: i32,
    group: i32,
};

pub const House = struct {
    version: f32,
    size: i32,
    category: i32,
    world: World,
    items: []Item,
};

pub const Blueprint = struct {
    house: House,
};

/// Draws floors from JSON Blueprint files
pub fn draw_floors(json_file: [:0]const u8) !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    // Load file
    const lot = rl.loadFileText(json_file);
    defer rl.unloadFileText(lot);

    // Parse JSON
    const parsed = try json.parseFromSlice(Blueprint, allocator, lot, .{});
    defer parsed.deinit();

    const blueprint = parsed.value;

    const floorLevel = rl.Vector3.init(0.0, 0.0, 0.0);

    for (blueprint.house.world.floors) |flr| {
        // Draw grass
        rl.drawPlane(floorLevel, rl.Vector2.init(flr.x, flr.y), rl.Color.green);
    }
}
