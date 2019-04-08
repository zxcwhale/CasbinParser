#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "casbin.h"

/*
 * parse() - Parse casbin message, you can custom it.
 * @cls_id:     Class ID.
 * @sub_id:     Sub ID.
 * @p:          Pointer to message's payload.
 */
void parse(const uint8_t cls_id, const uint8_t sub_id, const uint8_t *p)
{
        if (cls_id == ClassId_Nav && sub_id == NavSubId_Pv) {// NAV_PV
                const struct nav_pv *pv = (const struct nav_pv *)p;
                printf("navpv: run_time=%-10d ", pv->runTime);
                printf("lat=%-14.8f ", pv->lat);
                printf("lon=%-14.8f ", pv->lon);
                printf("spd3d=%-8.3f ", pv->speed3D);
                printf("spd2d=%-8.3f ", pv->speed2D);
                printf("heading=%-8.2f ", pv->heading);
                printf("\n");
        }
}

/*
 * on_recv_data() - On received casbin data.
 * @buf:        Buffer of casbin.
 * @size:       Size of buf
 * @r:          Pointer to casbin reader
 */
void on_recv_data(const uint8_t *buf, const int size, struct casbin_reader *r)
{
        int i;
        for (i = 0; i < size; i++)
                if (casbin_reader_putbyte(r, buf[i]))
                        parse(r->header->cls_id, r->header->sub_id, r->header->p);
}

/*
 * main() - Parse casbin data from sample file.
 */
int main(int argc, char *argv[])
{
        int readn = 0;
        uint8_t buf[512];
        FILE *f;
        struct casbin_reader r;

        if (argc != 2) {
                fprintf(stderr, "Command format:\n\t%s [sample file]", argv[0]);
                exit(1);

        }

        f = fopen(argv[1], "rb");
        if (f == NULL) {
                fprintf(stderr, "Can't open %s: %s\n", argv[1], strerror(errno));
                exit(1);
        }

        casbin_reader_reset(&r);
        while((readn = fread(buf, 1, sizeof(buf), f)) > 0)
                on_recv_data(buf, readn, &r);

        return 0;
}
