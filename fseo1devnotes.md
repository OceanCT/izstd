# when zstd use fse to encode symbols?

zstd mainly fse to encode the literal length, offset and match length tuples. So we only to care about ZSTD_encodeSequences_body function in zstd_compress.c, assuming we do not need to change how the ctable is built.


