#ifndef _TAB_PICT_H_
#define _TAB_PICT_H_

#if 1
#define IDP_PLAYER_NORMAL                 0
#define IDP_PLAYER_SINGLE                 0
#define IDP_PLAYER_REPEAT_SINGLE          19
#define IDP_PLAYER_REPEAT_ALL             38
#define IDP_AP4_SEQUENCE                  57
#define IDP_AP4_SINGLE                    74
//#define IDP_PLAYER_VOLUME                 91 //IDP_VOLUME
#define IDP_PLAYER_VOLUME                 IDP_VOLUME
#define IDP_BACKGROUND      		 110
#define IDP_EQ_3D                        2193
#define IDP_EQ_BAND_CURSOR               2201
#define IDP_EQ_BAND_FRAME                2207
#define IDP_EQ_BASS                      2229
#define IDP_EQ_BASS_SELECT               2253
#define IDP_EQ_BBE                 	 2277
#define IDP_EQ_MEDIUM                    2290
#define IDP_EQ_MEDIUM_SELECT         	 2307
#define IDP_EQ_NOTE               	 2324
#define IDP_EQ_TREBLE             	 2335
#define IDP_EQ_TREBLE_SELECT      	 2359
#define IDP_EQ_WND                       2383
#define IDP_GAME_SNAKE_BACKGROUND        3733
#define IDP_GAME_SNAKE_SQUARE       4760
#define IDP_GAME_SNAKE_STONE        4767
#define IDP_HOST                    4774
#define IDP_HOST_HUB                4793
#define IDP_HOST_MS                 4812
#define IDP_PLAYER_FFW              4831
#define IDP_PLAYER_FRW              4841
#define IDP_PLAYER_MARK_A           4851
#define IDP_PLAYER_MARK_AB          4861
#define IDP_PLAYER_MARK_B           4871
#define IDP_PLAYER_MP3              4881
#define IDP_PLAYER_PAUSE            4916
#define IDP_PLAYER_PLAY             4926
#define IDP_PLAYER_RECORD           4936
#define IDP_PLAYER_SCROLL_AB        4946
#define IDP_PLAYER_STOP             4981
#define IDP_PLAYER_WAV              4991
#define IDP_PLAYER_WMA              5026
#define IDP_PLAYER_WND_ICON         5061
#define IDP_SCROLLER_AB             5112
#define IDP_SCROLLER_EQ             5147
#define IDP_SCROLLER_ADDTOPLAYLIST  5182
#define IDP_SCROLLER_COPY           5217
#define IDP_SCROLLER_CUT            5252
#define IDP_SCROLLER_DELETE         5287
#define IDP_SCROLLER_FORMAT         5322
#define IDP_SCROLLER_FRAME_A        5357
#define IDP_SCROLLER_HOME           5444
#define IDP_SCROLLER_ID3            5479
#define IDP_SCROLLER_LOAD           5514
#define IDP_SCROLLER_LYR            5549
#define IDP_SCROLLER_MODE           5584
#define IDP_SCROLLER_PASTE          5619
#define IDP_SCROLLER_PLAY           5654
#define IDP_SCROLLER_PLAYER         5689
#define IDP_SCROLLER_PROPERTIES     5724
#define IDP_SCROLLER_RENAME         5759
#define IDP_SCROLLER_ROOT           5794
#define IDP_SCROLLER_SAVE           5829
#define IDP_SCROLLER_SELECTPLAYLIST   5864
#define IDP_SCROLLER_SORT             5899
#define IDP_SCROLLER_SPEED            5934
#define IDP_STATUS_BATT_FULL          5969
#define IDP_STATUS_BATT_LOW           5986
#define IDP_STATUS_BATT_MED           6003
#define IDP_STATUS_BATT_V_LOW         6020
#define IDP_STATUS_LOCK            6037
#define IDP_STATUS_MMC             6048
#define IDP_STATUS_TAPE            6058
#define IDP_VOLUME_LADYBIRD        6075
#define IDP_VOLUME_WND             6086
#define IDP_CURSOR                 7132
#define IDP_EXP_DISK_DEFAULT       7145
#define IDP_EXP_DISK_NF            7164
#define IDP_EXP_DISK_SD            7183
#define IDP_EXP_FOLDER_C           7202
#define IDP_EXP_FOLDER_O           7221
#define IDP_EXP_MUSIC_FILE         7240
#define IDP_EXP_OTHER_FILE         7259
#define IDP_EXP_PICTURE_FILE       7278
#define IDP_EXP_TEXT_FILE          7297
#define IDP_EXP_UPDATE_FILE        7316
#define IDP_EXP_WND_ICON            7335
#define IDP_GAME_TETRIS_BACKGROUND     7368
#define IDP_GAME_TETRIS_CONTAINER      8395
#define IDP_GAME_TETRIS_DIGIT_0      8860
#define IDP_GAME_TETRIS_DIGIT_1      8866
#define IDP_GAME_TETRIS_DIGIT_2      8872
#define IDP_GAME_TETRIS_DIGIT_3      8878
#define IDP_GAME_TETRIS_DIGIT_4      8884
#define IDP_GAME_TETRIS_DIGIT_5      8890
#define IDP_GAME_TETRIS_DIGIT_6      8896
#define IDP_GAME_TETRIS_DIGIT_7      8902
#define IDP_GAME_TETRIS_DIGIT_8       8908
#define IDP_GAME_TETRIS_DIGIT_9       8914
#define IDP_GAME_TETRIS_GAMEOVER      8920
#define IDP_GAME_TETRIS_LOGO          9013
#define IDP_GAME_TETRIS_NEXT          9126
#define IDP_GAME_TETRIS_STONEDEL      9345
#define IDP_GAME_TETRIS_STONESET      9352
#define IDP_ID3_WND                   9359
#define IDP_IDLE_EXPLORER_A           10517
#define IDP_IDLE_EXPLORER_I           10616
#define IDP_IDLE_EXPLORER_NAME        10651
#define IDP_IDLE_GAME_A               10734
#define IDP_IDLE_GAME_I               10833
#define IDP_IDLE_GAME_NAME            10868
#define IDP_IDLE_INFO_A                10951
#define IDP_IDLE_INFO_I                11050
#define IDP_IDLE_INFO_NAME             11085
#define IDP_IDLE_MSTORAGE_A            11168
#define IDP_IDLE_MSTORAGE_I            11267
#define IDP_IDLE_MSTORAGE_NAME         11302
#define IDP_IDLE_NAME_FRAME_L          11385
#define IDP_IDLE_NAME_FRAME_R          11404
#define IDP_IDLE_PLAYER_A              11423
#define IDP_IDLE_PLAYER_I              11522
#define IDP_IDLE_PLAYER_NAME           11557
#define IDP_IDLE_PLAYLIST_A            11640
#define IDP_IDLE_PLAYLIST_I            11739
#define IDP_IDLE_PLAYLIST_NAME         11774
#define IDP_IDLE_RADIO_A               11857
#define IDP_IDLE_RADIO_I               11956
#define IDP_IDLE_RADIO_NAME            11991
#define IDP_IDLE_RECORDER_A            12074
#define IDP_IDLE_RECORDER_I            12173
#define IDP_IDLE_RECORDER_NAME         12208
#define IDP_IDLE_SETTING_A             12291
#define IDP_IDLE_SETTING_I             12390
#define IDP_IDLE_SETTING_NAME          12425
#define IDP_INFO_DOWN                  12508
#define IDP_INFO_DOWN_SELECT           12519
#define IDP_INFO_UP                 	12530
#define IDP_INFO_UP_SELECT          	12541
#define IDP_INFO_WND                 	12552
#define IDP_MSTORAGE_COM                13131
#define IDP_MSTORAGE_FOLDER             13166
#define IDP_MSTORAGE_LINK               13217
#define IDP_MSTORAGE_PC       13300
#define IDP_MSTORAGE_POINT     13375
#define IDP_MSTORAGE_READY      13386
#define IDP_MSTORAGE_WND_ICON    13421
#define IDP_PL_WND_ICON           13478
#define IDP_RADIO_DIGIT_0         13511
#define IDP_RADIO_DIGIT_1          13554
#define IDP_RADIO_DIGIT_2            13597
#define IDP_RADIO_DIGIT_3            13640
#define IDP_RADIO_DIGIT_4              13683
#define IDP_RADIO_DIGIT_5               13726
#define IDP_RADIO_DIGIT_6               13769
#define IDP_RADIO_FM                    13812
#define IDP_RADIO_DIGIT_7    13831
#define IDP_RADIO_DIGIT_8    13874
#define IDP_RADIO_DIGIT_9     13917
#define IDP_RADIO_DIGIT_POINT   13960
#define IDP_RADIO_MONO           13965
#define IDP_RADIO_STEREO          14000
#define IDP_RADIO_WND_ICON         14035
#define IDP_STARTUP_ATMEL          14086
#define IDP_STARTUP_COCK             14305
#define IDP_STARTUP_SND3             14820
#define IDP_HOST_CDC                   15607
#define IDP_HOST_HID                    15624
#define IDP_IDLE_CDC_A                   15643
#define IDP_IDLE_CDC_I       15742
#define IDP_IDLE_CDC_NAME     15777
#define IDP_IDLE_EBOOK_A       15860
#define IDP_IDLE_EBOOK_I       15959
#define IDP_IDLE_EBOOK_NAME     15994
#define IDP_USB_DEVICE_CDC        16077
#define IDP_USB_DEVICE_HID_KBD     16112
#define IDP_USB_DEVICE_HID_MOUSE    16147
#define IDP_BOOT                    16182
#define IDP_BATT_CHARG               30060

