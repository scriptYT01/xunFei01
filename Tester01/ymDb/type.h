/*===========================================================================

        Copyright (c) 2007 UbiCore, All Rights Reserved.

                 A Header file for the type definition 

===========================================================================

===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


Index      who     when, what, where, why
--------   ---     ----------------------------------------------------------
1.          Freeman     Created
==========================================================================*/
/*!
 * \file
 *			Global type definitions should be defined here
 *
 * \brief
 *			All basic types are defined in this file.
 */

#ifndef TYPE_H
#define TYPE_H
/*===========================================================================
                             INCLUDE FILES
===========================================================================*/

/* None */

/*===========================================================================
                            TYPE DEFINITION 
===========================================================================*/

#define ADS_PACKED 
#define GCC_PACKED __attribute__ ((packed))

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef volatile unsigned char vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned int vu32;
typedef const volatile unsigned int vuc32;

#ifndef BOOLEAN
typedef unsigned char BOOLEAN;                 // unsigned 8-bit
#endif
#ifndef BYTE
typedef unsigned char BYTE;                 // unsigned 8-bit
#endif
#ifndef WORD
typedef unsigned short WORD;            // unsigned 16-bit
#endif
#ifndef DWORD
typedef unsigned long DWORD;                // unsigned 32-bit
#endif

#ifndef UINT8
typedef unsigned char UINT8;
#endif
#ifndef UINT16
typedef unsigned short UINT16;
#endif
#ifndef UINT32
typedef unsigned int UINT32;
#endif
#ifndef UINT64
typedef unsigned long long UINT64;					//pvoice amos add
#endif

#ifndef INT16
typedef signed short INT16;								//pvoice amos add
#endif

#ifndef INT32
typedef int INT32;
#endif

#ifndef INT64
typedef signed long long INT64;					//pvoice amos add
#endif

#ifndef INT
typedef int INT;
#endif
#ifndef CHAR
typedef char CHAR;                 // signed 8-bit
#endif
#ifndef INT8
typedef char INT8;                 // signed 8-bit
#endif

#define VOID void
#define PVOID	VOID *							//pvoice amos add

#define byte    BYTE
#define word    WORD
#define dword   DWORD

#define U8  u8
#define U16 u16
#define U32 u32

#define S8 char
#define S16 short
#define S32 int

#define s8 char
#define s16 short
#define s32 int

#ifndef BOOL
#define BOOL BYTE
#endif



#ifndef FALSE
#define FALSE (0x00) 
#endif

#ifndef TRUE
#define TRUE  (0x01) 
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef ENABLE
#define ENABLE	1
#endif

#ifndef DISABLE
#define  DISABLE	0
#endif

#ifndef FAILED
#define FAILED FALSE
#endif
#ifndef SUCCEED
#define SUCCEED TRUE
#endif

#ifndef PNULL
#define PNULL           0
#endif

#ifndef ICT_TRUE
#define ICT_TRUE		1
#endif

#ifndef ICT_FALSE
#define ICT_FALSE		0
#endif

#ifndef ICT_NULL
#define ICT_NULL        0
#endif 

#ifndef ICT_NO_ERR
#define ICT_NO_ERR		0
#endif

#ifndef ICT_ERR
#define ICT_ERR         (-1)
#endif

#ifndef BITDEF
#define BITDEF
enum BITDEFINEenum
{
	BIT0	=	(1 << 0),
	BIT1	=	(1 << 1),
	BIT2	=	(1 << 2),
	BIT3	=	(1 << 3),
	BIT4	=	(1 << 4),
	BIT5	=	(1 << 5),
	BIT6	=	(1 << 6),
	BIT7	=	(1 << 7),
	BIT8	=	(1 << 8),
	BIT9	=	(1 << 9),
	BIT10	=	(1 << 10),
	BIT11	=	(1 << 11),
	BIT12	=	(1 << 12),
	BIT13	=	(1 << 13),
	BIT14	=	(1 << 14),
	BIT15	=	(1 << 15)
};
#endif

typedef enum {
    ICT_OK,
    ICT_ERROR = 1
} ICT_STATUS;


