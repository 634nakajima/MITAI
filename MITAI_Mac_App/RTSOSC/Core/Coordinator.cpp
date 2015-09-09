//
//  Coordinator.cpp
//  SimpleSimulator
//
//  Created by NAKAJIMA Musashi on 12/06/18.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Coordinator.h"

Coordinator::Coordinator()
{}
Coordinator::Coordinator(Server *s, const char *osc) : Module(s,osc)
{
    init(s);
}

void Coordinator::init(Server *s)
{
    Module::init(s, "/Coordinator");
    addMethodToTCPServer("/SetMdtkn", "ssii", setMtkn, this);//ip,osc,tID,mColor
    addMethodToTCPServer("/DeleteMdtkn", "ssii", deleteMtkn, this);//ip,osc,tID,mColor
    addMethodToServer("/TileState", "iss", tileState, this);//tID
    
    ml = new ModuleList(s, "/ModuleList");
}

int Coordinator::tileState(const char   *path,
                           const char   *types,
                           lo_arg       **argv,
                           int          argc,
                           void         *data,
                           void         *user_data)
{
    Coordinator *co = (Coordinator *)user_data;
    
    for (std::map<int, MToken*>::iterator iter = co->mtknMap.begin(); iter!=co->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (tmp->tID == argv[0]->i) {
            for (int i=0; i<3; i++) {
                lo_send(lo_address_new((char *)argv[1],"6340"),
                        (char *)argv[2],
                        "ssii",
                        tmp->ip,
                        tmp->osc,
                        tmp->tID,
                        tmp->mColor);
                usleep(10000);
            }
            return 0;
        }
    }
    
    for (int i=0; i<3; i++) {
        lo_send(lo_address_new((char *)argv[1],"6340"),
                (char *)argv[2],
                "ssi",
                NULL,
                NULL,
                0);
        usleep(10000);
    }
    return 0;
}

int Coordinator::setMtkn(const char   *path,
                         const char   *types,
                         lo_arg       **argv,
                         int          argc,
                         void         *data,
                         void         *user_data)
{
    Coordinator *co = (Coordinator *)user_data;
    char ip[16];
    strcpy(ip, co->getSenderTCPIP());
    
    //エラー処理、既存のモジュールトークン確認
    for (std::map<int, MToken*>::iterator iter = co->mtknMap.begin(); iter!=co->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (strcmp(tmp->ip,ip)==0) {
            if (strcmp(tmp->osc,(char *)argv[1])==0) {
                return 0;
            }
        }
    }
    
    //先にタイルに登録されているモジュールの確認
    for (std::map<int, MToken*>::iterator iter = co->mtknMap.begin(); iter!=co->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (tmp->tID == argv[2]->i) {//先に登録されていればそのモジュールを消去
            co->deleteMtkn(tmp->tID);
            break;
        }
    }
    //モジュールトークンの生成
    MToken *m = new MToken();
    strcpy(m->ip, ip);
    strcpy(m->osc, (char *)argv[1]);
    
    if (argv[2]->i != -1) {//tIDが-1でなければ
        m->tID = argv[2]->i;
        m->mColor = argv[3]->i;
        
        co->mtknMap.insert(std::map<int, MToken*>::value_type(m->tID, m));
        printf("Coordinator_Set:%s, %s, Module Index:%d\n",
               ip,
               (char *)argv[1],
               argv[2]->i,
               argv[3]->i);
    }
    
    return 0;
}

int Coordinator::deleteMtkn(const char   *path,
                            const char   *types,
                            lo_arg       **argv,
                            int          argc,
                            void         *data,
                            void         *user_data)
{
    Coordinator *co = (Coordinator *)user_data;
    char ip[16];
    strcpy(ip, co->getSenderTCPIP());
    
    //エラー処理、既存のモジュールトークン確認
    for (std::map<int, MToken*>::iterator iter = co->mtknMap.begin(); iter!=co->mtknMap.end(); iter++) {
        MToken *tmp = iter->second;
        if (strcmp(tmp->ip,ip)==0) {
            if (strcmp(tmp->osc,(char *)argv[1])==0) {
                if (tmp->tID == argv[2]->i) {
                    printf("delete:%s,%s tID:%d Module Color:%d\n",
                           tmp->ip,
                           tmp->osc,
                           tmp->tID,
                           tmp->mColor);
                    
                    delete co->mtknMap[tmp->tID];
                    co->mtknMap.erase(tmp->tID);
                    return 0;
                }
            }
        }
    }
    
    return 0;
}

