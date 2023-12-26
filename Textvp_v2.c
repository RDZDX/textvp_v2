#include "Textvp_v2.h"

VMWCHAR file_pathw[100];
VMUINT8 *buffer;
VMFILE f_read;

int filledDsplByLines = 0;
int dynamicPosition = 0;
int fiksedPosition = 0;
int prviousFlPosit = 0;
int endFlPosition = 0;
int myFlPosOld = 0;
int myFlPosOld1 = 0;
int myFlPosOld2 = 0;
int pageCounter = 0;
int lastPressedKey = 0;
int endPressedKey = 0;

int failoGalasPriesTai = 0;

int lapu_kiekis = 0;
int simbl_kiek_pirm_lape = 0;
int x5_puslapiu_adr = 0;
int xPressedKey = 0;
int x10_procentu_lapu = 0;
int x10_procentu_adr = 0;
int trigeris_end = 0;
int selector_use = 0;
int clean_array = 0;
int plusPage = 0;

int myFlPosOld2Temp = 0;
int myFlPosOld1Trig = 0;


int simblKiekEinLape = 0;

//VMWSTR oneDisplay[20][61] = {0,0};
VMWCHAR oneDisplay[20][61];

VMINT trigeris = 1;
VMINT trigeris1 = 0;
VMINT trigeris2 = 0;
VMINT trigeris3 = 0;
VMINT trigeris4 = 0;
VMINT trigeris5 = 0;

/*****************************************************************************
 * FUNCTION
 *  vm_main
 * DESCRIPTION
 *
 * PARAMETERS
 *
 *
 * RETURNS
 *	none
 *****************************************************************************/

void vm_main(void) {
    layer_hdl[0] = -1;
    vm_reg_sysevt_callback(handle_sysevt);
    vm_reg_keyboard_callback(handle_keyevt);
    vm_font_set_font_size(VM_SMALL_FONT);
    checkFileExist();
}

/*****************************************************************************
 * FUNCTION
 *  handle_sysevt
 * DESCRIPTION
 *  This function handles system events
 * PARAMETERS
 *  event		    	[IN]      message id
 *  param               [IN]      parameter
 * RETURNS
 *	none
 *****************************************************************************/

void handle_sysevt(VMINT message, VMINT param) {

    switch (message) {
        case VM_MSG_CREATE:
        case VM_MSG_ACTIVE:
            layer_hdl[0] =
                vm_graphic_create_layer(0, 0, vm_graphic_get_screen_width(),
                                        vm_graphic_get_screen_height(), -1);
            vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(),
                                vm_graphic_get_screen_height());
            buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);
            vm_switch_power_saving_mode(turn_off_mode);
            break;

        case VM_MSG_PAINT:
            vm_switch_power_saving_mode(turn_off_mode);
            
            if (trigeris1 == 1) {trigeris2 = 1;}
            if (trigeris == 0) {trigeris1 = 1;}
            if (trigeris2 == 1) {vm_exit_app();}

            if (dynamicPosition != 0) {
               fromArrToScrolDisplay();
            } else {
               mre_read_file_display(file_pathw);
               if (failoGalasPriesTai != 1) {mre_read_file_display(file_pathw);}
            }

            break;

        case VM_MSG_INACTIVE:

            vm_switch_power_saving_mode(turn_on_mode);
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;

        case VM_MSG_QUIT:
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;
    }
}

/*****************************************************************************
 * FUNCTION
 *  handle_keyevt
 * DESCRIPTION
 *
 * PARAMETERS
 *
 *
 * RETURNS
 *	none
 *****************************************************************************/

