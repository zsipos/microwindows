####################################################################
# Microwindows and Nano-X configuration file
#
# This package can be configured to run on Linux (MIPS, ARM, POWERPC or x86)
# UNIX, ELKS, DJGPP, or RTEMS.
# On Linux, we've got drivers for Linux 2.x framebuffers, X11, or,
# svgalib for VGA hardware.
# In addition, a gpm or direct serial mouse driver can be configured.
# On ELKS, the bios screen driver and serial mouse driver are always used.
#
# Either Microwindows and/or Nano-X can be built.
# Microwindows and Nano-X have several demos.
#
# For MSDOS makes, see mcmwin.mak and mcnanox.mak
####################################################################

####################################################################
#
# build target platform
#
# Valid ARCH values are:
#
# LINUX-NATIVE
# LINUX-TCC
# LINUX-ARM
# LINUX-MIPS
# LINUX-POWERPC
# LINUX-SH
# FREEBSD-X86
# SOLARIS
# RTEMS
# DJGPP
# ELKS
#
# note: ELKS can't build client/server nano-X, nor widget lib
#
####################################################################
ARCH                     = LINUX-TCC
ARMTOOLSPREFIX           = arm-linux-
MIPSTOOLSPREFIX          = mipsel-linux-
POWERPCTOOLSPREFIX       = powerpc-linux-
SHTOOLSPREFIX            = sh-linux-gnu
RTEMSTOOLSPREFIX         = i386-rtemself-

####################################################################
#
# Compiling options
#
####################################################################
OPTIMIZE                 = Y
DEBUG                    = Y
VERBOSE                  = N
THREADSAFE               = N

####################################################################
#
# Libraries to build: microwin, nano-X, nanowidget, object frameworks
#
####################################################################
MICROWIN                 = Y
NANOX                    = N
SHAREDLIBS               = N
OBJFRAMEWORK             = N


####################################################################
#
# Demos to build
#
####################################################################
MICROWINDEMO             = Y
NANOXDEMO                = N

####################################################################
#
# Applications to build
#
####################################################################
NANOWM                   = Y

####################################################################
#
# The pixeltype of the native hardware or underlying graphics library.
# This definition defines the PIXELVAL to be 32, 16 or 8 bits wide.
# If using Linux framebuffer, set to MWPF_TRUECOLOR0888, and use fbset.
# It also enables GdArea/GrArea for this particular pixel packing format.
#
# define MWPF_PALETTE       /* pixel is packed 8 bits 1, 4 or 8 pal index*/
# define MWPF_TRUECOLOR0888 /* pixel is packed 32 bits 8/8/8 truecolor*/
# define MWPF_TRUECOLOR888  /* pixel is packed 24 bits 8/8/8 truecolor*/
# define MWPF_TRUECOLOR565  /* pixel is packed 16 bits 5/6/5 truecolor*/
# define MWPF_TRUECOLOR555  /* pixel is packed 16 bits 5/5/5 truecolor*/
# define MWPF_TRUECOLOR332  /* pixel is packed 8 bits 3/3/2 truecolor*/
#
####################################################################
SCREEN_PIXTYPE           = MWPF_TRUECOLOR0888

####################################################################
#
# NanoX: Put Y to the following line to link the nano-X application
# with the server.  This is required for ELKS, if no network is present,
# or for speed or debugging.  This affects the nano-X server only.
#
####################################################################
LINK_APP_INTO_SERVER     = N

####################################################################
# Shared memory support for Nano-X client/server protocol speedup
####################################################################
HAVE_SHAREDMEM_SUPPORT   = N

####################################################################
#
# File I/O support
# Supporting either below drags in libc stdio, which may not be wanted
#
####################################################################
HAVE_FILEIO              = Y

####################################################################
# BMP, GIF reading support
####################################################################
HAVE_BMP_SUPPORT         = Y
HAVE_GIF_SUPPORT         = Y
HAVE_PNM_SUPPORT         = Y
HAVE_XPM_SUPPORT         = Y

####################################################################
# JPEG support through libjpeg, see README.txt in contrib/jpeg
####################################################################
HAVE_JPEG_SUPPORT        = Y
INCJPEG                  = .
LIBJPEG                  = /usr/lib/libjpeg.a

####################################################################
# PNG support via libpng and libz
####################################################################
HAVE_PNG_SUPPORT         = N
INCPNG                   = /usr/include
LIBPNG                   = /usr/lib/libpng.a
LIBZ                     = /usr/lib/libz.a

####################################################################
# T1 adobe type1 font support thru t1lib
####################################################################
HAVE_T1LIB_SUPPORT       = N
INCT1LIB                 = /usr/include
LIBT1LIB                 = /usr/lib/libt1.a

####################################################################
# TrueType font support thru FreeType
####################################################################
HAVE_FREETYPE_SUPPORT    = Y
INCFTLIB                 = /usr/include
LIBFTLIB                 = /usr/lib/libttf.so
FREETYPE_FONT_DIR        = "fonts/truetype"

