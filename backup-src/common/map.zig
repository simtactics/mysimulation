pub const LotCategory = enum(u32) {
    none = 0,
    money = 1,
    offbeat = 2,
    romance = 3,
    services = 4,
    shopping = 5,
    skills = 6,
    welcome = 7,
    games = 8,
    entertainment = 9,
    residence = 10,
    community = 11, //cannot be set by users

    recent = 255, //for filter searches
};

pub const Top100Category = enum(32) {
    lot_money = 1,
    lot_offbeat = 2,
    lot_romance = 3,
    lot_services = 4,
    lot_shopping = 5,
    lot_skills = 6,
    lot_welcome = 7,
    lot_games = 8,
    lot_entertainment = 9,
    lot_residence = 10,
    avatar_most_famous = 11,
    avatar_best_karma = 12,
    avatar_friendliest = 13,
    avatar_most_infamous = 14,
    avatar_meanest = 15,
};

pub const UserReferenceType = enum(32) {
    EA = 1,
    MAXIS = 2,
    MOMI = 3,
    TSO = 4,
    AVATAR = 5,
};