void Coordinator::deleteMtkn(int tID)
{
    if (!mtknMap.count(tID)) return;
    
    char t[4];
    if (tID < 10) {
        t[0] = (tID + 0x30);
        t[1] = '\0';
    }else if (tID < 100) {
        t[0] = (tID/10 + 0x30);
        t[1] = (tID%10 + 0x30);
        t[2] = '\0';
    }else {
        t[0] = '1';
        t[1] = ((tID%100)/10 + 0x30);
        t[2] = ((tID%100)%10 + 0x30);
        t[3] = '\0';
    }
    char p[64];
    char *pp, *op;
    
    pp = p;
    op = mtknMap[tID]->osc;
    
    char *end = strstr(mtknMap[tID]->osc, "/Tile");
    
    while (op != end) {
        *pp++ = *op++;
    }
    *pp = '\0';
    printf("%s\n",p);
    
    lo_send(lo_address_new(mtknMap[tID]->ip,"6340"),
            p,
            "is",
            0,
            t);
    
    printf("delete:%s,%s tID:%d Module Color:%d\n",
           mtknMap[tID]->ip,
           p, mtknMap[tID]->tID,
           mtknMap[tID]->mColor);
    
    delete mtknMap[tID];
    mtknMap.erase(tID);
}

/*int Coordinator::deleteMtkn(const char   *path,
 const char   *types,
 lo_arg       **argv,
 int          argc,
 void         *data,
 void         *user_data)
 {
 Coordinator *co = (Coordinator *)user_data;
 co->deleteMtkn(argv[2]->i);
 
 return 0;
 }*/

void Coordinator::createModule(int tID, int mc) {
    ml->createModule(tID, mc);
}

void Coordinator::deleteModule(int tID, int mc) {
    ml->deleteModule(tID, mc);
}

void Coordinator::connect(int tID1, int tID2, const char *t)
{
    char m1OSC[64], m2OSC[64];
    
    //エラー処理
    if (!mtknMap.count(tID1) || !mtknMap.count(tID2)) {
        printf("err:connect tid1:%d, tid2:%d\n",tID1, tID2);
        return;
    }
    
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID1];
    MToken *m2 = mtknMap[tID2];
    
    //モジュールに対して接続するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcpy(m2OSC, m2->osc);
    strcat(m2OSC,t);
    strcat(m1OSC,"/SetRoute");
    
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_string(m, m2->ip);
    lo_message_add_string(m, m2OSC);
    
    data = lo_message_serialise(m, m1OSC, NULL, NULL);
    d_len = lo_message_length(m, m1OSC);
    
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, m1->ip, "6341");
    if (strcmp(this->IPAddr,lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st_tcp), data, d_len);
    }else {
        lo_send_message(lo_ip, m1OSC, m);
    }
    
    lo_message_free(m);
    lo_address_free(lo_ip);
    free(data);
}

void Coordinator::addConnection(int tID1, int tID2, const char *t)
{
    char m1OSC[64], m2OSC[64];
    
    //エラー処理
    if (!mtknMap.count(tID1) || !mtknMap.count(tID2)) {
        printf("err:connect tid1:%d, tid2:%d\n",tID1, tID2);
        return;
    }
    
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID1];
    MToken *m2 = mtknMap[tID2];
    
    //モジュールに対して接続するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcpy(m2OSC, m2->osc);
    strcat(m2OSC,t);
    strcat(m1OSC,"/AddRoute");
    
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_string(m, m2->ip);
    lo_message_add_string(m, m2OSC);
    
    data = lo_message_serialise(m, m1OSC, NULL, NULL);
    d_len = lo_message_length(m, m1OSC);
    
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, m1->ip, "6341");
    if (strcmp(this->IPAddr,lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st_tcp), data, d_len);
    }else {
        lo_send_message(lo_ip, m1OSC, m);
    }
    
    lo_message_free(m);
    lo_address_free(lo_ip);
    free(data);
}

