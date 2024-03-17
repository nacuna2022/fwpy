from intelhex import IntelHex

class firmware:
    def __init__(self):
        self.major = 0
        self.minor = 0
        self.patch = 0

    def loadhex(self, source):
        data = ""
        ih = IntelHex(source)
        i = ih.minaddr()
        maxaddr = ih.maxaddr()
        while (i < maxaddr):
            data += chr(ih[i])
            i += 1
        self.data = data.encode('latin-1')

    def loadbin(self, source):
        with open(source, "rb") as src:
            self.data = src.read()

    def load(self, source=None, form=None):
        if source is not None:
            self.source = source
        else:
            raise ValueError("No source file specified")

        if (form == "hex"):
            self.loadhex(source)
        elif (form == "bin"):
            self.loadbin(source)
        else:
            raise ValueError(f"loading of {form} is not supported.")

    def _extract_str(self, s):
        return s.decode('utf-8')

    def _str2int(self, s):
        return int.from_bytes(s, byteorder='little')

    def _extract_value(self, offset, count):
        return self._str2int(self.data[offset:offset+count])

    def _extract_u32(self, offset):
        return self._extract_value(offset, 4)

    def _extract_u16(self, offset):
        return self._extract_value(offset, 2)

    def _extract_u8(self, offset):
        return self._extract_value(offset, 1)

    def _get_signature_offset(self, signature):
        return self.data.find(signature.encode('latin-1'))

    def get_rom_hdr(self):
        offset = self._get_signature_offset("\x7f-ROM_IMG-\xf7")
        if (offset == -1):
            return None
        
        boot_info_ptr = self._extract_u32(offset + 16)
        version_info_ptr = self._extract_u32(offset + 20)
        product_info_ptr = self._extract_u32(offset + 24)
        dfu_info_ptr = self._extract_u32(offset + 28)
        parameters_info_ptr = self._extract_u32(offset + 32);
        return {"boot_info_ptr": hex(boot_info_ptr),
                "version_info_ptr": hex(version_info_ptr),
                "product_info_ptr": hex(product_info_ptr),
                "dfu_info_ptr": hex(dfu_info_ptr),
                "parameters_info_ptr": hex(parameters_info_ptr)}

    def get_boot_info(self):
        offset = self._get_signature_offset("\x7f-BOOTINFO-\xf7")
        if (offset == -1):
            return None

        load_addr = self._extract_u32(offset + 16)
        initial_sp = self._extract_u32(offset + 20);
        entry = self._extract_u32(offset + 24)
        rom_size = self._extract_u32(offset + 28)
        return {"load_addr": hex(load_addr),
                "rom_size": hex(rom_size),
                "initial_sp_addr": hex(initial_sp),
                "entry_addr": hex(entry)}

    def get_version_info(self):
        offset = self._get_signature_offset("\x7f-VERINFO-\xf7")
        if (offset == -1):
            return None

        major = self._extract_u8(offset + 16)
        minor = self._extract_u8(offset + 17)
        patch = self._extract_u8(offset + 18)
        build = self._extract_str(self.data[offset+19:offset+19+16])
        return {"major": major,
                "minor": minor,
                "patch": patch,
                "build_date": build}

    def get_product_info(self):
        offset = self._get_signature_offset("\x7f-PRODINFO-\xf7")
        if (offset == -1):
            return None

        mfg_id_len = self._extract_u8(offset + 79)
        mfg = [f"{m:02x}" for m in self.data[offset + 16: offset + 16 + mfg_id_len]]
        mdl_id_len = self._extract_u8(offset + 143)
        mdl = [f"{m:02x}" for m in self.data[offset + 80: offset + 80 + mdl_id_len]]

        mdl_code = self._extract_str(self.data[offset+144:offset+144+64])
        mdl_name = self._extract_str(self.data[offset+208:offset+208+64])

        return {"manufacturer_id": mfg,
                "model_id": mdl,
                "model_code": mdl_code,
                "model_name": mdl_name}

    def get_dfu_info(self):
        offset = self._get_signature_offset("\x7f-DFUINFO-\xf7")
        if (offset == -1):
            return None
        addr = self._extract_u32(offset + 16)
        pattern = self._extract_u32(offset + 20)
        return {"addr": hex(addr),
                "pattern": hex(pattern)}

    def get_parameters_info(self):
        offset = self._get_signature_offset("\x7f-PARAMINFO-\xf7")
        if (offset == -1):
            return None

        addr = self._extract_u32(offset + 16)
        addr = self._extract_u32(offset + 20)
        return {"cmdline_addr": 0,
                "cmdline_length": 0}


class dfusyx:
    def __init__(self):
        pass
