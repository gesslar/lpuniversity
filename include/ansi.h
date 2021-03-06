//ansi.h (DO NOT EDIT THIS FILE)

//Tacitus @ LPUniversity
//20-OCT-05
//Standard LPUniversity Macro

/* Redefinition warning protection */

#ifndef ANSI_INCLU

/* Foreground Colours */

#define BLK "%^BLACK%^"       /* Black     */
#define RED "%^RED%^"         /* Red       */
#define GRN "%^GREEN%^"       /* Green     */
#define YEL "%^YELLOW%^"      /* Yellow    */
#define BLU "%^BLUE%^"        /* Blue      */
#define MAG "%^MAGENTA%^"     /* Magenta   */
#define CYN "%^CYAN%^"        /* Cyan      */
#define WHT "%^WHITE%^"       /* White     */

/* High Intensity Foreground Colours */

#define HIR "%^BOLD%^%^RED%^"      /* HI-Red      */
#define HIG "%^BOLD%^%^GREEN%^"    /* HI-Green    */
#define HIY "%^BOLD%^%^YELLOW%^"   /* HI-Yellow   */
#define HIB "%^BOLD%^%^BLUE%^"     /* HI-Blue     */
#define HIM "%^BOLD%^%^MAGENTA%^"  /* HI-Magenta  */
#define HIC "%^BOLD%^%^CYAN%^"     /* HI-Cyan     */
#define HIW "%^BOLD%^%^WHITE%^"    /* HI-White    */

/* Back Ground Colours */

#define BBLK "%^B_BLACK%^"    /* B-Black     */ 
#define BRED "%^B_RED%^"      /* B-Red       */
#define BGRN "%^B_GREEN%^"    /* B-Green     */
#define BYEL "%^B_YELLOW%^"   /* B-Yellow    */
#define BBLU "%^B_BLUE%^"     /* B-Blue      */
#define BMAG "%^B_MAGENTA%^"  /* B-Magenta   */
#define BCYN "%^B_CYAN%^"     /* B-Cyan      */
#define BWHT "%^B_WHITE%^"    /* B-White     */

/* High Intensity Background Colours */

#define HBBLK "%^BOLD%^%^B_BLACK%^"     /* HIB-Black   */
#define HBRED "%^BOLD%^%^B_RED%^"       /* HIB-Red     */
#define HBGRN "%^BOLD%^%^B_GREEN%^"     /* HIB-Green   */
#define HBYEL "%^BOLD%^%^B_YELLOW%^"    /* HIB-Yellow  */
#define HBBLU "%^BOLD%^%^B_BLUE%^"      /* HIB-Blue    */
#define HBMAG "%^BOLD%^%^B_MAGENTA%^"   /* HIB-Magenta */
#define HBCYN "%^BOLD%^%^B_CYAN%^"      /* HIB-Cyan    */
#define HBWHT "%^BOLD%^%^B_WHITE%^"     /* HIB-White   */

/* Other */
#define NOR "%^RESET%^" /* RESET */
#define ANSI_INCLU
#endif
