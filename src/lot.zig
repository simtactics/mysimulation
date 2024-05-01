pub const Floor = struct {
    level: u8,
    x: u8,
    y: u8,
    value: u8,
};

pub const Floors = struct {
    floor: []Floor,
};

pub const World = struct {
    floors: Floors,
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

pub const Items = struct {
    Item: []Item,
};

pub const House = struct {
    size: u8,
    category: u8,
    world: World,
    items: Item,
};
