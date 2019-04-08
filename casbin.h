/*
 * =====================================================================================
 *
 *       Filename:  casbin.h
 *
 *    Description:  casbin header file
 *
 *        Version:  1.0
 *        Created:  2019/3/22 15:21:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jarod Lee (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __CASBIN_H
#define __CASBIN_H
enum {
        ClassId_Nav = 1,
        ClassId_Tim = 2,
        ClassId_Rxm = 3,
        ClassId_Ack = 5,
        ClassId_Cfg = 6,
        ClassId_Msg = 8,
        ClassId_Mon = 0xA,
        ClassId_Aid = 0xB,
        ClassId_Nmea = 0x4E
};

enum {
        NavSubId_Status = 0,
        NavSubId_Dop = 1,
        NavSubId_Sol = 2,
        NavSubId_Pv = 3,
        NavSubId_TimeUtc = 0x10,
        NavSubId_Clock = 0x11,
        NavSubId_GpsInfo = 0x20,
        NavSubId_BdsInfo = 0x21,
        NavSubId_GlnInfo = 0x22,
        NavSubId_ImuAtt = 0x06
};

enum {
        TimSubId_Tp = 0,
};

enum {
        RxmSubId_Measx = 0x10,
        RxmSubId_SvPos = 0x11,
        RxmSubId_Sensor = 0x07
};

enum {
        AckSubId_NAck = 0,
        AckSubId_Ack = 1
};

enum {
        CfgSubId_Prt = 0,
        CfgSubId_Msg = 1,
        CfgSubId_Rst = 2,
        CfgSubId_Tp  = 3,
        CfgSubId_Rate = 4,
        CfgSubId_Cfg = 5,
        CfgSubId_TMode = 6,
        CfgSubId_NavX = 7,
        CfgSubId_Group = 8,
        CfgSubId_Rf = 9,
        CfgSubId_PollMsg = 10,
        CfgSubId_Ins	= 0x10,
};

enum {
        MsgSubId_BdsUtc = 0,
        MsgSubId_BdsIon = 1,
        MsgSubId_BdsEph = 2,
        MsgSubId_GpsUtc = 5,
        MsgSubId_GpsIon = 6,
        MsgSubId_GpsEph = 7,
        MsgSubId_GlnEph = 8,
};

enum {
        MonSubId_Ver = 4,
        MonSubId_Hw = 9
};

enum {
        AidSubId_Ini 	= 1,
        AidSubId_Hui 	= 3,
        AidSubId_LTEDat = 5,
        AidSubId_LTEReq = 6,
        AidSubId_LTEEnd = 9,
};

enum {
        NmeaSubId_GGA = 0,
        NmeaSubId_GLL = 1,
        NmeaSubId_GSA = 2,
        NmeaSubId_GSV = 3,
        NmeaSubId_RMC = 4,
        NmeaSubId_VTG = 5,
        NmeaSubId_ZDA = 8,
        NmeaSubId_DHV = 0x13,
        NmeaSubId_GST = 7
};

typedef unsigned int U4;
typedef int I4;
typedef unsigned short U2;
typedef short I2;
typedef unsigned char U1;
typedef char I1;
typedef float R4;
typedef double R8;

struct nav_pv {
        U4 runTime;
        U1 posValid;
        U1 velValid;
        U1 system;
        U1 numSV;
        U1 numSVGPS;
        U1 numSVBDS;
        U1 numSVGLONASS;
        U1 res;
        R4 pDop;
        R8 lon;
        R8 lat;
        R4 height;
        R4 sepGeoid;
        R4 hAcc;
        R4 vAcc;
        R4 velN;
        R4 velE;
        R4 velU;
        R4 speed3D;
        R4 speed2D;
        R4 heading;
        R4 sAcc;
        R4 cAcc;
};

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

/*
 * struct cashdr - Common header of casbin message.
 * @banner:     First two bytes, should be 0xBA 0xCE.
 * @len:        Length of message's payload.
 * @cls_id:     Message's class ID.
 * @sub_id:     Message's sub ID.
 * @p:          Pointer to payload.
 */
struct cashdr {
        uint8_t         banner[2];
        uint16_t        len;
        uint8_t         cls_id;
        uint8_t         sub_id;
        uint8_t         p[2];
};

#define MAX_CASBIN_SIZE         2048
#define MAX_READER_SIZE         (MAX_CASBIN_SIZE + 10)
/*
 * struct casbin_reader - Read casbin message
 * @buf:        buffer of casbin message.
 * @header:     header point to casbin message.
 * @pos:        current position of buffer.
 * @is_reset:   is casbin_reader need reset.
 */
struct casbin_reader {
        union {
                uint8_t         buf[MAX_READER_SIZE];
                struct cashdr   header[1];
        };
        int     pos;
        int     is_reset;
};

void casbin_reader_reset(struct casbin_reader *r);
int casbin_reader_putbyte(struct casbin_reader *r, uint8_t c);

#endif

