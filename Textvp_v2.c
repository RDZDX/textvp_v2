#include "Textvp_v2.h"

VMWSTR path;
//VMCHAR file_name[101] = "default.txt";
VMUINT8 *buffer;

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

            mre_draw_black_rectangle();
            //mre_read_file_display(path);
            
            if (dynamicPosition != 0) {
               fromArrToScrolDisplay();
            }
            else {
               mre_read_file_display(path);
               if (failoGalasPriesTai != 1) {mre_read_file_display(path);}
               
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

    //VMCHAR cdynamicPosition[101] = {0};
    //VMWCHAR cdynamicPositionX[101];
    //VMCHAR cfiksedPosition[101] = {0};
    //VMWCHAR cfiksedPositionX[101];
    //VMCHAR cprviousFlPosit[101] = {0};
    //VMWCHAR cprviousFlPositX[101];
    //VMCHAR cendFlPosition[101] = {0};
    //VMWCHAR cendFlPositionX[101];
    //VMCHAR cfailoGalasPriesTai[101] = {0};
    //VMWCHAR cfailoGalasPriesTaiX[101];
    //VMCHAR cfailoGalasPriesTai1[101] = {0};
    //VMWCHAR cfailoGalasPriesTai1X[101];
    //VMCHAR cfailoGalasPriesTai2[101] = {0};
    //VMWCHAR cfailoGalasPriesTai2X[101];
    //VMCHAR cfailoGalasPriesTai3[101] = {0};
    //VMWCHAR cfailoGalasPriesTai3X[101];

    int percentData; 

    int trigerisq = 0;     //trigeris nustatymui "fiksedPosition = 0" po uzkrauto puslapio 
    int countertriger = 0; //trigeris didinti +1 kounteri ar ne

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_RIGHT_SOFTKEY) {
        if (layer_hdl[0] != -1) {
            vm_graphic_delete_layer(layer_hdl[0]);
            layer_hdl[0] = -1;
        }
        vm_free(path);
        vm_exit_app();
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_LEFT_SOFTKEY) {
        mre_draw_black_rectangle();
        selector_use = 1;
        vm_selector_run(0, FILE_PATH, file);
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_DOWN) {

       //mre_draw_black_rectangle();

       //Jei ne failo galas pries uzkrovima
       if (fiksedPosition != dynamicPosition) {
          prviousFlPosit = fiksedPosition;
       }

       fiksedPosition = dynamicPosition;

       fromArrToScrolDisplay();
       if (failoGalasPriesTai != 1) {mre_read_file_display(path);}

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

      lastPressedKey = 3;

    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_UP) {
        mre_draw_black_rectangle();

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

        mre_read_file_display(path);
        clean_array = 0;
        selector_use = 0;

        if (failoGalasPriesTai != 1) {mre_read_file_display(path);}

        if (trigerisq == 1) {
           fiksedPosition = 0;
           trigerisq = 0;
        }

        lastPressedKey = 2;
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM1) {
        mre_draw_black_rectangle();

        //percentData = (dynamicPosition * 100) / endFlPosition;
        percentData = (simblKiekEinLape * 100) / endFlPosition;

        if (failoGalasPriesTai == 1) { percentData = 100; }
        if (lapu_kiekis < 0) { lapu_kiekis = 1; }

        if (endPressedKey == 1) {
           endPressedKey = 0;
           pageCounter = lapu_kiekis;
        }

        vertical_scrolling_ucs2_text(path);

        sprintf(cPageCnter, "Page # %d of %d", pageCounter + plusPage, lapu_kiekis + plusPage);
        vm_ascii_to_ucs2(cPageCnterX, (strlen(cPageCnter) + 1) * 2, cPageCnter);
        vertical_scrolling_ucs2_text(cPageCnterX);

        percentData = (dynamicPosition * 100) / endFlPosition;
        if (failoGalasPriesTai == 1) { percentData = 100; }
        if (pageCounter == lapu_kiekis) { percentData = 100; }

        sprintf(cpercentPosition, "Percent: %d", percentData);
        vm_ascii_to_ucs2(cpercentPositionX, (strlen(cpercentPosition) + 1) * 2, cpercentPosition);
        vertical_scrolling_ucs2_text(cpercentPositionX);


        //sprintf(cdynamicPosition, "dynamicPosition: %d", dynamicPosition);
        //vm_ascii_to_ucs2(cdynamicPositionX, (strlen(cdynamicPosition) + 1) * 2, cdynamicPosition);
        //vertical_scrolling_ucs2_text(cdynamicPositionX);

        //sprintf(cfiksedPosition, "fiksedPosition: %d", fiksedPosition);
        //vm_ascii_to_ucs2(cfiksedPositionX, (strlen(cfiksedPosition) + 1) * 2, cfiksedPosition);
        //vertical_scrolling_ucs2_text(cfiksedPositionX);

        //sprintf(cprviousFlPosit, "prviousFlPosit: %d", prviousFlPosit);
        //vm_ascii_to_ucs2(cprviousFlPositX, (strlen(cprviousFlPosit) + 1) * 2, cprviousFlPosit);
        //vertical_scrolling_ucs2_text(cprviousFlPositX);

        //sprintf(cendFlPosition, "endFlPosition: %d", endFlPosition);
        //vm_ascii_to_ucs2(cendFlPositionX, (strlen(cendFlPosition) + 1) * 2, cendFlPosition);
        //vertical_scrolling_ucs2_text(cendFlPositionX);

        //sprintf(cfailoGalasPriesTai, "myFlPosOld: %d", myFlPosOld);
        //vm_ascii_to_ucs2(cfailoGalasPriesTaiX, (strlen(cfailoGalasPriesTai) + 1) * 2, cfailoGalasPriesTai);
        //vertical_scrolling_ucs2_text(cfailoGalasPriesTaiX);

        //sprintf(cfailoGalasPriesTai1, "myFlPosOld1: %d", myFlPosOld1);
        //vm_ascii_to_ucs2(cfailoGalasPriesTai1X, (strlen(cfailoGalasPriesTai1) + 1) * 2, cfailoGalasPriesTai1);
        //vertical_scrolling_ucs2_text(cfailoGalasPriesTai1X);

        //sprintf(cfailoGalasPriesTai3, "myFlPosOld2: %d", myFlPosOld2);
        //vm_ascii_to_ucs2(cfailoGalasPriesTai3X, (strlen(cfailoGalasPriesTai3) + 1) * 2, cfailoGalasPriesTai3);
        //vertical_scrolling_ucs2_text(cfailoGalasPriesTai3X);
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM9) {

       //if (endFlPosition > 380) {
       if (endFlPosition > 1200) {
          mre_draw_black_rectangle();
          dynamicPosition = endFlPosition - 380;
          fiksedPosition = endFlPosition - 380;
          prviousFlPosit = endFlPosition - 380;
          percentData = 100;
          failoGalasPriesTai = 1;
          mre_read_file_display(path);
          endPressedKey = 1;
          trigeris_end = 1;
       }

    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM7) {
        mre_draw_black_rectangle();

        dynamicPosition = 0;
        fiksedPosition = 0;
        prviousFlPosit = 0;
        lapu_kiekis = 0;
        failoGalasPriesTai = 0;
        lastPressedKey = 0;
        mre_read_file_display(path);
        pageCounter = 1;
        myFlPosOld1 = 0;
        trigeris_end = 0;

    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM8) {

       x5_puslapiu_adr = dynamicPosition + (simbl_kiek_pirm_lape * 4);

       if (x5_puslapiu_adr < endFlPosition - 380 && endFlPosition > 380 ) {
       //if (x5_puslapiu_adr < endFlPosition - 380) {
          mre_draw_black_rectangle();
          dynamicPosition = x5_puslapiu_adr;
          fiksedPosition = x5_puslapiu_adr;
          prviousFlPosit = x5_puslapiu_adr;
          pageCounter = pageCounter + 5;
          mre_read_file_display(path);
          xPressedKey = 1;
       }

       if (x5_puslapiu_adr >= endFlPosition - 380 && trigeris_end != 1) {
          mre_draw_black_rectangle();
          dynamicPosition = endFlPosition - 380;
          fiksedPosition = endFlPosition - 380;
          prviousFlPosit = endFlPosition - 380;
          percentData = 100;
          failoGalasPriesTai = 1;
          mre_read_file_display(path);
          endPressedKey = 1;
          trigeris_end = 1;
          pageCounter = lapu_kiekis;
       }

    }

 if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM6) {

       x10_procentu_lapu = (lapu_kiekis * 9) / 100;
       x10_procentu_adr = dynamicPosition + (simbl_kiek_pirm_lape * x10_procentu_lapu);

       if (x10_procentu_lapu >= 1 && x10_procentu_adr < endFlPosition - 380) {
          mre_draw_black_rectangle();
          dynamicPosition = x10_procentu_adr;
          fiksedPosition = x10_procentu_adr;
          prviousFlPosit = x10_procentu_adr;
          pageCounter = pageCounter + x10_procentu_lapu + 1;
          mre_read_file_display(path);
          xPressedKey = 1;
       }

       if (x10_procentu_adr >= endFlPosition - 380 && trigeris_end != 1) {
          mre_draw_black_rectangle();
          dynamicPosition = endFlPosition - 380;
          fiksedPosition = endFlPosition - 380;
          prviousFlPosit = endFlPosition - 380;
          percentData = 100;
          failoGalasPriesTai = 1;
          mre_read_file_display(path);
          endPressedKey = 1;
          trigeris_end = 1;
       }

    }

 if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_NUM2) {
        mre_draw_black_rectangle();
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

    VMFILE f_read;
    mre_draw_black_rectangle();
    path = vm_realloc(path, (wlen + 1) * 2);
    vm_wstrcpy(path, FILE_PATH);
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
    f_read = vm_file_open(path, MODE_READ, FALSE);
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

    if (selector_use == 1) {
       mre_draw_black_rectangle();
       // selector_use = 0;
    }
 
    VMFILE f_read;
    VMUINT nread;

    VMWSTR sValue21 = {0};
    VMWSTR sKonv_stringas = {0};
    VMWSTR sTemp_strng = {0};
    VMCHAR value1[MRE_STR_SIZE_MAX + 23] = {0};
    VMCHAR vns_simbl[MRE_STR_SIZE_MAX] = {0};
    VMCHAR value10[MRE_STR_SIZE_MAX + 23] = {0};
    VMCHAR nauj_strng[MRE_STR_SIZE_MAX + 23] = {0};
    VMCHAR value21[MRE_STR_SIZE_MAX + 23] = {0};
    VMCHAR konv_stringas[MRE_STR_SIZE_MAX + 23] = {0};
    VMCHAR knv_vns_simblX[MRE_STR_SIZE_MAX] = {0};
    VMWCHAR knv_vns_simbl[MRE_STR_SIZE_MAX] = {0};

    int myFlPosBackCurr = 0;
    //int myFlPosBackTemp = 0;
    //int myFlPosBackPrev = 0;

    if (clean_array == 0) {
       memset(oneDisplay, 0, sizeof(oneDisplay[0][0]) * 20 * 61);
    }

    f_read = vm_file_open(path, MODE_READ, FALSE);

    vm_file_seek(f_read, dynamicPosition, BASE_CURR);  // Permetame kursoriu i pozicija

    myFlPosOld2 = myFlPosOld;
    myFlPosOld = prviousFlPosit;
    myFlPosBackCurr = dynamicPosition;
    if (myFlPosOld1 == 0) {myFlPosOld1 = dynamicPosition;}

    int i, plotis, strng_plot, isve_i_ekr_eil_sk, nauj_strng_ilg, combUodegosIlgis, uodegosIlgis, prasisk_count;
    
    plotis = 0;
    strng_plot = 0;
    isve_i_ekr_eil_sk = 0;
    nauj_strng_ilg = 0;
    combUodegosIlgis = 0;
    uodegosIlgis = 0;
    prasisk_count = 0;
    plusPage = 0;

    for (i = 0; i < 20; i++)  // 20 tekstiniu ekrano eiluciu
    {
        if (isve_i_ekr_eil_sk == 20) {break;}

        vm_file_read(f_read, value1, 60, &nread);  // max 60 "." simboliu vienoje eiluteje 
        value1[nread] = '\0';
        dynamicPosition = vm_file_tell(f_read);  // Issaugome dabartine kursoriaus pozicija faile
        strcat(value10, value1);
        vm_chset_convert(VM_CHSET_UTF8, VM_CHSET_UCS2, value10, value21, (strlen(value10) + 1) * 2);
        sValue21 = (VMWSTR)value21;
        plotis = vm_graphic_get_string_width(sValue21);

        if (plotis < 228 && prasisk_count < 4 || plotis == 228 && prasisk_count < 4 ) {
           prasisk_count = prasisk_count + 1;
            continue;
        }

        prasisk_count = 0;

        char *ptr = value10;

        // while (*ptr != '\0' || *ptr != '\n') {

        while (*ptr != '\0') {

            sprintf(vns_simbl, "%c", *ptr);
            strcat(nauj_strng, vns_simbl);
            vm_chset_convert(VM_CHSET_UTF8, VM_CHSET_UCS2, nauj_strng, konv_stringas, (strlen(nauj_strng) + 1) * 2);
            sKonv_stringas = (VMWSTR)konv_stringas;
            strng_plot = vm_graphic_get_string_width(sKonv_stringas);

            //if (strng_plot > 235 || *ptr == '\n') {
            if (strng_plot > 235) {

                if (fiksedPosition == 0 || selector_use == 1) {vertical_scrolling_ucs2_text(sTemp_strng);}
                //if (dynamicPosition == 0 || selector_use == 1) {vertical_scrolling_ucs2_text(sTemp_strng);}
                if (clean_array == 0) {vm_wstrcpy((VMWSTR)&oneDisplay[isve_i_ekr_eil_sk - 1][61], sKonv_stringas);}
                nauj_strng_ilg = strlen(nauj_strng);  // uodegos ilgiui nustatyti
                isve_i_ekr_eil_sk = isve_i_ekr_eil_sk + 1;
                strcpy(nauj_strng, "");
                wstrcpy(sKonv_stringas, (VMWSTR)"");
                strng_plot = 0;
                ptr - 2;
            }


            sTemp_strng = sKonv_stringas;

            if (isve_i_ekr_eil_sk == 20) {
                break;
            }


            ptr++;
        }

         uodegosIlgis = strlen(value10) - nauj_strng_ilg;
        strcpy(value10, nauj_strng);
        strcpy(nauj_strng, "");

        if (vm_file_is_eof(f_read)) { break; } 

    }
 
     dynamicPosition = dynamicPosition - uodegosIlgis;
     if (endFlPosition > 0 && dynamicPosition > endFlPosition) {dynamicPosition = endFlPosition;}

     simblKiekEinLape = dynamicPosition;

    if (vm_file_is_eof(f_read)) {
                                                 //< 240
        //if (isve_i_ekr_eil_sk < 20 && strng_plot == 235 || isve_i_ekr_eil_sk < 20 && strng_plot < 235 || sKonv_stringas != '\0') {
        //if (isve_i_ekr_eil_sk < 20 && strng_plot <= 235 && sKonv_stringas != '\0') {
        if (isve_i_ekr_eil_sk < 20 && strng_plot != 0 && sKonv_stringas != '\0') {

            if (fiksedPosition == 0 || selector_use == 1) {vertical_scrolling_ucs2_text(sKonv_stringas);}
            //if (dynamicPosition == 0 || selector_use == 1) {vertical_scrolling_ucs2_text(sKonv_stringas);}
            if (clean_array == 0) {vm_wstrcpy((VMWSTR)&oneDisplay[isve_i_ekr_eil_sk - 1][61], sKonv_stringas);}
            dynamicPosition = myFlPosBackCurr;
            if (isve_i_ekr_eil_sk == 0 && wstrlen(sKonv_stringas) < 7 && dynamicPosition != 0) {plusPage = 1;}
        }

        if (endFlPosition == dynamicPosition) {dynamicPosition = myFlPosBackCurr;}

        strcpy(value10, "");
        filledDsplByLines = 0;
        nauj_strng_ilg = 0;
    }

    vm_file_close(f_read);

    uodegosIlgis = 0;
    nauj_strng_ilg = 0;
    combUodegosIlgis = 0;

    strcpy(value1, "");
    strcpy(vns_simbl, "");
    strcpy(nauj_strng, "");
    strcpy(value10, "");
    strcpy(value21, "");
    strcpy(konv_stringas, "");
    strcpy(knv_vns_simblX, "");

    //vm_audio_play_beep();

     if (lapu_kiekis == 0) {

       lapu_kiekis = endFlPosition / dynamicPosition;
       if (((float)endFlPosition / dynamicPosition) - lapu_kiekis != 0) {lapu_kiekis = lapu_kiekis + 1;}

    }

    if (simbl_kiek_pirm_lape == 0) {

       simbl_kiek_pirm_lape = dynamicPosition;

    }

    //if (dynamicPosition != 0 && prviousFlPosit == 0 ) {fromArrToScrolDisplay();}
    //if (dynamicPosition != 0 && fiksedPosition == 0 && selector_use == 0 ) {fromArrToScrolDisplay();}

    if (selector_use == 1) {
       selector_use = 0;
    }

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

    if (filledDsplByLines == 0) {abc = 2;}

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

    mre_draw_black_rectangle();

    VMINT drv;
    VMCHAR f_name[MRE_STR_SIZE_MAX + 1];
    VMWCHAR f_wname[MRE_STR_SIZE_MAX + 1];
    VMFILE f_read;
    VMWCHAR *FILE_PATH = 0;
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
    vm_ascii_to_ucs2(f_wname, MRE_STR_SIZE_MAX, f_name);
    f_read = vm_file_open(f_wname, MODE_READ, FALSE);

    pageCounter = 1;

    if (f_read < 0) {
        vm_file_close(f_read);
        selector_use = 1;
        vm_selector_run(0, FILE_PATH, file);
    } else {
        vm_file_seek(f_read, 0, BASE_END);
        endFlPosition = vm_file_tell(f_read);
        vm_file_close(f_read);
        size = (strlen(f_name) + 1) * 2;
        path = vm_malloc(size);
        vm_wstrcpy(path, f_wname);
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

     mre_draw_black_rectangle();
     int u = 0;
     for (u = 0; u < 20; u++) {
         vertical_scrolling_ucs2_text((VMWSTR)&oneDisplay[u][0]);
     }
}