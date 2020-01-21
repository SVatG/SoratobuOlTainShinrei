#!/usr/bin/python
import struct, sys, os

SYMS = ['ap_lcd_set_frame', 'lcd_set_frame', 'lcd_get_frame', 'delay_ms', 'lcd_get_bpp', 'lcd_get_cframe', 'lcd_flip', 'kbd_get_key', 'kbd_get_status',\
        'open_gui_key_msg', 'tv_get_openflag', 'tv_set_openflag', 'tv_get_closeflag', 'tv_set_closeflag', 'tv_disable_switch', 'tv_enable_switch', 'Read_Acc0',\
        'Memsic_SerialCommInit', 'Read_Acc', 'Custom_Memsic_test', 'Get_X', 'Get_Y', 'sys_judge_event', 'SysDisableBkLight', 'SysEnableShutDownPower',\
        'SysDisableCloseBkLight', '_lcd_get_frame', '_lcd_set_frame', '_sys_judge_event', '_kbd_get_status', '_kbd_get_key', '_waveout_open',\
        '_waveout_set_volume', 'jz_pm_pllconvert', 'strncasecmp', 'sys_get_ccpmp_config', 'vxGoHome', 'cmGetSysModel', 'cmGetSysVersion', 'fsys_fopen_flash',\
        'fsys_fclose_flash', 'get_dl_handle', 'get_game_vol', 'get_current_language', 'fsys_fopen', 'fsys_fclose', 'fsys_fread', 'fsys_remove', 'fsys_fwrite',\
        'fsys_fseek', 'fsys_ftell', 'fsys_feof', 'fsys_ferror', 'fsys_clearerr', 'fsys_findfirst', 'fsys_findnext', 'fsys_findclose', 'fsys_mkdir',\
        'fsys_rename', 'fsys_flush_cache', 'fsys_RefreshCache', 'fsys_fopenW', 'fsys_fcloseW', 'fsys_removeW', 'fsys_renameW', 'USB_Connect', 'USB_No_Connect',\
        'tv_open', 'tv_close', 'isTVON', 'pcm_ioctl', 'mdelay', 'HP_Mute_sw', 'pcm_can_write', 'waveout_open', 'waveout_close_at_once', 'waveout_write',\
        'waveout_close', 'waveout_can_write', 'waveout_set_volume', 'av_reg_object', 'av_unreg_object', 'av_queue_get', 'av_uft8_2_unicode',\
        'av_resize_packet', 'av_upper_4cc', 'av_begin_thread', 'av_end_thread', 'av_create_sem', 'av_wait_sem', 'av_wait_sem2', 'av_give_sem',\
        'av_destroy_sem', 'av_create_flag', 'av_wait_flag', 'av_give_flag', 'av_destroy_flag', 'av_delay', 'av_queue_init', 'av_queue_flush', 'av_queue_abort',\
        'av_queue_end', 'av_queue_put', 'dl_load', 'dl_free', 'dl_res_open', 'dl_res_get_size', 'dl_res_get_data', 'dl_res_close', 'dl_get_proc', 'memcpy',\
        'memset', 'abort', 'fprintf', 'fread', 'free', 'fseek', 'fwrite', 'malloc', 'printf', 'realloc', 'sprintf', 'sscanf', 'vsprintf', 'GUI_Exec',\
        'GUI_Lock', 'GUI_Unlock', 'GUI_TIMER_Create', 'GUI_TIMER_Delete', 'GUI_TIMER_SetPeriod', 'GUI_TIMER_Restart', 'LCD_Color2Index', 'LCD_GetXSize',\
        'LCD_GetYSize', 'WM_CreateWindow', 'WM_DeleteWindow', 'WM_SelectWindow', 'WM_DefaultProc', 'WM__SendMessage', 'WM_SetFocus', 'U8TOU32',\
        '__icache_invalidate_all', '__dcache_writeback_all', 'LcdGetDisMode', 'free_irq', 'spin_lock_irqsave', 'spin_unlock_irqrestore', 'detect_clock',\
        'udelay', 'serial_putc', 'serial_puts', 'serial_getc', 'TaskMediaFunStop', 'StartSwTimer', 'GetTickCount', 'OSCPUSaveSR', 'OSCPURestoreSR',\
        'OSFlagPost', 'OSQCreate', 'OSSemDel', 'OSSemPend', 'OSSemPost', 'OSSemCreate', 'OSTaskCreate', 'OSTaskDel', 'OSTimeDly', 'OSTimeGet', 'U8TOU16',\
        '_tcscmp', '_tcscpy', '__to_unicode_le', '__to_locale_ansi', 'udc_attached']

def CreateCCDL():
    CCDL = ""
    CCDL += struct.pack("<L", 0x4C444343)
    CCDL += struct.pack("<L", 0x00010000)
    CCDL += struct.pack("<L", 0x00020001)
    CCDL += struct.pack("<L", 0x00000004)
    CCDL += struct.pack("<L", 0x00000000)
    CCDL += struct.pack("<L", 0x00000000)
    CCDL += struct.pack("<L", 0x00000000)
    CCDL += struct.pack("<L", 0x00000000)
    return CCDL

