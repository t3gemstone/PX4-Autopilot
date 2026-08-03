/****************************************************************************
 *
 *   Copyright (C) 2024 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file board_identity.c
 * Implementation of AM67 based Board identity API.
 *
 * TODO: source a real unique id from the SoC (an eFuse/OTP field). For now a
 * fixed placeholder is used - uORB/parameters do not depend on uniqueness, but
 * two O1 boards will report the same GUID until this is implemented.
 */

#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/micro_hal.h>
#include <stdio.h>
#include <string.h>

#define CPU_UUID_BYTE_FORMAT_ORDER          {3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8}
#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00ff0000) >> 8) | (((x) & 0x0000ff00) << 8) | ((x) << 24))

static const uint16_t soc_arch_id = PX4_SOC_ARCH_ID;

/* Placeholder "unique" id (12 bytes / 3 words). TODO: read real SoC id. */
static const uint32_t am67_uuid[PX4_CPU_UUID_WORD32_LENGTH] = {
	0xa67a67a6, 0x00000001, 0x54334f31 /* "T3O1" */
};

typedef const uint8_t uuid_uint8_reorder_t[PX4_CPU_UUID_BYTE_LENGTH];

void board_get_uuid(uuid_byte_t uuid_bytes)
{
	uuid_uint8_reorder_t reorder = CPU_UUID_BYTE_FORMAT_ORDER;
	union {
		uuid_byte_t b;
		uuid_uint32_t w;
	} id;

	board_get_uuid32(id.w);

	for (int i = 0; i < PX4_CPU_UUID_BYTE_LENGTH; i++) {
		uuid_bytes[i] = id.b[reorder[i]];
	}
}

__EXPORT void board_get_uuid32(uuid_uint32_t uuid_words)
{
	for (unsigned i = 0; i < PX4_CPU_UUID_WORD32_LENGTH; i++) {
		uuid_words[i] = am67_uuid[i];
	}
}

int board_get_uuid32_formated(char *format_buffer, int size,
			      const char *format,
			      const char *seperator)
{
	uuid_uint32_t uuid;
	board_get_uuid32(uuid);

	int offset = 0;
	int sep_size = seperator ? strlen(seperator) : 0;

	for (unsigned i = 0; (offset < size - 1) && (i < PX4_CPU_UUID_WORD32_LENGTH); i++) {
		offset += snprintf(&format_buffer[offset], size - offset, format, uuid[i]);

		if (sep_size && (offset < size - sep_size - 1) && (i < PX4_CPU_UUID_WORD32_LENGTH - 1)) {
			strncat(&format_buffer[offset], seperator, size - offset);
			offset += sep_size;
		}
	}

	return 0;
}

int board_get_mfguid(mfguid_t mfgid)
{
	uint8_t *rv = &mfgid[0];

	for (unsigned i = 0; i < PX4_CPU_UUID_WORD32_LENGTH; i++) {
		uint32_t uuid_bytes = SWAP_UINT32(am67_uuid[(PX4_CPU_UUID_WORD32_LENGTH - 1) - i]);
		memcpy(rv, &uuid_bytes, sizeof(uint32_t));
		rv += sizeof(uint32_t);
	}

	return PX4_CPU_MFGUID_BYTE_LENGTH;
}

int board_get_mfguid_formated(char *format_buffer, int size)
{
	mfguid_t mfguid;

	board_get_mfguid(mfguid);

	int offset = 0;

	for (unsigned i = 0; offset < size && i < PX4_CPU_MFGUID_BYTE_LENGTH; i++) {
		offset += snprintf(&format_buffer[offset], size - offset, "%02x", mfguid[i]);
	}

	return offset;
}

int board_get_px4_guid(px4_guid_t px4_guid)
{
	uint8_t *pb = (uint8_t *) &px4_guid[0];

	*pb++ = (soc_arch_id >> 8) & 0xff;
	*pb++ = (soc_arch_id & 0xff);

	for (unsigned i = 0; i < PX4_GUID_BYTE_LENGTH - (sizeof(soc_arch_id) + PX4_CPU_UUID_BYTE_LENGTH); i++) {
		*pb++ = 0;
	}

	for (unsigned i = 0; i < PX4_CPU_UUID_WORD32_LENGTH; i++) {
		uint32_t uuid_bytes = SWAP_UINT32(am67_uuid[(PX4_CPU_UUID_WORD32_LENGTH - 1) - i]);
		memcpy(pb, &uuid_bytes, sizeof(uint32_t));
		pb += sizeof(uint32_t);
	}

	return PX4_GUID_BYTE_LENGTH;
}

int board_get_px4_guid_formated(char *format_buffer, int size)
{
	px4_guid_t px4_guid;
	board_get_px4_guid(px4_guid);

	int offset = 0;

	for (unsigned i = 0; offset < size && i < PX4_GUID_BYTE_LENGTH; i++) {
		offset += snprintf(&format_buffer[offset], size - offset, "%02x", px4_guid[i]);
	}

	return offset;
}
