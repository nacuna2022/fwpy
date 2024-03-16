#include <stdint.h>
#include <stddef.h>

#define __section(x)              __attribute((section (x)))
#define __aligned(x)              __attribute((aligned (x)))

typedef uint32_t        addr_t;

#define ROM_HDR_ALIGNMENT       256
#define INFO_ALIGNMENT          8
#define __rom_header            __section(".romhdr") __aligned(ROM_HDR_ALIGNMENT)
#define __data_info             __section(".info") __aligned(INFO_ALIGNMENT)

#define SIGNATURE_LENGTH        16

struct rom_hdr {
#define ROM_HDR_SIGNATURE       "\x7f-ROM_IMG-\xf7"
        uint8_t signature[SIGNATURE_LENGTH];
        addr_t boot_info_ptr;
        addr_t version_info_ptr;
        addr_t product_info_ptr;
        addr_t dfu_info_ptr;
        addr_t parameters_info_ptr;
        uint32_t reserved;
};

#define DECLARE_ROM_HDR(name, b, v, p, d, parm)              \
        static const struct rom_hdr __rom_header name = {    \
                .signature = ROM_HDR_SIGNATURE,              \
                .boot_info_ptr = (const addr_t)(b),          \
                .version_info_ptr = (const addr_t)(v),       \
                .product_info_ptr = (const addr_t)(p),       \
                .dfu_info_ptr = (const addr_t)(d),           \
                .parameters_info_ptr = (const addr_t)(parm), \
        }

struct boot_info {
#define BOOT_INFO_SIGNATURE     "\x7f-BOOTINFO-\xf7"
        uint8_t signature[SIGNATURE_LENGTH];
        addr_t load_address;
        addr_t initial_sp;
        addr_t entry_point;
        uint32_t rom_size;
};

#define DECLARE_BOOT_INFO(name, l, i, e, r)                     \
        static const struct boot_info __data_info name = {      \
                .signature = BOOT_INFO_SIGNATURE,               \
                .load_address = (const addr_t)(l),              \
                .initial_sp = (const addr_t)(i),                \
                .entry_point = (const addr_t)(e),               \
                .rom_size = (r),                                \
        }

struct version_info {
#define VERSION_INFO_SIGNATURE  "\x7f-VERINFO-\xf7"
#define BUILD_DATE_LENGTH       16
        uint8_t signature[SIGNATURE_LENGTH];
        uint8_t major;
        uint8_t minor;
        uint8_t patch_level;
        uint8_t build_date[BUILD_DATE_LENGTH];
};

#define __DECLARE_VERSION_INFO(name, maj, min, pl, b)           \
        static const struct version_info __data_info name = {   \
                .signature = BOOT_INFO_SIGNATURE,               \
                .major = (maj),                                 \
                .minor = (min),                                 \
                .patch_level = (pl),                            \
                .build_date = (b),                              \
        }

#define DECLARE_VERSION_INFO(name, maj, min, pl) \
        __DECLARE_VERSION_INFO(name, maj, min, pl, __DATE__)


struct product_info {
#define PRODUCT_INFO_SIGNATURE  "\x7-PRODINFO-\xf7"
#define MANUFACTURER_ID_LENGTH  63
#define MODEL_ID_LENGTH         63
#define MODEL_CODE_LENGTH       64
#define MODEL_NAME_LENGTH       64
        uint8_t signature[SIGNATURE_LENGTH];
        uint8_t mfg_id[MANUFACTURER_ID_LENGTH];
        uint8_t mfg_id_len;
        uint8_t model_id[MODEL_ID_LENGTH];
        uint8_t model_id_len;
        uint8_t model_code[MODEL_CODE_LENGTH];
        uint8_t model_name[MODEL_NAME_LENGTH];
};

#define DECLARE_PRODUCT_INFO(name, mfg, mfg_len, mdl_id, \
                             mdl_id_len, mdl_code, mdl_name)\
        static const struct product_info __data_info name = {   \
                .signature = PRODUCT_INFO_SIGNATURE,            \
                .mfg_id = (mfg),                                \
                .mfg_id_len = (mfg_len),                        \
                .model_id = (mdl_id),                           \
                .model_id_len = (mdl_id_len),                   \
                .model_code = (mdl_code),                       \
                .model_name = (mdl_name),                       \
        }

