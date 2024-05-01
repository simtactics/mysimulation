pub const Floor = struct {
    level: u8,
    x: u8,
    y: u8,
    value: u8,
};

pub const World = struct {
    floors: []Floor,
    walls: u8,
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
