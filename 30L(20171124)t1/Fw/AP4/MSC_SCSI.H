/*
**********************************************************************************************************
*                                           USB MASS STORAGE CLASS
*                                   SCSI Command and Bulk-Only Transport
*                                            SCSI Command H File
*
*                                  Copyright 2003-2004, JMicron Technology
*                            Science-Based Intustrial Park,Hsinchu, Taiwan, R. O. C.
*
* FILE     : MSC_SCSI.H
* VERSION  : V40623
* ENGINEER : KestrelHuang
**********************************************************************************************************
*/
//------------------------------------------------------
//                                                     |
//                      DEFINITION                     |
//                                                     |
//------------------------------------------------------
enum
{	
   SCSI_NO_ERROR,
   SCSI_COMMAND_PASSED,
   SCSI_COMMAND_FAILED,       //For not supported command to stall
   SCSI_COMMAND_INVALID,      //For zero length command failed,because they can't stall
   SCSI_COMMAND_DRY,
   SCSI_TSTUNIST_ERROR,
   SCSI_SIGNATURE_ERROR
};


enum
{
   TUST1,
   TUST2,
   TUST3,
   TUST4
};
#define CBWCB_SIZE                                    16
#define CBW_SIZE                                      31    //sizeof(tCBW)
#define CSW_SIZE                                      13    //sizeof(tCSW)

#define CBW_SIGNATURE_IP                              0x55534243
#define CSW_SIGNATURE_IP                              0x55534253

#define STOP_THE_MEDIA                                0x00
#define START_THE_MEDIA                               0x01
#define EJECT_THE_MEDIA                               0x02
#define RESERVED                                      0x03

#define READ_WRITE_ERROR_RECOVERY_PAGE                0x01
#define FLEXIBLE_DISK_PAGE                            0x05
#define CACHING_PAGE                                  0x08
#define REMOVABLE_BLOCK_ACCESS_CAPACITIES_PAGE        0x1B
#define TIMER_AND_PROTECT_PAGE                        0x1C
#define RETURN_ALL_PAGES                              0x3F

#define NO_SENSE                                      0
#define RECOVERD_DATA_WITH_RETRIES                    1
#define RECOVERD_DATA_WITH_ECC                        2
#define LOGICAL_DRIVE_BECOMING_READY                  3
#define LOGICAL_DRIVE_INITIALIZATION_REQUIRED         4
#define LOGICAL_UNIT_FORMAT_IN_PROGRESS               5
#define LOGICAL_DRIVE_DEVICE_IS_BUSY                  6
#define NO_REFERENCE_POSITION_FOUND                   7
#define LOGICAL_UNIT_COMMUNICATION_FAILURE            8
#define LOGICAL_UNIT_COMMUNICATION_TIME_OUT           9
#define LOGICAL_UNIT_COMMUNICATION_OVERRUN            10
#define MEDIUM_NOT_PRESENT                            11
#define USB_TO_HOST_SYSTEM_INTERFACE_FAILURE          12
#define INSUFFICIENT_RESOURCES                        13
#define UNKNOWN_ERROR                                 14
#define NO_SEEK_COMPLETE                              15
#define WRITE_FAULT                                   16
#define ID_CRC_ERROR                                  17
#define UNRECOVERD_READ_ERROR                         18
#define ADDRESS_MARK_NOT_FOUND_FOR_ID_FIELD           19
#define ADDRESS_MARK_NOT_FOUND_FOR_DATA_FIELD         20
#define RECORDED_ENTITY_NOT_FOUND                     21
#define CANNOT_READ_MEDIUM_UNKNOWN_FORMAT             22
#define FORMAT_COMMAND_FAILED                         23
#define DIAGNOSTIC_FAILURE_ON_COMPONENT_NN            24
#define PARAMETER_LIST_LENGTH_ERROR                   25
#define INVALID_COMMAND_OPERATION_CODE                26
#define LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE            27
#define INVALID_FIELD_IN_COMMAND_PACKET               28
#define LOGICAL_UNIT_NOT_SUPPORTED                    29
#define INVALID_FIELD_IN_PARAMETER_LIST               30
#define PARAMETER_NOT_SUPPORTED                       31
#define PARAMETER_VALUE_INVALID                       32
#define SAVING_PARAMETERS_NOT_SUPPORT                 33
#define NOT_READY_TO_READY_TRANSITION_MEDIA_CHANGED   34
#define POWER_ON_RESET_OR_BUS_DEVIC_RESET_OCCURRED    35
#define COMMANDS_CLEARED_BY_ANOTHER_INITIATOR         36
#define WRITE_PROTECTED_MEDIA                         37
#define OVERLAPPED_COMMAND_ATTEMPED                   38
#define REFRESH                                       39

