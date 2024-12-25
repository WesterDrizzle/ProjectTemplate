//
// Created by Coward on 2024/10/18.
//
#include "app.h"
#include "string.h"
#include "malloc.h"
#include "lvgl.h"

#define scr_get_width() lv_obj_get_width(lv_scr_act())
#define scr_get_height() lv_obj_get_height(lv_scr_act())
LV_FONT_DECLARE(myFont);


FRESULT  MenuInit(const char* path,lv_obj_t* submenu);
static lv_obj_t* MainMenu;

void test() {
	lv_obj_t* label = lv_label_create(lv_scr_act());
	lv_label_set_text(label,"������");
//    char path[30] = "0:/";
//    MainMenu = lv_list_create(lv_scr_act());
//    lv_obj_center(MainMenu);
//    lv_obj_set_size(MainMenu,scr_get_width(),scr_get_height());
//    MenuInit(path,MainMenu);
    //MenuInit(const char* path,lv_obj_t* submenu)


}

static void BtnEvent_Cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* tgt = lv_event_get_target(e);
    lv_obj_t*submenu = (lv_obj_t*)(e->user_data);
    if (code == LV_EVENT_CLICKED) {

        lv_obj_set_style_bg_color(tgt, lv_color_hex(0x47a7f1), LV_STATE_FOCUSED);
        lv_obj_set_style_bg_opa(tgt, 150, LV_STATE_FOCUSED);
        lv_obj_clear_flag((lv_obj_t*)(e->user_data),LV_OBJ_FLAG_HIDDEN);
        //printf("Clicked %s\r\n", lv_list_get_btn_text(submenu,tgt));
    }

}


static void BtnExitEvent_Cb(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* tgt = lv_event_get_target(e);
    lv_obj_t*submenu = (lv_obj_t*)(e->user_data);
    if (code == LV_EVENT_CLICKED) {

        lv_obj_set_style_bg_color(tgt, lv_color_hex(0x47a7f1), LV_STATE_FOCUSED);
        lv_obj_set_style_bg_opa(tgt, 150, LV_STATE_FOCUSED);
        lv_obj_add_flag((lv_obj_t*)(e->user_data),LV_OBJ_FLAG_HIDDEN);
        //printf("Clicked %s\r\n", lv_list_get_btn_text(submenu,tgt));
    }

}

FRESULT MenuInit(const char* path,lv_obj_t* submenu)
{
    FRESULT res;
    DIR dir;
    UINT i; //�������
    static FILINFO fno;
    char Filepath[128] = { 0 };
    char* DirPath;
	char*temp;
    res = f_opendir(&dir, path); //��Ŀ¼������״̬ �� Ŀ¼�����ָ��
    if (res == FR_OK)
    {			
		strcpy(Filepath, path);
		DirPath = strcat(Filepath, "/..");		
		lv_obj_t* exitbtn =( lv_obj_t*)my_mem_malloc(SRAMIN,sizeof(lv_obj_t));
		exitbtn = lv_list_add_btn(submenu, LV_SYMBOL_DIRECTORY,DirPath);
		lv_obj_add_event_cb(exitbtn,BtnExitEvent_Cb,LV_EVENT_CLICKED,submenu);
		memset(Filepath,0,sizeof(Filepath)/sizeof(Filepath[0]));

		
        while (1)
        {
            res = f_readdir(&dir, &fno); //��ȡĿ¼������״̬ �� �ļ���Ϣ��ָ��
            if (res != FR_OK || fno.fname[0] == 0)//Ϊnull��
            {
                printf("f_readdir end!\r\n");
                break;        //����ʧ�� �� ����β�����˳�
            }
            if ((fno.fattrib & AM_DIR) && !(fno.fattrib & AM_SYS)) //��Ŀ¼
            {
                printf(" path:%s,i=%d,File:%s\r\n", path, i, fno.fname);
                strcpy(Filepath, path);
                if (strcmp("0:/", path) != 0)  // �����Ŀ¼
                    DirPath = strcat(Filepath, "/");
                DirPath = strcat(Filepath, (const char*)fno.fname);
                printf(" DirPath:%s\r\n", DirPath);
				
                lv_obj_t* btn =( lv_obj_t*)my_mem_malloc(SRAMIN,sizeof(lv_obj_t));
                lv_obj_t* SubMenu =( lv_obj_t*)my_mem_malloc(SRAMIN,sizeof(lv_obj_t));
                btn = lv_list_add_btn(submenu, LV_SYMBOL_DIRECTORY,fno.fname);
                SubMenu = lv_list_create(lv_scr_act());
                lv_obj_center(SubMenu);
                lv_obj_set_size(SubMenu,scr_get_width(),scr_get_height());
                lv_obj_add_flag(SubMenu,LV_OBJ_FLAG_HIDDEN);
				lv_obj_add_event_cb(btn,BtnEvent_Cb,LV_EVENT_CLICKED,SubMenu);


                res = MenuInit(DirPath,SubMenu);
                if(res != FR_OK)
                {
                    printf("MenuInit fail! res:%d",res); //��ʧ��
                    break; //�˳�ѭ��
                }
            }
            else if(!(fno.fattrib & AM_SYS))
            {
                printf(" File:%s%s\r\n", path, fno.fname); //���ļ�
                lv_obj_t* btn = lv_list_add_btn(submenu, LV_SYMBOL_FILE,fno.fname);
            }
        }
    }else
    {
        printf(" DIR OPEN FAIL \r\n"); //���ļ�
    }
    f_closedir(&dir); //�ر�Ŀ¼
    printf(" f_closedir end!\r\n");
    return  res;

}