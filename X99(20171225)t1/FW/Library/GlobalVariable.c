
unsigned int gflag @ 0x6800;
unsigned int vol_idx @ 0x6802;
unsigned int ap3_file_number @ 0x6804;
unsigned int ap4_mp3_flag @ 0x6806;
unsigned int bak_ap4_num @ 0x6808;
unsigned int bak_book_num @ 0x680a;
unsigned int bak_page_num @ 0x680c;
unsigned int bak_control_mode_num @ 0x680e;
unsigned int volume_inc_flag @ 0x6810;
unsigned int bak_current_index @ 0x6812;

//unsigned int begin_R @ 0x6814;
//unsigned int over_R @ 0x6816;
//unsigned int begin_P @ 0x6818;
//unsigned int over_P @ 0x681a;
unsigned int systerm_flag @ 0x6814;
unsigned int g_string2[30] @ 0x681c;  //0x681c--6858
unsigned int remote_index_tab[32] @ 0x685A; //0x685A---0x687A
