/*
    bench.c - Demo module to benchmark open-source compression algorithms
    Copyright (C) Yann Collet 2012-2015

    GPL v2 License

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    You can contact the author at :
    - zstd source repository : https://github.com/Cyan4973/zstd
    - ztsd public forum : https://groups.google.com/forum/#!forum/lz4c
*/

/* **************************************
*  Compiler Options
****************************************/
/* Disable some Visual warning messages */
#ifdef _MSC_VER
#  define _CRT_SECURE_NO_WARNINGS                /* fopen */
#  pragma warning(disable : 4127)                /* disable: C4127: conditional expression is constant */
#endif

/* Unix Large Files support (>4GB) */
#define _FILE_OFFSET_BITS 64
#if (defined(__sun__) && (!defined(__LP64__)))   /* Sun Solaris 32-bits requires specific definitions */
#  define _LARGEFILE_SOURCE
#elif ! defined(__LP64__)                        /* No point defining Large file for 64 bit */
#  define _LARGEFILE64_SOURCE
#endif

/* S_ISREG & gettimeofday() are not supported by MSVC */
#if defined(_MSC_VER) || defined(_WIN32)
#  define BMK_LEGACY_TIMER 1
#endif


/* *************************************
*  Includes
***************************************/
#include <stdlib.h>      /* malloc, free */
#include <string.h>      /* memset */
#include <stdio.h>       /* fprintf, fopen, ftello64 */
#include <sys/types.h>   /* stat64 */
#include <sys/stat.h>    /* stat64 */

/* Use ftime() if gettimeofday() is not available */
#if defined(BMK_LEGACY_TIMER)
#  include <sys/timeb.h>  /* timeb, ftime */
#else
#  include <sys/time.h>   /* gettimeofday */
#endif

#include "mem.h"
#include "zstd_static.h"
#include "xxhash.h"
#include "datagen.h"      /* RDG_genBuffer */


/* *************************************
*  Compiler specifics
***************************************/
#if !defined(S_ISREG)
#  define S_ISREG(x) (((x) & S_IFMT) == S_IFREG)
#endif

#ifdef _MSC_VER
#define snprintf sprintf_s
#endif


/* *************************************
*  Constants
***************************************/
#define NBLOOPS    3
#define TIMELOOP   2500

#define KB *(1 <<10)
#define MB *(1 <<20)
#define GB *(1U<<30)

static const size_t maxMemory = (sizeof(size_t)==4)  ?  (2 GB - 64 MB) : (size_t)(1ULL << ((sizeof(size_t)*8)-31));
#define DEFAULT_CHUNKSIZE   (4 MB)

static U32 g_compressibilityDefault = 50;


/* *************************************
*  console display
***************************************/
#define DISPLAY(...)         fprintf(stderr, __VA_ARGS__)
#define DISPLAYLEVEL(l, ...) if (g_displayLevel>=l) { DISPLAY(__VA_ARGS__); }
static U32 g_displayLevel = 2;   /* 0 : no display;   1: errors;   2 : + result + interaction + warnings;   3 : + progression;   4 : + information */


/* *************************************
*  Exceptions
***************************************/
#ifndef DEBUG
#  define DEBUG 0
#endif
#define DEBUGOUTPUT(...) if (DEBUG) DISPLAY(__VA_ARGS__);
#define EXM_THROW(error, ...)                                             \
{                                                                         \
    DEBUGOUTPUT("Error defined at %s, line %i : \n", __FILE__, __LINE__); \
    DISPLAYLEVEL(1, "Error %i : ", error);                                \
    DISPLAYLEVEL(1, __VA_ARGS__);                                         \
    DISPLAYLEVEL(1, "\n");                                                \
    exit(error);                                                          \
}


/* *************************************
*  Benchmark Parameters
***************************************/
static int nbIterations = NBLOOPS;
static size_t g_blockSize = 0;

void BMK_SetNbIterations(int nbLoops)
{
    nbIterations = nbLoops;
    DISPLAY("- %i iterations -\n", nbIterations);
}

void BMK_SetBlockSize(size_t blockSize)
{
    g_blockSize = blockSize;
    DISPLAY("using blocks of size %u KB \n", (U32)(blockSize>>10));
}