#define SCSI_TEST_UNIT_READY                          0x00
#define SCSI_REZERO_UNIT                              0x01
#define SCSI_REQUEST_SENSE                            0x03
#define SCSI_FORMAT_UNIT                              0x04
#define SCSI_READ_BLOCK_LIMITS                        0x05
#define SCSI_REASSIGN_BLOCKS                          0x07
#define SCSI_READ_06                                  0x08
#define SCSI_WRITE_06                                 0x0A
#define SCSI_SEEK_06                                  0x0B
#define SCSI_READ_REVERSE                             0x0F
#define SCSI_WRITE_FILEMARKS                          0x10
#define SCSI_SPACE                                    0x11
#define SCSI_INQUIRY                                  0x12
#define SCSI_VERIFY_06                                0x13
#define SCSI_RECOVER_BUFFERED_DATA                    0x14
#define SCSI_MODE_SELECT_06                           0x15
#define SCSI_RESERVE                                  0x16
#define SCSI_RELEASE                                  0x17
#define SCSI_COPY                                     0x18
#define SCSI_ERASE                                    0x19
#define SCSI_MODE_SENSE_06                            0x1A
#define SCSI_START_STOP_UNIT                          0x1B
#define SCSI_RECEIVE_DIAGNOSTIC_RESULTS               0x1C
#define SCSI_SEND_DIAGNOSTIC                          0x1D
#define SCSI_PREVENT_ALLOW_MEDIUM_REMOVAL             0x1E
#define SCSI_READ_FORMAT_CAPACITIES                   0x23
#define SCSI_SET_WINDOW                               0x24
#define SCSI_READ_CAPACITY                            0x25
#define SCSI_READ_10                                  0x28
#define SCSI_READ_GENERATION                          0x29
#define SCSI_WRITE_10                                 0x2A
#define SCSI_SEEK_10                                  0x2B
#define SCSI_ERASE_10                                 0x2C
#define SCSI_READ_UPDATED_BLOCK                       0x2D
#define SCSI_WRITE_AND_VERIFY_10                      0x2E
#define SCSI_VERIFY_10                                0x2F
#define SCSI_SEARCH_DATA_HIGH_10                      0x30
#define SCSI_SEARCH_DATA_EQUAL_10                     0x31
#define SCSI_SEARCH_DATA_LOW_10                       0x32
#define SCSI_SET_LIMITS                               0x33
#define SCSI_PREFETCH                                 0x34
#define SCSI_SYNCHRONIZE_CACHE                        0x35
#define SCSI_LOCK_UNLOCK_CACHE                        0x36
#define SCSI_READ_DEFECT_DATA_10                      0x37
#define SCSI_MEDIUM_SCAN                              0x38
#define SCSI_COMPARE                                  0x39
#define SCSI_COPAND_VERIFY                            0x3A
#define SCSI_WRITE_BUFFER                             0x3B
#define SCSI_READ_BUFFER                              0x3C
#define SCSI_UPDATE_BLOCK                             0x3D
#define SCSI_READ_LONG                                0x3E
#define SCSI_WRITE_LONG                               0x3F
#define SCSI_CHANGE_DEFINITION                        0x40
#define SCSI_WRITE_SAME                               0x41
#define SCSI_READ_SUBCHANNEL                          0x42
#define SCSI_READ_TOC_PMA_ATIP                        0x43
#define SCSI_READ_HEADER                              0x44
#define SCSI_PLAY_AUDIO_10                            0x45
#define SCSI_GET_CONFIGURATION                        0x46
#define SCSI_PLAY_AUDIO_MSF                           0x47
#define SCSI_PLAY_AUDIO_TRACK_INDEX                   0x48
#define SCSI_PLAY_TRACK_RELATIVE                      0x49
#define SCSI_GET_EVENT_STATUS_NOTIFICATION            0x4A
#define SCSI_PAUSE_RESUME                             0x4B
#define SCSI_LOG_SELECT                               0x4C
#define SCSI_LOG_SENSE                                0x4D
#define SCSI_MODE_SELECT_10                           0x55
#define SCSI_MODE_SENSE_10                            0x5A
#define SCSI_READ_12                                  0xA8
#define SCSI_WRITE_12                                 0xAA
#define SCSI_MODE_CURRENT_VALUE                       0x00
#define SCSI_MODE_CHANGEABLE_VALUE                    0x40
#define SCSI_MODE_DEFAULT_VALUE                       0x80
#define SCSI_MODE_SAVED_VALUE                         0xC0
#define SCSI_REPORT_KEY                               0xA4
#define SCSI_WRITE_AND_VERIFY_12                      0xAE
#define SCSI_VERIFY_12                                0xAF
#define SCSI_READ_CD                                  0xBE