####################################################################
# PCF font support 
# Selecting HAVE_PCFGZ_SUPPORT will allow you to directly read
# .pcf.gz files, but it will add libz to the size of the server
####################################################################
HAVE_PCF_SUPPORT         = Y
HAVE_PCFGZ_SUPPORT       = Y

####################################################################
# Chinese Han Zi Ku font support
####################################################################
HAVE_HZK_SUPPORT         = N
HZK_FONT_DIR             = "fonts/chinese"

####################################################################
# Chinese BIG5 compiled in font support (big5font.c)
####################################################################
HAVE_BIG5_SUPPORT        = N

####################################################################
# Chinese GB2312 compiled in font support (gb2312font.c)
####################################################################
HAVE_GB2312_SUPPORT      = N

####################################################################
# Japanese JISX0213 compiled in font support (jisx0213-12x12.c)
####################################################################
HAVE_JISX0213_SUPPORT    = N

####################################################################
# Korean HANGUL font support (jo16x16.c)
####################################################################
HAVE_KSC5601_SUPPORT     = N

####################################################################
# Japanese EUC-JP support using MGL font
####################################################################
HAVE_EUCJP_SUPPORT       = N
EUCJP_FONT_DIR           = "fonts/japanese"

####################################################################
# Generate screen driver interface only with no fonts or clipping
####################################################################
NOFONTSORCLIPPING        = N

####################################################################
#
# Window move algorithms for Microwindows
# Change for tradeoff between cpu speed and looks
# ERASEMOVE repaints only backgrounds while window dragging, quicker.
# Otherwise an XOR redraw is used for window moves only after button up,
# quickest (should set for ELKS)
# UPDATEREGIONS paints in update clipping region only for better look and feel
#
####################################################################
ERASEMOVE                = Y
UPDATEREGIONS            = Y

####################################################################
#
# Use MS fonts (default no)
#
####################################################################
HAVEMSFONTS              = N

####################################################################
#
# Link with Gray Palette (valid only for 4bpp modes)
#
####################################################################
GRAYPALETTE              = N

####################################################################
#
# If the platform is running UNIX, Linux or RTEMS...
#
####################################################################
ifneq ($(ARCH), ELKS)

# X Window screen, mouse and kbd drivers
X11                      = N

ifeq ($(X11), Y)
# predefined model or select screen width, height, depth(palette mode only)
SCREEN_E15               = N
SCREEN_WIDTH             = 640
SCREEN_HEIGHT            = 480
SCREEN_DEPTH             = 4

# You may want to turn this on for XFree86 4.x or if your backing store
# isn't functioning properly
USE_EXPOSURE		 = Y

else

# framebuffer screen driver (linear and/or vga 4 planes)
# set VTSWITCH to include virtual terminal switch code
# set FBREVERSE to reverse bit orders in 1,2,4 bpp
# set FBVGA=N for all systems without VGA hardware (for MIPS must=N)
FRAMEBUFFER              = Y
FBVGA                    = N
VTSWITCH                 = Y
FBREVERSE                = N

# svgalib screen driver
VGALIB                   = N

# direct VGA hardware access screen driver
HWVGA                    = N

####################################################################
# Mouse or touch screen driver
# TPMOUSE for Linux-VR and Embedded Planet
# TPHELIO for VTech Helio
# ADSMOUSE for ADS Graphics Client
# IPAQMOUSE for Compaq iPAQ,Intel Assabet, ARM ucb1200 (/dev/h3600_ts 11,0)
# HARRIERMOUSE for NEC Harrier
# PSIONMOUSE for the Psion 5
# MTMOUSE for Microtouch touchpanel controller at RS232 port
####################################################################
GPMMOUSE                 = Y
SUNMOUSE                 = N
SERMOUSE                 = N
TPMOUSE                  = N
TPHELIO                  = N
ADSMOUSE                 = N
IPAQMOUSE                = N
HARRIERMOUSE             = N
PSIONMOUSE               = N
MTMOUSE                  = N
NOMOUSE                  = N

# keyboard or null kbd driver
TTYKBD                   = N
SCANKBD                  = Y
PIPEKBD                  = N
NOKBD                    = N

endif

####################################################################
# Screen driver specific configuration
# SA1100_LCD_LTLEND 4bpp driver with arm SA1100 LCD controller
# INVERT4BPP 4bpp inverted pixel driver for VTech Helio
####################################################################
SA1100_LCD_LTLEND        = N
INVERT4BPP               = N

####################################################################
#
# If the platform is a RTEMS box ....
#
####################################################################
ifeq ($(ARCH), RTEMS)

# Location & BSP information of the RTEMS build
RTEMS_BUILD              = /tools/build-i386-elf-rtems
RTEMS_BSP                = pc386
LINK_APP_INTO_SERVER     = Y

endif

endif

####################################################################
#
# If the platform is an ELKS box ...
#
####################################################################
ifeq ($(ARCH), ELKS)

# Higher speed asm driver, c driver of hercules screen driver
ASMVGADRIVER             = Y
CVGADRIVER               = N
HERCDRIVER               = N
DBGDRIVER                = N

# Mouse support
SERMOUSE                 = Y

endif
