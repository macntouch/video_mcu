//*****************************************************************************
//
// fswrapper.h - Public type definitons and function prototypes for the simple
//               file system wrapper module.
//
// Copyright (c) 2008-2011 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 7243 of the Stellaris Firmware Development Package.
//
//*****************************************************************************

#ifndef __FSWRAPPER_H__
#define __FSWRAPPER_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//! \addtogroup fswrapper_api
//! @{
//
//*****************************************************************************

typedef struct
{
    //
    //! This string provides a pseudo-directory name that will be used to
    //! identify this mount point in future calls to fs_open.  If this string
    //! is NULL, this indicates that this is the default file system which will
    //! be used when any simple filename, not including a leading directory
    //! name is passed or when the leading directory name is not found in the
    //! list of mount points passed to fs_init.
    //
    const char *pcNamePrefix;

    //
    //! A pointer to the start of the file system image that is to be used to
    //! satisfy requests for files whose name begins "/name" where
    //! "name" is the string provided in the pcNamePrefix field.  This pointer
    //! may point to either a position-dependent or position-independent file
    //! system image generated by the makefsfile executable or makefsdata
    //! Perl script.  If NULL, it is assumed that the FAT file system is to be
    //! used and that the drive number provided in ulDriveNum should be
    //! substituted for "name" in the supplied filename before attempting to
    //! open the FAT file.
    //
    unsigned char *pcFSImage;

    //
    //! If this mount point describes a logical drive in the FAT file system,
    //! this field indicates the drive number that is to be accessed.  This
    //! number will be substituted for the string provided in the pcNamePrefix
    //! field in the filename passed to fs_open before that filename is passed
    //! down to the FAT file system.  For example, if pcNamePrefix is "sdcard"
    //! and ulDriveNum is 0, a call to fs_open passing "/sdcard/images/logo.gif"
    //! will be passed to the FATfs f_open call as "/0/images/logo.gif".  This
    //! field is ignored if pcFSImage is not NULL (indicating that this mount
    //! point refers to a file system image rather than the FAT file system).
    //!
    unsigned long ulDriveNum;

    //
    //! This function pointer is called whenever a file is to be opened, read
    //! or (for read/write file systems) written on this file system.  If any
    //! special setup is required to allow access to the physical medium (for
    //! example, setting the SSI mode or clock frequency), the application may
    //! use this callback to perform that initialization.  If this field is
    //! NULL, no callback will be made.
    //
    void (*pfnEnable)(unsigned long ulFSIndex);

    //
    //! This callback is made after access to the physical medium has been
    //! completed.  An application may assume that no further access to the
    //! medium will be made until a call to pfnEnable is made.  If this field
    //! is NULL, no callback will be made.
    //
    void (*pfnDisable)(unsigned long ulFSIndex);
}
fs_mount_data;

//*****************************************************************************
//
// This marker, "FIMG", is placed at the beginning of a position-independent
// file system image to differentiate it from a position-dependent image.
//
//*****************************************************************************
#define FILE_SYSTEM_MARKER ((const struct fsdata_file *)0x474D4946)

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Public function prototypes
//
//*****************************************************************************
tBoolean fs_init(fs_mount_data *psMountPoints, unsigned long ulNumMountPoints);
void fs_tick(unsigned long ulTickMS);
struct fs_file *fs_open(char *name);
void fs_close(struct fs_file *file);
int fs_read(struct fs_file *file, char *buffer, int count);
tBoolean fs_map_path(const char *pcPath, char *pcMapped, int iLen);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __FSWRAPPER_H__
