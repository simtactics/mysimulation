const std = @import("std");
const rl = @import("raylib");

pub const Texture = struct {
    texture: []const u8,
    buffer: []const u8,
};

pub const View = struct {
    north: *Texture,
    south: *Texture,
};

pub const Motives = struct {
    room: i64, // Environment in later games
    hunger: i64,
    social: i64,
    comfort: i64,
    hygiene: i64,
    fun: i64,
    energy: i64,
    bladder: i64,
};

pub const Skills = struct {
    cooking: i64,
    mechanical: i64,
    charisma: i64,
    body: i64,
    logic: i64,
    creativity: i64,
};

/// Open object format with similar
pub const OpenIff = struct {
    name: []const u8,
    description: []const u8,
    version: i64,
    catagory: []const u8,
    /// Item to derive all logic from.
    /// By defualt, this comes from the base game.
    base: []const u8,
    motives: *Motives,
    skills: Skills,
    views: *View,

    // pub fn init(file: []const u8) OpenIff {

    // }
};
