/*****************************************************************************/

/*
 *      fskutil.c  --  utility functions for hfkernel's FSK modes
 *
 *      Copyright (C) 1996  Thomas Sailer (sailer@ife.ee.ethz.ch)
 *        Swiss Federal Institute of Technology (ETH), Electronics Lab
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/*****************************************************************************/

#include "fskl1.h"      
#include "fskutil.h"

/* --------------------------------------------------------------------- */

/* 
 * crc.c -- Calculate CRC
 * Mostly stolen from WAMPES
 */

/* --------------------------------------------------------------------- */

const unsigned short crc_ccitt_table[256] = {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
        0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
        0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
        0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
        0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
        0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
        0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
        0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
        0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
        0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
        0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
        0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
        0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
        0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
        0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
        0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

/* --------------------------------------------------------------------- */

long soft_time_dev(l1_soft_t *inp, int nbits, int spacing, l1_time_t tinc)
{
	long tacc = 0;
	int trcnt = 0;
	l1_soft_t *p1, *p2, c1, c2;
	int t1, t2;

	for (; nbits > 0; nbits--, inp += spacing) {
		p2 = (p1 = inp) + spacing;
		c1 = *p1++ > 0;
		c2 = *p2-- > 0;
		if (c1 == c2)
			continue;
		t1 = -spacing;
		t2 = spacing;
#if 0
		for (; (*p1 > 0) == c1; p1++, t1++);
		for (; (*p2 > 0) == c2; p2--, t2--);
#else
		if (c1) 
			for (; *p1 > 0; p1++, t1++);
		else
			for (; *p1 <= 0; p1++, t1++);
		if (c2) 
			for (; *p2 > 0; p2--, t2--);
		else
			for (; *p2 <= 0; p2--, t2--);
#endif
		tacc += (t1 + t2) * tinc;
		trcnt += 2; /* we add twice the required amount above */
	}
	if (trcnt <= 0)
		return 0;
	return tacc / trcnt;
}

__inline__ unsigned int hweight32(unsigned int w) 
{
        unsigned int res = (w & 0x55555555) + ((w >> 1) & 0x55555555);
        res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
        res = (res & 0x0F0F0F0F) + ((res >> 4) & 0x0F0F0F0F);
        res = (res & 0x00FF00FF) + ((res >> 8) & 0x00FF00FF);
        return (res & 0x0000FFFF) + ((res >> 16) & 0x0000FFFF);
}

__inline__ unsigned int hweight16(unsigned short w)
{
        unsigned short res = (w & 0x5555) + ((w >> 1) & 0x5555);
        res = (res & 0x3333) + ((res >> 2) & 0x3333);
        res = (res & 0x0F0F) + ((res >> 4) & 0x0F0F);
        return (res & 0x00FF) + ((res >> 8) & 0x00FF);
}

__inline__ unsigned int hweight8(unsigned char w)
{
        unsigned short res = (w & 0x55) + ((w >> 1) & 0x55);
        res = (res & 0x33) + ((res >> 2) & 0x33);
        return (res & 0x0F) + ((res >> 4) & 0x0F);
}

__inline__ void soft_to_hard(const int *inp, unsigned char *out, 
				    int nsamp, int spacing, int inv)
{
	unsigned char mask = 1;
	unsigned char invm = inv ? 0 : 0xff;

	memset(out, 0, (nsamp + 7) >> 3);
	while (nsamp > 0) {
		*out |= ((*inp >> 31) ^ invm) & mask;
		mask <<= 1;
		if (mask == 0) {
			mask = 1;
			out++;
		}
		inp += spacing;
		nsamp--;
	}
}

__inline__ void invert(unsigned char *c, int len)
{
	for (; len > 0; len--, c++)
		*c ^= 0xff;
}

__inline__ int freq_tracking(l1_soft_t *inmid, l1_soft_t *inlo, 
				    l1_soft_t *inhi, int nbits,
				    int spacing, l1_soft_t *valmid, 
				    l1_soft_t *vallo, l1_soft_t *valhi)
{
	l1_soft_t vm = 0, vl = 0, vh = 0;

	for (; nbits > 0; nbits--, inmid += spacing, inlo++, inhi++) {
		if (*inmid > 0) {
			vm += *inmid;
			vl += *inlo;
			vh += *inhi;
		} else {
			vm -= *inmid;
			vl -= *inlo;
			vh -= *inhi;
		}
	}
	*valmid = vm;
	*vallo = vl;
	*valhi = vh;
	if (vm > vl && vm > vh)
		return 0;
	if (vl > vh)
		return -1;
	return 1;
}

__inline__ unsigned int calc_crc_ccitt(unsigned char *data, int len,
					      unsigned int preset)
{
        unsigned int crc = preset;
        
        for(; len > 0; len--, data++)
                crc = ((crc >> 8) & 0xff) ^
                        crc_ccitt_table[(crc ^ *data) & 0xff];
        return (~crc) & 0xffff;
}


__inline__ unsigned int gtor_calc_crc_ccitt(unsigned char *data, 
						    int len, 
						   unsigned int preset)
{
        unsigned int crc = preset;
        
        for(; len > 2; len--, data++)
                crc = ((crc >> 8) & 0xff) ^
                        crc_ccitt_table[(crc ^ *data) & 0xff];
	crc = ((crc >> 8) & 0xff) ^ crc_ccitt_table[(crc ^ data[1]) & 0xff];
	crc = ((crc >> 8) & 0xff) ^ crc_ccitt_table[(crc ^ data[0]) & 0xff];
	return (~crc) & 0xffff;
}

/* --------------------------------------------------------------------- */
