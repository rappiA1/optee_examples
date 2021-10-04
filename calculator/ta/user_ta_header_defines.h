//include guards
#ifndef USER_TA_HEADER_DEFINES_H
#define USER_TA_HEADER_DEFINES_H

#include <calculator_ta.h>

/* UUID of the TA */
#define TA_UUID 	TA_CALCULATOR_UUID

/* TA properties: multi-instance TA, no specific attribute
 * TA_FLAG_EXEC_DDR is meaningless but mandated.
 */
#define TA_FLAGS	TA_FLAG_EXEC_DDR

/* Define a stack of size 2 kiB */
#define TA_STACK_SIZE	(2 * 1024)

/* Define allocatable memory of 32 kiB */
#define TA_DATA_SIZE	(32 * 1024)

#endif /* USER_TA_HEADER_DEFINES_H */
