#ifndef _DINGOO_H_
#define _DINGOO_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct DMADescriptor
{
	uint32_t dcmd;	/* DCMD value for the current transfer */
	uint32_t dsadr;	/* DSAR value for the current transfer */
	uint32_t dtadr;	/* DTAR value for the current transfer */
	uint32_t ddadr;	/* Points to the next descriptor + transfer count */
} DMADescriptor;

#define DMADescriptorAlignAttribute __attribute__ ((aligned (16)))

typedef struct WaveOpen
{
	uint32_t frequency;
	uint16_t bits;
	uint8_t channels;
	uint8_t volume;
} WaveOpen;

typedef struct KeyStatus
{
	uint32_t pressed;
	uint32_t released;
	uint32_t status;
} KeyStatus;

typedef uint32_t OS_CPU_SR;

typedef struct OS_EVENT
{
	uint8_t OSEventType; /* Type of event control block (see OS_EVENT_TYPE_xxxx)    */
	void *OSEventPtr; /* Pointer to message or queue structure                   */
	uint16_t OSEventCnt; /* Semaphore Count (not used if other EVENT type)          */
	// ....
} OS_EVENT;

// Should be moved
#ifndef FILE
typedef unsigned long FILE;
#endif

//Custom_Memsic_test
//GUI_Exec
//GUI_Lock
//GUI_TIMER_Create
//GUI_TIMER_Delete
//GUI_TIMER_Restart
//GUI_TIMER_SetPeriod
//GUI_Unlock
unsigned long GetTickCount();
//Get_X
//Get_Y
//HP_Mute_sw
//LCD_Color2Index
//LCD_GetXSize
//LCD_GetYSize
//LcdGetDisMode // not implemented
//Memsic_SerialCommInit
void OSCPURestoreSR(OS_CPU_SR cpu_sr);
OS_CPU_SR OSCPUSaveSR();
//OS_FLAGS OSFlagPost (OS_FLAG_GRP *pgrp,OS_FLAGS flags,uint8_t opt,uint8_t *err);
OS_EVENT *OSQCreate(void **start,uint16_t size);
OS_EVENT *OSSemCreate(uint16_t cnt);
OS_EVENT *OSSemDel(OS_EVENT *event, uint8_t option, uint8_t* error);
void OSSemPend(OS_EVENT *event,uint16_t timeout,uint8_t *error);
uint8_t OSSemPost(OS_EVENT *event);
uint8_t OSTaskCreate(void (*task)(),void *data,uint16_t stack_size,uint8_t priority);
uint8_t OSTaskDel(uint8_t priority);
void OSTimeDly(uint16_t ticks);
uint32_t OSTimeGet();
//Read_Acc
//Read_Acc0
//StartSwTimer
//SysDisableBkLight
//SysDisableCloseBkLight
//SysEnableShutDownPower
//TaskMediaFunStop
//U8TOU16
//U8TOU32
//USB_Connect
//USB_No_Connect
//WM_CreateWindow
//WM_DefaultProc
//WM_DeleteWindow
//WM_SelectWindow
//WM_SetFocus
//WM__SendMessage
void __dcache_writeback_all();
void __icache_invalidate_all();
char *__to_locale_ansi(wchar_t *);
//__to_unicode_le
uint32_t _kbd_get_key();
void _kbd_get_status(KeyStatus *);
void *_lcd_get_frame();
void _lcd_set_frame();
int _sys_judge_event();
//_tcscmp
//_tcscpy
void *_waveout_open(void *);
//_waveout_set_volume
//int _waveout_close(void *); // ???
//void _waveout_close_at_once(unsigned int); // ???
void abort();
//ap_lcd_set_frame
//av_begin_thread
//av_create_flag
//av_create_sem
//av_delay
//av_destroy_flag
//av_destroy_sem
//av_end_thread
//av_give_flag
//av_give_sem
//av_queue_abort
//av_queue_end
//av_queue_flush
//av_queue_get
//av_queue_init
//av_queue_put
//av_reg_object
//av_resize_packet
//av_uft8_2_unicode
//av_unreg_object
//av_upper_4cc
//av_wait_flag
//av_wait_sem
//av_wait_sem2
//cmGetSysModel
//cmGetSysVersion
//delay_ms
//detect_clock
//dl_free
//dl_get_proc
//dl_load
//dl_res_close
//dl_res_get_data
//dl_res_get_size
//dl_res_open
int fprintf(FILE *,const char *,...);
size_t fread(void *,size_t,size_t,FILE *);
void free(void *);
void free_irq(unsigned int irq);
int fseek(FILE *,long int,int);
//fsys_RefreshCache
//fsys_clearerr
int fsys_fclose(FILE *);
//fsys_fcloseW
//fsys_fclose_flash
int fsys_feof(FILE *);
int fsys_ferror(FILE *);
//fsys_findclose
//fsys_findfirst
//fsys_findnext
//fsys_flush_cache
FILE *fsys_fopen(const char *,const char *);
FILE *fsys_fopenW(const uint16_t *,const uint16_t *);
//fsys_fopen_flash
int fsys_fread(void *,size_t,size_t,FILE *);
int fsys_fseek(FILE *,long int,int);
long int fsys_ftell(FILE *);
int fsys_fwrite(const void *,size_t,size_t,FILE *);
//fsys_mkdir
//fsys_remove
//fsys_removeW
//fsys_rename
//fsys_renameW
size_t fwrite(const void *,size_t,size_t,FILE *);
//get_current_language
//get_dl_handle
//get_game_vol
//isTVON
//jz_pm_pllconvert
uint32_t kbd_get_key();
void kbd_get_status(KeyStatus *);
//lcd_flip
//lcd_get_bpp
void *lcd_get_cframe();
void *lcd_get_frame();
void lcd_set_frame();
void *malloc(size_t);
//mdelay
//memcpy
//memset
//open_gui_key_msg
int pcm_can_write();
int pcm_ioctl(unsigned int cmd, unsigned long arg);
int printf(const char *,...);
void *realloc(void *,size_t);
int serial_getc();
void serial_putc(const char c);
//serial_puts
//spin_lock_irqsave
//spin_unlock_irqrestore
int sprintf(char *,const char *,...);
//sscanf
//strncasecmp
//sys_get_ccpmp_config
//sys_judge_event
//tv_close
//tv_disable_switch
//tv_enable_switch
//tv_get_closeflag
//tv_get_openflag
//tv_open
//tv_set_closeflag
//tv_set_openflag
//udc_attached
//udelay
//vsprintf
//vxGoHome
int waveout_can_write();
extern int waveout_close(void *);
//waveout_close_at_once
void *waveout_open(WaveOpen *woinfo);
//waveout_set_volume
int waveout_write(int dummy,char *buffer,int count);