#define SCSI_REQUEST_SENSE_SIZE                       sizeof( tSCSI_REQUEST_SENSE_DATA )
#define SCSI_FORMAT_UNIT_SIZE                         sizeof( tSCSI_FORMAT_UNIT_DATA )
#define SCSI_INQUIRY_SIZE                             sizeof( tSCSI_INQUIRY_DATA )
#define SCSI_READ_FORMAT_CAPACITY_LIST_HEADER_SIZE    0x04
#define SCSI_READ_FORMAT_MAXIMUM_SIZE                 0x08
#define SCSI_READ_CAPACITY_SIZE                       sizeof( tSCSI_READ_CAPACITY_DATA )

#define SCSI_MODE_PARAMETER_HEADER_SIZE               sizeof( tSCSI_MODE_PARAMETER_HEADER_DATA )
#define SCSI_MODE_RW_ERR_RECOVERY_PAGE_SIZE           sizeof( tSCSI_MODE_RW_ERR_RECOVERY_PAGE_DATA )
#define SCSI_MODE_FLEXIBLE_DISK_PAGE_SIZE             sizeof( tSCSI_MODE_FLEXIBLE_DISK_PAGE_DATA )
#define SCSI_MODE_CACHING_PAGE_SIZE                   sizeof( tSCSI_MODE_CACHING_PAGE_DATA )
#define SCSI_MODE_REM_BLK_ACC_CAP_PAGE_SIZE           sizeof( tSCSI_MODE_REM_BLK_ACC_CAP_PAGE_DATA )
#define SCSI_MODE_TIMER_AND_PROTECT_PAGE_SIZE         sizeof( tSCSI_MODE_TIMER_AND_PROTECT_PAGE_DATA )

#define ALL_PAGE_SIZE                                 SCSI_MODE_PARAMETER_HEADER_SIZE + SCSI_MODE_RW_ERR_RECOVERY_PAGE_SIZE + \
            	                                       SCSI_MODE_FLEXIBLE_DISK_PAGE_SIZE + SCSI_MODE_CACHING_PAGE_SIZE +       \
                                                      SCSI_MODE_REM_BLK_ACC_CAP_PAGE_SIZE