/*===========================================================================
                                CONSTANTS
===========================================================================*/

/* None */

/*===========================================================================
                                  MACROS
===========================================================================*/

#ifdef LOCAL
#undef LOCAL
#endif

#define LOCAL static

#define LSB(a)          ((a).v[0])  /*!< Get LSB from word type */
#define MSB(a)          ((a).v[1])  /*!< Get MSB from word type */

#define LOWER_LSB(a)    ((a).v[0])  /*!< Get LSB from dword type */
#define LOWER_MSB(a)    ((a).v[1])
#define UPPER_LSB(a)    ((a).v[2])
#define UPPER_MSB(a)    ((a).v[3])  /*!< Get MSB from dword type */

//#define ZB_OSTimeDly(x) OSTimeDly(x)
//#define GUI_OSTimeDly(x)    OSTimeDly(x)
/*===========================================================================
                                  ENUMS
===========================================================================*/

// General
enum {
        DEBUG_BIT_ERROR         = 1U << 0,
        DEBUG_BIT_INFO          = 1U << 1,
};

// Voice Station (Server connection)
enum {
        DEBUG_BIT_CONNECT       = 1U << 2,
        DEBUG_BIT_RXTX          = 1U << 3,
        DEBUG_BIT_RXRES         = 1U << 4,
};

// Smart Stand
enum {
        DEBUG_BIT_VR_CODE       = 1U << 2,
        DEBUG_BIT_UART          = 1U << 3,
};


/*===========================================================================
                                  STRUCTURES
===========================================================================*/
/*!
 * BYTE_VAL: Byte type union structure with the bit access \
 */
typedef union _BYTE_VAL
{
    struct
    {
        unsigned int b0:1;
        unsigned int b1:1;
        unsigned int b2:1;
        unsigned int b3:1;
        unsigned int b4:1;
        unsigned int b5:1;
        unsigned int b6:1;
        unsigned int b7:1;
    } bits;
    BYTE Val;
} BYTE_VAL;

/*!
 * WORD_VAL: WORD type union structure with the bit access \
 */
typedef union _WORD_VAL
{
    BYTE v[2];
    WORD Val;
    struct
    {
        BYTE LSB;
        BYTE MSB;
    } byte;
} WORD_VAL;

/*!
 * DWORD_VAL: DWORD type union structure with the bit access 
 */
typedef union _DWORD_VAL
{
    DWORD Val;
    struct
    {
        BYTE LOLSB;
        BYTE LOMSB;
        BYTE HILSB;
        BYTE HIMSB;
    } byte;
    struct
    {
        WORD LSW;
        WORD MSW;
    } word;
    BYTE v[4];
    struct
    {
        unsigned int b0:1;
        unsigned int b1:1;
        unsigned int b2:1;
        unsigned int b3:1;
        unsigned int b4:1;
        unsigned int b5:1;
        unsigned int b6:1;
        unsigned int b7:1;
        unsigned int b8:1;
        unsigned int b9:1;
        unsigned int b10:1;
        unsigned int b11:1;
        unsigned int b12:1;
        unsigned int b13:1;
        unsigned int b14:1;
        unsigned int b15:1;
        unsigned int b16:1;
        unsigned int b17:1;
        unsigned int b18:1;
        unsigned int b19:1;
        unsigned int b20:1;
        unsigned int b21:1;
        unsigned int b22:1;
        unsigned int b23:1;
        unsigned int b24:1;
        unsigned int b25:1;
        unsigned int b26:1;
        unsigned int b27:1;
        unsigned int b28:1;
        unsigned int b29:1;
        unsigned int b30:1;
        unsigned int b31:1;
    } bits;
} DWORD_VAL;

typedef struct
{
    u32 length;
    u8 *data;
} TU8_BUF;

typedef struct
{
    u32 length;
    u16 *data;
} TU16_BUF;

typedef struct
{
    u32 length;
    u32 *data;
} TU32_BUF;

/*===========================================================================
                        GLOBAL VARIABLE DECLARATIONS
===========================================================================*/
/* None */


/*===========================================================================
                           FUNCTION PROTOTYPES
===========================================================================*/
/* NONE */

#endif /* TYPE_H */
