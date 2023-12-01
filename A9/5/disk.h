#ifndef __disk_h__
#define __disk_h__

/**
 * Start the disk subsystem
 *    interrupt_service_routine is the procedure that the subsystem calls when a read completes
 */
void disk_start         (void (*interrupt_service_routine) ());

/**
 * Scheudle a disk read for block number blocno
 *    the integer contained in that block is copied into *resultBuf when the read completes
 */
void disk_schedule_read (int* resultBuf, int blockno);

#endif
