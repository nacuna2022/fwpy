#!/usr/bin/python3
from fwpy import firmware
import argparse

def report_rom_hdr(fw):
    rom_hdr = fw.get_rom_hdr()
    if (rom_hdr is not None):
        print("ROM Header:")
        print(f"\tboot info ptr: {rom_hdr['boot_info_ptr']}")
        print(f"\tversion info ptr: {rom_hdr['version_info_ptr']}")
        print(f"\tproduct info ptr: {rom_hdr['product_info_ptr']}")
        print(f"\tdfu info ptr: {rom_hdr['dfu_info_ptr']}")
        print(f"\tparameters info ptr: {rom_hdr['parameters_info_ptr']}")
    else:
        print("no rom header found!")

def report_boot_info(fw):
    boot_info = fw.get_boot_info()
    if (boot_info is not None):
        print("BOOT INFO:")
        print(f"\tload address: {boot_info['load_addr']}")
        print(f"\tinitial sp address: {boot_info['initial_sp_addr']}")
        print(f"\tentry point: {boot_info['entry_addr']}")
        print(f"\trom size: {boot_info['rom_size']}")
    else:
        print("no boot info found!")

def report_version_info(fw):
    version_info = fw.get_version_info()
    if (version_info is not None):
        v = version_info
        version = f"{v['major']}.{v['minor']}.{v['patch']}"
        print(f"VERSION INFO:")
        print(f"\tversion: {version}")
        print(f"\tbuild date: {v['build_date']}")
    else:
        print("no version info found!")

def report_prod_info(fw):
    prod_info = fw.get_product_info()
    if (prod_info is not None):
        print(f"PRODUCT INFO:")
        print(f"\tmanufacturer id : {prod_info['manufacturer_id']}")
        print(f"\tmodel id: {prod_info['model_id']}")
        print(f"\tmodel code: {prod_info['model_code']}")
        print(f"\tmodel name: {prod_info['model_name']}")

def report_dfu_info(fw):
    dfu_info = fw.get_dfu_info()
    if (dfu_info is not None):
        print(f"DFU INFO:")
        print(f"\tload address: {dfu_info['addr']}")
        print(f"\tpattern: {dfu_info['pattern']}")
    else:
        print("no dfu info found!")

def report_params_info(fw):
    params_info = fw.get_parameters_info()
    if (params_info is not None):
        print(f"PARAMS INFO:")
        print(f"\tcommandline address: {params_info['cmdline_addr']}")
        print(f"\tcommandline buffer size: {params_info['cmdline_length']}")
    else:
        print("no params info found!")

def report_all(fw):
    report_rom_hdr(fw)
    report_boot_info(fw)
    report_version_info(fw)
    report_prod_info(fw)
    report_dfu_info(fw)
    report_params_info(fw)


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Print information about a Musictribe hex or bin file")
    parser.add_argument("input_file")
    parser.add_argument("-t",
                        default="hex",
                        choices=["hex", "bin"],
                        help="define whether input_file is hex or bin (default: hex)")
    parser.add_argument("-r",
                        default="all",
                        choices=["all", "rom", "boot", "version", "product", "dfu", "params"],
                        help="report information (default: all)")
        
    args = parser.parse_args()
    fw = firmware()
    if (args.t == "hex"):
        fw.loadhex(args.input_file)
    else:
        fw.loadbin(args.input_file)

    if (args.r == "all"):
        report_all(fw)
    elif (args.r == "rom"):
        report_rom_hdr(fw)
    elif (args.r == "boot"):
        report_boot_info(fw)
    elif (args.r == "version"):
        report_version_info(fw)
    elif (args.r == "dfu"):
        report_dfu_info(fw)
    elif (args.r == "params"):
        report_params_info(fw)
    else:
        print(f"report type {args.r} is not yet supported")













    


