#pragma once

/* ===== include files =============================== */

#include <stdint.h>

/* ===== external constants and macros =============== */

#ifdef DBPFREADER_EXPORTS
#define DBPFREADER_API __declspec(dllexport)
#else
#define DBPFREADER_API __declspec(dllimport)
#endif

/* Return values */
#define DBPF_E_OK 0u
#define DBPF_E_NOT_OK 1u
#define DBPF_E_FILE_IO_FAILED 2u
#define DBPF_E_FILE_INVALID_FORMAT 3u
#define DBPF_E_INVALID_VERSION 4u
#define DBPF_E_OUT_OF_RANGE 5u
#define DBPF_RET_VAL_NUMBER 6u

/* dbpf_bool values */
#define DBPF_FLASE 0u
#define DBPF_TRUE 1u

/* NULL pointer */
#define DBPF_NULL ((void *)0)

/* Index entries types */
#define DBPF_INDEX_ENTRY_TYPE_THUMB 0x3c1af1f2u

/* DBPF_2_X versions */
#define DBPF_2_X_VERSION_MAJOR 2u
#define DBPF_2_X_VERSION_MINOR 1u
#define DBPF_2_X_INDEX_VERSION_MAJOR 0u
#define DBPF_2_X_INDEX_VERSION_MINOR 3u

/* ===== external types ============================== */

typedef uint8_t dbpf_ret;
typedef uint8_t dbpf_bool;

#pragma pack(1)
typedef struct {
  uint8_t magic[4]; /* "DBPF" */
  uint32_t majorVersion;
  uint32_t minorVersion;
  uint32_t unknown1;     /* unused */
  uint32_t unknown2;     /* unused */
  uint32_t unknown3;     /* should be zero in DBPF 2.0 */
  uint32_t dateCreated;  /* Unix time stamp */
  uint32_t dateModified; /* Unix time stamp */
  uint32_t indexMajorVersion;
  uint32_t indexEntryCount;
  uint32_t indexFirstEntryOffset;
  uint32_t indexSize;
  uint32_t holeEntryCount;
  uint32_t holeOffset;
  uint32_t holeSize;
  uint32_t indexMinorVersion;
  uint32_t indexOffset;
  uint32_t unknown4;
  uint8_t reserver[24];
} dbpf_header;

typedef struct {
  uint32_t type;
  uint32_t group;
  uint32_t instanceHigh;
  uint32_t instanceLow;
  uint32_t offset;
  uint32_t fileSize; /* size of compressed data */
  uint32_t memSize;  /* size of uncompressed data */
  uint16_t compressed;
  uint16_t unknown;
} dbpf_2_x_index_entry;
#pragma pack(8) /* back to default */

typedef struct {
  uint8_t *data;
  dbpf_header *header;
  const char *path;
  int32_t dataSize;
} dbpf_archive;

/* ===== external functions declaration ============== */

DBPFREADER_API dbpf_ret dbpf_init(dbpf_archive *dbpf, const char *path);

DBPFREADER_API void dbpf_free(dbpf_archive *dbpf);

DBPFREADER_API dbpf_bool dbpf_is_initialized(dbpf_archive *dbpf);

DBPFREADER_API dbpf_ret dbpf_print_info(dbpf_archive *dbpf);

DBPFREADER_API void dbpf_print_ret(dbpf_ret retVal);

/* dbpf_2_x functions*/

DBPFREADER_API dbpf_bool dbpf_2_x_is_version_valid(dbpf_archive *dbpf);

DBPFREADER_API dbpf_ret dbpf_2_x_get_index_entry(dbpf_archive *dbpf,
                                                 dbpf_2_x_index_entry **entry,
                                                 uint32_t entryNumber);

DBPFREADER_API dbpf_ret dbpf_2_x_print_index_entry(dbpf_2_x_index_entry *entry);