/* ********************************************************
*  Private functions
**********************************************************/

#if defined(BMK_LEGACY_TIMER)

static int BMK_GetMilliStart(void)
{
  /* Based on Legacy ftime()
  *  Rolls over every ~ 12.1 days (0x100000/24/60/60)
  *  Use GetMilliSpan to correct for rollover */
  struct timeb tb;
  int nCount;
  ftime( &tb );
  nCount = (int) (tb.millitm + (tb.time & 0xfffff) * 1000);
  return nCount;
}

#else

static int BMK_GetMilliStart(void)
{
  /* Based on newer gettimeofday()
  *  Use GetMilliSpan to correct for rollover */
  struct timeval tv;
  int nCount;
  gettimeofday(&tv, NULL);
  nCount = (int) (tv.tv_usec/1000 + (tv.tv_sec & 0xfffff) * 1000);
  return nCount;
}

#endif


static int BMK_GetMilliSpan( int nTimeStart )
{
  int nSpan = BMK_GetMilliStart() - nTimeStart;
  if ( nSpan < 0 )
    nSpan += 0x100000 * 1000;
  return nSpan;
}

static U64 BMK_getFileSize(const char* infilename)
{
    int r;
#if defined(_MSC_VER)
    struct _stat64 statbuf;
    r = _stat64(infilename, &statbuf);
#else
    struct stat statbuf;
    r = stat(infilename, &statbuf);
#endif
    if (r || !S_ISREG(statbuf.st_mode)) return 0;   /* No good... */
    return (U64)statbuf.st_size;
}


/* ********************************************************
*  Bench functions
**********************************************************/
typedef struct
{
    const char* srcPtr;
    size_t srcSize;
    char*  cPtr;
    size_t cRoom;
    size_t cSize;
    char*  resPtr;
    size_t resSize;
} blockParam_t;

#define MIN(a,b) ((a)<(b) ? (a) : (b))
#define MAX(a,b) ((a)>(b) ? (a) : (b))