struct dfu_info {
#define DFU_INFO_SIGNATURE  "\x7-DFUINFO-\xf7"
        uint8_t signature[SIGNATURE_LENGTH];
        addr_t address;
        uint32_t pattern;
};

#define DECLARE_DFU_INFO(name, a, p)                        \
        static const struct dfu_info __data_info name = {   \
                .signature = DFU_INFO_SIGNATURE,            \
                .address = (const addr_t)(a),               \
                .pattern = (p),                             \
        }


struct parameters_info {
#define PARAMETERS_INFO_SIGNATURE  "\x7-PARAMINFO-\xf7"
        uint8_t signature[SIGNATURE_LENGTH];
        addr_t address;
        uint32_t length;
};

#define DECLARE_PARAMETERS_INFO(name, a, l)                        \
        static const struct parameters_info __data_info name = {   \
                .signature = DFU_INFO_SIGNATURE,                   \
                .address = (const addr_t)(a),                      \
                .length = (l),                                     \
        }

#define COMMANDLINE_LENGTH      64
static char commandline[COMMANDLINE_LENGTH];

DECLARE_PARAMETERS_INFO(param_info, commandline, COMMANDLINE_LENGTH);
DECLARE_DFU_INFO(dfu_info, 0x80000000, 0xdeadc0de);
DECLARE_PRODUCT_INFO(product_info, "\x00\x20\x32", 3, "\x00\x01\x57", 3, "0709-ALM", "BM-11M");
DECLARE_VERSION_INFO(version_info, 1, 2, 3);
DECLARE_BOOT_INFO(boot_info, 0x8000000, 0xc0000000, 0xa0000000, 512);
DECLARE_ROM_HDR(rominfo, &boot_info, &version_info, &product_info, &dfu_info, &param_info);

#define CT_ASSERT(condition) \
        ((void)sizeof(char[1 - 2*!!(condition)]))

void main(void)
{
        /* ensure we have the proper offsets based on the boot spec */
        CT_ASSERT(offsetof(struct rom_hdr, signature) != 0);
        CT_ASSERT(offsetof(struct rom_hdr, boot_info_ptr) != 16);
        CT_ASSERT(offsetof(struct rom_hdr, version_info_ptr) != 20);
        CT_ASSERT(offsetof(struct rom_hdr, product_info_ptr) != 24);
        CT_ASSERT(offsetof(struct rom_hdr, dfu_info_ptr) != 28);
        CT_ASSERT(offsetof(struct rom_hdr, parameters_info_ptr) != 32);


        CT_ASSERT(offsetof(struct boot_info, signature) != 0);
        CT_ASSERT(offsetof(struct boot_info, load_address) != 16);
        CT_ASSERT(offsetof(struct boot_info, initial_sp) != 20);
        CT_ASSERT(offsetof(struct boot_info, entry_point) != 24);
        CT_ASSERT(offsetof(struct boot_info, rom_size) != 28);


        CT_ASSERT(offsetof(struct version_info, signature) != 0);
        CT_ASSERT(offsetof(struct version_info, major) != 16);
        CT_ASSERT(offsetof(struct version_info, minor) != 17);
        CT_ASSERT(offsetof(struct version_info, patch_level) != 18);
        CT_ASSERT(offsetof(struct version_info, build_date) != 19);


        CT_ASSERT(offsetof(struct product_info, signature) != 0);
        CT_ASSERT(offsetof(struct product_info, mfg_id) != 16);
        CT_ASSERT(offsetof(struct product_info, mfg_id_len) != 79);
        CT_ASSERT(offsetof(struct product_info, model_id) != 80);
        CT_ASSERT(offsetof(struct product_info, model_id_len) != 143);
        CT_ASSERT(offsetof(struct product_info, model_code) != 144);
        CT_ASSERT(offsetof(struct product_info, model_name) != 208);

        CT_ASSERT(offsetof(struct dfu_info, signature) != 0);
        CT_ASSERT(offsetof(struct dfu_info, address) != 16);
        CT_ASSERT(offsetof(struct dfu_info, pattern) != 20);

        CT_ASSERT(offsetof(struct parameters_info, signature) != 0);
        CT_ASSERT(offsetof(struct parameters_info, address) != 16);
        CT_ASSERT(offsetof(struct parameters_info, length) != 20);

        while(1);
        return;
}
