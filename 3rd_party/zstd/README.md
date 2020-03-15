 **Zstd**, short for Zstandard, is a fast lossless compression algorithm, targeting real-time compression scenarios at zlib-level compression ratio.

It is provided as a BSD-license package, hosted on Github.

|Branch      |Status   |
|------------|---------|
|master      | [![Build Status](https://travis-ci.org/Cyan4973/zstd.svg?branch=master)](https://travis-ci.org/Cyan4973/zstd) |
|dev         | [![Build Status](https://travis-ci.org/Cyan4973/zstd.svg?branch=dev)](https://travis-ci.org/Cyan4973/zstd) |

As a reference, several fast compression algorithms were tested and compared to [zlib] on a Core i7-3930K CPU @ 4.5GHz, using [lzbench], an open-source in-memory benchmark by @inikep compiled with gcc 5.2.1, on the [Silesia compression corpus].

[lzbench]: https://github.com/inikep/lzbench
[Silesia compression corpus]: http://sun.aei.polsl.pl/~sdeor/index.php?page=silesia


|Name             | Ratio | C.speed | D.speed |
|-----------------|-------|--------:|--------:|
|                 |       |   MB/s  |  MB/s   |
|**zstd 0.5.1 -1**|**2.876**|**330**| **890** |
| [zlib] 1.2.8 -1 | 2.730 |    95   |   360   |
| brotli -0       | 2.708 |   220   |   430   |
| QuickLZ 1.5     | 2.237 |   510   |   605   |
| LZO 2.09        | 2.106 |   610   |   870   |
| [LZ4] r131      | 2.101 |   620   |  3100   |
| Snappy 1.1.3    | 2.091 |   480   |  1600   |
| LZF 3.6         | 2.077 |   375   |   790   |

[zlib]:http://www.zlib.net/
[LZ4]: http://www.lz4.org/

Zstd can also offer stronger compression ratio at the cost of compression speed. 
Speed vs Compression trade-off is configurable by small increment. Decompression speed is preserved and remain roughly the same at all settings, a property shared by most LZ compression algorithms, such as [zlib].

The following test is run on a Core i7-3930K CPU @ 4.5GHz, using [lzbench], an open-source in-memory benchmark by @inikep compiled with gcc 5.2.1, on the [Silesia compression corpus].

Compression Speed vs Ratio | Decompression Speed
---------------------------|--------------------
![Compression Speed vs Ratio](images/Cspeed4.png "Compression Speed vs Ratio") | ![Decompression Speed](images/Dspeed4.png "Decompression Speed")

Several algorithms can produce higher compression ratio at slower speed, falling outside of the graph.
For a larger picture including very slow modes, [click on this link](images/DCspeed5.png) .


### The case for Small Data compression

Above chart provides results applicable to large files or large streams scenarios (200 MB for this case).
Small data (< 64 KB) come with different perspectives.
The smaller the amount of data to compress, the more difficult it is to achieve any significant compression.
On reaching the 1 KB region, it becomes almost impossible to compress anything.
This problem is common to any compression algorithms, and throwing CPU power at it achieves little gains.

The reason is, compression algorithms learn from past data how to compress future data.
But at the beginning of a new file, there is no "past" to build upon.

To solve this situation, Zstd now offers a __training mode__,
which can be used to make the algorithm fit a selected type of data, by providing it with some samples.
The result of the training is a file called "dictionary", which can be loaded before compression and decompression.
Using this dictionary, the compression ratio achievable on small data improves dramatically :

| Collection Name    | Direct compression | Dictionary Compression | Gains      | Average unit | Range       |
| ---------------    | ------------------ | ---------------------- | ---------  | ------------:| -----       |
| Small JSON records | x1.331 - x1.366	  | x5.860 - x6.830        | ~ __x4.7__ | 300          | 200 - 400   |
| Mercurial events   | x2.322 - x2.538    | x3.377 - x4.462        | ~ __x1.5__ | 1.5 KB       | 20 - 200 KB |	
| Large JSON docs    | x3.813 - x4.043    | x8.935 - x13.366       | ~ __x2.8__ | 6 KB         | 800 - 20 KB |	

These compression gains are achieved without any speed loss, and prove in general a bit faster to compress and decompress.

Dictionary work if there is some correlation in a family of small data (there is no _universal dictionary_).
Hence, deploying one dictionary per type of data will provide the greater benefits.

Large documents will benefit proportionally less, since dictionary gains are mostly effective in the first few KB.
Then, the compression algorithm will rely more and more on already decoded content to compress the rest of the file.

#### Dictionary compression How To :

##### _Using the Command Line Utility_ :

1) Create the dictionary

`zstd --train FullPathToTrainingSet/* -o dictionaryName`

2) Compression with dictionary

`zstd FILE -D dictionaryName`

3) Decompress with dictionary

`zstd --decompress FILE.zst -D dictionaryName`

##### _Using API_ :

1) Create dictionary

```
#include "zdict.h"
(...)
/* Train a dictionary from a memory buffer `samplesBuffer`, 
   where `nbSamples` samples have been stored concatenated. */
size_t dictSize = ZDICT_trainFromBuffer(dictBuffer, dictBufferCapacity,
                                        samplesBuffer, samplesSizes, nbSamples);
```

2) Compression with dictionary

```
#include "zstd.h"
(...)
ZSTD_CCtx* context = ZSTD_createCCtx();
size_t compressedSize = ZSTD_compress_usingDict(context, dst, dstCapacity, src, srcSize, dict, dictSize, compressionLevel);
```

3) Decompress with dictionary

```
#include "zstd.h"
(...)
ZSTD_DCtx* context = ZSTD_createDCtx();
size_t regeneratedSize = ZSTD_decompress_usingDict(context, dst, dstCapacity, cSrc, cSrcSize, dict, dictSize);
```


### Status

Zstd has not yet reached "stable format" status. It doesn't guarantee yet that its current compression format will remain stable in future versions. During this period, it can still change to adapt new optimizations still being investigated. "Stable Format" is projected H1 2016, and will be tagged `v1.0`.

That being said, the library is now fairly robust, able to withstand hazards situations, including invalid inputs. It also features legacy support, so that documents compressed with current and previous version of zstd can still be decoded in the future. 
Library reliability has been tested using [Fuzz Testing](https://en.wikipedia.org/wiki/Fuzz_testing), with both [internal tools](programs/fuzzer.c) and [external ones](http://lcamtuf.coredump.cx/afl). Therefore, Zstandard is considered safe for testings, even within production environments.

### Branch Policy

The "dev" branch is the one where all contributions will be merged before reaching "master". If you plan to propose a patch, please commit into the "dev" branch or its own feature branch. Direct commit to "master" are not permitted.


### Trivia

Zstd entropy stage is provided by [Huff0 and FSE, from Finite State Entropy library](https://github.com/Cyan4973/FiniteStateEntropy).

Its memory requirement can be configured to fit into low-memory hardware configurations, or servers handling multiple connections/contexts in parallel.