//------------------------------------------------------
//                                                     |
//                 FUNCTION PROTOTYPE                  |
//                                                     |
//------------------------------------------------------
#ifdef _MSC_SCSI
void  SCSI_DATA_HOOK                   ( void );
void  SCSI_READ_MEM_HOOK               ( void );
//void  SCSI_WRITE_MEM_HOOK              ( void );
//void  SCSI_MOTOR_ON_HOOK               ( void );
//void  SCSI_MOTOR_OFF_HOOK              ( void );
//void  SCSI_EJECT_CD_HOOK               ( void );
//void  EPD_STALL_HOOK                   ( void );
//void  EPE_STALL_HOOK                   ( void );

void  SCSI_CBW_DECODER                 ( void );
void  SCSI_CSW_ENCODER                 ( void );

void  ubTEST_UNIT_READY                ( void );
void  ubREZERO_UNIT                    ( void );
void  ubREQUEST_SENSE                  ( void );
void  ubFORMAT_UNIT                    ( void );
void  ubINQUIRY                        ( void );
void  ubMODE_SELECT                    ( void );
void  ubMODE_SENSE                     ( void );

void  MODE_PAGE_0x01                   ( void *pvDATA );
void  MODE_PAGE_0x05                   ( void *pvDATA );
void  MODE_PAGE_0x08                   ( void *pvDATA );
void  MODE_PAGE_0x1B                   ( void *pvDATA );
//void  MODE_PAGE_0x1C                   ( void *pvDATA );

void  ubSTART_STOP_UNIT                ( void );
void  ubSEND_DIAGNOSTIC                ( void );
void  ubPREVENT_ALLOW_MEDIUM_REMOVAL   ( void );
void  ubREAD_FORMAT_CAPACITIES         ( void );
void  ubREAD_CAPACITY                  ( void );
//void  ubSCSI_READ                      ( void );
//void  ubSCSI_WRITE                     ( void );
void  ubSEEK                           ( void );
void  ubWRITE_AND_VERIFY               ( void );
void  ubVERIFY                         ( void );
#endif

//----------------------------------------------------------------
//Extern function
extern   void  SCSI_DATA_HOOK                   ( void );
extern   void  SCSI_READ_MEM_HOOK               ( void );
//extern   void  SCSI_WRITE_MEM_HOOK              ( void );
//extern   void  SCSI_MOTOR_ON_HOOK               ( void );
//extern   void  SCSI_MOTOR_OFF_HOOK              ( void );
//extern   void  SCSI_EJECT_CD_HOOK               ( void );
//         void  EPD_STALL_HOOK                   ( void );
//         void  EPE_STALL_HOOK                   ( void );

extern   void  SCSI_CBW_DECODER                 ( void );
extern   void  SCSI_CSW_ENCODER                 ( void );

extern   void  ubTEST_UNIT_READY                ( void );
extern   void  ubREZERO_UNIT                    ( void );
extern   void  ubREQUEST_SENSE                  ( void );
extern   void  ubFORMAT_UNIT                    ( void );
extern   void  ubINQUIRY                        ( void );
extern   void  ubMODE_SELECT                    ( void );
extern   void  ubMODE_SENSE                     ( void );

extern   void  MODE_PAGE_0x01                   ( void *pvDATA );
extern   void  MODE_PAGE_0x05                   ( void *pvDATA );
extern   void  MODE_PAGE_0x08                   ( void *pvDATA );
extern   void  MODE_PAGE_0x1B                   ( void *pvDATA );

extern   void  ubSTART_STOP_UNIT                ( void );
extern   void  ubSEND_DIAGNOSTIC                ( void );
extern   void  ubPREVENT_ALLOW_MEDIUM_REMOVAL   ( void );
extern   void  ubREAD_FORMAT_CAPACITIES         ( void );
extern   void  ubREAD_CAPACITY                  ( void );
//extern   void  ubSCSI_READ                      ( void );
//extern   void  ubSCSI_WRITE                     ( void );
extern   void  ubSEEK                           ( void );
extern   void  ubWRITE_AND_VERIFY               ( void );
extern   void  ubVERIFY                         ( void );