#define IDP_STATUS_BATT_CHARGE         30128
#define IDP_STATUS_BLUETOOTH			30145
#define IDP_STATUS_REAL_VOLUME			30162
#define IDP_STATUS_VER_VOLUME			30179
#define IDP_STATUS_NO_BLUETOOTH			30196
#define IDP_STATUS_WAIYAN_LOG			30213
#define IDP_STATUS_DINGYA_LOG			31456

#define IDP_VOLUME                 		32699
#define IDP_STATUS_NEWEAST_LOG			32720
#else
// ID of the Pictures stored in NF (IDP)
#define IDP_PLAYER_NORMAL                 0
#define IDP_PLAYER_SINGLE                 0
#define IDP_PLAYER_REPEAT_SINGLE          0x13//17
#define IDP_PLAYER_REPEAT_ALL             0x26//34

#define IDP_AP4_SEQUENCE                  0x39//0  + 51
#define IDP_AP4_SINGLE                    0x4A//19 + 51
#define IDP_PLAYER_VOLUME                 0x5B//38 + 51
#define IDP_BACKGROUND                    0x6E//51 + 51
#define IDP_EQ_3D                         0x891//1027 + 1056 + 51 + 51
#define IDP_EQ_BAND_CURSOR                0x899//1037 + 1056 + 51 + 51
#define IDP_EQ_BAND_FRAME                 0x89E//1043 + 1056 + 51 + 51
#define IDP_EQ_BASS                       0x8B5//1049 + 1056 + 51 + 51
#define IDP_EQ_BASS_SELECT                0x8CD//1069 + 1056 + 51 + 51
#define IDP_EQ_BBE                        0x8E5//1089 + 1056 + 51 + 51
#define IDP_EQ_MEDIUM                     0x8F2//1103 + 1056 + 51 + 51
#define IDP_EQ_MEDIUM_SELECT              0x903//1122 + 1056 + 51 + 51
#define IDP_EQ_NOTE                       0x914//1141 + 1056 + 51 + 51
#define IDP_EQ_TREBLE                     0x91F//1149 + 1056 + 51 + 51
#define IDP_EQ_TREBLE_SELECT              0x937//1169 + 1056 + 51 + 51
#define IDP_EQ_WND                        0x94E//1189 + 1056 + 51 + 51
#define IDP_GAME_SNAKE_BACKGROUND         0xBF2//2512 + 1056 + 51 + 51
#define IDP_GAME_SNAKE_SQUARE             0xE95//3539 + 1056 + 51 + 51
#define IDP_GAME_SNAKE_STONE              0x1298//3546 + 1056 + 51 + 51
#define IDP_HOST                          0x129F//3553 + 1056 + 51 + 51
#define IDP_HOST_HUB                      0x12A6//3571 + 1056 + 51 + 51
#define IDP_HOST_MS                       0x12B9//3589 + 1056 + 51 + 51
#define IDP_PLAYER_FFW                    0x12CC//3606 + 1056 + 51 + 51
#define IDP_PLAYER_FRW                    0x12DF//3616 + 1056 + 51 + 51
#define IDP_PLAYER_MARK_A                 0x12E9//3626 + 1056 + 51 + 51
#define IDP_PLAYER_MARK_AB                0x12F3//3636 + 1056 + 51 + 51
#define IDP_PLAYER_MARK_B                 0x12FD//3646 + 1056 + 51 + 51
#define IDP_PLAYER_MP3                    0x1307//3656 + 1056 + 51 + 51
#define IDP_PLAYER_PAUSE                  0x1311//3689 + 1056 + 51 + 51
#define IDP_PLAYER_PLAY                   0x1334//3699 + 1056 + 51 + 51
#define IDP_PLAYER_RECORD                 0x133E//3709 + 1056 + 51 + 51
#define IDP_PLAYER_SCROLL_AB              0x1348//3719 + 1056 + 51 + 51
#define IDP_PLAYER_STOP                   0x1352//3754 + 1056 + 51 + 51
#define IDP_PLAYER_WAV                    0x1375//3764 + 1056 + 51 + 51
#define IDP_PLAYER_WMA                    0x137F//3797 + 1056 + 51 + 51
#define IDP_PLAYER_WND_ICON               0x13A2//3830 + 1056 + 51 + 51
#define IDP_SCROLLER_AB                   0x13C5//3871 + 1056 + 51 + 51
#define IDP_SCROLLER_EQ                   0x13F8//3906 + 1056 + 51 + 51
#define IDP_SCROLLER_ADDTOPLAYLIST        0x141B//3941 + 1056 + 51 + 51
#define IDP_SCROLLER_COPY                 0x143E//3976 + 1056 + 51 + 51
#define IDP_SCROLLER_CUT                  0x1461//4011 + 1056 + 51 + 51
#define IDP_SCROLLER_DELETE               0x1484//4046 + 1056 + 51 + 51
#define IDP_SCROLLER_FORMAT               0x14A7//4081 + 1056 + 51 + 51
#define IDP_SCROLLER_FRAME_A              0x14CA//4116 + 1056 + 51 + 51
#define IDP_SCROLLER_HOME                 0x14ED//4199 + 1056 + 51 + 51
#define IDP_SCROLLER_ID3                  0x1544//4234 + 1056 + 51 + 51
#define IDP_SCROLLER_LOAD                 0x1567//4269 + 1056 + 51 + 51
#define IDP_SCROLLER_LYR                  0x158A//4304 + 1056 + 51 + 51
#define IDP_SCROLLER_MODE                 0x15AD//4339 + 1056 + 51 + 51
#define IDP_SCROLLER_PASTE                0x15D0//4374 + 1056 + 51 + 51
#define IDP_SCROLLER_PLAY                 0x15F3//4409 + 1056 + 51 + 51
#define IDP_SCROLLER_PLAYER               0x1616//4444 + 1056 + 51 + 51
#define IDP_SCROLLER_PROPERTIES           0x1639//4479 + 1056 + 51 + 51
#define IDP_SCROLLER_RENAME               0x165C//4514 + 1056 + 51 + 51
#define IDP_SCROLLER_ROOT                 0x167F//4549 + 1056 + 51 + 51
#define IDP_SCROLLER_SAVE                 0x16A2//4584 + 1056 + 51 + 51
#define IDP_SCROLLER_SELECTPLAYLIST       0x16C5//4619 + 1056 + 51 + 51
#define IDP_SCROLLER_SORT                 0x16E8//4654 + 1056 + 51 + 51
#define IDP_SCROLLER_SPEED                0x170B//4689 + 1056 + 51 + 51
#define IDP_STATUS_BATT_FULL              0x1751//4724 + 1056 + 51 + 51
#define IDP_STATUS_BATT_LOW               0x1762//4740 + 1056 + 51 + 51
#define IDP_STATUS_BATT_MED               0x1773//4756 + 1056 + 51 + 51
#define IDP_STATUS_BATT_V_LOW             0x1784//4772 + 1056 + 51 + 51
#define IDP_STATUS_LOCK                   0x1784//4788 + 1056 + 51 + 51
#define IDP_STATUS_MMC                    0x1795//4798 + 1056 + 51 + 51
#define IDP_STATUS_TAPE                   0x17A0//4807 + 1056 + 51 + 51
#define IDP_VOLUME_LADYBIRD               0x17AA//4822 + 1056 + 51 + 51
#define IDP_VOLUME_WND                    0x17BB//4830 + 1056 + 51 + 51
#define IDP_CURSOR                        5803 + 1056 + 51 + 51
#define IDP_EXP_DISK_DEFAULT              5816 + 1056 + 51 + 51
#define IDP_EXP_DISK_NF                   5829 + 1056 + 51 + 51
#define IDP_EXP_DISK_SD                   5841 + 1056 + 51 + 51
#define IDP_EXP_FOLDER_C                  5853 + 1056 + 51 + 51
#define IDP_EXP_FOLDER_O                  5866 + 1056 + 51 + 51
#define IDP_EXP_MUSIC_FILE                5880 + 1056 + 51 + 51
#define IDP_EXP_OTHER_FILE                5892 + 1056 + 51 + 51
#define IDP_EXP_PICTURE_FILE              5904 + 1056 + 51 + 51
#define IDP_EXP_TEXT_FILE                 5916 + 1056 + 51 + 51
#define IDP_EXP_UPDATE_FILE               5928 + 1056 + 51 + 51
#define IDP_EXP_WND_ICON                  5940 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_BACKGROUND        5973 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_CONTAINER         7000 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_DIGIT_0           7513 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_DIGIT_1           7521 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_DIGIT_2           7529 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_DIGIT_3           7537 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_DIGIT_4           7545 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_DIGIT_5           7553 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_DIGIT_6           7561 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_DIGIT_7           7569 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_DIGIT_8           7577 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_DIGIT_9           7585 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_GAMEOVER          7593 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_LOGO              7676 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_NEXT              7849 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_STONEDEL          8027 + 1056 + 51 + 51
#define IDP_GAME_TETRIS_STONESET          8034 + 1056 + 51 + 51
#define IDP_ID3_WND                       8041 + 1056 + 51 + 51
#define IDP_IDLE_EXPLORER_A               9160 + 1056 + 51 + 51
#define IDP_IDLE_EXPLORER_I               9259 + 1056 + 51 + 51
#define IDP_IDLE_EXPLORER_NAME            9294 + 1056 + 51 + 51
#define IDP_IDLE_GAME_A                   9363 + 1056 + 51 + 51
#define IDP_IDLE_GAME_I                   9462 + 1056 + 51 + 51
#define IDP_IDLE_GAME_NAME                9497 + 1056 + 51 + 51
#define IDP_IDLE_INFO_A                   9566 + 1056 + 51 + 51
#define IDP_IDLE_INFO_I                   9665 + 1056 + 51 + 51
#define IDP_IDLE_INFO_NAME                9700 + 1056 + 51 + 51
#define IDP_IDLE_MSTORAGE_A               9769 + 1056 + 51 + 51
#define IDP_IDLE_MSTORAGE_I               9868 + 1056 + 51 + 51
#define IDP_IDLE_MSTORAGE_NAME            9903 + 1056 + 51 + 51
#define IDP_IDLE_NAME_FRAME_L             9972 + 1056 + 51 + 51
#define IDP_IDLE_NAME_FRAME_R             9985 + 1056 + 51 + 51
#define IDP_IDLE_PLAYER_A                 9998 + 1056 + 51 + 51
#define IDP_IDLE_PLAYER_I                 10097 + 1056 + 51 + 51
#define IDP_IDLE_PLAYER_NAME              10132 + 1056 + 51 + 51
#define IDP_IDLE_PLAYLIST_A               10201 + 1056 + 51 + 51
#define IDP_IDLE_PLAYLIST_I               10300 + 1056 + 51 + 51
#define IDP_IDLE_PLAYLIST_NAME            10335 + 1056 + 51 + 51
#define IDP_IDLE_RADIO_A                  10404 + 1056 + 51 + 51
#define IDP_IDLE_RADIO_I                  10503 + 1056 + 51 + 51
#define IDP_IDLE_RADIO_NAME               10538 + 1056 + 51 + 51
#define IDP_IDLE_RECORDER_A               10607 + 1056 + 51 + 51
#define IDP_IDLE_RECORDER_I               10706 + 1056 + 51 + 51
#define IDP_IDLE_RECORDER_NAME            10741 + 1056 + 51 + 51
#define IDP_IDLE_SETTING_A                10810 + 1056 + 51 + 51
#define IDP_IDLE_SETTING_I                10909 + 1056 + 51 + 51
#define IDP_IDLE_SETTING_NAME             10944 + 1056 + 51 + 51
#define IDP_INFO_DOWN                     11013 + 1056 + 51 + 51
#define IDP_INFO_DOWN_SELECT              11021 + 1056 + 51 + 51
#define IDP_INFO_UP                       11029 + 1056 + 51 + 51
#define IDP_INFO_UP_SELECT                11037 + 1056 + 51 + 51
#define IDP_INFO_WND                      11045 + 1056 + 51 + 51
#define IDP_MSTORAGE_COM                  11606 + 1056 + 51 + 51
#define IDP_MSTORAGE_FOLDER               11638 + 1056 + 51 + 51
#define IDP_MSTORAGE_LINK                 11677 + 1056 + 51 + 51
#define IDP_MSTORAGE_PC                   11750 + 1056 + 51 + 51
#define IDP_MSTORAGE_POINT                11813 + 1056 + 51 + 51
#define IDP_MSTORAGE_READY                11821 + 1056 + 51 + 51
#define IDP_MSTORAGE_WND_ICON             11853 + 1056 + 51 + 51
#define IDP_PL_WND_ICON                   11919 + 1056 + 51 + 51
#define IDP_RADIO_DIGIT_0                 11952 + 1056 + 51 + 51
#define IDP_RADIO_DIGIT_1                 11997 + 1056 + 51 + 51
#define IDP_RADIO_DIGIT_2                 12042 + 1056 + 51 + 51
#define IDP_RADIO_DIGIT_3                 12087 + 1056 + 51 + 51
#define IDP_RADIO_DIGIT_4                 12132 + 1056 + 51 + 51
#define IDP_RADIO_DIGIT_5                 12177 + 1056 + 51 + 51
#define IDP_RADIO_DIGIT_6                 12222 + 1056 + 51 + 51
#define IDP_RADIO_FM                      12267 + 1056 + 51 + 51
#define IDP_RADIO_DIGIT_7                 12279 + 1056 + 51 + 51
#define IDP_RADIO_DIGIT_8                 12324 + 1056 + 51 + 51
#define IDP_RADIO_DIGIT_9                 12369 + 1056 + 51 + 51
#define IDP_RADIO_DIGIT_POINT             12414 + 1056 + 51 + 51
#define IDP_RADIO_MONO                    12419 + 1056 + 51 + 51
#define IDP_RADIO_STEREO                  12451 + 1056 + 51 + 51
#define IDP_RADIO_WND_ICON                12483 + 1056 + 51 + 51
#define IDP_STARTUP_ATMEL                 12524 + 1056 + 51 + 51
#define IDP_STARTUP_COCK                  12759 + 1056 + 51 + 51
#define IDP_STARTUP_SND3                  13226 + 1056 + 51 + 51
#define IDP_HOST_CDC                      13971 + 1056 + 51 + 51
#define IDP_HOST_HID                      13983 + 1056 + 51 + 51
#define IDP_IDLE_CDC_A                    14000 + 1056 + 51 + 51
#define IDP_IDLE_CDC_I                    14099 + 1056 + 51 + 51
#define IDP_IDLE_CDC_NAME                 14134 + 1056 + 51 + 51
#define IDP_IDLE_EBOOK_A                  14203 + 1056 + 51 + 51
#define IDP_IDLE_EBOOK_I                  14302 + 1056 + 51 + 51
#define IDP_IDLE_EBOOK_NAME               14337 + 1056 + 51 + 51
#define IDP_USB_DEVICE_CDC                14406 + 1056 + 51 + 51
#define IDP_USB_DEVICE_HID_KBD            14441 + 1056 + 51 + 51
#define IDP_USB_DEVICE_HID_MOUSE          14476 + 1056 + 51 + 51
#define IDP_BOOT                          14511 + 1056 + 51 + 51
#define IDP_BATT_CHARG                    27475 + 1056 + 51 + 51

#define IDP_STATUS_BATT_CHARGE            0x75B0
#endif
//Picture storage format
//	+0 : coding format
//	+1 : width in pixels
//	+2 : height in pixels
//	+3 : first display data byte


#endif   //#ifndef _TAB_PICT_H_