void handle_keyevt(VMINT event, VMINT keycode) {
    VMWCHAR *FILE_PATH = NULL;
    VMCHAR cPageCnter[101] = {0};
    VMWCHAR cPageCnterX[101];
    VMCHAR cpercentPosition[101] = {0};
    VMWCHAR cpercentPositionX[101];

    int percentData; 

    int trigerisq = 0;     //trigeris nustatymui "fiksedPosition = 0" po uzkrauto puslapio 
    int countertriger = 0; //trigeris didinti +1 kounteri ar ne

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_RIGHT_SOFTKEY) {
        if (layer_hdl[0] != -1) {
            vm_graphic_delete_layer(layer_hdl[0]);
            layer_hdl[0] = -1;
        }
        vm_exit_app();
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_LEFT_SOFTKEY) {
        selector_use = 1;
        vm_selector_run(0, FILE_PATH, file);
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_DOWN) {
       //Jei ne failo galas pries uzkrovima
       if (fiksedPosition != dynamicPosition) {
          prviousFlPosit = fiksedPosition;
       }

       fiksedPosition = dynamicPosition;

       fromArrToScrolDisplay();
       if (failoGalasPriesTai != 1) {mre_read_file_display(file_pathw);}

       //Jei ne failo galas po uzkrovimo
       if (fiksedPosition != dynamicPosition) {
          countertriger = 1;
       }

       //Jei failo galas ir failoGalasPriesTai == 0 && pageCounter != 1 arba dvieju lapu tekstas
       if (fiksedPosition == dynamicPosition && failoGalasPriesTai == 0 && pageCounter != 1 || fiksedPosition == dynamicPosition && prviousFlPosit == 0 && failoGalasPriesTai == 0) {
          countertriger = 1;
          failoGalasPriesTai = 1;
       }

       if (dynamicPosition > 0 && dynamicPosition == prviousFlPosit && failoGalasPriesTai == 0 && pageCounter == 1) {
          countertriger = 1;
          failoGalasPriesTai = 1;
       }

       //Jei antras lapas ir lastPressedKey == 2 ?
       if (dynamicPosition != 0 && dynamicPosition == fiksedPosition && lastPressedKey == 2) {
          countertriger = 1;
       }

       if (dynamicPosition == 0 && fiksedPosition == 0 && prviousFlPosit == 0) {
          countertriger = 0;
       }

      if (pageCounter == lapu_kiekis) { countertriger = 0; }

      //vieno lapo tekstas
      if (countertriger == 1) {pageCounter = pageCounter + 1;}

      if (xPressedKey == 1) {xPressedKey = 0;}

      myFlPosOld2Temp = 0;

      //myFlPosOld2 = 0;

      myFlPosOld1Trig = 0;

      filledDsplByLines = 0;  

      lastPressedKey = 3;

    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_UP) {
        trigeris3 = 0;
        if (fiksedPosition == prviousFlPosit && pageCounter != lapu_kiekis && xPressedKey != 1 || prviousFlPosit == 0 && dynamicPosition != 0 ) {
            dynamicPosition = 0;
            trigerisq = 1;
        }
        else if (dynamicPosition - fiksedPosition == myFlPosOld1 && myFlPosOld2 == 0 || myFlPosOld1Trig == 1) {
           myFlPosOld1Trig = 1;
           dynamicPosition = myFlPosOld1;
        }
        else { 
           if (myFlPosOld2Temp == 0) {myFlPosOld2Temp = myFlPosOld2;}
           if (lastPressedKey == 2) {myFlPosOld2 = myFlPosOld2Temp;}
           dynamicPosition = myFlPosOld2;

        }

        if (pageCounter != 1 && fiksedPosition != dynamicPosition && lastPressedKey != 2) {
           pageCounter = pageCounter - 1;
        }

        clean_array = 1;
        selector_use = 1;

        mre_read_file_display(file_pathw);
        clean_array = 0;
        selector_use = 0;

        if (failoGalasPriesTai != 1) {mre_read_file_display(file_pathw);}

        if (trigerisq == 1) {
           fiksedPosition = 0;
           trigerisq = 0;
        }

        filledDsplByLines = 0;  
        lastPressedKey = 2;
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM1) {
        //percentData = (dynamicPosition * 100) / endFlPosition;
        percentData = (simblKiekEinLape * 100) / endFlPosition;

        if (failoGalasPriesTai == 1) { percentData = 100; }
        if (lapu_kiekis < 0) { lapu_kiekis = 1; }

        if (endPressedKey == 1) {
           endPressedKey = 0;
           pageCounter = lapu_kiekis;
        }

        vertical_scrolling_ucs2_text(file_pathw);

        sprintf(cPageCnter, "Page # %d of %d", pageCounter + plusPage, lapu_kiekis + plusPage);
        vm_ascii_to_ucs2(cPageCnterX, (strlen(cPageCnter) + 1) * 2, cPageCnter);
        vertical_scrolling_ucs2_text(cPageCnterX);

        percentData = (dynamicPosition * 100) / endFlPosition;
        if (failoGalasPriesTai == 1) { percentData = 100; }
        if (pageCounter == lapu_kiekis) { percentData = 100; }

        sprintf(cpercentPosition, "Percent: %d", percentData);
        vm_ascii_to_ucs2(cpercentPositionX, (strlen(cpercentPosition) + 1) * 2, cpercentPosition);
        vertical_scrolling_ucs2_text(cpercentPositionX);
        filledDsplByLines = 0;  
       
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM9) {
       //if (endFlPosition > 380) {
       if (endFlPosition > 1200) {
          dynamicPosition = endFlPosition - 380;
          fiksedPosition = endFlPosition - 380;
          prviousFlPosit = endFlPosition - 380;
          percentData = 100;
          failoGalasPriesTai = 1;
          mre_read_file_display(file_pathw);
          endPressedKey = 1;
          trigeris_end = 1;
       }

    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM7) {
        dynamicPosition = 0;
        fiksedPosition = 0;
        prviousFlPosit = 0;
        lapu_kiekis = 0;
        failoGalasPriesTai = 0;
        lastPressedKey = 0;
        mre_read_file_display(file_pathw);
        pageCounter = 1;
        myFlPosOld1 = 0;
        trigeris_end = 0;

    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM8) {


       x5_puslapiu_adr = dynamicPosition + (simbl_kiek_pirm_lape * 4);

       if (x5_puslapiu_adr < endFlPosition - 380 && endFlPosition > 380 ) {
       //if (x5_puslapiu_adr < endFlPosition - 380) {
          dynamicPosition = x5_puslapiu_adr;
          fiksedPosition = x5_puslapiu_adr;
          prviousFlPosit = x5_puslapiu_adr;
          pageCounter = pageCounter + 5;
          mre_read_file_display(file_pathw);
          xPressedKey = 1;
       }

       if (x5_puslapiu_adr >= endFlPosition - 380 && trigeris_end != 1) {
          dynamicPosition = endFlPosition - 380;
          fiksedPosition = endFlPosition - 380;
          prviousFlPosit = endFlPosition - 380;
          percentData = 100;
          failoGalasPriesTai = 1;
          mre_read_file_display(file_pathw);
          endPressedKey = 1;
          trigeris_end = 1;
          pageCounter = lapu_kiekis;
       }

    }

 if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM6) {

       x10_procentu_lapu = (lapu_kiekis * 9) / 100;
       x10_procentu_adr = dynamicPosition + (simbl_kiek_pirm_lape * x10_procentu_lapu);

       if (x10_procentu_lapu >= 1 && x10_procentu_adr < endFlPosition - 380) {
          dynamicPosition = x10_procentu_adr;
          fiksedPosition = x10_procentu_adr;
          prviousFlPosit = x10_procentu_adr;
          pageCounter = pageCounter + x10_procentu_lapu + 1;
          mre_read_file_display(file_pathw);
          xPressedKey = 1;
       }

       if (x10_procentu_adr >= endFlPosition - 380 && trigeris_end != 1) {
          dynamicPosition = endFlPosition - 380;
          fiksedPosition = endFlPosition - 380;
          prviousFlPosit = endFlPosition - 380;
          percentData = 100;
          failoGalasPriesTai = 1;
          mre_read_file_display(file_pathw);
          endPressedKey = 1;
          trigeris_end = 1;
       }

    }

 if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM2) {
        fromArrToScrolDisplay();
    }
}

