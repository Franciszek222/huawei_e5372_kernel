/*
 * Compressed RAM based swap device
 *
 * Copyright (C) 2008, 2009, 2010  Nitin Gupta
 *
 * This code is released using a dual license strategy: BSD/GPL
 * You can choose the licence that better fits your requirements.
 *
 * Released under the terms of 3-clause BSD License
 * Released under the terms of GNU General Public License Version 2.0
 *
 * Project home: http://compcache.googlecode.com
 */

#ifndef _RAMZSWAP_IOCTL_H_
#define _RAMZSWAP_IOCTL_H_

struct ramzswap_ioctl_stats {
	u64	disksize;               /* user specified or equal to backing swap
	                                 * size (if present) */
	u64	num_reads;              /* failed + successful */
	u64	num_writes;             /* --do-- */
	u64	failed_reads;           /* should NEVER! happen */
	u64	failed_writes;          /* can happen when memory is too low */
	u64	invalid_io;             /* non-swap I/O requests */
	u64	notify_free;            /* no. of swap slot free notifications */
	u32	pages_zero;             /* no. of zero filled pages */
	u32	good_compress_pct;      /* no. of pages with compression ratio<=50% */
	u32	pages_expand_pct;       /* no. of incompressible pages */
	u32	pages_stored;
	u32	pages_used;
	u64	orig_data_size;
	u64	compr_data_size;
	u64	mem_used_total;
} __attribute__ ((packed, aligned(4)));

#define RZSIO_SET_DISKSIZE_KB   _IOW('z', 0, size_t)
#define RZSIO_GET_STATS         _IOR('z', 1, struct ramzswap_ioctl_stats)
#define RZSIO_INIT              _IO('z', 2)
#define RZSIO_RESET             _IO('z', 3)

#endif
