#ifndef  FALSE
#define  FALSE   0
#endif

#ifndef  TRUE
#define  TRUE    1
#endif

#define SD_Resv_Start_Sector 130UL
#define BOOT_CODE_START_SECTOR	2UL

#define CACHE_SIZE  2048		// 
#define CACHE_SECTOR 4			// CACHE_SIZE/512, this should be 2^n
#define CACHE_USED_MASK  3			// == CACHE_SECTOR-1
#define CACHE_SECTOR_MASK 0xFFFFFFFC	// ==  ~CACHE_USED_MASK
#define CACHE_BLOCK_MASK 0xFFFFFF00	// assume 128K per block

#define USB_CACHE_SIZE  4096		// 
#define USB_CACHE_SECTOR 8			// CACHE_SIZE/512, this should be 2^n
#define USB_CACHE_USED_MASK  7			// == CACHE_SECTOR-1
#define USB_CACHE_SECTOR_MASK 0xFFFFFFF8	// ==  ~CACHE_USED_MASK

#define USB_CACHE_SIZE8K  8192		// 
#define USB_CACHE_SECTOR8K 16			// CACHE_SIZE/512, this should be 2^n
#define USB_CACHE_USED_MASK8K  15			// == CACHE_SECTOR-1
#define USB_CACHE_SECTOR_MASK8K 0xFFFFFFF0	// ==  ~CACHE_USED_MASK

extern __X unsigned char SD_Catch[512];
extern unsigned int _is_sd_mounted;
extern unsigned long SDTL_FAT_StartSector;
extern unsigned long SDTL_resv_TotalSector;
extern unsigned long SDTL_FAT_TotalSector;

extern SDTL_ERR sdtl_boot_write(unsigned int sector, __X void* dst);
//extern SDTL_ERR sdtl_boot_read(unsigned int sector, __X void* dst);
extern void SD1_go_inactive(void);
extern void SD2_go_inactive(void);
extern int sdtl_unmount(void);

//********小工具程式*********************************************
extern void DWORD_TO_BYTES(__X unsigned char *p, unsigned long n);
extern unsigned long SDTL_ConvertDWORD(__X void *Dword);

extern void SetFlashBusy(void);
extern void ClearFlashBusy(void);

extern int  _cache_dirty, _cache_used, cache_first_read;
extern __X unsigned char *_cache;
extern unsigned long _cache_sector, cache_read_sector;
extern int cache_inited;
extern unsigned char sdtl_catch_buf[2048];
extern unsigned char sdtl_usb_buf[2048];

// for USB

extern int  usb_cache_dirty, usb_cache_used, usb_cache_first_read;
extern __X unsigned char *usb_cache;
extern unsigned long usb_cache_write_sector, usb_cache_read_sector;
extern int usb_cache_inited;

extern __X unsigned char sdtl_usb_read_buf[4096];

extern unsigned char sdtl_usb_writr_buf[4096];


//8K cache
extern unsigned int usb_cache8k_dirty, usb_cache8k_used, usb_cache8k_first_read;
extern __X unsigned char *usb_cache8k;
extern unsigned long usb_cache8k_write_sector, usb_cache8k_read_sector;
extern int usb_cache8k_inited;

extern __X unsigned char sdtl_usb_read_buf8k[8192];

extern unsigned char sdtl_usb_writr_buf8k[8192];



// for SD2
extern unsigned int _is_sd2_mounted;
extern unsigned long SDTL2_FAT_StartSector;
extern unsigned long SDTL2_resv_TotalSector;
extern unsigned long SDTL2_FAT_TotalSector;