void Coordinator::addConnection(int tID1, int tID2, int outID, const char *t)
{
    char m1OSC[64], m2OSC[64];
    
    //エラー処理
    if (!mtknMap.count(tID1) || !mtknMap.count(tID2)) {
        printf("err:connect tid1:%d, tid2:%d\n",tID1, tID2);
        return;
    }
    
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID1];
    MToken *m2 = mtknMap[tID2];
    
    //モジュールに対して接続するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcpy(m2OSC, m2->osc);
    strcat(m2OSC,t);
    strcat(m1OSC,"/AddRoute");
    
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_string(m, m2->ip);
    lo_message_add_string(m, m2OSC);
    lo_message_add_int32(m, outID);
    
    data = lo_message_serialise(m, m1OSC, NULL, NULL);
    d_len = lo_message_length(m, m1OSC);
    
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, m1->ip, "6341");
    if (strcmp(this->IPAddr,lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st_tcp), data, d_len);
        
    }else {
        lo_send_message(lo_ip, m1OSC, m);
    }
    
    lo_message_free(m);
    lo_address_free(lo_ip);
    free(data);
}

void Coordinator::disconnect(int tID1, int tID2, const char *t)
{
    char m1OSC[64], m2OSC[64];
    
    //エラー処理
    if (!mtknMap.count(tID1) || !mtknMap.count(tID2)) {
        //printf("err:disconnect\n");
        return;
    }
    
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID1];
    MToken *m2 = mtknMap[tID2];
    
    //モジュールに対して切断するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcpy(m2OSC, m2->osc);
    strcat(m2OSC,t);
    strcat(m1OSC,"/DeleteRoute");
    
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_string(m, m2->ip);
    lo_message_add_string(m, m2OSC);
    
    data = lo_message_serialise(m, m1OSC, NULL, NULL);
    d_len = lo_message_length(m, m1OSC);
    
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, m1->ip, "6341");
    if (strcmp(this->IPAddr,lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st_tcp), data, d_len);
    }else {
        lo_send_message(lo_ip, m1OSC, m);
    }
    
    lo_message_free(m);
    lo_address_free(lo_ip);
    free(data);
}

void Coordinator::disconnect(int tID1, int tID2, int outID, const char *t)
{
    char m1OSC[64], m2OSC[64];
    
    //エラー処理
    if (!mtknMap.count(tID1) || !mtknMap.count(tID2)) {
        //printf("err:disconnect\n");
        return;
    }
    
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID1];
    MToken *m2 = mtknMap[tID2];
    
    //モジュールに対して切断するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcpy(m2OSC, m2->osc);
    strcat(m2OSC,t);
    strcat(m1OSC,"/DeleteRoute");
    
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_string(m, m2->ip);
    lo_message_add_string(m, m2OSC);
    lo_message_add_int32(m, outID);
    data = lo_message_serialise(m, m1OSC, NULL, NULL);
    d_len = lo_message_length(m, m1OSC);
    
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, m1->ip, "6341");
    if (strcmp(this->IPAddr,lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st_tcp), data, d_len);
    }else {
        lo_send_message(lo_ip, m1OSC, m);
    }
    
    lo_message_free(m);
    lo_address_free(lo_ip);
    free(data);
}

void Coordinator::disconnectAll(int tID, const char *t)
{
    char m1OSC[64];
    
    //エラー処理
    if (!mtknMap.count(tID)) {
        //printf("err:disconnect\n");
        return;
    }
    
    //モジュールトークン取得
    MToken *m1 = mtknMap[tID];
    
    //モジュールに対して切断するルートのアドレスを送信
    strcpy(m1OSC, m1->osc);
    strcat(m1OSC,"/DeleteAllRoute");
    
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_string(m, t);
    
    data = lo_message_serialise(m, m1OSC, NULL, NULL);
    d_len = lo_message_length(m, m1OSC);
    
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, m1->ip, "6341");
    if (strcmp(this->IPAddr,lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st_tcp), data, d_len);
    }else {
        lo_send_message(lo_ip, m1OSC, m);
    }
    
    lo_message_free(m);
    lo_address_free(lo_ip);
    free(data);
}

int Coordinator::mIDofOSCPath(const char *osc)
{
    for (std::list<MToken*>::iterator iter = ml->mList.begin(); iter!=ml->mList.end(); iter++) {
        MToken* mt = (*iter);
        if (strstr(mt->osc, osc)) {
            return mt->tID;
        }
    }
    return 0;
}

Coordinator::~Coordinator()
{
    for (std::map<int, MToken*>::iterator iter = mtknMap.begin(); iter!=mtknMap.end(); iter++) {
        deleteModule(iter->second->tID, iter->second->mColor);
        delete iter->second;
    }
    
    mtknMap.clear();
    delete ml;
    
    deleteMethodFromTCPServer("/SetMdtkn", "ssii");
    deleteMethodFromTCPServer("/DeleteMdtkn", "ssii");
    deleteMethodFromServer("/TileState", "iss");
    
}