/*****************************************************************************
 * FUNCTION
 *  file
 * DESCRIPTION
 *
 * PARAMETERS
 *
 *
 * RETURNS
 *	none
 *****************************************************************************/

VMINT file(VMWCHAR *FILE_PATH, VMINT wlen) {

    trigeris = 1;
    trigeris1 = 0;
    trigeris2 = 0;
    trigeris3 = 0;
    trigeris4 = 0;
    trigeris5 = 0;

    vm_wstrcpy(file_pathw, FILE_PATH);
    pageCounter = 1;
    failoGalasPriesTai = 0;
    filledDsplByLines = 0;
    dynamicPosition = 0;
    fiksedPosition = 0;
    prviousFlPosit = 0;
    lastPressedKey = 0;
    lapu_kiekis = 0;
    simbl_kiek_pirm_lape = 0;
    x5_puslapiu_adr = 0;
    xPressedKey = 0;
    x10_procentu_lapu = 0;
    x10_procentu_adr = 0;
    trigeris_end = 0;
    simblKiekEinLape = 0;
    f_read = vm_file_open(file_pathw, MODE_READ, FALSE);
    vm_file_seek(f_read, 0, BASE_END);
    endFlPosition = vm_file_tell(f_read);
    vm_file_close(f_read);

    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  mre_read_file_display
 * DESCRIPTION
 *  This function displays file's cotent on display as 20 text lines
 *  which is actually input textbox
 * PARAMETERS
 *  file name				[IN]      name of file
 * RETURNS
 *	result                  [OUT]     some code for error and success
 *****************************************************************************/
VMINT mre_read_file_display(VMWSTR path) {

    VMUINT nread;

    VMWCHAR sValue21[MRE_STR_SIZE_MAX + 23] = {0};
    VMWCHAR sKonv_stringas[MRE_STR_SIZE_MAX + 23] = {0};
    VMCHAR value1[60 + 1] = {0};
    VMCHAR vns_simbl[1 + 1] = {0};
    VMCHAR value10[MRE_STR_SIZE_MAX + 23] = {0}; //uodega max 15 simboliu ilgio? 20 ?
    VMCHAR nauj_strng[60 + 1] = {0};
    VMCHAR value21[MRE_STR_SIZE_MAX + 23] = {0}; //76x2 ?
    //VMWCHAR value21[MRE_STR_SIZE_MAX + 23];
    VMCHAR konv_stringas[MRE_STR_SIZE_MAX + 23] = {0}; //124?
    //VMWCHAR konv_stringas[MRE_STR_SIZE_MAX + 23];

    int myFlPosBackCurr = 0;

    if(trigeris5 == 0) {mre_draw_black_rectangle();}

    if (clean_array == 0) {
       memset(oneDisplay, 0, sizeof(oneDisplay[0][0]) * 20 * 61);
    }

    f_read = vm_file_open(path, MODE_READ, FALSE);

    vm_file_seek(f_read, dynamicPosition, BASE_CURR);  // Permetame kursoriu i pozicija

    myFlPosOld2 = myFlPosOld;
    myFlPosOld = prviousFlPosit;
    myFlPosBackCurr = dynamicPosition;
    if (myFlPosOld1 == 0) {myFlPosOld1 = dynamicPosition;}

    int i, plotis, strng_plot, isve_i_ekr_eil_sk, nauj_strng_ilg, combUodegosIlgis, uodegosIlgis, prasisk_count, prasisk_count1, prasisk_count2, ilgism;
    
    plotis = 0;
    strng_plot = 0;
    isve_i_ekr_eil_sk = 0;
    nauj_strng_ilg = 0;
    combUodegosIlgis = 0;
    uodegosIlgis = 0;
    prasisk_count = 0;
    plusPage = 0;
    prasisk_count1 = 0;
    prasisk_count2 = 0;
    ilgism = 0;

    for (i = 0; i < 20; i++)  // 20 tekstiniu ekrano eiluciu
    {

        if (ilgism < 61) {
           vm_file_read(f_read, value1, 61, &nread);  // constants: on 240' display on one text line 60 slimest symbols "." or 15 widest symbols "--"
           value1[nread] = '\0';
           dynamicPosition = vm_file_tell(f_read);  // Issaugome dabartine kursoriaus pozicija faile
           strcat(value10, value1);
           ilgism = strlen(value10);
        }

        vm_chset_convert(VM_CHSET_UTF8, VM_CHSET_UCS2, value10, value21, (ilgism + 1) * 2);
        //vm_ascii_to_ucs2(value21, (ilgism + 1) * 2, value10);
        vm_wstrcpy(sValue21, (VMWSTR)value21);
        plotis = vm_graphic_get_string_width(sValue21);
        prasisk_count2 = 0;

        if (plotis < 228 && prasisk_count < 3 && !vm_file_is_eof(f_read) || plotis == 228 && prasisk_count < 3 && !vm_file_is_eof(f_read)) {
           prasisk_count = prasisk_count + 1;
           if (prasisk_count != 0){prasisk_count1 = 1;} 
           continue;
        }


        prasisk_count = 0;
        char *ptr = value10;

        while (*ptr != '\0') {

            if (*ptr != '\n') { 
               sprintf(vns_simbl, "%c", *ptr);
               strcat(nauj_strng, vns_simbl);
            }

            ilgism = strlen(nauj_strng);

            //if (ilgism > 14 || *ptr == '\n' || prasisk_count2 > 14 || ilgism < 15) {
            if (isve_i_ekr_eil_sk < 20 && ilgism > 14 || isve_i_ekr_eil_sk < 20 && *ptr == '\n' || isve_i_ekr_eil_sk < 20 && prasisk_count2 > 14) {
               vm_chset_convert(VM_CHSET_UTF8, VM_CHSET_UCS2, nauj_strng, konv_stringas, (ilgism + 1) * 2);
               //vm_ascii_to_ucs2(konv_stringas, (ilgism + 1) * 2, nauj_strng);
               vm_wstrcpy(sKonv_stringas, (VMWSTR)konv_stringas);
               strng_plot = vm_graphic_get_string_width(sKonv_stringas);
            }

            if (strng_plot > 235 && isve_i_ekr_eil_sk < 20 || *ptr == '\n' && isve_i_ekr_eil_sk < 20) {

                //if (fiksedPosition == 0 || selector_use == 1) {vertical_scrolling_ucs2_text(sKonv_stringas);}
                if (fiksedPosition == 0 && trigeris3 == 0 || selector_use == 1 && trigeris3 == 0) {
                   vertical_scrolling_ucs2_text(sKonv_stringas);
                }
                if (clean_array == 0) {
                   vm_wstrcpy((VMWSTR)&oneDisplay[isve_i_ekr_eil_sk - 1][61], sKonv_stringas);
                   trigeris4 = 1;
                }
                isve_i_ekr_eil_sk = isve_i_ekr_eil_sk + 1;
                strcpy(nauj_strng, "");
                strng_plot = 0;
            }

            prasisk_count2 = prasisk_count2 + 1;
            ptr++;
        }

        strcpy(value10, nauj_strng);
        strcpy(nauj_strng, "");
        if (vm_file_is_eof(f_read) || isve_i_ekr_eil_sk == 20) { break; }

    }
 
     dynamicPosition = dynamicPosition - ilgism;
     if (endFlPosition > 0 && dynamicPosition > endFlPosition) {dynamicPosition = endFlPosition;}

     simblKiekEinLape = dynamicPosition;

    if (vm_file_is_eof(f_read)) {

        if (isve_i_ekr_eil_sk < 20 && strng_plot != 0 && sKonv_stringas != '\0') {

            //if (fiksedPosition == 0 || selector_use == 1) {vertical_scrolling_ucs2_text(sKonv_stringas);}
            if (fiksedPosition == 0 && trigeris3 == 0 || selector_use == 1 && trigeris3 == 0) {
               vertical_scrolling_ucs2_text(sKonv_stringas);
            }
            if (clean_array == 0) {
               vm_wstrcpy((VMWSTR)&oneDisplay[isve_i_ekr_eil_sk - 1][61], sKonv_stringas);
               trigeris4 = 1;
            }
            dynamicPosition = myFlPosBackCurr;
            if (isve_i_ekr_eil_sk == 0 && wstrlen(sKonv_stringas) < 7 && dynamicPosition != 0) {plusPage = 1;}
        }

        if (endFlPosition == dynamicPosition) {dynamicPosition = myFlPosBackCurr;}

        strcpy(value10, "");
    }

    vm_file_close(f_read);

     if (lapu_kiekis == 0) {

       lapu_kiekis = endFlPosition / dynamicPosition;
       if (((float)endFlPosition / dynamicPosition) - lapu_kiekis != 0) {lapu_kiekis = lapu_kiekis + 1;}

    }

    if (simbl_kiek_pirm_lape == 0) {

       simbl_kiek_pirm_lape = dynamicPosition;

    }

    if (selector_use == 1) {
       selector_use = 0;
    }

    filledDsplByLines = 0;
    if (trigeris4 == 1) {trigeris3 = 1;}
    trigeris5 = 1;
    return 0;
}

/*****************************************************************************
 * FUNCTION
 *  vertical_scrolling_ucs2_text
 * DESCRIPTION
 *
 * PARAMETERS
 *
 * RETURNS
 *
 *****************************************************************************/
void vertical_scrolling_ucs2_text(VMWSTR ucs2_string) {
    int max_height;
    int font_height = 16;
    int i;
    int abc = 0;

    if (filledDsplByLines == 0) {
       abc = 2;
       mre_draw_black_rectangle();
    }

    max_height = vm_graphic_get_screen_height();
    vm_graphic_clear_layer_bg(layer_hdl[0]);
    //vm_graphic_textout_by_baseline(buffer, MRE_SCREEN_START_X, filledDsplByLines + abc, ucs2_string, wstrlen(ucs2_string), VM_COLOR_WHITE, 12);
    vm_graphic_textout_by_baseline(buffer, MRE_SCREEN_START_X, filledDsplByLines + abc, ucs2_string, wstrlen(ucs2_string), VM_COLOR_BLACK, 12);
    vm_graphic_flush_layer(layer_hdl, 1);
    filledDsplByLines += font_height;
}

/*****************************************************************************
 * FUNCTION
 *  checkFileExist
 * DESCRIPTION
 *
 * PARAMETERS
 *
 * RETURNS
 *
 *****************************************************************************/
void checkFileExist(void) {

    VMINT drv;
    VMCHAR f_name[MRE_STR_SIZE_MAX + 1];
    VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1];
    int size = 0;

    VMWCHAR fullPath[100];
    VMWCHAR appName[100];
    VMWCHAR newName[100];
    VMCHAR asciiAppName[100];
    VMCHAR file_name[100];

    vm_get_exec_filename(fullPath);
    vm_get_filename(fullPath, appName);
    vm_ucs2_to_ascii(asciiAppName, 100, appName);
    memcpy(file_name, asciiAppName, strlen(asciiAppName) - 3);
    file_name[strlen(asciiAppName) - 3] = '\0';
    strcat(file_name, "txt");

    if ((drv = vm_get_removable_driver()) < 0) {
        drv = vm_get_system_driver();
    }

    sprintf(f_name, "%c:\\%s", drv, file_name);
    vm_ascii_to_ucs2(file_pathw, (strlen(f_name) + 1) * 2, f_name);
    f_read = vm_file_open(file_pathw, MODE_READ, FALSE);

    pageCounter = 1;

    if (f_read < 0) {
        vm_file_close(f_read);
        selector_use = 1;
        trigeris = vm_selector_run(0, 0, file);
    } else {
        vm_file_seek(f_read, 0, BASE_END);
        endFlPosition = vm_file_tell(f_read);
        vm_file_close(f_read);
    }
}

