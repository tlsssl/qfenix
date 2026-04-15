/* SPDX-License-Identifier: BSD-3-Clause */
#ifndef __OSCOMPAT_H__
#define __OSCOMPAT_H__

#include <stdint.h>

/*
 * Little-endian load/store helpers for wire-format protocol fields.
 * Qualcomm DIAG / Firehose / Sahara payloads are little-endian; using
 * memcpy into host-endian scalars breaks on big-endian hosts (e.g. MIPS
 * BE for OpenWrt ath79). These helpers are endian-neutral.
 */
static inline uint16_t read_le16(const void *p)
{
	const uint8_t *b = (const uint8_t *)p;

	return (uint16_t)b[0] | ((uint16_t)b[1] << 8);
}

static inline uint32_t read_le32(const void *p)
{
	const uint8_t *b = (const uint8_t *)p;

	return (uint32_t)b[0] | ((uint32_t)b[1] << 8) |
	       ((uint32_t)b[2] << 16) | ((uint32_t)b[3] << 24);
}

static inline uint64_t read_le64(const void *p)
{
	return (uint64_t)read_le32(p) |
	       ((uint64_t)read_le32((const uint8_t *)p + 4) << 32);
}

static inline void write_le16(void *p, uint16_t v)
{
	uint8_t *b = (uint8_t *)p;

	b[0] = (uint8_t)(v & 0xff);
	b[1] = (uint8_t)((v >> 8) & 0xff);
}

static inline void write_le32(void *p, uint32_t v)
{
	uint8_t *b = (uint8_t *)p;

	b[0] = (uint8_t)(v & 0xff);
	b[1] = (uint8_t)((v >> 8) & 0xff);
	b[2] = (uint8_t)((v >> 16) & 0xff);
	b[3] = (uint8_t)((v >> 24) & 0xff);
}

static inline void write_le64(void *p, uint64_t v)
{
	write_le32(p, (uint32_t)v);
	write_le32((uint8_t *)p + 4, (uint32_t)(v >> 32));
}

#ifndef _WIN32

#include <err.h>

#define O_BINARY 0

#else // _WIN32

#include <sys/time.h>
#include <stdbool.h>

/* S_ISLNK may not be defined on all Windows toolchains */
#ifndef S_ISLNK
#define S_ISLNK(m) 0
#endif

void timeradd(const struct timeval *a, const struct timeval *b, struct timeval *result);

void err(int eval, const char *fmt, ...);
void errx(int eval, const char *fmt, ...);
void warn(const char *fmt, ...);
void warnx(const char *fmt, ...);

char *strcasestr(const char *haystack, const char *needle);

#endif

#endif
