pub const Asset = struct {
    Group: u32,
    File: u32,
    Type: u32,
};

pub const FileError = enum {
    FERR_NOT_FOUND,
    FERR_OPEN,
    FERR_BLANK,
    FERR_MEMORY,
    FERR_READ,
    FERR_UNRECOGNIZED,
    FERR_INVALIDDATA,
};

pub const ImageFormat = enum {
    FIMG_BGR24,
    FIMG_BGRA32,
};

pub const Image = struct {
    Width: u8,
    Height: u8,
    Format: ImageFormat,
    Data: u8,
};
