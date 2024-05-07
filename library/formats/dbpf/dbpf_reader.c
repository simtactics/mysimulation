/* ===== include files =============================== */

#include "dbpf_reader.h"
#include <stdio.h>
#include <stdlib.h>

/* ===== internal constants and macros =============== */

const char *const dbpf_return_value_strings[DBPF_RET_VAL_NUMBER] =
{
	"Success",
	"General failure",
	"File input/outpu operation failed",
	"Invalid file format",
	"Invalid archive or index version",
	"Argument out of range"
};

/* ===== internal functions declaration ============== */

static void dbpf_reset(dbpf_archive *dbpf);

/* ===== external functions definition =============== */

dbpf_ret dbpf_init(dbpf_archive *dbpf, const char * path)
{
	dbpf_ret retVal = DBPF_E_NOT_OK;

	if (DBPF_NULL != dbpf && DBPF_NULL != path)
	{
		FILE *fp = DBPF_NULL;

		dbpf->path = path;
		fp = fopen(dbpf->path, "rb");

		if (DBPF_NULL != fp) {
			/* get file size*/
			fseek(fp, 0L, SEEK_END);
			dbpf->dataSize = ftell(fp);
			rewind(fp);

			dbpf->data = malloc(sizeof(uint8_t) * dbpf->dataSize);

			if (DBPF_NULL != dbpf->data) {
				uint32_t readDataSize = fread(dbpf->data, sizeof(uint8_t), dbpf->dataSize, fp);
				if (readDataSize == dbpf->dataSize) {
					if (sizeof(dbpf_header) <= dbpf->dataSize &&
						'D' == dbpf->data[0] &&
						'B' == dbpf->data[1] &&
						'P' == dbpf->data[2] &&
						'F' == dbpf->data[3]
						)
					{
						dbpf->header = (dbpf_header *)dbpf->data;
						retVal = DBPF_E_OK;
					}
					else
					{
						retVal = DBPF_E_FILE_INVALID_FORMAT;
						free(dbpf->data);
						dbpf_reset(dbpf);
					}
				}
				else {
					free(dbpf->data);
					dbpf_reset(dbpf);
				}
			}

			fclose(fp);
		}
		else {
			retVal = DBPF_E_FILE_IO_FAILED;
		}
	}

	return retVal;
}

void dbpf_free(dbpf_archive *dbpf)
{
	if (DBPF_NULL != dbpf)
	{
		if (DBPF_NULL != dbpf->data)
		{
			free(dbpf->data);
		}
		dbpf_reset(dbpf);
	}
}

dbpf_bool dbpf_is_initialized(dbpf_archive *dbpf)
{
	return (
		DBPF_NULL != dbpf &&
		DBPF_NULL != dbpf->data &&
		0 < dbpf->dataSize		   &&
		DBPF_NULL != dbpf->header &&
		DBPF_NULL != dbpf->path
		);
}

dbpf_ret dbpf_print_info(dbpf_archive *dbpf) {
	dbpf_ret retVal = DBPF_E_NOT_OK;

	if (dbpf_is_initialized(dbpf))
	{
		retVal = DBPF_E_OK;

		printf("File:\t\t\t\t%s\n", dbpf->path);
		printf("DBPF version:\t\t\t%d.%d\n", dbpf->header->majorVersion, dbpf->header->minorVersion);
		printf("Size:\t\t\t\t%d\n", dbpf->dataSize);
		printf("Index version:\t\t\t%d.%d\n", dbpf->header->indexMajorVersion, dbpf->header->indexMinorVersion);
		printf("Index entry count:\t\t%d\n", dbpf->header->indexEntryCount);
		printf("Index size:\t\t\t%d\n", dbpf->header->indexSize);
		printf("Index entry size:\t\t%d\n", dbpf->header->indexSize / dbpf->header->indexEntryCount);
		printf("Index offset:\t\t\t%d\n", dbpf->header->indexOffset);
		printf("Index first entry offset:\t%d\n", dbpf->header->indexFirstEntryOffset);
		printf("Index type:\t\t\t%d\n", *((uint32_t*)(dbpf->data + dbpf->header->indexOffset)));
	}

	return retVal;
}

void dbpf_print_ret(dbpf_ret retVal)
{
	printf("DBPF status: ");
	if (DBPF_RET_VAL_NUMBER > retVal)
	{
		printf("%s", dbpf_return_value_strings[retVal]);
	}
	else
	{
		printf("Invalid return value");
	}
	printf("\n");
}

/* dbpf_2_x functions */

dbpf_bool dbpf_2_x_is_version_valid(dbpf_archive *dbpf)
{
	dbpf_bool isVersionValid = DBPF_FLASE;

	if (dbpf_is_initialized(dbpf))
	{
		isVersionValid = (
			DBPF_2_X_VERSION_MAJOR == dbpf->header->majorVersion      &&
			DBPF_2_X_VERSION_MINOR == dbpf->header->minorVersion      &&
			DBPF_2_X_INDEX_VERSION_MAJOR == dbpf->header->indexMajorVersion &&
			DBPF_2_X_INDEX_VERSION_MINOR == dbpf->header->indexMinorVersion
			);
	}

	return isVersionValid;
}

dbpf_ret dbpf_2_x_get_index_entry(dbpf_archive *dbpf, dbpf_2_x_index_entry **entry, uint32_t entryNumber)
{
	dbpf_ret retVal = DBPF_E_NOT_OK;

	if (dbpf_is_initialized(dbpf) && DBPF_NULL != entry) {
		if (dbpf_2_x_is_version_valid(dbpf))
		{
			if (entryNumber < dbpf->header->indexEntryCount)
			{
				retVal = DBPF_E_OK;

				/* entry address := data address + index offset + size of index type + size of index entry * requested entry number */
				*entry = (dbpf_2_x_index_entry *)(dbpf->data + dbpf->header->indexOffset + sizeof(uint32_t) + sizeof(dbpf_2_x_index_entry) * entryNumber);
			}
			else
			{
				retVal = DBPF_E_OUT_OF_RANGE;
			}
		}
		else
		{
			retVal = DBPF_E_INVALID_VERSION;
		}
	}

	return retVal;
}

dbpf_ret dbpf_2_x_print_index_entry(dbpf_2_x_index_entry *entry)
{
	dbpf_ret retVal = DBPF_E_NOT_OK;

	if (DBPF_NULL != entry) {
		retVal = DBPF_E_OK;

		printf("Type:\t\t0x%08x\n", entry->type);
		printf("Group:\t\t0x%08x\n", entry->group);
		printf("Instance high:\t0x%08x\n", entry->instanceHigh);
		printf("Instance low:\t0x%08x\n", entry->instanceLow);
		printf("Offset:\t\t0x%08x\n", entry->offset);
		printf("File size:\t0x%08x\n", entry->fileSize);
		printf("Mem size:\t0x%08x\n", entry->memSize);
		printf("Compressed:\t0x%04x\n", entry->compressed);
		printf("Unknown:\t0x%04x\n", entry->unknown);
	}

	return retVal;
}

/* ===== internal functions definition =============== */

static void dbpf_reset(dbpf_archive *dbpf)
{
	if (DBPF_NULL != dbpf) {
		dbpf->data = DBPF_NULL;
		dbpf->dataSize = 0;
		dbpf->header = DBPF_NULL;
		dbpf->path = DBPF_NULL;
	}
}

