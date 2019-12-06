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
#define MAXLINES  20
#define DIST      15
#define MINPOS    0
#define LINEWIDTH 90
#define FIFO "/tmp/zsiposfifo"

static GR_WINDOW_ID   g_wid;
static GR_SCREEN_INFO g_si;
static GR_GC_ID       g_gc;
static int            g_row = 0;

static void clear_screen()
{
    GrSetGCForeground(g_gc, GR_COLOR_BLACK);
    GrFillRect(g_wid, g_gc, 0, 0, g_si.cols, g_si.rows);

    GrSetGCForeground(g_gc, GR_COLOR_WHITE);
    GrSetGCBackground(g_gc, GR_COLOR_BLACK);
    g_row = 0;
}

static void draw_text(char *str, int row)
{
    GrText(g_wid, g_gc, 20, row, str, -1, GR_TFTOP);
}

static void nextline(char *str)
{
	char mytext[LINEWIDTH + 1];
	char myclear[LINEWIDTH * 2 + 1];
	int pos;

	if (strcmp(str, "#clear\n") == 0) {
		clear_screen();
		return;
	}

	// clear following line
	sprintf(myclear, "%*s", LINEWIDTH * 2, " ");
	if ((g_row + 1) < MAXLINES) {
		pos = (g_row + 1) * DIST;
		draw_text(myclear, pos);
	}
	// write line
	sprintf(mytext, "%.*s", LINEWIDTH, str);
	pos = g_row * DIST;
	draw_text(mytext, pos);
	// increment row
	g_row += 1;
	if (g_row == MAXLINES)
		g_row = 0;
}

int
main(int argc, char **argv)
{
    GR_EVENT event;
    FILE    *fp;
    char     text[LINEWIDTH+1];

    if (GrOpen() < 0) {
        fprintf(stderr, "Couldn't connect to Nano-X server\n");
        return 1;
    }

    GrGetScreenInfo(&g_si);
	g_wid = GrNewWindowEx(GR_WM_PROPS_NODECORATE, "nxdisplay",
			GR_ROOT_WINDOW_ID, 0, 0, g_si.cols, g_si.rows, GR_COLOR_BLACK);
	GrSelectEvents(g_wid, GR_EVENT_MASK_EXPOSURE);
	GrMapWindow(g_wid);

    g_gc = GrNewGC();
    clear_screen();

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
        while (fgets(text, LINEWIDTH, fp)) {
            nextline(text);
            GrCheckNextEvent(&event);
        }
        fclose(fp);
    }
    GrDestroyGC(g_gc);
    GrClose();
    return 0;
}