/*****************************************************************************
 * FUNCTION
 *  mre_draw_black_rectangle
 * DESCRIPTION
 *  This function draws rectangle on screen
 * PARAMETERS
 *
 * RETURNS
 *
 *****************************************************************************/
void mre_draw_black_rectangle(void) {

    filledDsplByLines = 0;

    //vm_graphic_fill_rect(buffer, MRE_SCREEN_START_X, MRE_SCREEN_START_Y, vm_graphic_get_screen_width(), vm_graphic_get_screen_height(), VM_COLOR_BLACK, VM_COLOR_BLACK);
    vm_graphic_fill_rect(buffer, MRE_SCREEN_START_X, MRE_SCREEN_START_Y, vm_graphic_get_screen_width(), vm_graphic_get_screen_height(), VM_COLOR_WHITE, VM_COLOR_WHITE);
    vm_graphic_flush_layer(layer_hdl, 1);
}

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
void fromArrToScrolDisplay(void) {

     int u = 0;
     for (u = 0; u < 20; u++) {
         vertical_scrolling_ucs2_text((VMWSTR)&oneDisplay[u][0]);
     }
}
/*****************************************************************************
 * FUNCTION
 *  create_app_txt_filename
 * DESCRIPTION
 *  
 * PARAMETERS
 *
 * RETURNS
 *
 *****************************************************************************/
