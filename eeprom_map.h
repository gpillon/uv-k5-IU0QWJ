#ifndef EEPROM_MAP_H
#define EEPROM_MAP_H

//
// EEPROM Memory Map for 999 MR Channels (24M02 EEPROM, 256KB)
//
// The original firmware used a 24C64 (8KB) with 200 channels.
// This layout uses a 24M02 (256KB) to support 999 MR channels.
//
// DESIGN PRINCIPLES:
//   - Calibration data stays at ORIGINAL addresses (0x1E00-0x1FFF)
//     so factory calibration is never lost.
//   - Settings keep their original addresses where possible.
//   - Channel data (records, names, attributes) moves to 0x2000+
//     since 999 channels don't fit before calibration.
//

#define EEPROM_NUM_MR_CHANNELS    999
#define EEPROM_CH_RECORD_SIZE     16
#define EEPROM_CH_NAME_SIZE       16
#define EEPROM_NUM_VFO_BANDS      7
#define EEPROM_VFO_RECORD_SIZE    32

// ===== Settings (original addresses, 0x0E40-0x0F4F) =====

#define EEPROM_FM_PRESETS          0x0E40  // 48 bytes (20 FM channels)
#define EEPROM_SETTINGS_0E70      0x0E70  // General settings 1 (8 bytes)
#define EEPROM_SETTINGS_0E78      0x0E78  // General settings 2 (8 bytes)
#define EEPROM_FM_CONFIG          0x0E88  // FM config (8 bytes)
#define EEPROM_KEY_SETTINGS       0x0E90  // Key/beep settings (8 bytes)
#define EEPROM_PASSWORD           0x0E98  // Power-on password (8 bytes)
#define EEPROM_VOICE_RSSI         0x0EA0  // Voice/RSSI settings (8 bytes)
#define EEPROM_ALARM_ROGER        0x0EA8  // Alarm/Roger/Battery (8 bytes)
#define EEPROM_WELCOME_0          0x0EB0  // Welcome string 1 (16 bytes)
#define EEPROM_WELCOME_1          0x0EC0  // Welcome string 2 (16 bytes)
#define EEPROM_DTMF_SETTINGS_1    0x0ED0  // DTMF settings 1 (8 bytes)
#define EEPROM_DTMF_SETTINGS_2    0x0ED8  // DTMF settings 2 (8 bytes)
#define EEPROM_ANI_ID             0x0EE0  // ANI DTMF ID (8 bytes)
#define EEPROM_KILL_CODE          0x0EE8  // Kill code (8 bytes)
#define EEPROM_REVIVE_CODE        0x0EF0  // Revive code (8 bytes)
#define EEPROM_DTMF_UP_CODE       0x0EF8  // DTMF UP code (16 bytes)
#define EEPROM_DTMF_DOWN_CODE     0x0F08  // DTMF DOWN code (16 bytes)
#define EEPROM_SCAN_LIST          0x0F18  // Scan list settings (8 bytes)
#define EEPROM_AES_KEY            0x0F30  // AES key (16 bytes)
#define EEPROM_FLOCK_MISC         0x0F40  // F_LOCK/misc (8 bytes)

// VFO indices expanded to 16 bytes for uint16_t channel numbers.
// Moved from original 0x0E80 (which only had 8 bytes) to freed
// channel-name area (original names were at 0x0F50-0x1BD0).
#define EEPROM_VFO_INDICES        0x0F50  // VFO indices (16 bytes)

// ===== DTMF Contacts (original address) =====
#define EEPROM_DTMF_CONTACTS      0x1C00  // 16 x 16 = 256 bytes → ends 0x1CFF

// ===== Calibration (ORIGINAL addresses, 0x1E00-0x1FFF) =====
// NEVER MOVE THESE - factory calibration data!
#define EEPROM_CAL_BASE           0x1E00
#define EEPROM_SQUELCH_UHF        0x1E00  // 96 bytes
#define EEPROM_SQUELCH_VHF        0x1E60  // 96 bytes
#define EEPROM_RSSI_CAL_HIGH      0x1EC0  // 8 bytes
#define EEPROM_RSSI_CAL_LOW       0x1EC8  // 8 bytes
#define EEPROM_TX_POWER_CAL       0x1ED0  // 112 bytes (7 bands x 16)
#define EEPROM_BATTERY_CAL        0x1F40  // 12 bytes
#define EEPROM_BATTERY_CAL_EXT    0x1F48  // 8 bytes
#define EEPROM_VOX1_THRESHOLD     0x1F50  // 24 bytes
#define EEPROM_VOX0_THRESHOLD     0x1F68  // 24 bytes
#define EEPROM_CRYSTAL_VOL_DAC    0x1F88  // 8 bytes
#define EEPROM_BUILD_OPTIONS      0x1FF0  // 8 bytes (F4HWN settings)

// ===== MR Channel Records (0x2000+, 999 x 16 = 15984 bytes) =====
#define EEPROM_MR_CH_BASE         0x2000
#define EEPROM_MR_CH_ADDR(ch)     (EEPROM_MR_CH_BASE + (ch) * EEPROM_CH_RECORD_SIZE)
//  End: 0x5E70

// ===== VFO/Band Records (7 x 32 = 224 bytes) =====
#define EEPROM_VFO_BASE           0x5E70
#define EEPROM_VFO_ADDR(band,vfo) (EEPROM_VFO_BASE + (band) * EEPROM_VFO_RECORD_SIZE + (vfo) * EEPROM_CH_RECORD_SIZE)
//  End: 0x5F50

// ===== Channel Attributes (1006 bytes, padded to 1008) =====
#define EEPROM_CH_ATTR_BASE       0x5F50
//  End: 0x6340

// ===== Channel Names (999 x 16 = 15984 bytes) =====
#define EEPROM_CH_NAME_BASE       0x6340
#define EEPROM_CH_NAME_ADDR(ch)   (EEPROM_CH_NAME_BASE + (ch) * EEPROM_CH_NAME_SIZE)
//  End: 0xA1B0

// ===== Aircopy limit (copy settings area, stop before calibration) =====
#define EEPROM_AIRCOPY_LIMIT      0x1E00

// ===== Total channel data size =====
#define EEPROM_TOTAL_SIZE         0xA1B0

// ===== Code Overlay Storage (after channel data, still in first 64KB page) =====
// Loaded into RAM on demand by OVERLAY_Load().
// Written by CHIRP during firmware setup.
#define EEPROM_OVLY_SPECTRUM      0xA200  // up to 8KB → ends 0xC200
#define EEPROM_OVLY_FM            0xC200  // up to 4KB → ends 0xD200
#define EEPROM_OVLY_AIRCOPY       0xD200  // up to 4KB → ends 0xE200
#define EEPROM_OVLY_MENU          0xE200  // up to 6KB → ends 0xFA00
#define EEPROM_OVLY_END           0xFA00

// ===== Maximum writable address =====
#define EEPROM_MAX_ADDR           0xFA00

// ===== Sentinel for invalid channel =====
#define INVALID_CHANNEL           0xFFFF

#endif