static int BMK_benchMem(const void* srcBuffer, size_t srcSize,
                        const char* displayName, int cLevel,
                        const size_t* fileSizes, U32 nbFiles,
                        const void* dictBuffer, size_t dictBufferSize)
{
    const size_t blockSize = (g_blockSize ? g_blockSize : srcSize) + (!srcSize);   /* avoid div by 0 */
    const U32 maxNbBlocks = (U32) ((srcSize + (blockSize-1)) / blockSize) + nbFiles;
    size_t largestBlockSize = 0;
    blockParam_t* const blockTable = (blockParam_t*) malloc(maxNbBlocks * sizeof(blockParam_t));
    const size_t maxCompressedSize = ZSTD_compressBound(srcSize) + (maxNbBlocks * 1024);   /* add some room for safety */
    void* const compressedBuffer = malloc(maxCompressedSize);
    void* const resultBuffer = malloc(srcSize);
    ZSTD_CCtx* refCtx = ZSTD_createCCtx();
    ZSTD_CCtx* ctx = ZSTD_createCCtx();
    ZSTD_DCtx* refDCtx = ZSTD_createDCtx();
    ZSTD_DCtx* dctx = ZSTD_createDCtx();
    U64 crcOrig = XXH64(srcBuffer, srcSize, 0);
    U32 nbBlocks = 0;

    /* init */
    if (strlen(displayName)>17) displayName += strlen(displayName)-17;   /* can only display 17 characters */

    /* Memory allocation & restrictions */
    if (!compressedBuffer || !resultBuffer || !blockTable || !refCtx || !ctx || !refDCtx || !dctx)
        EXM_THROW(31, "not enough memory");

    /* Init blockTable data */
    {
        U32 fileNb;
        const char* srcPtr = (const char*)srcBuffer;
        char* cPtr = (char*)compressedBuffer;
        char* resPtr = (char*)resultBuffer;
        for (fileNb=0; fileNb<nbFiles; fileNb++) {
            size_t remaining = fileSizes[fileNb];
            U32 nbBlocksforThisFile = (U32)((remaining + (blockSize-1)) / blockSize);
            U32 blockEnd = nbBlocks + nbBlocksforThisFile;
            for ( ; nbBlocks<blockEnd; nbBlocks++) {
                size_t thisBlockSize = MIN(remaining, blockSize);
                blockTable[nbBlocks].srcPtr = srcPtr;
                blockTable[nbBlocks].cPtr = cPtr;
                blockTable[nbBlocks].resPtr = resPtr;
                blockTable[nbBlocks].srcSize = thisBlockSize;
                blockTable[nbBlocks].cRoom = ZSTD_compressBound(thisBlockSize);
                srcPtr += thisBlockSize;
                cPtr += blockTable[nbBlocks].cRoom;
                resPtr += thisBlockSize;
                remaining -= thisBlockSize;
                if (thisBlockSize > largestBlockSize) largestBlockSize = thisBlockSize;
    }   }   }

    /* warmimg up memory */
    RDG_genBuffer(compressedBuffer, maxCompressedSize, 0.10, 0.50, 1);

    /* Bench */
    {
        int loopNb;
        size_t cSize = 0;
        double fastestC = 100000000., fastestD = 100000000.;
        double ratio = 0.;
        U64 crcCheck = 0;

        DISPLAY("\r%79s\r", "");
        for (loopNb = 1; loopNb <= nbIterations; loopNb++) {
            int nbLoops;
            int milliTime;
            U32 blockNb;

            /* Compression */
            DISPLAY("%2i-%-17.17s :%10u ->\r", loopNb, displayName, (U32)srcSize);
            memset(compressedBuffer, 0xE5, maxCompressedSize);

            nbLoops = 0;
            milliTime = BMK_GetMilliStart();
            while (BMK_GetMilliStart() == milliTime);
            milliTime = BMK_GetMilliStart();
            while (BMK_GetMilliSpan(milliTime) < TIMELOOP) {
                ZSTD_compressBegin_advanced(refCtx, dictBuffer, dictBufferSize, ZSTD_getParams(cLevel, MAX(dictBufferSize, largestBlockSize)));
                for (blockNb=0; blockNb<nbBlocks; blockNb++) {
                    size_t rSize = ZSTD_compress_usingPreparedCCtx(ctx, refCtx,
                                        blockTable[blockNb].cPtr,  blockTable[blockNb].cRoom,
                                        blockTable[blockNb].srcPtr,blockTable[blockNb].srcSize);
                    if (ZSTD_isError(rSize)) EXM_THROW(1, "ZSTD_compress_usingPreparedCCtx() failed : %s", ZSTD_getErrorName(rSize));
                    blockTable[blockNb].cSize = rSize;
                }
                nbLoops++;
            }
            milliTime = BMK_GetMilliSpan(milliTime);

            cSize = 0;
            for (blockNb=0; blockNb<nbBlocks; blockNb++)
                cSize += blockTable[blockNb].cSize;
            if ((double)milliTime < fastestC*nbLoops) fastestC = (double)milliTime / nbLoops;
            ratio = (double)srcSize / (double)cSize;
            DISPLAY("%2i-%-17.17s :%10i ->%10i (%5.3f),%6.1f MB/s\r", loopNb, displayName, (int)srcSize, (int)cSize, ratio, (double)srcSize / fastestC / 1000.);

#if 1
            /* Decompression */
            memset(resultBuffer, 0xD6, srcSize);  /* warm result buffer */

            nbLoops = 0;
            milliTime = BMK_GetMilliStart();
            while (BMK_GetMilliStart() == milliTime);
            milliTime = BMK_GetMilliStart();

            for ( ; BMK_GetMilliSpan(milliTime) < TIMELOOP; nbLoops++) {
                ZSTD_decompressBegin_usingDict(refDCtx, dictBuffer, dictBufferSize);
                for (blockNb=0; blockNb<nbBlocks; blockNb++) {
                    size_t regenSize = ZSTD_decompress_usingPreparedDCtx(dctx, refDCtx,
                        blockTable[blockNb].resPtr, blockTable[blockNb].srcSize,
                        blockTable[blockNb].cPtr, blockTable[blockNb].cSize);
                    if (ZSTD_isError(regenSize)) {
                        DISPLAY("ZSTD_decompress_usingPreparedDCtx() failed on block %u : %s",
                                  blockNb, ZSTD_getErrorName(regenSize));
                        goto _findError;
                    }
                    blockTable[blockNb].resSize = regenSize;
            }   }

            milliTime = BMK_GetMilliSpan(milliTime);
            if ((double)milliTime < fastestD*nbLoops) fastestD = (double)milliTime / nbLoops;
            DISPLAY("%2i-%-17.17s :%10i ->%10i (%5.3f),%6.1f MB/s ,%6.1f MB/s\r", loopNb, displayName, (int)srcSize, (int)cSize, ratio, (double)srcSize / fastestC / 1000., (double)srcSize / fastestD / 1000.);

            /* CRC Checking */
_findError:
            crcCheck = XXH64(resultBuffer, srcSize, 0);
            if (crcOrig!=crcCheck) {
                size_t u;
                DISPLAY("\n!!! WARNING !!! %14s : Invalid Checksum : %x != %x\n", displayName, (unsigned)crcOrig, (unsigned)crcCheck);
                for (u=0; u<srcSize; u++) {
                    if (((const BYTE*)srcBuffer)[u] != ((const BYTE*)resultBuffer)[u]) {
                        U32 segNb, bNb, pos;
                        size_t bacc = 0;
                        printf("Decoding error at pos %u ", (U32)u);
                        for (segNb = 0; segNb < nbBlocks; segNb++) {
                            if (bacc + blockTable[segNb].srcSize > u) break;
                            bacc += blockTable[segNb].srcSize;
                        }
                        pos = (U32)(u - bacc);
                        bNb = pos / (128 KB);
                        printf("(block %u, sub %u, pos %u) \n", segNb, bNb, pos);
                        break;
                    }
                    if (u==srcSize-1) {  /* should never happen */
                        printf("no difference detected\n");
                }   }
                break;
            }
#endif
        }

        if (crcOrig == crcCheck)
            DISPLAY("%2i-%-17.17s :%10i ->%10i (%5.3f),%6.1f MB/s ,%6.1f MB/s \n", cLevel, displayName, (int)srcSize, (int)cSize, ratio, (double)srcSize / fastestC / 1000., (double)srcSize / fastestD / 1000.);
        else
            DISPLAY("%2i-\n", cLevel);
    }

    /* clean up */
    free(compressedBuffer);
    free(resultBuffer);
    ZSTD_freeCCtx(refCtx);
    ZSTD_freeCCtx(ctx);
    ZSTD_freeDCtx(refDCtx);
    ZSTD_freeDCtx(dctx);
    return 0;
}


