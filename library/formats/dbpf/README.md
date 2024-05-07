# dbpf_reader

A compact DBPF (Database Packed File) reader library. This library provides a simple interface in C for reading DBPF files. It was originally created for reading of .package files used in The Sims 4, but it can be easily extended for support of other DBPF versions. The format which is currently fully supported is DBPF 2.1 (index version 0.3).

## DBPF - Database Packed File

DBPF is a proprietary archive format used in many games developed by Maxis Studios, including The Sims series, SimCity series and Spore.
Further reading on DBPF format:

- [SC4D Encyclopaedia - DBPF](https://wiki.sc4devotion.com/index.php?title=DBPF)
- [Sims Wiki - Sims 3: DBPF](http://simswiki.info/Sims_3:DBPF)
- [Mod The Sims - Database Packed File](https://modthesims.info/wiki.php?title=DBPF)

### Code example

Below you can find a simple code example for extracting thumbnail files from a .package file (The Sims 4).

```c
#include <stdio.h>
#include <stdint.h>
#include "dbpf_reader.h" /* for dbpf_reader library interface */
#include "zlib.h"        /* for decompression of files */

int main(int argc, char **argv)
{
	/* Structure containing single dbpf archive information. */
	dbpf_archive dbpf;

	/* Read the archive from example.package file. A buffer will be dynamically
	   allocated for the archive content. */
	dbpf_ret retVal = dbpf_init(&dbpf, "example.package");

	if (DBPF_E_OK != retVal)
	{
		/* Write error information to stdout. No resources were dynamically
		   allocated. */
		dbpf_print_ret(retVal);
		return 1;
	}
	/* Pointer to a singe index entry. Notice the 2_x prefix indicating that this type
	   is only applicable for DBPF version 2.x. Index entries have various layouts
	   depending on DBPF version. */
	dbpf_2_x_index_entry *entry;

	char thumbnailFileName[256];

	for (uint32_t entryNum = 0u; entryNum < dbpf.header->indexEntryCount; entryNum++)
	{
		retVal = dbpf_2_x_get_index_entry(&dbpf, &entry, entryNum);
		if (DBPF_E_OK != retVal)
		{
			/* Write error information to stdout */
			dbpf_print_ret(retVal);
			continue;
		}

		if (DBPF_INDEX_ENTRY_TYPE_THUMB != entry->type)
		{
			/* Skip this entry as we are only intereseted in extracting
			   thumbnails. */
			continue;
		}

		uint32_t ucompSize = entry->memSize;
		uint32_t compSize = entry->fileSize & (~0x80000000);

		/* Allocate buffer for decompressed thumbnail file */
		uint8_t *resourceBuffer = malloc(sizeof(uint8_t) * ucompSize);

		if (!resourceBuffer)
		{
			printf("Failed to allocate memory\n");
			dbpf_free(&dbpf);
			return 1;
		}

		/* Decompress the thumbnail file using zlib uncompress */
		int res = uncompress((Bytef *)resourceBuffer,
				     &ucompSize,
				     (Bytef *)(dbpf.data + entry->offset),
				     compSize);
		if (Z_OK != res) {
			printf("Decompression for entry %lu failed\n", entryNum);
			free(resourceBuffer);
			continue;
		}

		/* Use high DWORD of instance ID in hex as filename */
		sprintf(thumbnailFileName, "0x%08x.jpg", entry->instanceHigh);
		FILE *fp = fopen(thumbnailFileName, "wb");
		if (fp)
		{
			(void) fwrite(resourceBuffer, 1u, ucompSize, fp);
			fclose(fp);
		}

		free(resourceBuffer);
	}
	/* Free allocated resources */
	dbpf_free(&dbpf);

	return 0;
}
```
