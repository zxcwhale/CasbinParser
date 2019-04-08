/*
 * =====================================================================================
 *
 *       Filename:  casbin.c
 *
 *    Description:  casbin source file
 *
 *        Version:  1.0
 *        Created:  2019/3/22 15:28:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jarod Lee (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include "casbin.h"

/*
 * First two bytes of casbin message.
 */
#define BANNER "\xBA\xCE"

/*
 * casbin_reader_reset() - Reset casbin_reader
 * @r:  Reader.
 * Reset the casbin_reader to zero.
 */
void casbin_reader_reset(struct casbin_reader *r)
{
        memset(r, 0, sizeof(*r));
}

/*
 * casbin_reader_checksum() - Check the casbin message's checksum in casbin_reader
 * @r:  Reader.
 * If the checksum check is passed return 1, else return 0.
 */
int casbin_reader_checksum(struct casbin_reader *r)
{
        uint32_t chk = 0;
        memcpy(&chk, r->buf + 6 + r->header->len, 4);

        uint32_t sum = (r->header->sub_id << 24) + (r->header->cls_id << 16) + r->header->len;
        uint32_t *p = (uint32_t *)r->header->p;
        int i = 0;
        for (i = 0; i < r->header->len / 4; i++)
                sum += p[i];

        return chk == sum;
}

/*
 * casbin_reader_putbyte() - Put one byte into casbin_reader
 * @r:  Reader.
 * @c:  The byte to be put into casbin_reader.
 * If casbin_reader contains valid casbin message return 1, else return 0.
 */
int casbin_reader_putbyte(struct casbin_reader *r, uint8_t c)
{
        if (r->is_reset)
                casbin_reader_reset(r);

        if (c == 0xBA && memcmp(r->buf, BANNER, 2) != 0)
                casbin_reader_reset(r);

        r->buf[r->pos] = c;
        r->pos = (r->pos + 1) % MAX_READER_SIZE;

        if (r->pos >= 4 && memcmp(r->buf, BANNER, 2) == 0) {
                if (r->header->len > MAX_CASBIN_SIZE) {
                        r->is_reset = 1;
                        return 0;
                }

                if (r->header->len + 10 == r->pos) {
                        r->is_reset = 1;
                        if (casbin_reader_checksum(r))
                                return 1;

                }
        }
        return 0;
}
