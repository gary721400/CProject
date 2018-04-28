//------------------------------------------------------------------------------
	#define sdtl_mount		sd1_mount
	#define sdtl_resv_size		sd1_res_size
	#define sdtl_get_size		sd1_fat_size
	#define sdtl_resv_read		sd1_res_read
	#define sdtl_resv_write		sd1_res_write
	#define sdtl_read		sd1_read
	#define sdtl_write		sd1_write
	#define sdtl_cache_init		sd1_cache2k_init
	#define sdtl_cache_read		sd1_cache_read
	#define sdtl_cache_write	sd1_cache_write
	#define sdtl_cache_flush	sd1_cache_flush
	#define sdtl_allocate		sd1_allocate
	#define SD_Drive		SD1_Drive
//------
	__asm{ .define _sdtl_mount	 _sd1_mount        }
	__asm{ .define _sdtl_resv_size	 _sd1_res_size     }
	__asm{ .define _sdtl_get_size	 _sd1_fat_size     }
	__asm{ .define _sdtl_resv_read	 _sd1_res_read     }
	__asm{ .define _sdtl_resv_write	 _sd1_res_write    }
	__asm{ .define _sdtl_read	 _sd1_read         }
	__asm{ .define _sdtl_write	 _sd1_write        }
	__asm{ .define _sdtl_cache_init	 _sd1_cache2k_init }
	__asm{ .define _sdtl_cache_read	 _sd1_cache_read   }
	__asm{ .define _sdtl_cache_write _sd1_cache_write  }
	__asm{ .define _sdtl_cache_flush _sd1_cache_flush  }
	__asm{ .define _sdtl_allocate    _sd1_allocate     }
//------
	#define spif_res_size		spif_resv_size
	#define spif_res_read		spif_resv_read
	#define spif_res_write		spif_resv_write
//------
	__asm{ .define _spif_res_size	 _spif_resv_size   }
	__asm{ .define _spif_res_read	 _spif_resv_read   }
	__asm{ .define _spif_res_write	 _spif_resv_write  }
//------------------------------------------------------------------------------
