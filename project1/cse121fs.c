/*
 *   CSE 121 Project 1: A Simple File System
 *   Copyright (C) 2006-2007  Alex C. Snoeren <snoeren@cs.ucsd.edu>
 *
 *   FUSE: Filesystem in Userspace
 *   Copyright (C) 2001-2005  Miklos Szeredi <miklos@szeredi.hu>
 *
 *   This program can be distributed under the terms of the GNU GPL.
 *   See the file COPYING.
 */

#define FUSE_USE_VERSION 26

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#include <fuse.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/statfs.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif

#include "cse121fs.h"

/**
 * Initialize filesystem
 *
 * You need the filename that you are using as disk to be passed to this 
 * function, since it is not taking any parameters, hard code the file name 
 * (we defined this file name in cse121fs.h as DISKFILE). This function will 
 * use that file as the disk image to mount the application-level filesystem.  
 * You also need to open this disk file and store the file discriptor in a
 * global variable or super block to read and write from this file later (or
 * you can open this file whenever you needed and close it after that).
 * Once a mount occurs, that file will be used as the disk for all further 
 * file system commands you will implement (until an xmp_unmount occurs).  
 * Remember that this is an application level file system.  From the point 
 * of view of a user of the application, it will appear like a structured 
 * file system.  However, in reality the disk image is merely one contiguous 
 * file in the operating system.
 * 
 * This is also when you will need to check the integrity of your file 
 * system.  For example, if a crash occurred during a file create after an
 * inode was allocated but before a directory entry is updated, such an error 
 * should be found and fixed in xmp_mount.
 *
 * The return value will passed in the private_data field of
 * fuse_context to all file operations and as a parameter to the
 * xmp_unmount() method.
 *
 */
static void* xmp_mount(struct fuse_conn_info *conn) {
}

/*
 *
 * Unmount is responsible for unmounting the file system. In addition to 
 * closing the disk image file descriptor, unmount will need to write out 
 * any necessary meta data that might be required the next time the file 
 * system is mounted. For instance, it might note that the filesystem was 
 * cleanly unmounted, speeding up the integrity check the next time the 
 * file system is mounted.
 *
 */
static void xmp_unmount (void *private_data) {
}

/** 
 *
 * Given an absolute path to a file/directory 
 * (i.e., /foo/bar ---all paths will start with the root directory, "/"),
 * you need to return the file attributes that is similar stat system call. 
 *
 * However you don't need to fill all the attributes except file size, 
 * file size in blocks and block size of file system. 
 *
 * You can ignore the 'st_dev', 'st_blksize' and 'st_ino' fields 
 * (FUSE ignore these values and for more information look into fuse.h file).
 * 
 * For st_mode you have to send the S_IFDIR if the entry is directory 
 * or S_IFREG if it is a simple file
 *
 * Since we are not implementing permissions, 
 * we pass fixed values to st_uid (5), st_gid(500) entries.
 *
 * We are also not implementing hard links,
 * so set the st_nlinks to 3 (just as safe side). 
 *
 * Its up to you to implemet the time fields (st_atime, st_mtime, st_ctime), 
 * if didn't impement them just return current time.
 * 
 * The only thing you would be returning correctly is the file size and 
 * size in blocks (st_size, st_blocks) and file system block size (st_blksize)
 *
 * The following are common steps to implement this function:
 * 1. Resolve the directory path -- this
 *    will probably involve reading each directory in the path to get
 *    the location of the file entry (or location of data block with
 *    the file's inode) of the specified file.
 * 
 * 2. Fill the file size, file size in blocks, block size information
 *
 * You should return 0 on success or -1 on error (e.g., the path doesn't
 * exist).
 */
static int xmp_getattr(const char *path, struct stat *stbuf)
{

    /* 
     * I am setting some pluasible values but you can change the following to 
     * other values.
     */
    stbuf->st_nlink = 3;
    stbuf->st_uid   = 5;
    stbuf->st_gid   = 500;
    stbuf->st_rdev  = 0;
    stbuf->st_atime = time(NULL);
    stbuf->st_mtime = time(NULL);
    stbuf->st_ctime = time(NULL);

    /* you have to implement the following fields correctly 

    if (The path represents the directory)
        stbuf->st_mode  = 0777 | S_IFDIR;
    else 
        stbuf->st_mode  = 0777 | S_IFREG;

    stbuf->st_size    =  // file size
    stbuf->st_blocks  =  // file size in blocks
    stbuf->st_blksize =  // block size of you file system

    */
    return 0;
}

/**
 * Given an absolute path to a directory (which may or may not end in
 * '/'), xmp_mkdir will create a new directory named dirname in that
 * directory. Ignore the mode parameter as we are not implementing 
 * the permissions. The steps your implementation will do to make a new
 * directory will be the following: 
 *
 * 1. Resolve the directory path
 *
 * 2. Allocate and initialize a new directory block  
 *
 * 3. Update the directory block for 'path' by adding an entry for 
 *    the newly created directory. 
 *
 * 4. You also need to create appropriate entries for "." and ".." in the 
 *    New directory
 *
 * You should return 0 on success or -1 on error (e.g., the path doesn't
 * exist, or dirname already does).
 *
 */
static int xmp_mkdir(const char *path, mode_t mode)
{
    return 0;
}