def CreateIMPT(Offset, Size):
    IMPT = ""
    IMPT += struct.pack("<L", 0x54504D49)
    IMPT += struct.pack("<L", 0x00000008)
    IMPT += struct.pack("<L", Offset)
    IMPT += struct.pack("<L", Size)
    IMPT += struct.pack("<L", 0x00000000)
    IMPT += struct.pack("<L", 0x00000000)
    IMPT += struct.pack("<L", 0x00000000)
    IMPT += struct.pack("<L", 0x00000000)
    return IMPT

def CreateEXPT(Offset, Size):
    EXPT = ""
    EXPT += struct.pack("<L", 0x54505845)
    EXPT += struct.pack("<L", 0x00000009)
    EXPT += struct.pack("<L", Offset)
    EXPT += struct.pack("<L", Size)
    EXPT += struct.pack("<L", 0x00000000)
    EXPT += struct.pack("<L", 0x00000000)
    EXPT += struct.pack("<L", 0x00000000)
    EXPT += struct.pack("<L", 0x00000000)
    return EXPT

def CreateRAWD(Offset, Size, dl_main, TotalSize):
    RAWD = ""
    RAWD += struct.pack("<L", 0x44574152)
    RAWD += struct.pack("<L", 0x00000001)
    RAWD += struct.pack("<L", Offset)
    RAWD += struct.pack("<L", Size)
    RAWD += struct.pack("<L", 0x00000000)
    RAWD += struct.pack("<L", dl_main)
    RAWD += struct.pack("<L", 0x80A00000)
    RAWD += struct.pack("<L", TotalSize)
    return RAWD

if len(sys.argv) < 2:
   print "USAGE: elf2app.py basename [resource_file]"
   sys.exit(0)
os.system("mipsel-elf-objcopy -O binary -j .dingoo %s.elf %s.bin" % (sys.argv[1], sys.argv[1]))
F = open("%s.bin" % (sys.argv[1]), "rb")
RawData = F.read()
F.close()
while((len(RawData) % 16) != 0):
     RawData += chr(0)
F = os.popen("mipsel-elf-objdump --adjust-vma=0x80a00000 -t %s.elf" % (sys.argv[1]))
Syms = F.readlines()
F.close()
F = os.popen("mipsel-elf-objdump --adjust-vma=0x80a00000 -h %s.elf" % (sys.argv[1]))
Sections = F.readlines()
F.close()
BssSize = 0x00000000
for s in Sections:
    if s[4:8] == ".bss":
       BssSize = int("0x"+(s.split()[2]), 16)
idx = 0
while(Syms[idx][0:13] != "SYMBOL TABLE:"):
     idx += 1
Syms = Syms[idx+1:]
while(Syms[-1] == ""):
     Syms = Syms[0:-1]
ImportedSyms = []
AppMain = 0x00000000
GetExt = 0x00000000
DlMain = 0x00000000
for s in Syms:
    s = s.strip()
    S = s.split()
    if len(S) < 6:
       continue
    if S[5] in SYMS:
       ImportedSyms.append((S[5],int("0x"+S[0],16)))
    if S[5] == "AppMain":
       AppMain = int("0x"+S[0],16)
    if S[5] == "getext":
       GetExt = int("0x"+S[0],16)
    if S[5] == "dl_main":
       DlMain = int("0x"+S[0],16) 
ImportTable = struct.pack("<LLLL", len(ImportedSyms), 0, 0, 0)
ImportStrings = ""
for s in ImportedSyms:
    ImportTable += struct.pack("<LLLL", len(ImportStrings), 0, 0x00020000, s[1])
    ImportStrings += s[0] + chr(0)
    while((len(ImportStrings) % 4) != 0):
         ImportStrings += chr(0)
ImportData = ImportTable + ImportStrings
while((len(ImportData) % 16) != 0):
     ImportData += chr(0)
ExportTable = struct.pack("<LLLL", 2, 0, 0, 0)
ExportStrings = ""
ExportTable += struct.pack("<LLLL", len(ExportStrings), 0, 0x00020000, GetExt)
ExportStrings += "getext" + chr(0)
while((len(ExportStrings) % 4) != 0):
     ExportStrings += chr(0)
ExportTable += struct.pack("<LLLL", len(ExportStrings), 0, 0x00020000, AppMain)
ExportStrings += "AppMain" + chr(0)
while((len(ExportStrings) % 4) != 0):
     ExportStrings += chr(0)
ExportData = ExportTable + ExportStrings
while((len(ExportData) % 16) != 0):
     ExportData += chr(0)
Offset =  4* 32
F = open("%s.app" % (sys.argv[1]), "wb")
F.write(CreateCCDL())
F.write(CreateIMPT(Offset, len(ImportData)))
Offset += len(ImportData)
F.write(CreateEXPT(Offset, len(ExportData)))
Offset += len(ExportData)
F.write(CreateRAWD(Offset, len(RawData), DlMain, len(RawData)+BssSize))
Offset += len(RawData)
F.write(ImportData)
F.write(ExportData)
F.write(RawData)
if len(sys.argv) > 2:
   R = open(sys.argv[2], "rb")
   ResData = R.read()
   R.close()
   F.write(ResData)
F.close()

