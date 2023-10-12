#pragma once

#include <system_error>

/** esp error codes as specified in:
 * https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/error-codes.html
 */

namespace esp_error {
	enum class codes {
		FAIL = -1,
		OK = 0,
		ERR_NO_MEM = 0x101,
		ERR_INVALID_ARG = 0x102,
		ERR_INVALID_STATE = 0x103,
		ERR_INVALID_SIZE = 0x104,
		ERR_NOT_FOUND = 0x105,
		ERR_NOT_SUPPORTED = 0x106,
		ERR_TIMEOUT = 0x107,
		ERR_INVALID_RESPONSE = 0x108,
		ERR_INVALID_CRC = 0x109,
		ERR_INVALID_VERSION = 0x10a,
		ERR_INVALID_MAC = 0x10b,
		ERR_NOT_FINISHED = 0x10c,
		ERR_NVS_BASE = 0x1100,
		ERR_NVS_NOT_INITIALIZED = 0x1101,
		ERR_NVS_NOT_FOUND = 0x1102,
		ERR_NVS_TYPE_MISMATCH = 0x1103,
		ERR_NVS_READ_ONLY = 0x1104,
		ERR_NVS_NOT_ENOUGH_SPACE = 0x1105,
		ERR_NVS_INVALID_NAME = 0x1106,
		ERR_NVS_INVALID_HANDLE = 0x1107,
		ERR_NVS_REMOVE_FAILED = 0x1108,
		ERR_NVS_KEY_TOO_LONG = 0x1109,
		ERR_NVS_PAGE_FULL = 0x110a,
		ERR_NVS_INVALID_STATE = 0x110b,
		ERR_NVS_INVALID_LENGTH = 0x110c,
		ERR_NVS_NO_FREE_PAGES = 0x110d,
		ERR_NVS_VALUE_TOO_LONG = 0x110e,
		ERR_NVS_PART_NOT_FOUND = 0x110f,
		ERR_NVS_NEW_VERSION_FOUND = 0x1110,
		ERR_NVS_XTS_ENCR_FAILED = 0x1111,
		ERR_NVS_XTS_DECR_FAILED = 0x1112,
		ERR_NVS_XTS_CFG_FAILED = 0x1113,
		ERR_NVS_XTS_CFG_NOT_FOUND = 0x1114,
		ERR_NVS_ENCR_NOT_SUPPORTED = 0x1115,
		ERR_NVS_KEYS_NOT_INITIALIZED = 0x1116,
		ERR_NVS_CORRUPT_KEY_PART = 0x1117,
		ERR_NVS_CONTENT_DIFFERS = 0x1118,
		ERR_NVS_WRONG_ENCRYPTION = 0x1119,
		ERR_ULP_BASE = 0x1200,
		ERR_ULP_SIZE_TOO_BIG = 0x1201,
		ERR_ULP_INVALID_LOAD_ADDR = 0x1202,
		ERR_ULP_DUPLICATE_LABEL = 0x1203,
		ERR_ULP_UNDEFINED_LABEL = 0x1204,
		ERR_ULP_BRANCH_OUT_OF_RANGE = 0x1205,
		ERR_OTA_BASE = 0x1500,
		ERR_OTA_PARTITION_CONFLICT = 0x1501,
		ERR_OTA_SELECT_INFO_INVALID = 0x1502,
		ERR_OTA_VALIDATE_FAILED = 0x1503,
		ERR_OTA_SMALL_SEC_VER = 0x1504,
		ERR_OTA_ROLLBACK_FAILED = 0x1505,
		ERR_OTA_ROLLBACK_INVALID_STATE = 0x1506,
		ERR_EFUSE = 0x1600,
		OK_EFUSE_CNT = 0x1601,
		ERR_EFUSE_CNT_IS_FULL = 0x1602,
		ERR_EFUSE_REPEATED_PROG = 0x1603,
		ERR_CODING = 0x1604,
		ERR_NOT_ENOUGH_UNUSED_KEY_BLOCKS = 0x1605,
		ERR_DAMAGED_READING = 0x1606,
		ERR_IMAGE_BASE = 0x2000,
		ERR_IMAGE_FLASH_FAIL = 0x2001,
		ERR_IMAGE_INVALID = 0x2002,
		ERR_WIFI_BASE = 0x3000,
		ERR_WIFI_NOT_INIT = 0x3001,
		ERR_WIFI_NOT_STARTED = 0x3002,
		ERR_WIFI_NOT_STOPPED = 0x3003,
		ERR_WIFI_IF = 0x3004,
		ERR_WIFI_MODE = 0x3005,
		ERR_WIFI_STATE = 0x3006,
		ERR_WIFI_CONN = 0x3007,
		ERR_WIFI_NVS = 0x3008,
		ERR_WIFI_MAC = 0x3009,
		ERR_WIFI_SSID = 0x300a,
		ERR_WIFI_PASSWORD = 0x300b,
		ERR_WIFI_TIMEOUT = 0x300c,
		ERR_WIFI_WAKE_FAIL = 0x300d,
		ERR_WIFI_WOULD_BLOCK = 0x300e,
		ERR_WIFI_NOT_CONNECT = 0x300f,
		ERR_WIFI_POST = 0x3012,
		ERR_WIFI_INIT_STATE = 0x3013,
		ERR_WIFI_STOP_STATE = 0x3014,
		ERR_WIFI_NOT_ASSOC = 0x3015,
		ERR_WIFI_TX_DISALLOW = 0x3016,
		ERR_WIFI_TWT_FULL = 0x3017,
		ERR_WIFI_TWT_SETUP_TIMEOUT = 0x3018,
		ERR_WIFI_TWT_SETUP_TXFAIL = 0x3019,
		ERR_WIFI_TWT_SETUP_REJECT = 0x301a,
		ERR_WIFI_DISCARD = 0x301b,
		ERR_WIFI_REGISTRAR = 0x3033,
		ERR_WIFI_WPS_TYPE = 0x3034,
		ERR_WIFI_WPS_SM = 0x3035,
		ERR_ESPNOW_BASE = 0x3064,
		ERR_ESPNOW_NOT_INIT = 0x3065,
		ERR_ESPNOW_ARG = 0x3066,
		ERR_ESPNOW_NO_MEM = 0x3067,
		ERR_ESPNOW_FULL = 0x3068,
		ERR_ESPNOW_NOT_FOUND = 0x3069,
		ERR_ESPNOW_INTERNAL = 0x306a,
		ERR_ESPNOW_EXIST = 0x306b,
		ERR_ESPNOW_IF = 0x306c,
		ERR_DPP_FAILURE = 0x3097,
		ERR_DPP_TX_FAILURE = 0x3098,
		ERR_DPP_INVALID_ATTR = 0x3099,
		ERR_MESH_BASE = 0x4000,
		ERR_MESH_WIFI_NOT_START = 0x4001,
		ERR_MESH_NOT_INIT = 0x4002,
		ERR_MESH_NOT_CONFIG = 0x4003,
		ERR_MESH_NOT_START = 0x4004,
		ERR_MESH_NOT_SUPPORT = 0x4005,
		ERR_MESH_NOT_ALLOWED = 0x4006,
		ERR_MESH_NO_MEMORY = 0x4007,
		ERR_MESH_ARGUMENT = 0x4008,
		ERR_MESH_EXCEED_MTU = 0x4009,
		ERR_MESH_TIMEOUT = 0x400a,
		ERR_MESH_DISCONNECTED = 0x400b,
		ERR_MESH_QUEUE_FAIL = 0x400c,
		ERR_MESH_QUEUE_FULL = 0x400d,
		ERR_MESH_NO_PARENT_FOUND = 0x400e,
		ERR_MESH_NO_ROUTE_FOUND = 0x400f,
		ERR_MESH_OPTION_NULL = 0x4010,
		ERR_MESH_OPTION_UNKNOWN = 0x4011,
		ERR_MESH_XON_NO_WINDOW = 0x4012,
		ERR_MESH_INTERFACE = 0x4013,
		ERR_MESH_DISCARD_DUPLICATE = 0x4014,
		ERR_MESH_DISCARD = 0x4015,
		ERR_MESH_VOTING = 0x4016,
		ERR_MESH_XMIT = 0x4017,
		ERR_MESH_QUEUE_READ = 0x4018,
		ERR_MESH_PS = 0x4019,
		ERR_MESH_RECV_RELEASE = 0x401a,
		ERR_ESP_NETIF_BASE = 0x5000,
		ERR_ESP_NETIF_INVALID_PARAMS = 0x5001,
		ERR_ESP_NETIF_IF_NOT_READY = 0x5002,
		ERR_ESP_NETIF_DHCPC_START_FAILED = 0x5003,
		ERR_ESP_NETIF_DHCP_ALREADY_STARTED = 0x5004,
		ERR_ESP_NETIF_DHCP_ALREADY_STOPPED = 0x5005,
		ERR_ESP_NETIF_NO_MEM = 0x5006,
		ERR_ESP_NETIF_DHCP_NOT_STOPPED = 0x5007,
		ERR_ESP_NETIF_DRIVER_ATTACH_FAILED = 0x5008,
		ERR_ESP_NETIF_INIT_FAILED = 0x5009,
		ERR_ESP_NETIF_DNS_NOT_CONFIGURED = 0x500a,
		ERR_ESP_NETIF_MLD6_FAILED = 0x500b,
		ERR_ESP_NETIF_IP6_ADDR_FAILED = 0x500c,
		ERR_ESP_NETIF_DHCPS_START_FAILED = 0x500d,
		ERR_FLASH_BASE = 0x6000,
		ERR_FLASH_OP_FAIL = 0x6001,
		ERR_FLASH_OP_TIMEOUT = 0x6002,
		ERR_FLASH_NOT_INITIALISED = 0x6003,
		ERR_FLASH_UNSUPPORTED_HOST = 0x6004,
		ERR_FLASH_UNSUPPORTED_CHIP = 0x6005,
		ERR_FLASH_PROTECTED = 0x6006,
		ERR_HTTP_BASE = 0x7000,
		ERR_HTTP_MAX_REDIRECT = 0x7001,
		ERR_HTTP_CONNECT = 0x7002,
		ERR_HTTP_WRITE_DATA = 0x7003,
		ERR_HTTP_FETCH_HEADER = 0x7004,
		ERR_HTTP_INVALID_TRANSPORT = 0x7005,
		ERR_HTTP_CONNECTING = 0x7006,
		ERR_HTTP_EAGAIN = 0x7007,
		ERR_HTTP_CONNECTION_CLOSED = 0x7008,
		ERR_ESP_TLS_BASE = 0x8000,
		ERR_ESP_TLS_CANNOT_RESOLVE_HOSTNAME = 0x8001,
		ERR_ESP_TLS_CANNOT_CREATE_SOCKET = 0x8002,
		ERR_ESP_TLS_UNSUPPORTED_PROTOCOL_FAMILY = 0x8003,
		ERR_ESP_TLS_FAILED_CONNECT_TO_HOST = 0x8004,
		ERR_ESP_TLS_SOCKET_SETOPT_FAILED = 0x8005,
		ERR_ESP_TLS_CONNECTION_TIMEOUT = 0x8006,
		ERR_ESP_TLS_SE_FAILED = 0x8007,
		ERR_ESP_TLS_TCP_CLOSED_FIN = 0x8008,
		ERR_MBEDTLS_CERT_PARTLY_OK = 0x8010,
		ERR_MBEDTLS_CTR_DRBG_SEED_FAILED = 0x8011,
		ERR_MBEDTLS_SSL_SET_HOSTNAME_FAILED = 0x8012,
		ERR_MBEDTLS_SSL_CONFIG_DEFAULTS_FAILED = 0x8013,
		ERR_MBEDTLS_SSL_CONF_ALPN_PROTOCOLS_FAILED = 0x8014,
		ERR_MBEDTLS_X509_CRT_PARSE_FAILED = 0x8015,
		ERR_MBEDTLS_SSL_CONF_OWN_CERT_FAILED = 0x8016,
		ERR_MBEDTLS_SSL_SETUP_FAILED = 0x8017,
		ERR_MBEDTLS_SSL_WRITE_FAILED = 0x8018,
		ERR_MBEDTLS_PK_PARSE_KEY_FAILED = 0x8019,
		ERR_MBEDTLS_SSL_HANDSHAKE_FAILED = 0x801a,
		ERR_MBEDTLS_SSL_CONF_PSK_FAILED = 0x801b,
		ERR_MBEDTLS_SSL_TICKET_SETUP_FAILED = 0x801c,
		ERR_WOLFSSL_SSL_SET_HOSTNAME_FAILED = 0x8031,
		ERR_WOLFSSL_SSL_CONF_ALPN_PROTOCOLS_FAILED = 0x8032,
		ERR_WOLFSSL_CERT_VERIFY_SETUP_FAILED = 0x8033,
		ERR_WOLFSSL_KEY_VERIFY_SETUP_FAILED = 0x8034,
		ERR_WOLFSSL_SSL_HANDSHAKE_FAILED = 0x8035,
		ERR_WOLFSSL_CTX_SETUP_FAILED = 0x8036,
		ERR_WOLFSSL_SSL_SETUP_FAILED = 0x8037,
		ERR_WOLFSSL_SSL_WRITE_FAILED = 0x8038,
		ERR_HTTPS_OTA_BASE = 0x9000,
		ERR_HTTPS_OTA_IN_PROGRESS = 0x9001,
		ERR_PING_BASE = 0xa000,
		ERR_PING_INVALID_PARAMS = 0xa001,
		ERR_PING_NO_MEM = 0xa002,
		ERR_HTTPD_BASE = 0xb000,
		ERR_HTTPD_HANDLERS_FULL = 0xb001,
		ERR_HTTPD_HANDLER_EXISTS = 0xb002,
		ERR_HTTPD_INVALID_REQ = 0xb003,
		ERR_HTTPD_RESULT_TRUNC = 0xb004,
		ERR_HTTPD_RESP_HDR = 0xb005,
		ERR_HTTPD_RESP_SEND = 0xb006,
		ERR_HTTPD_ALLOC_MEM = 0xb007,
		ERR_HTTPD_TASK = 0xb008,
		ERR_HW_CRYPTO_BASE = 0xc000,
		ERR_HW_CRYPTO_DS_HMAC_FAIL = 0xc001,
		ERR_HW_CRYPTO_DS_INVALID_KEY = 0xc002,
		ERR_HW_CRYPTO_DS_INVALID_DIGEST = 0xc004,
		ERR_HW_CRYPTO_DS_INVALID_PADDING = 0xc005,
		ERR_MEMPROT_BASE = 0xd000,
		ERR_MEMPROT_MEMORY_TYPE_INVALID = 0xd001,
		ERR_MEMPROT_SPLIT_ADDR_INVALID = 0xd002,
		ERR_MEMPROT_SPLIT_ADDR_OUT_OF_RANGE = 0xd003,
		ERR_MEMPROT_SPLIT_ADDR_UNALIGNED = 0xd004,
		ERR_MEMPROT_UNIMGMT_BLOCK_INVALID = 0xd005,
		ERR_MEMPROT_WORLD_INVALID = 0xd006,
		ERR_MEMPROT_AREA_INVALID = 0xd007,
		ERR_MEMPROT_CPUID_INVALID = 0xd008,
		ERR_TCP_TRANSPORT_BASE = 0xe000,
		ERR_TCP_TRANSPORT_CONNECTION_TIMEOUT = 0xe001,
		ERR_TCP_TRANSPORT_CONNECTION_CLOSED_BY_FIN = 0xe002,
		ERR_TCP_TRANSPORT_CONNECTION_FAILED = 0xe003,
		ERR_TCP_TRANSPORT_NO_MEM = 0xe004,
		ERR_NVS_SEC_BASE = 0xf000,
		ERR_NVS_SEC_HMAC_KEY_NOT_FOUND = 0xf001,
		ERR_NVS_SEC_HMAC_KEY_BLK_ALREADY_USED = 0xf002,
		ERR_NVS_SEC_HMAC_KEY_GENERATION_FAILED = 0xf003,
		ERR_NVS_SEC_HMAC_XTS_KEYS_DERIV_FAILED = 0xf004
	};

