pub const Branding = struct {
    logo: []const u8,
    background: []const u8,
};

pub const Config = struct {
    height: i64,
    width: i64,
    game_path: []const u8,
    branding: *Branding,

    pub fn init(self: Config) Config {
        return self;
    }
};