#define CONTROL_BUTTON_A 0x80000000
#define CONTROL_BUTTON_B 0x00200000
#define CONTROL_BUTTON_X 0x00010000
#define CONTROL_BUTTON_Y 0x00000040
#define CONTROL_BUTTON_START 0x00000800
#define CONTROL_BUTTON_SELECT 0x00000400

#define CONTROL_TRIGGER_LEFT 0x00000100
#define CONTROL_TRIGGER_RIGHT 0x20000000

#define CONTROL_DPAD_UP 0x00100000
#define CONTROL_DPAD_DOWN 0x08000000
#define CONTROL_DPAD_LEFT 0x10000000
#define CONTROL_DPAD_RIGHT 0x00040000

#define CONTROL_POWER 0x00000080

// PCM ioctl command
#define PCM_SET_SAMPLE_RATE	0
#define PCM_SET_CHANNEL	 1
#define PCM_SET_FORMAT	 2
#define PCM_SET_VOL	 3
#define PCM_GET_VOL	 4

// Audio Sample Format
#define	AFMT_U8	 8
#define AFMT_S16_LE	 16



/*
pcm_ioctl(PCM_SET_SAMPLE_RATE, 44100); //48000,44100
pcm_ioctl(PCM_SET_FORMAT, AFMT_S16_LE);
pcm_ioctl(PCM_SET_CHANNEL, 1);
pcm_ioctl(PCM_SET_VOL, 100);	// 100%  

int pcm_write(char *buffer, int count)
*/

/*
// put some raw pcm wave data here
unsigned char mySound_16kHz_16bits_mono_LE[] = {...};
   mywaveinfo.frequency = 16000;
   mywaveinfo.bits = 16;
   mywaveinfo.channels = 1;
   mywaveinfo.volume = 100;

   waveout_open(&mywaveinfo);

   waveout_write(0, mySound_16kHz_16bits_mono_LE, sizeof(mySound_16kHz_16bits_mono_LE));
*/

#endif