static size_t BMK_findMaxMem(U64 requiredMem)
{
    size_t step = 64 MB;
    BYTE* testmem = NULL;

    requiredMem = (((requiredMem >> 26) + 1) << 26);
    requiredMem += 2 * step;
    if (requiredMem > maxMemory) requiredMem = maxMemory;

    while (!testmem) {
        requiredMem -= step;
        testmem = (BYTE*)malloc((size_t)requiredMem);
    }
    free(testmem);
    return (size_t)(requiredMem - step);
}

static void BMK_benchCLevel(void* srcBuffer, size_t benchedSize,
                            const char* displayName, int cLevel,
                            const size_t* fileSizes, unsigned nbFiles,
                            const void* dictBuffer, size_t dictBufferSize)
{
    if (cLevel < 0) {
        int l;
        for (l=1; l <= -cLevel; l++)
            BMK_benchMem(srcBuffer, benchedSize,
                         displayName, l,
                         fileSizes, nbFiles,
                         dictBuffer, dictBufferSize);
        return;
    }
    BMK_benchMem(srcBuffer, benchedSize,
                 displayName, cLevel,
                 fileSizes, nbFiles,
                 dictBuffer, dictBufferSize);
}

static U64 BMK_getTotalFileSize(const char** fileNamesTable, unsigned nbFiles)
{
    U64 total = 0;
    unsigned n;
    for (n=0; n<nbFiles; n++)
        total += BMK_getFileSize(fileNamesTable[n]);
    return total;
}

