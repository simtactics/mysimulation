pub const CursorType = enum {
    Normal,
    ArrowUp,
    ArrowUpLeft,
    ArrowUpRight,
    ArrowDown,
    ArrowDownLeft,
    ArrowDownRight,
    ArrowLeft,
    ArrowRight,
    LiveNothing,
    LiveObjectUnavail,
    LivePerson,
    IBeam,

    SimsRotate,
    SimsRotateNE,
    SimsRotateSE,
    SimsRotateSW,
    SimsRotateNW,

    SimsMove,
    SimsPlace,

    Hourglass,

    LiveObjectAvail,
    LiveObject1Star,
    LiveObject2Star,
    LiveObject3Star,
    LiveObject4Star,
    LiveObject5Star,
    LiveObjectSpecial,
};