/** Read directory
 *
 * Given an absolute path to a directory, xmp_readdir will return 
 * all the files and directories in that directory. The steps you 
 * will take will be the following:
 *
 * 1. Resolve the directory entry from its absolute path to get 
 *    the address of the directory block that corresponds to "path"
 *
 * 2. For each valid entry in the directory block, copy the file 
 *    name into an array (buf) using the function filler. The filler
 *    is already implemeted in fuse and its sample use is shown in 
 *    fusexmp.c file and you should pass the zero to the offest 
 *    paramter of filler function. fuse.h file has some more infomration
 *    about how to implement this function and another way of implementing it.
 *
 * For time being ignore the fuse_file_info parameter. If you would 
 * like to use please see the open function in fuse.h file for more 
 * detials.
 *
 * You should return 0 on success or -1 on error (e.g., the directory 
 * does not exist).
 */
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
{
    return 0;
}

/*
 * Given an absolute path to a file (for example /a/b/myFile), xmp_create 
 * will create a new file named myFile in the /a/b directory. 
 * Ignore the mode parameter as we are not implementing 
 * the permissions. Also ignore the rdev parameter. 
 * The steps your implementation will do to make a new
 * file will be the following: (similar to the xmp_mkdir except the foruth step)
 *
 * 1. Resolve the directory path
 *
 * 2. Allocate and initialize a new directory entry  
 *
 * 3. Update the directory entry for 'path' by adding an entry for 
 *    the newly created file. 
 *
 *
 * You should return 0 on success or -1 on error (e.g., the path doesn't
 * exist, or file name already exists).
 */
static int xmp_create(const char *path, mode_t mode, dev_t rdev)
{
    return 0;
}

/**
 * The function xmp_read provides the ability to read data from 
 * an absolute path 'path,' which should specify an existing file.
 * It will attempt to read 'size' bytes from the specified file 
 * on your filesystem into the memory address 'buf'. The return 
 * value is the amount of bytes actually read; if the file is 
 * smaller than size, xmp_read will simply return the most amount
 * of bytes it could read. 
 * 
 * For time being ignore the fuse_file_info parameter. If you would 
 * like to use please see the open function in fuse.h file for more 
 * detials.
 *
 * On error, xmp_read will return -1.  The actual implementation 
 * of xmp_read is dependent on how files are allocated.
 *
 */
static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi)
{
    return 0;
}

/**
 * The function xmp_write will attempt to write 'size' bytes from 
 * memory address 'buf' into a file specified by an absolute 'path'
 *
 * For time being ignore the fuse_file_info parameter. If you would 
 * like to use please see the open function in fuse.h file for more 
 * detials.
 *
 * On error (e.g., the path does not exist) xmp_write will return -1, 
 * otherwise xmp_write should return the number of bytes written. 
 * 
 * Note size is not necessarily an integral number of blocks. 
 * Similar to xmp_read, the actual implementation of xmp_write will 
 * depend on how you decide to allocate file space.
 *
 */
static int xmp_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi)
{
    return 0;
}

/**
 * This function deletes the last component of the path (e.g., /a/b/c you 
 * need to remove the file 'c' from the directory /a/b). The steps you 
 * will take will be the following:
 *
 * 1. Locate the directory entry for the file by resolving the path - this
 *    will probably involve reading each directory in the path to get
 *    the location of the first data block (or location of data block with
 *    the file's inode) of the specified file.
 *
 * 2. Update the directory entry for the deleted file
 *
 * 3. Free any data blocks used by the file
 *
 * Again, these steps are very general and the actual logistics of how 
 * to locate data blocks of a file, how to free data blocks and how to update
 * directory entries are dependent on your implementation of directories
 * and file allocation.
 *
 * For time being ignore the fuse_file_info parameter. If you would 
 * like to use please see the open function in fuse.h file for more 
 * detials.
 *
 * On error (e.g., the path does not exist) xmp_delete will return -1, 
 * otherwise xmp_delete should return the number of bytes written. 
 *
 */
static int xmp_delete(const char *path)
{
    return 0;
}

/**
 * The function rename will rename a file or directory named by the
 * string 'oldpath' and rename it to the file name specified by 'newpath'.
 *
 * As usual, return 0 on success, -1 on failure.
 *
 */
static int xmp_rename(const char *from, const char *to)
{
    return 0;
}

static struct fuse_operations xmp_oper = {
    .init    = xmp_mount,
    .destroy = xmp_unmount,
    .getattr = xmp_getattr,
    .mkdir	 = xmp_mkdir,
    .readdir = xmp_readdir,
    .mknod	 = xmp_create,
    .read	 = xmp_read,
    .write	 = xmp_write,
    .unlink	 = xmp_delete,
    .rename	 = xmp_rename,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}

/***************** DO NOT MODIFY THE FUNCTIONS BELOW ***************/
/*
 * before calling these function you need to open the file that is being
 * used for disk (DISKFILE) and pass the file discriptor to these functions
 */

/* Read a block from disk */
int dread(int fd, int blocknum, char *buf) {
  if(lseek(fd, blocknum*BLOCKSIZE, SEEK_SET)<0)
    return -1;

  if (read(fd, buf, BLOCKSIZE) != BLOCKSIZE) {
    perror("dread");
    return -1;
  }
  return BLOCKSIZE;
}

/* Write a block to disk */
int dwrite(int fd, int blocknum, char *buf)
{
  if(lseek(fd, blocknum*BLOCKSIZE, SEEK_SET)<0)
    return -1;
  if (write(fd, buf, BLOCKSIZE) != BLOCKSIZE) {
    perror("dwrite");
    return -1;
  }
  return BLOCKSIZE;
}

