#ifndef _VRE_APP_WIZARDTEMPLATE_
#define	_VRE_APP_WIZARDTEMPLATE_

#define MRE_STR_SIZE_MAX            (100)
#define MRE_FILE_CREATE_FAILED  	(-5)
#define MRE_FILE_OPEN_FAILED        (-3)
#define MRE_FILE_NOT_WRITTEN        (-4)
#define MRE_TEXTBOX_CANCEL          (2)
#define MRE_ERROR                   (-1)
#define MRE_GET_FILE_TIME_FAILED    (-6)
#define MRE_DEF_STRING_SIZE	        (30)
#define MRE_INTIAL_VALUE		(0)
#define MRE_SCREEN_START_X 		(0)
#define MRE_SCREEN_START_Y 		(0)

/* included files */

#include "vmio.h"
#include "vmgraph.h"
#include "vmstdlib.h"
#include <string.h>
#include "vmchset.h"
#include "stdint.h"

/* ---------------------------------------------------------------------------
 * global variables
 * ------------------------------------------------------------------------ */

VMINT		layer_hdl[1];				////layer handle array.

/* ---------------------------------------------------------------------------
 * local variables
 * ------------------------------------------------------------------------ */

/*****************************************************************************
 * FUNCTION
 *  handle_sysevt
 * DESCRIPTION
 *
 * PARAMETERS
 *
 * RETURNS
 *	none
*****************************************************************************/
void handle_sysevt(VMINT message, VMINT param);
/*****************************************************************************
 * FUNCTION
 *  handle_keyevt
 * DESCRIPTION
 *
 * PARAMETERS
 *
 * RETURNS
 *	none
*****************************************************************************/
void handle_keyevt(VMINT event, VMINT keycode);
/*****************************************************************************
 * FUNCTION
 *  file
 * DESCRIPTION
 *
 * PARAMETERS
 *
 * RETURNS
 *	none
*****************************************************************************/
VMINT file(VMWCHAR *FILE_PATH, VMINT wlen);
/*****************************************************************************
 * FUNCTION
 *  vertical_scrolling_ucs2_text
 * DESCRIPTION
 *
 * PARAMETERS
 *
 * RETURNS
 *	none
*****************************************************************************/
void vertical_scrolling_ucs2_text(VMWSTR ucs2_string);
/*****************************************************************************
 * FUNCTION
 *  mre_read_file_display
 * DESCRIPTION
 *
 * PARAMETERS
 *
 * RETURNS
 *	none
*****************************************************************************/
VMINT mre_read_file_display(VMWSTR path);
/*****************************************************************************
 * FUNCTION
 *  checkFileExist
 * DESCRIPTION
 *
 * PARAMETERS
 *
 * RETURNS
 *	none
*****************************************************************************/
void checkFileExist(void);
/*****************************************************************************
 * FUNCTION
 *  mre_draw_black_rectangle
 * DESCRIPTION
 *
 * PARAMETERS
 *
 * RETURNS
 *	none
*****************************************************************************/
void mre_draw_black_rectangle(void);
/*****************************************************************************
 * FUNCTION
 *  fromArrToScrolDisplay
 * DESCRIPTION
 * 
 * PARAMETERS
 *
 * RETURNS
 *
 *****************************************************************************/
void fromArrToScrolDisplay(void);

#endif


