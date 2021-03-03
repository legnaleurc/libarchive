/*-
 * Copyright (c) 2021 Wei-Cheng Pan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ARCHIVE_RAR_PRIVATE_H_INCLUDED
#define ARCHIVE_RAR_PRIVATE_H_INCLUDED

#ifndef __LIBARCHIVE_BUILD
#error This header is only to be used internally to libarchive.
#endif

struct rar
{
  /* Entries from main RAR header */
  unsigned main_flags;
  unsigned long file_crc;
  char reserved1[2];
  char reserved2[4];
  char encryptver;

  /* File header entries */
  char compression_method;
  unsigned file_flags;
  int64_t packed_size;
  int64_t unp_size;
  time_t mtime;
  long mnsec;
  mode_t mode;
  char *filename;
  char *filename_save;
  size_t filename_save_size;
  size_t filename_allocated;

  /* File header optional entries */
  char salt[8];
  time_t atime;
  long ansec;
  time_t ctime;
  long cnsec;
  time_t arctime;
  long arcnsec;

  /* Fields to help with tracking decompression of files. */
  int64_t bytes_unconsumed;
  int64_t bytes_remaining;
  int64_t bytes_uncopied;
  int64_t offset;
  int64_t offset_outgoing;
  int64_t offset_seek;
  char valid;
  unsigned int unp_offset;
  unsigned int unp_buffer_size;
  unsigned char *unp_buffer;
  unsigned int dictionary_size;
  char start_new_block;
  char entry_eof;
  unsigned long crc_calculated;
  int found_first_header;
  char has_endarc_header;
  struct data_block_offsets *dbo;
  unsigned int cursor;
  unsigned int nodes;
  char filename_must_match;

  /* LZSS members */
  struct huffman_code maincode;
  struct huffman_code offsetcode;
  struct huffman_code lowoffsetcode;
  struct huffman_code lengthcode;
  unsigned char lengthtable[HUFFMAN_TABLE_SIZE];
  struct lzss lzss;
  char output_last_match;
  unsigned int lastlength;
  unsigned int lastoffset;
  unsigned int oldoffset[4];
  unsigned int lastlowoffset;
  unsigned int numlowoffsetrepeats;
  int64_t filterstart;
  char start_new_table;

  /* PPMd Variant H members */
  char ppmd_valid;
  char ppmd_eod;
  char is_ppmd_block;
  int ppmd_escape;
  CPpmd7 ppmd7_context;
  CPpmd7z_RangeDec range_dec;
  IByteIn bytein;

  /*
   * String conversion object.
   */
  int init_default_conversion;
  struct archive_string_conv *sconv_default;
  struct archive_string_conv *opt_sconv;
  struct archive_string_conv *sconv_utf8;
  struct archive_string_conv *sconv_utf16be;

  /*
   * Bit stream reader.
   */
  struct rar_br {
#define CACHE_TYPE	uint64_t
#define CACHE_BITS	(8 * sizeof(CACHE_TYPE))
    /* Cache buffer. */
    CACHE_TYPE		 cache_buffer;
    /* Indicates how many bits avail in cache_buffer. */
    int			 cache_avail;
    ssize_t		 avail_in;
    const unsigned char *next_in;
  } br;

  /*
   * Custom field to denote that this archive contains encrypted entries
   */
  int has_encrypted_entries;
};

#endif /* ARCHIVE_RAR_HEADER_PRIVATE_H_INCLUDED */
