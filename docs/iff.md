# IFF

**Ported over from [NioTSO Wiki](http://wiki.niotso.org/IFF)**

**Interchange File Format** (**IFF**) is a chunk-based file format for binary resource data intended to promote a common model for store and use by an executable.

The IFF standard was written by EA in 1985 to provide a more universal way to look at files. The chunk scheme defined an easy way to categorize data inside one file, which allows any unrecognized data inside the file to be skipped. It became common for various software companies to manage data with IFF. The chunk scheme was adopted by PNG in 1996.

Microsoft and Apple saw potential in it—the potential of not having to write their own formats from scratch. As a result, in the 1990s arrived Microsoft's RIFF and Apple's AIFF audio formats. RIFF (*Resource* Interchange File Format) is used as the structure for .wav files and .avi files. AIFF (*Audio* Interchange File Format) is used for audio alone, plainly marked as .aiff files.

Widespread use of IFF has waned over time. EA continues to use it on occasion. In The Sims 1 and The Sims Online, IFF is used to hold object data (including floors, walls, and characters), game options, and various other types of information.

## Extensions

-   ``.iff`` - The default extension. IFF files under this extension generally contain object data, game options, and Edith UI strings. In The Sims 1, they also may contain characters, career descriptions, and many other types of information.
-   ``.flr`` - Floor
-   ``.wll`` - Wall
-   ``.spf`` - Sprite file. IFF files under this extension split off the graphical components of an object, allowing the development of these graphics to proceed relatively independently of the development of the object itself.
-   ``.stx`` - String file. In a similar way to .spf, IFF files under this extension split off the text strings of an object, allowing translations to occur independently of other development. This scheme and file extension is not used in The Sims Online.

In The Sims Online, almost every object consists of a .iff part, a .spf part, and a .otf part. .iff is used for catalog text, thumbnails, pie menu strings, behaviors, and everything else apart from the sprites, which are stored in the .spf part. Tuning constants are defined in the .otf part.

## Body

IFF uses a chunk structure to organize all data in the file. IFF chunks are meant to be conceived by the developer, rather than follow a universal standard.

The IFF layout consists of a 64-byte header followed by all chunks listed in succession.

All chunk parameters must be in big-endian format. In The Sims 1 and The Sims Online, all data *inside* the chunks is in little-endian format.

## Header

The TS1/TSO IFF header is 64 bytes and follows one of two formats, corresponding to version 2.0 and version 2.5. Version 2.5 modifies 2.0 to include the offset of the Resource Map chunk (`rsmp`); there is otherwise no difference between version 2.0 and 2.5.

### Version 2.0

Version 2.0 can be found in House00.iff in the UserData folder in The Sims 1. The header consists of the following 64-byte signature:

| ASCII approximation                                             | Hexadecimal representation                                                                                                                                                                      |
|-----------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| IFF FILE 2.0:TYPE FOLLOWED BY SI ZE JAMIE DOORNBOS & MAXIS 1996 | 49 46 46 20 46 49 4C 45 20 32 2E 35 3A 54 59 50 45 20 46 4F 4C 4C 4F 57 45 44 20 42 59 20 53 49 5A 45 00 20 4A 41 4D 49 45 20 44 4F 4F 52 4E 42 4F 53 20 26 20 4D 41 58 49 53 20 31 39 39 36 00 |

### Version 2.5

The version 2.5 header starts with the following 60-byte signature:

| ASCII approximation                                          | Hexadecimal representation                                                                                                                                                          |
|--------------------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| IFF FILE 2.5:TYPE FOLLOWED BY SI ZE JAMIE DOORNBOS & MAXIS 1 | 49 46 46 20 46 49 4C 45 20 32 2E 35 3A 54 59 50 45 20 46 4F 4C 4C 4F 57 45 44 20 42 59 20 53 49 5A 45 00 20 4A 41 4D 49 45 20 44 4F 4F 52 4E 42 4F 53 20 26 20 4D 41 58 49 53 20 31 |

The last 4 bytes are set to the offset of the Resource Map chunk (`rsmp`) in big-endian byte order.

## Chunks

Chunks follow the following format:

-   **Type** - A 4-byte non-null-terminated ASCII string identifying the type of the chunk
-   **Size** - A 4-byte unsigned integer specifying the size of the entire chunk, including header and data
-   **Chunk ID** - A 2-byte number mapping a unique numerical value to the chunk within the IFF file
-   **Flags** - A 2-byte number equal to 0x0000 or 0x0010; the meaning of the flag bits is unknown
-   **Label** - A 64-byte ASCII string serving as a text label to the chunk. This field is ignored by the game; it serves only as comment.
-   **Data** - The binary data of the chunk. The size of the data is equal to *Size* - 76 bytes, as 76 is the size of the header.

## Chunk types

Note that in contrast with the IFF container data described earlier on this page, all chunk types use **little-endian** ordered data.

This page is still incomplete. A lot of stuff is documented for us in these libraries provided by Simtech:

-   <http://homepage.mac.com/petergould/IFFS/IFFS_source.zip>
-   <http://simtech.sourceforge.net/data/srcs.zip>
-   <http://simtech.sourceforge.net/data/libsim.zip>
-   <http://simtech.sourceforge.net/data/pgms.zip>

### Arry

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### BCON

This chunk type holds a number of constants that behavior code can refer to. Labels may be provided for them in a TRCN chunk with the same ID.

**Format:**

-   **Number of constants** - A 1-byte unsigned integer specifying the number of constants defined in this chunk
-   **Type/Flags** - A 1-byte integer typically either 0x00 or 0x80. The purpose of this field is unknown.
-   **Constants** - As many 2-byte integer constants as defined by the first field, identified by their zero-based index

### BHAV

This chunk type holds Behavior code in SimAntics.

### BMP\_

This chunk type holds a regular Windows BMP file.

### CARR

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### CATS

This chunk type holds a single string key/value pair back to back, using C strings.

The only occurrence of this type of chunk in both The Sims and The Sims Online is in fire.iff, whose contents are "You should never see this.\n" followed by "Fire". It is not used ingame. The name possibly stands for "catalog string". The Sims Transmogrifier labels these fields Description and Name, respectively.

### CST

That's `'C' 'S' 'T' '\0'`. Equivalent in format to [STR#](#STR.23 "wikilink"). Whether or not this chunk type relates with [Caret-separated text](CST "wikilink") is unknown.

### CTSS

Catalog text strings; equivalent in format to [STR#](#STR.23 "wikilink")

### DGRP

This chunk type collects SPR# and SPR2 resources into a "drawing group" which can be used to display one tile of an object from all directions and zoom levels. Objects which span across multiple tiles have a separate DGRP chunk for each tile. A DGRP chunk always consists of 12 images (one for every direction/zoom level combination), which in turn contain info about one or more sprites.

[This page](http://homepage.mac.com/petergould/IFFS/Sprites/sprites.html) has a more in-depth explanation of how compiled bitmaps are positioned on a tile.

Integer sizes described below as "x-byte/y-byte" correspond to versions 20000/20001 and 20003/20004 respectively.

-   **Version** - A 2-byte unsigned integer. The most commonly used version in TSO is 20004, but 20000, 20001 and 20003 are also found occasionally.
-   **Image count** - A 2-byte/4-byte unsigned integer specifying the number of images found in this chunk. It is always 12 (number of camera rotations\*number of zoom levels or 4\*3).
-   **Images** - For each image:
    -   **Sprite count** - A 2-byte unsigned integer specifying the number of sprites used in this image (in versions 20000 and 20001)
    -   **Direction flags** - A 1-byte/4-byte integer specifying which direction the image is rendered in. Possible values are 0x01, 0x04, 0x10 and 0x40 (right rear, right front, left front and left rear respectively).
    -   **Zoom level** - A 1-byte/4-byte integer. Possible values are 1, 2 and 3 (1 being the farthest and 3 being the closest).
    -   **Sprite count** - A 4-byte unsigned integer (in versions 20003 and 20004, same as the sprite count field above).
    -   **Sprite info** - For each sprite:
        -   **Type** - A 2-byte signed integer found only in versions 20000 and 20001. Known values are -3 and -5. Very little is known about the format of entries using -5. According to [SimTech](http://simtech.sourceforge.net/tech/dgrp.html), Transmogrifier is known to ignore those entries, so it is likely that they are no longer used or ignored by the game.
        -   **SPR chunk ID** - A 2-byte/4-byte integer specifying the ID of the associated SPR# or SPR2 chunk.
        -   **SPR frame number** - A 2-byte/4-byte integer specifying the index of the frame used within the SPR chunk.
        -   **Flags** - A 2-byte integer found in versions 20000 and 20001. Bit 1, when set, causes the displayed image to be mirrored. Bit 3 determines if this sprite should be cached during rendering. It is often set in objects that are always being animated, such as fire. It is also set on sprites which are temporary such as food inside a fridge which is never visible for long and is conditional on the fridge having stock.
        -   **Sprite X offset** - A 2-byte/4-byte signed integer specifying the sprite's X offset from the center of the compiled image.
        -   **Sprite Y offset** - A 2-byte/4-byte signed integer specifying the sprite's Y offset from the center of the compiled image.
        -   **Object Z offset** - A 4-byte IEEE 754 float specifying an offset that affects the Z axis placement of the object (measured in grid units). It is found starting from version 20001.
        -   **Flags** - A 4-byte integer found in versions 20003 and 20004, same as the flags field above.
        -   **Object X offset** - A 4-byte IEEE 754 float specifying an offset that affects the X axis placement of the object (measured in grid units). It is only found in version 20004.
        -   **Object Y offset** - A 4-byte IEEE 754 float specifying an offset that affects the Y axis placement of the object (measured in grid units). It is only found in version 20004.

### EXPi

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### FAMh

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### FAMI

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### FAMs

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

Holds the last name of a family in The Sims 1; equivalent in format to [STR#](#STR.23 "wikilink").

### FBMP

This chunk type holds a regular Windows BMP file, as does [BMP\_](IFF#BMP "wikilink").

### FCNS

This chunk type defines global floating-point tuning constants. It is found exclusively in global.iff.

-   **Zero** - 4 bytes always set to zero
-   **Version** - A 4-byte integer specifying the version of the FCNS chunk; should be 1 or 2
-   **Magic number** - 4 bytes set to either the ASCII string "SNCF" or zero
-   **Constant count** - A 4-byte unsigned integer specifying the number of constants defined by this FCNS chunk
-   **Constants** - For each constant:
    -   **Name** - A string providing a name for this constant
    -   **Value** - A 4-byte little-endian float specifying the value of this constant
    -   **Description** - A string providing a description for this constant

      
    Strings in FCNS versions 1 and 2 are formatted in the same way as [TRCN](#TRCN "wikilink") versions 0/1 and 2 respectively.

### FLRm

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### FWAV

This chunk type holds a null-terminated string that specifies the name of a track/event pair. This chunk type is used by the [SimAntics](SimAntics "wikilink") "Play sound event" instruction, which looks up the track/event pair with this name in the [EVT](EVT "wikilink") files of the game, and then sends the event to the track.

### GLOB

This chunk type contains a filename to a semi-global IFF file, without the file extension. Semi-global files define shared common resources that are typically used in several other IFF files. A GLOB chunk, when present, allows other chunks in the same file to reference the semi-global file's resources.

The string can be either a null-terminated C string or a Pascal string. The chunk format does not specify which is being used. Some GLOB chunks are padded with 0xA3 bytes, usually to 256 bytes including the filename string length.

### HOUS

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### NBRS

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### NGBH

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### OBJD

This is an object definition, the main chunk for an object and the first loaded by the VM. There can be multiple master OBJDs in an IFF, meaning that one IFF file can define multiple objects. This chunk contains nearly hundred fields, and as such is a nightmare to document. Please feel free to edit if there are any mistakes to be found. All fields are 16-bit unsigned integers unless otherwise noted.

-   **Version** - A 4-byte unsigned integer specifying the version of this OBJD chunk. Known versions and their respective chunk sizes are (all versions except 136 are found in The Sims Online):
    -   136 (80 fields)
    -   138a (95 fields) - Used for The Sims 1 base game objects?
    -   138b (108 fields) - Used for The Sims 1 expansion objects?
    -   139 (96 fields)
    -   140 (97 fields)
    -   141 (97 fields)
    -   142 (105 fields)

<!-- -->

-   **Initial stack size** - The amount of size to allocate on the stack before running any functions for this object.
-   **Base graphic ID** - The ID of the first (or only) DGRP resource associated with this object. *May be zero*!
-   **Num graphics** - The number of DGRP resources that are used to display various states of the object.
-   **Main ID** - The ID of the BHAV that contains the code making up the main function for this object.
-   **Gardening ID** - The ID of the BHAV that contains the code making up the gardening function for this object.
-   **TTAB ID** - The ID of the TTAB (Tree TABle) associated with this object. TTAB contains the pie-menu options for this object.
-   **Interaction group** - No idea what this is for. See [SimTech](http://simtech.sourceforge.net/tech/objd.html).
-   **Type** - Which type of object this is.
    -   **0** - Unknown
    -   **2** - Person
    -   **4** - Normal buyable object
    -   **7** - Roaches, Stoves2, TrClownGen, AnimTester, HelpSystem, JobFinder, NPCController, Stoves, Tutorial, VisitGenerator, phonecall, unsnacker, CCPhonePlugin or EStove
    -   **8** - Stairs, doors, pool diving board & ladder or windows
    -   **9** - Cursor, for instance the yellow gridlines that appear around buy mode objects or the wall placement cursor.
    -   **10** - Prize tokens (not labeled in behaviour.iff - this value probably doesn't exist in TSO, since the notion of prize tokens doesn't exist)
    -   **11** - "Internal" temporary location for drop or shoo.
    -   **34** - Food (not labeled in behaviour.iff)

<!-- -->

-   **Master ID** - Used to identify multi-tile objects. If this is 0, the object occupies a single tile. If this is non-zero, all OBJDs in this IFF file with the same ID as this OBJD belongs to this multi-tile object.
-   **Sub Index** - *This is ignored if Master ID is 0!* If Master-ID is non-zero, this can be -1, meaning that the current OBJD is the master for this multi-tile object. The master OBJD should be used to reference everything except for graphics for the other parts of the object (though it is not known if the other fields are set to 0 in the other OBJDs). If the Sub Index isn't -1, it gives the (x,y) offset for the part of an object represented by the OBJD. The upper byte is the zero-relative y offset, while the lower byte is the zero-relative x offset. Increasing x values are left-to-right across the face of the object. Increasing y values are front-to-back (larger values are "behind" the face). The x and y may be reversed; also how the object is oriented (i.e., which way the front faces) is not clear.
-   **Wash hands ID** - The ID of the BHAV that contains the code making up the wash hands function for this object.
-   **Anim table ID** - If non-zero, this contains the ID of a STR# chunk that contains a list of animations associated with this object. Includes a2o (adult to object) and a2a (adult to adult) animations.
-   **GUID** - A 4-byte unique ID for this object. It is not known if this is set to 0 for multi-tile objects.
-   **Disabled** - Whether or not the object is allowed to show up in the game.
-   **Portal** - Portals in the game are defined as staircases. It is unclear what this field is used for in practice.
-   **Price** - The price of this object.
-   **Body strings ID** - The ID of a STR# resource that contains strings for this character, including things like the gender, sex, color, body type, and how to dress them for various circumstances. This field should be ignored unless this object is a character.
-   **SLOT ID** - ID of a SLOT resource for this object. The SLOT resource provides routing infomation that allows a character to approach the object.
-   **Allow Intersection** - The ID of a BHAV that contains a function (tree) used as a last resort for intersection calculation (to determine if two objects are allowed to intersect).
-   **Unknown** - 4 unknown bytes. SimTech says that values occur in these from time to time. They are assumed to be 0 by default.
-   **Prepare food ID** - The ID of the BHAV chunk that contains the Prepare Food function for this object. Assumed to be 0 if the object isn't a sim.
-   **Cook food ID** - The ID of the BHAV chunk that contains the Cook Food function for this object. Assumed to be 0 if the object isn't a sim.
-   **Place on surface ID** - The ID of the BHAV chunk that contains the Place on Surface function for this object. Assumed to be 0 if the object isn't a sim.
-   **Dispose ID** - The ID of the BHAV chunk that contains the Dispose function for this object. Assumed to be 0 if the object isn't a sim or food (?)
-   **Eat food** - The ID of the BHAV chunk that contains the Eat Food function for this object. Assumed to be 0 if the object isn't a sim.
-   **Pickup from slot ID** - The ID of the BHAV chunk that contains the Pickup From Slot function for this object. Assumed to be 0 if the object isn't food.
-   **Wash dish ID** - The ID of the BHAV chunk that contains the Wash Dish function for this object. Assumed to be 0 if this object isn't a sim.
-   **Eating surface ID** - The ID of the BHAV chunk that contains the Eating Surface function for this object. Assumed to be 0 if this object isn't a sim.
-   **Sit** - The ID of the BHAV chunk that contains the Sit function for this object. Assumed to be 0 if this object isn't a sim.
-   **Stand** - The ID of the BHAV chunk that contains the Stand function for this object.
-   **Sale Price** - The current price of this object. Assumed to be 0 if this object is a sim.
-   **Initial depreciation** - The inital depreciation of this object, subtracted after it is bought.
-   **Daily depreciation** - The daily depreciation of this object, subtracted daily.
-   **Self depreciating** - If self-depreciating is set to one, the object must provide its own logic to calculate the current price. If self depreciating is set to zero, the game provides logic to calcuate the current price, using the initial depreciation, daily depreciation, and depreciation limit. The initial depreciation is subtracted at the end of the first day. At the end of each subsequent day, the daily depreciation is subtracted but the price will never go below the depreciation limit.
-   **Depreciation limit** - The price of this object will never go below this limit.

### OBJf

This chunk type assigns BHAV subroutines to a number of events that occur in (or outside of?) the object, which are described in behavior.iff chunk 00F5.

-   **Zero** - 4 bytes set to zero
-   **Version** - A 4-byte unsigned integer specifying the version of this OBJf chunk; should be 0
-   **Magic number** - 4 bytes set to either the ASCII string "fJBO" or zero
-   **Function count** - A 4-byte unsigned integer specifying the number of condition/action function pairs defined by the OBJf chunk.
-   **Function table** - For each pair:
    -   **Condition function** - A 2-byte unsigned integer specifying the chunk ID of the conditional BHAV subroutine. If specified, the action function will be executed only if the condition function returns true; otherwise, the action function will simply always be executed.
    -   **Action function** - A 2-byte unsigned integer specifying the chunk ID of the action BHAV subroutine.

  
  
The event each function pair corresponds to is defined by the index of the condition/action pair.

### ObjM

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### objt

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### Optn

This chunk type stores current values for game options set by the user.

-   **Zero** - 4 bytes set to 0
-   **Version** - A 4-byte integer specifying the version of the Optn chunk; should be 0
-   **Magic number** - 4 bytes set to either the ASCII string "ntpO" or zero
-   **Flags** - These fields are unknown. The flags total to 24 bytes in The Sims 1, 25 bytes in the first release version of TSO (1.145.1.0), and 26 bytes in TSO New and Improved (1.1097.1.0) and onwards through to the last version of EA-Land.

### PALT

This chunk type holds a color palette. Its format is as follows:

-   **Version** - A 4-byte unsigned integer specifying the version of this PALT data; should be equal to 1
-   **Color entries count** - A 4-byte unsigned integer specifying the number of color entries defined in this color palette. Must be \>=1 and \<=256. All palettes in TS1 and TSO have all 256 colors defined, although often not all are used.
-   **Reserved** - 8 bytes set to zero
-   **Color entry data** - As many color entries as defined by *color entries count*. Each color entry is a 24-bit color value in RGB format.

### pers

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### POSI

This chunk type holds [Edith](SimAntics#Edith "wikilink") metadata in a way similar to [TREE](#TREE "wikilink") chunks. POSI chunks are associated with a BHAV chunk with the same ID, and provide coordinates for tree nodes used by Edith, as well as comment nodes not included in BHAV chunks.

### Rcon

Equivalent in format to [TPRP](#TPRP "wikilink"), apart from the magic number field which is set to the ASCII string "nocR" or zero.

### SIMI

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### SLOT

### SPR#

This chunk type holds a number of paletted sprites that share a common color palette and lack z-buffers and alpha buffers. SPR# chunks can be either big-endian or little-endian, which must be determined by comparing the first two bytes to zero (since no version number uses more than two bytes).

-   **Version** - A 4-byte unsigned integer specifying the version of this SPR# chunk; should be 502, 503, 504, 505, or 1001
-   **Sprite count** - A 4-byte unsigned integer specifying the number of sprites defined in this chunk; in version 1001, this field is often set to -1
-   **Palette ID** - A 4-byte unsigned integer referring to the chunk ID of a PALT used as the color palette for each sprite; this field may be set to -1 to indicate that an all-black, all-white, or similar color palette should be used. doors3.spf suggests that this color should be pure yellow (#FFFF00), but we're not sure what context the color would fall into anyway.
-   **Offset table** (Versions 502-505) - As many 4-byte unsigned integers as specified by the sprite count, each denoting the byte offset to the corresponding sprite from the beginning of this SPR# chunk

<!-- -->

-   **Sprites** - For each sprite:
    -   **Sprite version** (Version 1001) - A 4-byte unsigned integer equivalent to the SPR# chunk's version number
    -   **Sprite size** (Version 1001) - A 4-byte unsigned integer specifying the size of this sprite in bytes, excluding the version and size fields
    -   **Reserved** - 4 bytes always set to zero
    -   **Height** - A 2-byte unsigned integer specifying the height of this sprite in pixels
    -   **Width** - A 2-byte unsigned integer specifying the width of this sprite in pixels
    -   **Sprite data** - A variable number of rows encoded with RLE of a sort and terminated by an end marker. Each row command consists of a 1-byte command followed by a 1-byte count. The commands are:
        -   0x00 - Start marker; the count byte is ignored
        -   0x04 - Fill this row with pixel data that directly follows; the count byte of the row command denotes the size in bytes of the row's command/count bytes together with the supplied pixel data. In the pixel data, each pixel command consists of a 1-byte pixel command, followed by a 1-byte pixel count, followed lastly by a block of pixel data padded to a multiple of 2 bytes. If the row is not filled completely, the remainder is transparent. The pixel commands are:
            -   0x01 - Leave the next *pixel count* pixels as transparent. This pixel command has no pixel data.
            -   0x02 - Fill the next *pixel count* pixels with a single palette color. The pixel data is two bytes: the first byte denotes the palette color index, and the second byte is padding (which is always equal to the first byte but is ignored).
            -   0x03 - Set the next *pixel count* pixels to the palette color indices defined by the pixel data provided directly after this command. Each byte in the pixel data, minus the padding byte at the very end (if it exists), is a color index value to be copied to the row.
        -   0x05 - End marker. The count byte is always 0, but may be ignored.
        -   0x09 - Leave the next *count* rows as transparent.
        -   0x10 - Start marker, equivalent to 0x00; the count byte is ignored

### SPR2

This chunk type holds a number of paletted sprites that may have z-buffer and/or alpha channels.

-   **Version** - A 4-byte unsigned integer; should be 1000 or 1001

**Version 1000**

-   **Sprite count** - A 4-byte unsigned integer specifying the number of sprites defined in this chunk. A sprite consists of a color channel, a z-buffer channel, and/or an alpha channel all at a single zoom and a single point in time.
-   **Default palette** - A 4-byte unsigned integer specifying the Chunk ID of the [PALT](#PALT "wikilink") chunk contained within the same IFF file to use as the default color palette for color channels defined in this chunk. As the z-buffer and alpha channels are 8-bit grayscale, they do not refer to a color palette.
-   **Offset table** - As many 4-byte unsigned integers as specified by the sprite count, each denoting the byte offset to the corresponding sprite from the beginning of this SPR2 chunk

**Version 1001**

-   **Default palette** - A 4-byte unsigned integer specifying the Chunk ID of the [PALT](#PALT "wikilink") chunk contained within the same IFF file to use as the default color palette for color channels defined in this chunk. As the z-buffer and alpha channels are 8-bit grayscale, they do not refer to a color palette.
-   **Sprite count** - A 4-byte unsigned integer specifying the number of sprites defined in this chunk. A sprite consists of a color channel, a z-buffer channel, and/or an alpha channel all at a single zoom and a single point in time.

<!-- -->

-   **Sprite data** - For each sprite:
    -   **Sprite version** (Version 1001) - A 4-byte unsigned integer equivalent to the SPR2 chunk's version number
    -   **Sprite size** (Version 1001) - A 4-byte unsigned integer specifying the size of this sprite in bytes, excluding the version and size fields
    -   **Width** - A 2-byte unsigned integer specifying the width of each image in this sprite
    -   **Height** - A 2-byte unsigned integer specifying the height of each image in this sprite
    -   **Flags** - A 4-byte unsigned integer specifying the flags attributed to this sprite. Bit 0x01 means that the sprite has a color channel, bit 0x02 means that the sprite has a z-buffer channel, and bit 0x04 means that the sprite has an alpha channel. Due to a limitation in the sprite encoder, earlier channels are prerequisites for later channels, so only values of 0x01, 0x03, and 0x07 are valid.
    -   **Palette** - A 2-byte unsigned integer specifying the Chunk ID of a PALT chunk to use in place of *Default palette*, but in practice this is never done. This value is always either the same as the default or is the null value 0xA3A3.
    -   **Transparent color** - A 2-byte unsigned integer specifying the color index in this sprite's color palette that was used for pixels that are now fully transparent; furthermore, any pixels in the color channel defined with this color that were not given an alpha value must be assigned an opacity of zero and a z-buffer value of 255 (if the z-buffer channel is specified).
    -   **Y location** - A 2-byte signed integer specifying the offset to add to the Y direction in order to align this sprite correctly, in the case of multi-tile objects
    -   **X location** - A 2-byte signed integer specifying the offset to add to the X direction in order to align this sprite correctly, in the case of multi-tile objects
    -   **Sprite data** - A variable number of rows encoded with RLE of a sort and terminated by an end marker, encoding all specified channels at the same time. Each row command consists of a 2-byte little-endian integer consisting of a 3-bit command in the high order and a 13-bit count in the low order. The commands are:
        -   0x00 - Fill this row with pixel data that directly follows; the count byte of the row command denotes the size in bytes of the row's command/count bytes together with the supplied pixel data. In the pixel data, each pixel command consists of a 3-bit/13-bit command/count header followed by a block of pixel data padded to a multiple of 2 bytes. If the row is not filled completely, the remainder is transparent. The pixel commands are:
            -   0x01 (color and z-buffer required) - Set the next *pixel count* pixels in the z-buffer and color channels to the values defined by the pixel data provided directly after this command. Every group of 2 bytes in the pixel data provides a luminosity (z-buffer) or color index (color) value to be copied to the row for the z-buffer channel and color channel, respectively, in that order, using the full opacity value of 255 for each pixel that is not the transparent color.
            -   0x02 (color, z-buffer, and alpha required)) - Set the next *pixel count* pixels in the z-buffer, color, and alpha channels to the values defined by the pixel data provided directly after this command. Every group of 3 bytes in the pixel data, minus the padding byte at the very end (if it exists), provides a luminosity (z-buffer and alpha) or color index (color) value to be copied to the row for the z-buffer, color, and alpha channels, respectively, in that order. The alpha channel data is grayscale in the range 0-31, and the z buffer is in range 0-255.
            -   0x03 - Leave the next *pixel count* pixels in the color channel filled with the transparent color, in the z-buffer channel filled with 255, and in the alpha channel filled with 0. This pixel command has no pixel data.
            -   0x06 - Set the next *pixel count* pixels in the color channel to the palette color indices defined by the pixel data provided directly after this command. Every byte in the pixel data, minus the padding byte at the very end (if it exists), provides a color index value to be copied to the row for the color channel using the full opacity value of 255 and the closest z-buffer value of 0 if the pixel is not the transparent color, or otherwise the no opacity value of 0 and the farthest z-buffer value of 255.
        -   0x04 - Leave the next *count* rows in the color channel filled with the transparent color, in the z-buffer channel filled with 255, and in the alpha channel filled with 0.
        -   0x05 - Sprite end marker; the count byte is always 0, but may be ignored

### STR#

This chunk type holds text strings.

-   **Version** - A 2-byte signed integer; should be 00 00 (0), FF FF (−1), FE FF (−2), FD FF (−3), or FC FF (−4).

Some STR# chunks in the game do not specify any data after the version field. For those that do, the rest of the chunk is as follows:

**Format: 00 00** (0)

-   **Number of strings** - A 2-byte unsigned integer specifying the number of strings contained in this chunk
-   **Strings** - As many Pascal strings as defined by the previous field; each Pascal string is a 1-byte length followed by that many bytes of character data

**Format: FF FF** (−1)  
This format changes 00 00 to use C strings rather than Pascal strings.

-   **Number of strings** - A 2-byte unsigned integer specifying the number of strings contained in this chunk
-   **Strings** - As many null-terminated strings as defined by the previous field

**Format: FE FF** (−2)  
This format changes FF FF to use string pairs rather than single strings.

-   **Number of string pairs** - A 2-byte unsigned integer specifying the number of string pairs contained in this chunk
-   **String pairs** - As many pairs of two null-terminated strings as defined by the previous field. The second string directly follows the first. Usually, the first string of the pair is the data and the second string is a comment. In most cases, the comment string is empty, so it looks like the main string is terminated with two null characters.

**Format: FD FF** (−3)  
This format changes FE FF to assign a language code to each string pair.

-   **Number of string pairs** - A 2-byte unsigned integer specifying the number of string pairs contained in this chunk
-   **String pairs** - As many pairs of two null-terminated strings as defined by the previous field, each pair preceded by a 1-byte language code from the table below. The second string directly follows the first. Usually, the first string of the pair is the data and the second string is a comment. In most cases, the comment string is empty, so it looks like the main string is terminated with two null characters.

**Format: FC FF** (−4)  
This format is only used in The Sims Online. The format uses Pascal-like strings that use a variable-length coding for the length, and it groups all string pairs by their language code. Presumably, these two changes were made as a tiny performance improvement; the Pascal-like strings avoid the slowness in copying null-terminated strings into memory (null-terminated strings require two passes over the string: one to calculate the length, and then, after allocating that much memory, another to copy the data over; Pascal-like strings do not require the first pass) while avoiding the 255-character limit of regular Pascal strings; the pre-sorting of strings by their language codes avoids the need for the client to sort the strings itself (which would require either resizable arrays or the use of two passes over all of the strings per language; one to find the count, and then, after allocating memory for that many strings, another to copy the data over).

-   **Number of language sets** - A 1-byte unsigned integer specifying the number of language sets. This field is always 20; even strings that aren't translated specify 20 language sets; the ones which are not translated remain empty.
-   **Language sets** - As many language sets as defined by the previous field. For each language set:
    -   **Number of string pairs** - A 2-byte unsigned integer specifying the number of string pairs contained in this language set
    -   **String pairs** - As many pairs of two Pascal-like strings as defined by the previous field, each pair preceded by a 1-byte *Language set index* (which is equivalent to the language code minus 1); the language set index must be the same for each string pair in a given language set. The second string directly follows the first. Usually, the first string of the pair is the data and the second string is a comment. Each string uses a variable length coding for the length, whereby the length is 1 or more bytes large. If the first byte's most significant bit (msb) is not set, then the byte is used as is. Otherwise, the bottom 7 bits of the first byte are used as the bottom 7 bits of the length, and the process repeats with the second byte for the next 7 bits of the length. For example, the 1-byte sequence "43" means length 0x43 = 67; the 2-byte sequence "B2 31" means length (0xB2-0x80)\|(0x31\<\<7) = 6322; the 3-byte sequence "A6 91 5E" means length (0xA6-0x80)\|((0x91-0x80)\<\<7)\|(0x5E\<\<14) = 1542310.

**Language codes**

| Code | Language                | Character encoding                     |
|------|-------------------------|----------------------------------------|
| 0    | unused (default)        | Windows-1252 (derived from ISO-8859-1) |
| 1    | English (US)            | Windows-1252 (derived from ISO-8859-1) |
| 2    | English (International) | Windows-1252 (derived from ISO-8859-1) |
| 3    | French                  | Windows-1252 (derived from ISO-8859-1) |
| 4    | German                  | Windows-1252 (derived from ISO-8859-1) |
| 5    | Italian                 | Windows-1252 (derived from ISO-8859-1) |
| 6    | Spanish                 | Windows-1252 (derived from ISO-8859-1) |
| 7    | Dutch                   | Windows-1252 (derived from ISO-8859-1) |
| 8    | Danish                  | Windows-1252 (derived from ISO-8859-1) |
| 9    | Swedish                 | Windows-1252 (derived from ISO-8859-1) |
| 10   | Norwegian               | Windows-1252 (derived from ISO-8859-1) |
| 11   | Finnish                 | Windows-1252 (derived from ISO-8859-1) |
| 12   | Hebrew                  | n/a                                    |
| 13   | Russian                 | n/a                                    |
| 14   | Portuguese              | Windows-1252 (derived from ISO-8859-1) |
| 15   | Japanese                | Windows-932 (derived from Shift-JIS)   |
| 16   | Polish                  | Windows-1250 (derived from ISO-8859-2) |
| 17   | Simplified Chinese      | Windows-936 (derived from GB2312)      |
| 18   | Traditional Chinese     | Windows-950 (derived from Big5)        |
| 19   | Thai                    | Windows-874 (derived from TIS-620)     |
| 20   | Korean                  | Windows-949 (derived from EUC-KR)      |

The Sims 1 has language codes for Hebrew and Russian, but the entries for these languages are always either absent, blank, or untranslated. The Sims 1 did not have an official release in Russia or Israel. However, there have been several unofficial translations of the game into Russian by 8bit.ru, Torum Media, Русский проект (Russian Project), Gamecity, AGS, and 7Wolf. (There is also an unofficial translation of the game into Czech by XSTeam.)

### TATT

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### THMB

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### TMPL

This chunk type provides a name and type for some or all fields in another chunk, whose name is usually provided in the TMPL chunk's label. The number of fields described in a TMPL chunk must be determined by the size of the chunk itself.

For each field:

-   **Name** - A Pascal string providing a name for this field
-   **Type** - A 4-byte non-null-terminated string denoting this field's type: DLNG, UWRD, CSTR, or CASE. CASE has also been used for entries that do not describe a field, but provide further information regarding the previous one (such as default values).

### TPRP

This chunk type provides descriptions for arguments and local variables found in a BHAV chunk. Each TPRP chunk corresponds to a BHAV chunk with the same ID.

-   **Reserved** - 4 bytes, always zero
-   **Version** - A 4-byte integer specifying the version of the TPRP chunk. Known versions are 2, 3, 4 and 5.
-   **Magic number** - 4 bytes set to either the ASCII string "PRPT" or zero
-   **Argument count** - A 4-byte unsigned integer specifying the number of arguments described by this chunk
-   **Local count** - A 4-byte unsigned integer specifying the number of local variables described by this chunk
-   **Argument labels** - As many strings as denoted by the argument count field. Strings in versions up to 4 are equivalent in format to [TRCN](#TRCN "wikilink") versions 0 and 1. Strings in version 5 are Pascal-like strings starting with one byte specifying their length, possibly encoded similarly to [STR#](#STR.23 "wikilink") FCFF strings.
-   **Local labels** - As many strings as denoted by the label count field
-   **Unknown** - 4 bytes found in versions up to 4
-   **Unknown** - 1 byte found in version 5
-   **Argument flags** - As many 1-byte flags as denoted by the argument count field, each set to 0 or 1.
-   **Local flags** - As many 1-byte flags as denoted by the local count field, each set to 0 or 1.
-   **Unknown** - 4 bytes found starting from version 3
-   **Unknown** - 4 bytes found starting from version 4

### TRCN

This chunk type provides text labels and suggested ranges for constants defined in a BCON chunk bearing the same chunk ID. These labels and ranges are provided to the programmer in [Edith](SimAntics#Edith "wikilink") and are not used ingame.

-   **Reserved** - 4 bytes always equal to zero
-   **Version** - A 4-byte unsigned integer specifying the version of this TRCN chunk. May be 0, 1, or 2.
-   **Magic number** - 4 bytes set to either the ASCII string "NCRT" or zero
-   **Metadata count** - A 4-byte unsigned integer specifying the number of constants for which metadata is defined in this chunk; this must exactly match the number of constants defined in the associated BCON chunk
-   **Metadata** - For each behavior constant:
    -   **Is used** - A 4-byte integer marking whether or not this behavior constant is used ingame
    -   **Default value** - A 4-byte integer specifying the default value for this constant
    -   **Name** - A string defining a name for the corresponding BCON entry
    -   **Comment** - A comment which appears in the comment box when the programmer has selected this BCON entry

  
  
In versions 0 and 1, strings are stored as null-terminated C strings padded to a length—consisting of both the string and the null character—which is a multiple of 2 using the 0xA3 byte after the null character. Version 2 uses non-null-terminated Pascal-like strings with no padding encoded the same way as those in [STR#](#STR.23 "wikilink") FCFF chunks; this is seen in FCNS chunks.

The following range-related fields exist only in versions 1 and 2:

-   -   **Range enforced** - A 1-byte integer possibly defining whether the range is enforced by Edith
    -   **Range min** - A 2-byte integer specifying the minimum value for this behavior constant
    -   **Range max** - A 2-byte integer specifying the maximum value for this behavior constant

### TREE

This chunk type carries the box-and-arrow formatting used by [Edith](SimAntics#Edith "wikilink"), and readable by Codex. This chunk type can be found in some NPCs in The Sims 1 and is ignored by the game.

### TTAB

This chunk type defines a list of interactions for an object and assigns a BHAV subroutine for each interaction. The pie menu labels shown to the user are stored in a TTAs chunk with the same ID.

-   **Interaction count** - A 2-byte unsigned integer specifying the number of interactions defined by this chunk
-   **Version** - A 2-byte unsigned integer specifying the version of this TTAB chunk. The highest known version is 11; versions found in The Sims Online which are documented below are 5, 8, 9, 10 and 11.
-   **Compression code** - The 1-byte field encoding compression code (always 1), found in versions 9 and 10.

In versions 9 and 10, the following fields are compressed using the [field encoding algorithm](http://simtech.sourceforge.net/tech/misc.html#fields). 2-byte fields use encoding widths of 5, 8, 13 and 16, while 4-byte fields use widths of 6, 11, 21 and 32 (including the sign bit).

-   **Interactions** - For each interaction:
    -   **Action function** - A 2-byte chunk ID referring to the BHAV subroutine executed when the interaction is executed.
    -   **Guard function** - A 2-byte chunk ID referring to the guard BHAV subroutine. The action function is only allowed to run if the guard function returns true, or if the field is set to zero.
    -   **Motive entry count** - A 4-byte unsigned integer specifying the number of motive entries defined in this interaction (always 16)
    -   **Flags** - A 4-byte unsigned integer used as a bit field
    -   **TTAs index** - A 4-byte unsigned integer specifying the index of the string used by this interaction in the corresponding TTAs chunk. This is treated as the ID of the interaction by SimAntics, for example the Push Interaction primitive with interaction number 7 will push the interaction owned by the specified object with the TTAs index of 7 onto the stack object's queue.
    -   **Attenuation code** (added in version 7) - A 4-byte unsigned integer that controls the attenuation value described below. The possible values for this field are 0 to 4, which correspond to custom (specified in the next field), none, low, moderate and high respectively. Presumably, the attenuation values of these constants are 0, 0.02, 0.1 and 0.2, which are the most commonly used values in TTAB versions predating this field.
    -   **Attenuation value** - A 4-byte IEEE 754 float. If the attenuation code is non-zero, this field is usually zero/garbage, otherwise it specifies how quickly the interaction advertisement fades.
    -   **Autonomy threshold** - A 4-byte unsigned integer, usually 50 or 100. Only Sims whose autonomy level is higher than or equal to the threshold are able to perform this interaction autonomously.
    -   **Joining index** - A 4-byte signed integer that, if not -1, indicates the ID of a joinable activity.

Each object broadcasts a number of advertisements that describe how strongly a motive is affected by the object. These values are then used by a Sim's autonomy logic to determine how likely they are going to perform this action, based on current motive states and the Sim's personality and skills.

-   -   **Motive entries** - For each motive:
        -   **Effect range minimum** (added in version 7) - A 2-byte signed integer specifying the lowest end of the advertised motive effect range. If this field is not present or set to zero, the maximum value is used.
        -   **Effect range maximum** - A 2-byte signed integer specifying the highest end of the advertised motive effect range.
        -   **Personality modifier** (added in version 7) - A 2-byte unsigned integer used to determine a value within the range specified by the two previous fields. Possible values for this field are 1 to 22, labeled in behaviour.iff chunk 00E8. For example, if the personality modifier is 14 (cleaning skill) and the effect range is 1-8, then the strength of the motive advertisement to a Sim with level 5 (50%) cleaning skill would be 4.
    -   **Unknown** (added in version 10) - A 4-byte unsigned integer

### TTAs

Pie menu text strings; equivalent in format to [STR#](#STR.23 "wikilink")

### uChr

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

### WALm

*This chunk type isn't used in any of the IFF files found in The Sims Online.*

## Resource Map (`rsmp`)

The `rsmp` chunk consists of information about each chunk's position in the file for quick accessing specific chunks. Its *Chunk ID* is zero, and its *Label* is blank (all 0x00 characters).

Its data consists of the following header (with little-endian integers):

-   **Reserved** - 4 bytes set to zero
-   **Version** - A 4-byte unsigned integer specifying the version of the Resource Map; 0 is used in The Sims 1, 1 in The Sims Online
-   **Magic number** - 4 bytes set to either the non-null-terminated ASCII string "pmsr" ("rsmp" byte-swapped) or 0
-   **Size** - A 4-byte unsigned integer specifying the size of the entire `rsmp` chunk. In rsmp version 1, this field is usually set to 0 but may be set to garbage (as in the case of userdata/options.iff). This field should be ignored.
-   **Chunk type count** - A 4-byte unsigned integer specifying the total number of unique chunk types in the file, excluding `rsmp` and any of type `XXXX`

Then, for each chunk type:

-   **Type** - The *byte-swapped* 4-byte chunk type identifier
-   **Count** - A 4-byte unsigned integer referring to the number of chunks in the IFF file that bear this chunk type

  
Then, for each chunk of this type:

-   -   **Offset** - A 4-byte unsigned integer referring to the offset from the beginning of the IFF file to this chunk
    -   **Chunk ID** - An unsigned integer referring to the byte-swapped *Chunk ID* of this chunk; in rsmp version 0, this number is 2 bytes, whereas in version 1, this number is 4 bytes
    -   **Flags** - A 2-byte unsigned integer referring to the byte-swapped *Flags* of this chunk
    -   **Label** - In rsmp version 0, this field is a null-terminated version of this chunk's label (all bytes after the first null character are discarded). If the string is an even number of characters, an extra null character is added. In version 1, this field is a Pascal string.

The `rsmp` chunk is optional. Some IFF files in the game (examples?) have a rsmp chunk that is empty or misses entries in the middle somewhere, so don't always rely on it.
