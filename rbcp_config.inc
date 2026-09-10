; rbcp_config.s — RBCP reference implementation configuration settings
; Copyright (C) 2026 Piers Finlayson <piers@piers.rocks>

; C64 bootloader kernal configuration for RBCP.

; Set to the high byte of the address the ROM is mapped to in the host system.
; For example, the C64 kernal ROM is mapped to $E000, hence $E0.
CONFIG_ROM_BASE_HI = $E0

; Define C64C_COMBINED to build for a C64C, where a single 23128 serves BASIC
; and the kernal as one 16KB image, with the kernal in the upper half.  Leave
; it undefined to build a standalone 8KB C64 kernal ROM.
;
; CONFIG_ROM_SIZE is the total size of this RBCP ROM image in bytes.
;
; CONFIG_ROM_BASE_OFFSET is the byte offset, within the ROM image, of the byte
; served at CONFIG_ROM_BASE_HI * $100.  The device sees offsets within the
; image, not host addresses, so RBCP values sent to the device are derived
; from it.  On the C64C the kernal occupies the upper 8KB, so host $E000 is
; image offset $2000.
;
; A second constant is needed because the host window is discontiguous on the
; C64C - BASIC at $A000-$BFFF, the kernal at $E000-$FFFF, with a $2000 hole
; between.  Adjusting CONFIG_ROM_BASE_HI instead would give a command page of
; $40 rather than $20.
.ifdef C64C_COMBINED
CONFIG_ROM_SIZE = $4000
CONFIG_ROM_BASE_OFFSET = $2000
.else
CONFIG_ROM_SIZE = $2000
CONFIG_ROM_BASE_OFFSET = $0000
.endif

; Set to the high byte to be used for RBCP command page.  If not required
; for other things, $E0xx is a good choice in which case the HI value is $E0.
CONFIG_RBCP_CMD_PAGE = $E0

; The command page value relative to the start of the ROM image.  This is
; the value used to configure the device.
CONFIG_RBCP_CMD_PAGE_REL = ((CONFIG_RBCP_CMD_PAGE - CONFIG_ROM_BASE_HI) * $100 + CONFIG_ROM_BASE_OFFSET) / $100

; Set to the base address of the back channel region, if used.  Should not
; conflict with the RBCP address read 
CONFIG_RBCP_BCH_BASE = $E100

; Back-channel region start address, as a ROM-relative offset. Must be
; 4-byte aligned. A base of $E100 leaves the first 256 bytes of the kernal
; unmodified for normal ROM content, and to be used as the command page.
; That is image offset $0100 on the 8KB build and $2100 on the C64C build.
CONFIG_RBCP_BCH_START = (CONFIG_RBCP_BCH_BASE - (CONFIG_ROM_BASE_HI * $100)) + CONFIG_ROM_BASE_OFFSET

; Set to the size of the back-channel region, including the header, in bytes.
CONFIG_RBCP_BCH_SIZE = 1024

; Set these to values that are not used by the ROM image in the progress and
; response byte locations (Offsets +$04 and +$05 in the back channel region).
; RBCP uses these values AND THE BITWISE INVERSE OF THESE VALUES to detect
; whether the device has updated them, so both the values configured here and
; their bitwise inverses must be unused in the ROM image as the appropriate
; locations.
CONFIG_RBCP_COMPLETE = $BB  ; inverse = $44
CONFIG_RBCP_STATUS_OK = $CC ; inverse = $33

; Set to a timeout for RBCP to wait for responses in command-response mode.
; This is an arbitrary value with no fixed unit.  $00 = wait forever.
CONFIG_RBCP_POLL_TIMEOUT = $FF

; A timeout to wait for non-volatile commits to complete, as these are likely
; to involve flash erases which take a long time (ms).
; This is an arbitrary value with no fixed unit.  $00 = wait forever.  The
; maximum is $FFFF
CONFIG_RBCP_NV_POLL_TIMEOUT = $FFFF

; Set to the number of times to retry a command in command-response mode, if
; the device fails to acknowledge it within the timeout via a token increment.
; $00 = no retries.
CONFIG_RBCP_TIMEOUT_RETRIES = $00

; Used by RBCP to pause after sending a command when _not_ in command-response
; mode, to ensure the device has time to process it (as no back-channel is
; available to detect when the device is ready for the next command).  This is
; an arbitrary value with no fixed unit.  $00 = no pause.  Depending on your
; host system and device, you may need to experiemnt with this value to find
; the optimal setting.
; - $04 works using One ROM host-control plugin and a C64 bootloader kernal
; - $10 works with logging enabled with the One ROM host-control plugin
CONFIG_RBCP_CMD_PAUSE = $04

; Set to the base address of the ZP block that the RBCP library should use.
; Must be at least 16 bytes long.
CONFIG_RBCP_ZP_BASE = $F0
CONFIG_RBCP_ZP_LENGTH = 16