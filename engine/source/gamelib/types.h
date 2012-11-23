/*
 * OpenBOR - http://www.LavaLit.com
 * -----------------------------------------------------------------------
 * All rights reserved, see LICENSE in OpenBOR root for details.
 *
 * Copyright (c) 2004 - 2011 OpenBOR Team
 */

#ifndef GAMELIB_TYPES_H
#define GAMELIB_TYPES_H

#ifndef TRANSPARENT_IDX
#define		TRANSPARENT_IDX		0x00
#endif

#define	ANYNUMBER		 2

#if SYMBIAN
#pragma pack(4)
#elif DOS || SDL
#pragma pack(push,4)
#endif

#define PIXEL_8          0
#define PIXEL_x8         1
#define PIXEL_16         2
#define PIXEL_32         4


#define BLEND_SCREEN     0
#define BLEND_MULTIPLY   1
#define BLEND_OVERLAY    2
#define BLEND_HARDLIGHT  3
#define BLEND_DODGE      4
#define BLEND_HALF       5
#define BLEND_RGB        6

#define MAX_BLENDINGS    7
/*
#define _copy24bitp(pd, ps) (pd)[0] = (ps)[0]; (pd)[1] = (ps)[1]; (pd)[2] = (ps)[2];
#define _copy24bit(pd, v) (pd)[0] = ((unsigned char*)(&(v)))[0]; (pd)[1] = ((unsigned char*)(&(v)))[1]; (pd)[2] = ((unsigned char*)(&(v)))[2];
*/
extern int pixelformat;
extern int screenformat;
// in bitmap.c
extern int pixelbytes[(int)5];

#define PAL_BYTES (screenformat==PIXEL_8?768:(pixelbytes[(int)screenformat]*256))

// Define fixed-size integer types - these used to be in gfxtypes.h
// TODO: use these for throughout the engine where fixed-length types are needed
#if PSP
#include "psptypes.h"
#elif WII
#include "gctypes.h"
#elif ((__STDC_VERSION__ >= 199901L) || (defined(__GNUC__) && (__GNUC__ >= 3))) && (!defined(DC))
#include <stdint.h>
typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
#else // MSVC - no C99 support :(
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64; // FIXME: not sure if this will have the correct length in MSVC 64-bit mode
typedef unsigned long long u64;
#endif

// Define the "bool" type and standard values for "true" and "false"
#if (__STDC_VERSION__ >= 199901L) || (defined(__GNUC__) && (__GNUC__ >= 3))
#include <stdbool.h>
#else // MSVC again... :(
#if !defined(bool)
#define bool s8
#endif
#if !defined(true) && !defined(false)
#define true 1
#define false 0
#endif
#endif

//scr
#define screen_magic ((int)0x726373)

typedef struct{
	int magic;
	int	width;
	int	height;
	int pixelformat;
#if PSP
	int dummy[3]; //temporary debug values
#endif
	unsigned char* palette;
	unsigned char data[ANYNUMBER];
}s_screen;


typedef struct{
	int	width;
	int	height;
	int	planar;
	int	banked;		// Still unused
	unsigned char *	data;
}s_vram;


//bmp
#define bitmap_magic ((int)0x706d62)

typedef struct{
	int magic;
	int	width;
	int	height;
	int pixelformat;
#if PSP
	int dummy[3]; //temporary debug values
#endif
	unsigned char* palette;
	unsigned char data[ANYNUMBER];
}s_bitmap;

//spr
#define sprite_magic ((int)0x727073)

typedef struct spritestruct{
	int magic;
	int	centerx;
	int	centery;
	int offsetx;
	int offsety;
	int srcwidth;
	int srcheight;
	int	width;
	int	height;
	int pixelformat;
	struct spritestruct* mask;
	unsigned char* palette;
	int data[ANYNUMBER];
}s_sprite;

struct sprite_list{
	char *filename;
	s_sprite *sprite;
	int ref;
	struct sprite_list *next;
};
typedef struct sprite_list s_sprite_list;

typedef struct{
	s_sprite_list *node;
	int  centerx;
	int  centery;
}s_sprite_map;