static void BMK_loadFiles(void* buffer, size_t bufferSize,
                          size_t* fileSizes,
                          const char** fileNamesTable, unsigned nbFiles)
{
    BYTE* buff = (BYTE*)buffer;
    size_t pos = 0;
    unsigned n;

    for (n=0; n<nbFiles; n++) {
        size_t readSize;
        U64 fileSize = BMK_getFileSize(fileNamesTable[n]);
        FILE* f = fopen(fileNamesTable[n], "rb");
        if (f==NULL) EXM_THROW(10, "impossible to open file %s", fileNamesTable[n]);
        DISPLAYLEVEL(2, "Loading %s...       \r", fileNamesTable[n]);
        if (fileSize > bufferSize-pos) fileSize = bufferSize-pos;
        readSize = fread(buff+pos, 1, (size_t)fileSize, f);
        if (readSize != (size_t)fileSize) EXM_THROW(11, "could not read %s", fileNamesTable[n]);
        pos += readSize;
        fileSizes[n] = (size_t)fileSize;
        fclose(f);
    }
}

static void BMK_benchFileTable(const char** fileNamesTable, unsigned nbFiles,
                               const char* dictFileName, int cLevel)
{
    void* srcBuffer;
    size_t benchedSize;
    void* dictBuffer = NULL;
    size_t dictBufferSize = 0;
    size_t* fileSizes = (size_t*)malloc(nbFiles * sizeof(size_t));
    U64 totalSizeToLoad = BMK_getTotalFileSize(fileNamesTable, nbFiles);
    char mfName[20] = {0};
    const char* displayName = NULL;

    if (!fileSizes) EXM_THROW(12, "not enough memory for fileSizes");

    /* Load dictionary */
    if (dictFileName != NULL) {
        U64 dictFileSize = BMK_getFileSize(dictFileName);
        if (dictFileSize > 64 MB) EXM_THROW(10, "dictionary file %s too large", dictFileName);
        dictBufferSize = (size_t)dictFileSize;
        dictBuffer = malloc(dictBufferSize);
        if (dictBuffer==NULL) EXM_THROW(11, "not enough memory for dictionary (%u bytes)", (U32)dictBufferSize);
        BMK_loadFiles(dictBuffer, dictBufferSize, fileSizes, &dictFileName, 1);
    }

    /* Memory allocation & restrictions */
    benchedSize = BMK_findMaxMem(totalSizeToLoad * 3) / 3;
    if ((U64)benchedSize > totalSizeToLoad) benchedSize = (size_t)totalSizeToLoad;
    if (benchedSize < totalSizeToLoad)
        DISPLAY("Not enough memory; testing %u MB only...\n", (U32)(benchedSize >> 20));
    srcBuffer = malloc(benchedSize);
    if (!srcBuffer) EXM_THROW(12, "not enough memory");

    /* Load input buffer */
    BMK_loadFiles(srcBuffer, benchedSize, fileSizes, fileNamesTable, nbFiles);

    /* Bench */
    snprintf (mfName, sizeof(mfName), " %u files", nbFiles);
    if (nbFiles > 1) displayName = mfName;
    else displayName = fileNamesTable[0];

    BMK_benchCLevel(srcBuffer, benchedSize,
                    displayName, cLevel,
                    fileSizes, nbFiles,
                    dictBuffer, dictBufferSize);

    /* clean up */
    free(srcBuffer);
    free(dictBuffer);
    free(fileSizes);
}


static void BMK_syntheticTest(int cLevel, double compressibility)
{
    char name[20] = {0};
    size_t benchedSize = 10000000;
    void* srcBuffer = malloc(benchedSize);

    /* Memory allocation */
    if (!srcBuffer) EXM_THROW(21, "not enough memory");

    /* Fill input buffer */
    RDG_genBuffer(srcBuffer, benchedSize, compressibility, 0.0, 0);

    /* Bench */
    snprintf (name, sizeof(name), "Synthetic %2u%%", (unsigned)(compressibility*100));
    BMK_benchCLevel(srcBuffer, benchedSize, name, cLevel, &benchedSize, 1, NULL, 0);

    /* clean up */
    free(srcBuffer);
}


int BMK_benchFiles(const char** fileNamesTable, unsigned nbFiles,
                   const char* dictFileName, int cLevel)
{
    double compressibility = (double)g_compressibilityDefault / 100;

    if (nbFiles == 0)
        BMK_syntheticTest(cLevel, compressibility);
    else
        BMK_benchFileTable(fileNamesTable, nbFiles, dictFileName, cLevel);
    return 0;
}

