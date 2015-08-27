//
//  Object.cpp
//  RTSOSCApp
//
//  Created by Musashi Nakajima on 2015/07/04.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#include "Object.h"

#pragma mark ----- Object -----

Object::Object(Server *s, const char *osc) : Module(s,osc)
{
    addMethodToServer("/Intensity", "ii", something, this);
    d = NULL;
}

int Object::something(const char   *path,
                          const char   *types,
                          lo_arg       **argv,
                          int          argc,
                          void         *data,
                          void         *user_data)
{
    Object *cm = (Object *)user_data;
    Data *d = new Data(argv[0]->i,argv[1]->i);
    if (cm->cb && cm->ud) cm->cb(d, cm->ud);
    delete d;
    return 0;
}

void Object::setCallback(void (*callback)(Data *data, void *user_data), void *user_data) {
    cb = callback;
    ud = user_data;
}

Object::~Object()
{
    if (d) delete d;
}

#pragma mark ----- ObjectManager -----
void ObjectManager::init(Server *s, const char *osc)
{
    ModuleManager::init(s, osc);
    objectID = -1;
}

Object *ObjectManager::initModule(Server *s, const char *osc)
{
    Object *o = new Object(s, osc);
    o->setCallback(moduleCallback, this);
    return o;
}

void ObjectManager::setData(int d, int dataID)
{
    for (auto iter = mList.begin(); iter != mList.end(); iter++) {
        Object *m = (Object *)(*iter);
        if (m->d) delete m->d;
        m->d = new Data(d,dataID);
        m->sendDataTo(d, 0, 0);
    }
}

void ObjectManager::setSerialCallback(void (*callback)(void *data, int s, void *user_data), void *user_data) {
    writeData = callback;
    ud = user_data;
}

void ObjectManager::serialCallback(void *data, int size, void *user_data) {
    ObjectManager *cmm = (ObjectManager *)user_data;
    int *dp = (int *)data;
    cmm->setData(*dp, cmm->objectID);
    
    struct timeval myTime;    // time_t構造体を定義．1970年1月1日からの秒数を格納するもの
    struct tm *time_st;       // tm構造体を定義．年月日時分秒をメンバ変数に持つ構造体
    
    /* 時刻取得 */
    gettimeofday(&myTime, NULL);    // 現在時刻を取得してmyTimeに格納．通常のtime_t構造体とsuseconds_tに値が代入される
    time_st = localtime(&myTime.tv_sec);    // time_t構造体を現地時間でのtm構造体に変換
    
    printf("Date:%02d:%02d:%02d.%06d\t",     // 現在時刻
           time_st->tm_hour,          // 時
           time_st->tm_min,           // 分
           time_st->tm_sec,           // 秒
           myTime.tv_usec            // マイクロ秒
           );

    printf("SerialCallback:%d, %d\n",*dp, cmm->objectID);
}

void ObjectManager::moduleCallback(Data *data, void *user_data) {
    ObjectManager *cmm = (ObjectManager *)user_data;
    //cmm->setData(data->value, data->dataID);
    
    if(cmm->writeData && cmm->ud && (cmm->objectID != -1)) {
        int d = data->value;
        unsigned char data[2] = {(unsigned char)cmm->objectID, (unsigned char)d};
        cmm->writeData(data, sizeof(data), cmm->ud);
    }
}
