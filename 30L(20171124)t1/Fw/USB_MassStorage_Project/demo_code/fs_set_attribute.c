#include "KBDM9_DDK.H"


#pragma CODE = FSSetAttribute_CODE
#pragma DATA = FSSetAttribute_XRAM

FS_ERR FSSetAttribute(char *spec, unsigned char attr)
{
    FCFILE *file ;//= &internal_slot;
	int err, drive = ParseDrive(&spec);

    if( drive < 0 ) {
        return FS_INVALID_SPEC;
    }

    if( 0 == gdrv[drive].fsinfo->total_sectors ){
        return FS_FORMAT_ERROR;
    }
    if( isCWriteProtect(drive) ){
        return FS_ACCESS_DENIED;
    }

    SetFileSystemBusy(drive);

	file = &(gdrv[drive].fsinfo->internal_slot);

    if( FS_NO_ERROR != (err = GetInternalFCB(&spec)) ){
        goto GoodBye_FCSetAttribute;
    }


    if(FS_NO_ERROR != (err = FindFile(file, spec))) {
        goto GoodBye_FCSetAttribute;
    }


    attr &= ~(FS_ATTR_SYSTEM|FS_ATTR_DIRECTORY);
    file->attribute &= ~(FS_ATTR_READONLY|FS_ATTR_HIDDEN|FS_ATTR_ARCHIVE|FS_ATTR_VOLUME);
    file->attribute |= attr;

    UpdateDirectoryEntry(file, 0, 0);

    if( FS_NO_ERROR != (err = FlushBuffer(drive)) ) {
        goto GoodBye_FCSetAttribute;
    }

    if(gdrv[drive].drv_flush) {
        if( gdrv[drive].drv_flush() ) {
            err = FS_DRIVE_ERROR;
            goto GoodBye_FCSetAttribute;
        }
    }

GoodBye_FCSetAttribute:

    ClearFileSystemBusy(drive);
    return err;
}