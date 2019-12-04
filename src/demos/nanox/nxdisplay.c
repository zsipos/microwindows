/*
nxdisplay : Show msg in screen
Author: Esther Bergter
Version 0.1 14.06.2018
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "nano-X.h"
#include "nxcolors.h"

#define DEFAULT_DATA_FILE "/etc/nxcal.dat"
#define MAXLINES 20
#define DIST     15
#define MINPOS   0
#define LINEWIDTH 80
#define FIFO "/tmp/zsiposfifo"

static GR_WINDOW_ID g_wid;
static GR_SCREEN_INFO g_si;

static int g_rotate = MWPORTRAIT_NONE;

void
draw_text(char *str, int row, GR_GC_ID gc)
{
    int tw, th, tb;

    //GrGetGCTextSize(gc, str, -1, GR_TFTOP, &tw, &th, &tb);
    GrText(g_wid, gc, 20, row, str, -1, GR_TFTOP);
}

void nextline(char *str, GR_GC_ID gc)
{
  static int row = 0;
  char mytext[LINEWIDTH+1];
  char myclear[LINEWIDTH*2+1];
  int pos;

  // clear following line
  sprintf(myclear, "%*s", LINEWIDTH*2, " ");
  pos = ((row + 1) % MAXLINES) * DIST; 
  draw_text(myclear, pos, gc);
  // write line
  sprintf(mytext, "%.*s", LINEWIDTH, str);
  pos = row * DIST;
  draw_text(mytext, pos, gc);
  // increment row
  row = (row + 1) % MAXLINES; 
 }

int
main(int argc, char **argv)
{
    int ret = 0;
    int count = 0;
    FILE *fp;
    char text[LINEWIDTH+1];

    if (GrOpen() < 0) {
        fprintf(stderr, "Couldn't connect to Nano-X server\n");
        return 1;
    }

    GrGetScreenInfo(&g_si);
    g_rotate = g_si.portrait;
    g_wid = GrNewWindowEx(GR_WM_PROPS_NODECORATE, "nxcal", GR_ROOT_WINDOW_ID,
        0, 0, g_si.cols, g_si.rows, GR_COLOR_BLACK);
    GrSelectEvents(g_wid, GR_EVENT_MASK_EXPOSURE);
    GrMapWindow(g_wid);
    // warte auf startschuss
    while (1) {
        GR_EVENT event;
        GrGetNextEvent(&event);

        if (event.type == GR_EVENT_TYPE_EXPOSURE) {
            break;
        }
    }
    GR_GC_ID gc = GrNewGC();
    GrSetGCForeground(gc, MWRGB(30, 30, 30));
    GrFillRect(g_wid, gc, 0, 0, g_si.cols, g_si.rows);

    GrSetGCForeground(gc, GR_COLOR_WHITE);
    GrSetGCBackground(gc, MWRGB(30, 30, 30));

    unlink(FIFO); // ignore error
    while (1) {
        if ((fp = fopen(FIFO, "r")) == NULL) {
            if (mkfifo(FIFO, 0666)) {
                perror("mkfifo");
                break;
            }
            if ((fp = fopen(FIFO, "r")) == NULL) { // immer noch nicht
                perror("fopen FIFO");
                break;
            }
        }
        while (fgets(text, LINEWIDTH, fp))
            nextline(text, gc);
        fclose(fp);
    }
    GrDestroyGC(gc);
    GrClose();
    return 0;
}