	struct category : std::error_category {
		const char *name() const noexcept override {
			return "esp";
		}
		// The use of a heap allocated string is painful
		// but sadly baked into the std::error_category interface.
		std::string message(int ev) const override {
			switch (static_cast<codes>(ev)) {
				using enum codes;
			case FAIL:
				return "Generic esp_err_t code indicating failure";
			case OK:
				return "esp_err_t value indicating success";
			case ERR_NO_MEM:
				return "Out of memory";
			case ERR_INVALID_ARG:
				return "Invalid argument";
			case ERR_INVALID_STATE:
				return "Invalid state";
			case ERR_INVALID_SIZE:
				return "Invalid size";
			case ERR_NOT_FOUND:
				return "Requested resource not found";
			case ERR_NOT_SUPPORTED:
				return "Operation or feature not supported";
			case ERR_TIMEOUT:
				return "Operation timed out";
			case ERR_INVALID_RESPONSE:
				return "Received response was invalid";
			case ERR_INVALID_CRC:
				return "CRC or checksum was invalid";
			case ERR_INVALID_VERSION:
				return "Version was invalid";
			case ERR_INVALID_MAC:
				return "MAC address was invalid";
			case ERR_NOT_FINISHED:
				return "Operation has not fully completed";
			case ERR_NVS_BASE:
				return "Starting number of error codes";
			case ERR_NVS_NOT_INITIALIZED:
				return "The storage driver is not initialized";
			case ERR_NVS_NOT_FOUND:
				return "A requested entry couldn't be found or namespace doesn't exist yet and mode is NVS_READONLY";
			case ERR_NVS_TYPE_MISMATCH:
				return "The type of set or get operation doesn't match the type of value stored in NVS";
			case ERR_NVS_READ_ONLY:
				return "Storage handle was opened as read only";
			case ERR_NVS_NOT_ENOUGH_SPACE:
				return "There is not enough space in the underlying storage to save the value";
			case ERR_NVS_INVALID_NAME:
				return "Namespace name doesn't satisfy constraints";
			case ERR_NVS_INVALID_HANDLE:
				return "Handle has been closed or is NULL";
			case ERR_NVS_REMOVE_FAILED:
				return "The value wasn't updated because flash write operation has failed. The value was written however, and update will be finished after re-initialization of nvs, provided that flash operation doesn't fail again.";
			case ERR_NVS_KEY_TOO_LONG:
				return "Key name is too long";
			case ERR_NVS_PAGE_FULL:
				return "Internal error; never returned by nvs API functions";
			case ERR_NVS_INVALID_STATE:
				return "NVS is in an inconsistent state due to a previous error. Call nvs_flash_init and nvs_open again, then retry.";
			case ERR_NVS_INVALID_LENGTH:
				return "String or blob length is not sufficient to store data";
			case ERR_NVS_NO_FREE_PAGES:
				return "NVS partition doesn't contain any empty pages. This may happen if NVS partition was truncated. Erase the whole partition and call nvs_flash_init again.";
			case ERR_NVS_VALUE_TOO_LONG:
				return "Value doesn't fit into the entry or string or blob length is longer than supported by the implementation";
			case ERR_NVS_PART_NOT_FOUND:
				return "Partition with specified name is not found in the partition table";
			case ERR_NVS_NEW_VERSION_FOUND:
				return "NVS partition contains data in new format and cannot be recognized by this version of code";
			case ERR_NVS_XTS_ENCR_FAILED:
				return "XTS encryption failed while writing NVS entry";
			case ERR_NVS_XTS_DECR_FAILED:
				return "XTS decryption failed while reading NVS entry";
			case ERR_NVS_XTS_CFG_FAILED:
				return "XTS configuration setting failed";
			case ERR_NVS_XTS_CFG_NOT_FOUND:
				return "XTS configuration not found";
			case ERR_NVS_ENCR_NOT_SUPPORTED:
				return "NVS encryption is not supported in this version";
			case ERR_NVS_KEYS_NOT_INITIALIZED:
				return "NVS key partition is uninitialized";
			case ERR_NVS_CORRUPT_KEY_PART:
				return "NVS key partition is corrupt";
			case ERR_NVS_CONTENT_DIFFERS:
				return "Internal error; never returned by nvs API functions. NVS key is different in comparison";
			case ERR_NVS_WRONG_ENCRYPTION:
				return "NVS partition is marked as encrypted with generic flash encryption. This is forbidden since the NVS encryption works differently.";
			case ERR_ULP_BASE:
				return "Offset for ULP-related error codes";
			case ERR_ULP_SIZE_TOO_BIG:
				return "Program doesn't fit into RTC memory reserved for the ULP";
			case ERR_ULP_INVALID_LOAD_ADDR:
				return "Load address is outside of RTC memory reserved for the ULP";
			case ERR_ULP_DUPLICATE_LABEL:
				return "More than one label with the same number was defined";
			case ERR_ULP_UNDEFINED_LABEL:
				return "Branch instructions references an undefined label";
			case ERR_ULP_BRANCH_OUT_OF_RANGE:
				return "Branch target is out of range of B instruction = ";
			case ERR_OTA_BASE:
				return "Base error code for ota_ops api";
			case ERR_OTA_PARTITION_CONFLICT:
				return "Error if request was to write or erase the current running partition";
			case ERR_OTA_SELECT_INFO_INVALID:
				return "Error if OTA data partition contains invalid content";
			case ERR_OTA_VALIDATE_FAILED:
				return "Error if OTA app image is invalid";
			case ERR_OTA_SMALL_SEC_VER:
				return "Error if the firmware has a secure version less than the running firmware.";
			case ERR_OTA_ROLLBACK_FAILED:
				return "Error if flash does not have valid firmware in passive partition and hence rollback is not possible";
			case ERR_OTA_ROLLBACK_INVALID_STATE:
				return "Error if current active firmware is still marked in pending validation state = ";
			case ERR_EFUSE:
				return "Base error code for efuse api.";
			case OK_EFUSE_CNT:
				return "OK the required number of bits is set.";
			case ERR_EFUSE_CNT_IS_FULL:
				return "Error field is full.";
			case ERR_EFUSE_REPEATED_PROG:
				return "Error repeated programming of programmed bits is strictly forbidden.";
			case ERR_CODING:
				return "Error while a encoding operation.";
			case ERR_NOT_ENOUGH_UNUSED_KEY_BLOCKS:
				return "Error not enough unused key blocks available";
			case ERR_DAMAGED_READING:
				return "Error. Burn or reset was done during a reading operation leads to damage read data. This error is internal to the efuse component and not returned by any public API.";
			case ERR_IMAGE_BASE:
				return "ESP_ERR_IMAGE_BASE";
			case ERR_IMAGE_FLASH_FAIL:
				return "ESP_ERR_IMAGE_FLASH_FAIL";
			case ERR_IMAGE_INVALID:
				return "ESP_ERR_IMAGE_INVALID";
			case ERR_WIFI_BASE:
				return "Starting number of WiFi error codes";
			case ERR_WIFI_NOT_INIT:
				return "WiFi driver was not installed by esp_wifi_init";
			case ERR_WIFI_NOT_STARTED:
				return "WiFi driver was not started by esp_wifi_start";
			case ERR_WIFI_NOT_STOPPED:
				return "WiFi driver was not stopped by esp_wifi_stop";
			case ERR_WIFI_IF:
				return "WiFi interface error";
			case ERR_WIFI_MODE:
				return "WiFi mode error";
			case ERR_WIFI_STATE:
				return "WiFi internal state error";
			case ERR_WIFI_CONN:
				return "WiFi internal control block of station or soft-AP error";
			case ERR_WIFI_NVS:
				return "WiFi internal NVS module error";
			case ERR_WIFI_MAC:
				return "MAC address is invalid";
			case ERR_WIFI_SSID:
				return "SSID is invalid";
			case ERR_WIFI_PASSWORD:
				return "Password is invalid";
			case ERR_WIFI_TIMEOUT:
				return "Timeout error";
			case ERR_WIFI_WAKE_FAIL:
				return "WiFi is in sleep state(RF closed and wakeup fail";
			case ERR_WIFI_WOULD_BLOCK:
				return "The caller would block";
			case ERR_WIFI_NOT_CONNECT:
				return "Station still in disconnect status";
			case ERR_WIFI_POST:
				return "Failed to post the event to WiFi task";
			case ERR_WIFI_INIT_STATE:
				return "Invalid WiFi state when init/deinit is called";
			case ERR_WIFI_STOP_STATE:
				return "Returned when WiFi is stopping";
			case ERR_WIFI_NOT_ASSOC:
				return "The WiFi connection is not associated";
			case ERR_WIFI_TX_DISALLOW:
				return "The WiFi TX is disallowed";
			case ERR_WIFI_TWT_FULL:
				return "no available flow id";
			case ERR_WIFI_TWT_SETUP_TIMEOUT:
				return "Timeout of receiving twt setup response frame, timeout times can be set during twt setup";
			case ERR_WIFI_TWT_SETUP_TXFAIL:
				return "TWT setup frame tx failed";
			case ERR_WIFI_TWT_SETUP_REJECT:
				return "The twt setup request was rejected by the AP";
			case ERR_WIFI_DISCARD:
				return "Discard frame";
			case ERR_WIFI_REGISTRAR:
				return "WPS registrar is not supported";
			case ERR_WIFI_WPS_TYPE:
				return "WPS type error";
			case ERR_WIFI_WPS_SM:
				return "WPS state machine is not initialized";
			case ERR_ESPNOW_BASE:
				return "ESPNOW error number base.";
			case ERR_ESPNOW_NOT_INIT:
				return "ESPNOW is not initialized.";
			case ERR_ESPNOW_ARG:
				return "Invalid argument";
			case ERR_ESPNOW_NO_MEM:
				return "Out of memory";
			case ERR_ESPNOW_FULL:
				return "ESPNOW peer list is full";
			case ERR_ESPNOW_NOT_FOUND:
				return "ESPNOW peer is not found";
			case ERR_ESPNOW_INTERNAL:
				return "Internal error";
			case ERR_ESPNOW_EXIST:
				return "ESPNOW peer has existed";
			case ERR_ESPNOW_IF:
				return "Interface error";
			case ERR_DPP_FAILURE:
				return "Generic failure during DPP Operation";
			case ERR_DPP_TX_FAILURE:
				return "DPP Frame Tx failed OR not Acked";
			case ERR_DPP_INVALID_ATTR:
				return "Encountered invalid DPP Attribute";
			case ERR_MESH_BASE:
				return "Starting number of MESH error codes";
			case ERR_MESH_WIFI_NOT_START:
				return "ESP_ERR_MESH_WIFI_NOT_START";
			case ERR_MESH_NOT_INIT:
				return "ESP_ERR_MESH_NOT_INIT";
			case ERR_MESH_NOT_CONFIG:
				return "ESP_ERR_MESH_NOT_CONFIG";
			case ERR_MESH_NOT_START:
				return "ESP_ERR_MESH_NOT_START";
			case ERR_MESH_NOT_SUPPORT:
				return "ESP_ERR_MESH_NOT_SUPPORT";
			case ERR_MESH_NOT_ALLOWED:
				return "ESP_ERR_MESH_NOT_ALLOWED";
			case ERR_MESH_NO_MEMORY:
				return "ESP_ERR_MESH_NO_MEMORY";
			case ERR_MESH_ARGUMENT:
				return "ESP_ERR_MESH_ARGUMENT";
			case ERR_MESH_EXCEED_MTU:
				return "ESP_ERR_MESH_EXCEED_MTU";
			case ERR_MESH_TIMEOUT:
				return "ESP_ERR_MESH_TIMEOUT";
			case ERR_MESH_DISCONNECTED:
				return "ESP_ERR_MESH_DISCONNECTED";
			case ERR_MESH_QUEUE_FAIL:
				return "ESP_ERR_MESH_QUEUE_FAIL";
			case ERR_MESH_QUEUE_FULL:
				return "ESP_ERR_MESH_QUEUE_FULL";
			case ERR_MESH_NO_PARENT_FOUND:
				return "ESP_ERR_MESH_NO_PARENT_FOUND";
			case ERR_MESH_NO_ROUTE_FOUND:
				return "ESP_ERR_MESH_NO_ROUTE_FOUND";
			case ERR_MESH_OPTION_NULL:
				return "ESP_ERR_MESH_OPTION_NULL";
			case ERR_MESH_OPTION_UNKNOWN:
				return "ESP_ERR_MESH_OPTION_UNKNOWN";
			case ERR_MESH_XON_NO_WINDOW:
				return "ESP_ERR_MESH_XON_NO_WINDOW";
			case ERR_MESH_INTERFACE:
				return "ESP_ERR_MESH_INTERFACE";
			case ERR_MESH_DISCARD_DUPLICATE:
				return "ESP_ERR_MESH_DISCARD_DUPLICATE";
			case ERR_MESH_DISCARD:
				return "ESP_ERR_MESH_DISCARD";
			case ERR_MESH_VOTING:
				return "ESP_ERR_MESH_VOTING";
			case ERR_MESH_XMIT:
				return "ESP_ERR_MESH_XMIT";
			case ERR_MESH_QUEUE_READ:
				return "ESP_ERR_MESH_QUEUE_READ";
			case ERR_MESH_PS:
				return "ESP_ERR_MESH_PS";
			case ERR_MESH_RECV_RELEASE:
				return "ESP_ERR_MESH_RECV_RELEASE";
			case ERR_ESP_NETIF_BASE:
				return "ESP_ERR_ESP_NETIF_BASE";
			case ERR_ESP_NETIF_INVALID_PARAMS:
				return "ESP_ERR_ESP_NETIF_INVALID_PARAMS";
			case ERR_ESP_NETIF_IF_NOT_READY:
				return "ESP_ERR_ESP_NETIF_IF_NOT_READY";
			case ERR_ESP_NETIF_DHCPC_START_FAILED:
				return "ESP_ERR_ESP_NETIF_DHCPC_START_FAILED";
			case ERR_ESP_NETIF_DHCP_ALREADY_STARTED:
				return "ESP_ERR_ESP_NETIF_DHCP_ALREADY_STARTED";
			case ERR_ESP_NETIF_DHCP_ALREADY_STOPPED:
				return "ESP_ERR_ESP_NETIF_DHCP_ALREADY_STOPPED";
			case ERR_ESP_NETIF_NO_MEM:
				return "ESP_ERR_ESP_NETIF_NO_MEM";
			case ERR_ESP_NETIF_DHCP_NOT_STOPPED:
				return "ESP_ERR_ESP_NETIF_DHCP_NOT_STOPPED";
			case ERR_ESP_NETIF_DRIVER_ATTACH_FAILED:
				return "ESP_ERR_ESP_NETIF_DRIVER_ATTACH_FAILED";
			case ERR_ESP_NETIF_INIT_FAILED:
				return "ESP_ERR_ESP_NETIF_INIT_FAILED";
			case ERR_ESP_NETIF_DNS_NOT_CONFIGURED:
				return "ESP_ERR_ESP_NETIF_DNS_NOT_CONFIGURED";
			case ERR_ESP_NETIF_MLD6_FAILED:
				return "ESP_ERR_ESP_NETIF_MLD6_FAILED";
			case ERR_ESP_NETIF_IP6_ADDR_FAILED:
				return "ESP_ERR_ESP_NETIF_IP6_ADDR_FAILED";
			case ERR_ESP_NETIF_DHCPS_START_FAILED:
				return "ESP_ERR_ESP_NETIF_DHCPS_START_FAILED";
			case ERR_FLASH_BASE:
				return "Starting number of flash error codes";
			case ERR_FLASH_OP_FAIL:
				return "ESP_ERR_FLASH_OP_FAIL";
			case ERR_FLASH_OP_TIMEOUT:
				return "ESP_ERR_FLASH_OP_TIMEOUT";
			case ERR_FLASH_NOT_INITIALISED:
				return "ESP_ERR_FLASH_NOT_INITIALISED";
			case ERR_FLASH_UNSUPPORTED_HOST:
				return "ESP_ERR_FLASH_UNSUPPORTED_HOST";
			case ERR_FLASH_UNSUPPORTED_CHIP:
				return "ESP_ERR_FLASH_UNSUPPORTED_CHIP";
			case ERR_FLASH_PROTECTED:
				return "ESP_ERR_FLASH_PROTECTED";
			case ERR_HTTP_BASE:
				return "Starting number of HTTP error codes";
			case ERR_HTTP_MAX_REDIRECT:
				return "The error exceeds the number of HTTP redirects";
			case ERR_HTTP_CONNECT:
				return "Error open the HTTP connection";
			case ERR_HTTP_WRITE_DATA:
				return "Error write HTTP data";
			case ERR_HTTP_FETCH_HEADER:
				return "Error read HTTP header from server";
			case ERR_HTTP_INVALID_TRANSPORT:
				return "There are no transport support for the input scheme";
			case ERR_HTTP_CONNECTING:
				return "HTTP connection hasn't been established yet";
			case ERR_HTTP_EAGAIN:
				return "Mapping of errno EAGAIN to esp_err_t";
			case ERR_HTTP_CONNECTION_CLOSED:
				return "Read FIN from peer and the connection closed";
			case ERR_ESP_TLS_BASE:
				return "Starting number of ESP-TLS error codes";
			case ERR_ESP_TLS_CANNOT_RESOLVE_HOSTNAME:
				return "Error if hostname couldn't be resolved upon tls connection";
			case ERR_ESP_TLS_CANNOT_CREATE_SOCKET:
				return "Failed to create socket";
			case ERR_ESP_TLS_UNSUPPORTED_PROTOCOL_FAMILY:
				return "Unsupported protocol family";
			case ERR_ESP_TLS_FAILED_CONNECT_TO_HOST:
				return "Failed to connect to host";
			case ERR_ESP_TLS_SOCKET_SETOPT_FAILED:
				return "failed to set/get socket option";
			case ERR_ESP_TLS_CONNECTION_TIMEOUT:
				return "new connection in esp_tls_low_level_conn connection timeouted";
			case ERR_ESP_TLS_SE_FAILED:
				return "ESP_ERR_ESP_TLS_SE_FAILED";
			case ERR_ESP_TLS_TCP_CLOSED_FIN:
				return "ESP_ERR_ESP_TLS_TCP_CLOSED_FIN";
			case ERR_MBEDTLS_CERT_PARTLY_OK:
				return "mbedtls parse certificates was partly successful";
			case ERR_MBEDTLS_CTR_DRBG_SEED_FAILED:
				return "mbedtls api returned error";
			case ERR_MBEDTLS_SSL_SET_HOSTNAME_FAILED:
				return "mbedtls api returned error";
			case ERR_MBEDTLS_SSL_CONFIG_DEFAULTS_FAILED:
				return "mbedtls api returned error";
			case ERR_MBEDTLS_SSL_CONF_ALPN_PROTOCOLS_FAILED:
				return "mbedtls api returned error";
			case ERR_MBEDTLS_X509_CRT_PARSE_FAILED:
				return "mbedtls api returned error";
			case ERR_MBEDTLS_SSL_CONF_OWN_CERT_FAILED:
				return "mbedtls api returned error";
			case ERR_MBEDTLS_SSL_SETUP_FAILED:
				return "mbedtls api returned error";
			case ERR_MBEDTLS_SSL_WRITE_FAILED:
				return "mbedtls api returned error";
			case ERR_MBEDTLS_PK_PARSE_KEY_FAILED:
				return "mbedtls api returned failed";
			case ERR_MBEDTLS_SSL_HANDSHAKE_FAILED:
				return "mbedtls api returned failed";
			case ERR_MBEDTLS_SSL_CONF_PSK_FAILED:
				return "mbedtls api returned failed";
			case ERR_MBEDTLS_SSL_TICKET_SETUP_FAILED:
				return "mbedtls api returned failed";
			case ERR_WOLFSSL_SSL_SET_HOSTNAME_FAILED:
				return "wolfSSL api returned error";
			case ERR_WOLFSSL_SSL_CONF_ALPN_PROTOCOLS_FAILED:
				return "wolfSSL api returned error";
			case ERR_WOLFSSL_CERT_VERIFY_SETUP_FAILED:
				return "wolfSSL api returned error";
			case ERR_WOLFSSL_KEY_VERIFY_SETUP_FAILED:
				return "wolfSSL api returned error";
			case ERR_WOLFSSL_SSL_HANDSHAKE_FAILED:
				return "wolfSSL api returned failed";
			case ERR_WOLFSSL_CTX_SETUP_FAILED:
				return "wolfSSL api returned failed";
			case ERR_WOLFSSL_SSL_SETUP_FAILED:
				return "wolfSSL api returned failed";
			case ERR_WOLFSSL_SSL_WRITE_FAILED:
				return "wolfSSL api returned failed";
			case ERR_HTTPS_OTA_BASE:
				return "ESP_ERR_HTTPS_OTA_BASE";
			case ERR_HTTPS_OTA_IN_PROGRESS:
				return "ESP_ERR_HTTPS_OTA_IN_PROGRESS";
			case ERR_PING_BASE:
				return "ESP_ERR_PING_BASE";
			case ERR_PING_INVALID_PARAMS:
				return "ESP_ERR_PING_INVALID_PARAMS";
			case ERR_PING_NO_MEM:
				return "ESP_ERR_PING_NO_MEM";
			case ERR_HTTPD_BASE:
				return "Starting number of HTTPD error codes";
			case ERR_HTTPD_HANDLERS_FULL:
				return "All slots for registering URI handlers have been consumed";
			case ERR_HTTPD_HANDLER_EXISTS:
				return "URI handler with same method and target URI already registered";
			case ERR_HTTPD_INVALID_REQ:
				return "Invalid request pointer";
			case ERR_HTTPD_RESULT_TRUNC:
				return "Result string truncated";
			case ERR_HTTPD_RESP_HDR:
				return "Response header field larger than supported";
			case ERR_HTTPD_RESP_SEND:
				return "Error occured while sending response packet";
			case ERR_HTTPD_ALLOC_MEM:
				return "Failed to dynamically allocate memory for resource";
			case ERR_HTTPD_TASK:
				return "Failed to launch server task/thread";
			case ERR_HW_CRYPTO_BASE:
				return "Starting number of HW cryptography module error codes";
			case ERR_HW_CRYPTO_DS_HMAC_FAIL:
				return "HMAC peripheral problem";
			case ERR_HW_CRYPTO_DS_INVALID_KEY:
				return "ESP_ERR_HW_CRYPTO_DS_INVALID_KEY";
			case ERR_HW_CRYPTO_DS_INVALID_DIGEST:
				return "ESP_ERR_HW_CRYPTO_DS_INVALID_DIGEST";
			case ERR_HW_CRYPTO_DS_INVALID_PADDING:
				return "ESP_ERR_HW_CRYPTO_DS_INVALID_PADDING";
			case ERR_MEMPROT_BASE:
				return "Starting number of Memory Protection API error codes";
			case ERR_MEMPROT_MEMORY_TYPE_INVALID:
				return "ESP_ERR_MEMPROT_MEMORY_TYPE_INVALID";
			case ERR_MEMPROT_SPLIT_ADDR_INVALID:
				return "ESP_ERR_MEMPROT_SPLIT_ADDR_INVALID";
			case ERR_MEMPROT_SPLIT_ADDR_OUT_OF_RANGE:
				return "ESP_ERR_MEMPROT_SPLIT_ADDR_OUT_OF_RANGE";
			case ERR_MEMPROT_SPLIT_ADDR_UNALIGNED:
				return "ESP_ERR_MEMPROT_SPLIT_ADDR_UNALIGNED";
			case ERR_MEMPROT_UNIMGMT_BLOCK_INVALID:
				return "ESP_ERR_MEMPROT_UNIMGMT_BLOCK_INVALID";
			case ERR_MEMPROT_WORLD_INVALID:
				return "ESP_ERR_MEMPROT_WORLD_INVALID";
			case ERR_MEMPROT_AREA_INVALID:
				return "ESP_ERR_MEMPROT_AREA_INVALID";
			case ERR_MEMPROT_CPUID_INVALID:
				return "ESP_ERR_MEMPROT_CPUID_INVALID";
			case ERR_TCP_TRANSPORT_BASE:
				return "Starting number of TCP Transport error codes";
			case ERR_TCP_TRANSPORT_CONNECTION_TIMEOUT:
				return "Connection has timed out";
			case ERR_TCP_TRANSPORT_CONNECTION_CLOSED_BY_FIN:
				return "Read FIN from peer and the connection has closed = ";
			case ERR_TCP_TRANSPORT_CONNECTION_FAILED:
				return "Failed to connect to the peer";
			case ERR_TCP_TRANSPORT_NO_MEM:
				return "Memory allocation failed";
			case ERR_NVS_SEC_BASE:
				return "Starting number of error codes";
			case ERR_NVS_SEC_HMAC_KEY_NOT_FOUND:
				return "HMAC Key required to generate the NVS encryption keys not found";
			case ERR_NVS_SEC_HMAC_KEY_BLK_ALREADY_USED:
				return "Provided eFuse block for HMAC key generation is already in use";
			case ERR_NVS_SEC_HMAC_KEY_GENERATION_FAILED:
				return "Failed to generate/write the HMAC key to eFuse";
			case ERR_NVS_SEC_HMAC_XTS_KEYS_DERIV_FAILED:
				return "Failed to derive the NVS encryption keys based on the HMAC-based scheme";
			default : return "(unrecognized error)";
			}
		}
	};

	inline std::error_code make_error_code(codes e) {
		static category cat;
		return {static_cast<int>(e), cat};
	}
}

template <>
struct std::is_error_code_enum<esp_error::codes> : public std::true_type{};