void set_blendtables(unsigned char* tables[]); // set global blend tables for 8bit mode

typedef unsigned char (*transpixelfunc)(unsigned char* table, unsigned char src, unsigned char dest);
typedef unsigned short (*blend16fp)(unsigned short, unsigned short);
typedef unsigned (*blend32fp)(unsigned, unsigned);

blend16fp getblendfunction16(int alpha);
blend32fp getblendfunction32(int alpha);

extern blend16fp blendfunctions16[MAX_BLENDINGS];
extern blend32fp blendfunctions32[MAX_BLENDINGS];
extern unsigned char* blendtables[MAX_BLENDINGS];
extern unsigned channelr,channelg,channelb;
extern int usechannel;

unsigned short colour16(unsigned char r, unsigned char g, unsigned char b);
unsigned colour32(unsigned char r, unsigned char g, unsigned char b);

#if 0
void u8revcpy(unsigned char*pa, const unsigned char*pb, unsigned len);
void u8revpcpy(unsigned char*pa, const unsigned char*pb, unsigned char*pp, unsigned len);
void u8pcpy(unsigned char*pa, const unsigned char*pb, unsigned char* pp, unsigned len);

void u16revpcpy(unsigned short* pdest, const unsigned char* psrc, unsigned short* pp, unsigned len);
void u16pcpy(unsigned short* pdest, const unsigned char* psrc, unsigned short* pp, unsigned len);

void u32revpcpy(unsigned* pdest, const unsigned char* psrc, unsigned* pp, unsigned len);
void u32pcpy(unsigned* pdest, const unsigned char* psrc, unsigned* pp, unsigned len);
#endif

typedef struct{
	union
	{
	int amplitude;
	float beginsize;
	};
	union{
	float wavelength;
	float endsize;
	};
	int wavetime;
	union{
	float wavespeed;
	int	perspective;
	};
	int watermode;
}water_transform;

typedef struct
{
	unsigned char* table;
	void* fp;
	unsigned fillcolor;
	int flag:1;
	int alpha:8;
	int remap:8;
	int flipx:1;
	int flipy:1;
	int transbg:1;
	int fliprotate:1; // entity only, whether the flip is affected by the entity's facing(not the sprite's flip )
	int rotate:11; // 360 degrees
	int scalex;
	int scaley;
	int shiftx;
	int centerx;   // shift centerx
	int centery;   //shift centery
	int xrepeat;
	int yrepeat;
	int xspan;
	int yspan;
	unsigned char channelr;
	unsigned char channelg;
	unsigned char channelb;
	water_transform water;
}s_drawmethod;

typedef enum 
{
	gfx_screen,
	gfx_bitmap,
	gfx_sprite
}gfx_type;

/*
typedef enum
{
	ct_rectangle,
	ct_mask_screen

}clipping_type;


//screen clipping array clipp
typedef struct 
{
	int type;
	int x;
	int y;
	int width;
	int height;
	s_screen mask_screen;
}s_clipping;
*/

typedef struct 
{
	union{
		s_screen * screen;
		s_sprite * sprite;
		s_bitmap * bitmap;
		void*      handle;
	};
}gfx_entry;

typedef struct 
{
	int x; //x
	int y; //y
	int tx; //texture coords x
	int ty; //texture coords y
}vert2d;

typedef struct 
{
	int ulx; //upper left x
	int uly; //upper left y
	int lrx; //lower right x
	int lry; //lower right y
}rect2d;


typedef struct
{
	short hRes;        // Horizontal Resolution
	short vRes;		 // Vertical Resolution
	short hShift;	     // Offset for X-Axis Text
	short vShift;	     // Offset for Y-Axis Text
	short dOffset;	 // Offset for Debug Text
	short shiftpos[4];
	char filter;
	char mode;
	char pixel;
	float hScale;    // Multiplier for X-Axis
	float vScale;    // Multiplier for Y-Axis

}s_videomodes;

#if SYMBIAN
#pragma pack(0)
#elif DOS || SDL
#pragma pack(pop)
#endif

#endif