void create_app_txt_filename(VMWSTR text) {

    VMWCHAR fullPath[100];
    VMWCHAR appName[100];
    VMWCHAR wfile_extension[4];
    VMCHAR file_extension[4] = "txt";

    vm_get_exec_filename(fullPath);
    vm_get_filename(fullPath, appName);
    vm_ascii_to_ucs2(wfile_extension, (strlen(file_extension) + 1) * 2, file_extension);
    vm_wstrncpy(text, appName, vm_wstrlen(appName) - 3);
    vm_wstrcat(text, wfile_extension);

}
/*****************************************************************************
 * FUNCTION
 *  create_auto_full_path_name
 * DESCRIPTION
 *  
 * PARAMETERS
 *
 * RETURNS
 *
 *****************************************************************************/
void create_auto_full_path_name(VMWSTR result, VMWSTR fname) {

    VMINT drv;
    VMCHAR fAutoFileName[100];
    VMWCHAR wAutoFileName[100];

    if ((drv = vm_get_removable_driver()) < 0) {
       drv = vm_get_system_driver();
    }

    sprintf(fAutoFileName, "%c:\\", drv);
    vm_ascii_to_ucs2(wAutoFileName, (strlen(fAutoFileName) + 1) * 2, fAutoFileName);
    vm_wstrcat(wAutoFileName, fname);
    vm_wstrcpy(result, wAutoFileName);

}