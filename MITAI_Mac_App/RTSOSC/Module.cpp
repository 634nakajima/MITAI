//
//  Module.cpp
//  System1
//
//  Created by NAKAJIMA Musashi on 12/05/27.
//  Copyright 2012年 __MyCompanyName__. All rights reserved.
//

#include "Module.h"

void Module::setTID(int t)
{
    tID = t;
}

void Module::setOutNum(int on) {
    outNum = on;
    for(int i=1;i<outNum;i++) {
        RoutingTable *r = new RoutingTable();
        r->aNum = 0;
        rts.push_back(r);
    }
}

int Module::setRoute(const char   *path,
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data)
{
    Module *mod = (Module *)user_data;
    mod->setRoute((char *)argv[0], (char *)argv[1]);
    return 0;
}

int Module::addRoute(const char   *path, 
                     const char   *types, 
                     lo_arg       **argv, 
                     int          argc,
                     void         *data, 
                     void         *user_data)
{
    Module *mod = (Module *)user_data;
    if(argc == 2)
        mod->addRoute((char *)argv[0], (char *)argv[1]);
    else
        mod->addRoute((char *)argv[0], (char *)argv[1], argv[2]->i);

    return 0;
}

int Module::deleteRoute(const char   *path, 
                        const char   *types, 
                        lo_arg       **argv, 
                        int          argc,
                        void         *data, 
                        void         *user_data)
{
    Module *mod = (Module *)user_data;
    if(argc == 2)
        mod->deleteRoute((char *)argv[0], (char *)argv[1]);
    else
        mod->deleteRoute((char *)argv[0], (char *)argv[1], argv[2]->i);
    return 0;
}

int Module::deleteAllRoute(const char   *path, 
						   const char   *types, 
						   lo_arg       **argv, 
						   int          argc,
						   void         *data, 
						   void         *user_data)
{
    Module *mod = (Module *)user_data;
    mod->deleteAllRoute((char *)argv[0]);

    return 0;
}

Module::Module()
{}

Module::Module(Server *s, const char *osc)
{
    st = s;
    getAddr();
    setOSCAddr(osc);
    rTable = new RoutingTable();
    rTable->aNum = 0;
    rts.push_back(rTable);
    tID = -1;
    mColor = -1;
    outNum = 1;
    
    addMethodToTCPServer("/SetRoute", "ss", Module::setRoute, this);
    addMethodToTCPServer("/AddRoute", "ss", Module::addRoute, this);
    addMethodToTCPServer("/AddRoute", "ssi", Module::addRoute, this);
    addMethodToTCPServer("/DeleteRoute", "ss", Module::deleteRoute, this);
    addMethodToTCPServer("/DeleteRoute", "ssi", Module::deleteRoute, this);
    addMethodToTCPServer("/DeleteAllRoute", "s", Module::deleteAllRoute, this);}

void Module::init(Server *s, const char *osc)
{
    st = s;
    getAddr();
    setOSCAddr(osc);
    rTable = new RoutingTable();
    rTable->aNum = 0;
    rts.push_back(rTable);
    tID = -1;
    mColor = -1;
    outNum = 1;
    
    addMethodToTCPServer("/SetRoute", "ss", Module::setRoute, this);
    addMethodToTCPServer("/AddRoute", "ss", Module::addRoute, this);
    addMethodToTCPServer("/AddRoute", "ssi", Module::addRoute, this);
    addMethodToTCPServer("/DeleteRoute", "ss", Module::deleteRoute, this);
    addMethodToTCPServer("/DeleteRoute", "ssi", Module::deleteRoute, this);
	addMethodToTCPServer("/DeleteAllRoute", "s", Module::deleteAllRoute, this);
}

void Module::sendSetMdtkn()
{
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, CoIP, "6341");
    lo_send(lo_ip,
            "/Coordinator/SetMdtkn",
            "ssii",
            IPAddr,
            OSCAddr,
            tID,
            mColor);
    lo_address_free(lo_ip);
}

void Module::sendDeleteMdtkn()
{
    lo_address lo_ip = lo_address_new_with_proto(LO_TCP, CoIP, "6341");
    lo_send(lo_ip,
            "/Coordinator/DeleteMdtkn",
            "ssii",
            IPAddr,
            OSCAddr,
            tID,
            mColor);
    lo_address_free(lo_ip);
}

void Module::module_send(Data *d, lo_address lo_ip, const char *osc)
{
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_int32(m, d->value);
    lo_message_add_int32(m, d->dataID);
    
    data = lo_message_serialise(m, osc, NULL, NULL);
    d_len = lo_message_length(m, osc);
    
    if (strcmp(this->IPAddr,lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st), data, d_len);
    }else {
        lo_send_message(lo_ip, osc, m);
    }
    
    lo_message_free(m);
    free(data);
}

void Module::module_send(lo_blob b, lo_address lo_ip, const char *osc)
{
    void *data;
    unsigned long d_len;

    lo_message m = lo_message_new();
    lo_message_add_blob(m, b);
    data = lo_message_serialise(m, osc, NULL, NULL);
    d_len = lo_message_length(m, osc);

    if (strcmp(this->IPAddr,lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st), data, d_len);
    }else {
        lo_send_message(lo_ip, osc, m);
    }

    lo_message_free(m);
    free(data);
}

void Module::module_send(int value, int dataID, lo_address lo_ip, const char *osc)
{
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_int32(m, value);
    lo_message_add_int32(m, dataID);
    data = lo_message_serialise(m, osc, NULL, NULL);
    d_len = lo_message_length(m, osc);
    
    if (strcmp(this->IPAddr,lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st), data, d_len);
    }else {
        lo_send_message(lo_ip, osc, m);
    }
    
    lo_message_free(m);
    free(data);
}

void Module::module_send(float value, int dataID, lo_address lo_ip, const char *osc)
{
    void *data;
    unsigned long d_len;
    
    lo_message m = lo_message_new();
    lo_message_add_float(m, value);
    lo_message_add_int32(m, dataID);
    data = lo_message_serialise(m, osc, NULL, NULL);
    d_len = lo_message_length(m, osc);
    
    if (strcmp(this->IPAddr,lo_address_get_hostname(lo_ip))==0) {
        lo_server_dispatch_data(lo_server_thread_get_server(st->st), data, d_len);
    }else {
        lo_send_message(lo_ip, osc, m);
    }
    
    lo_message_free(m);
    free(data);
}

void Module::sendData(float value, int dataID)
{
    for (int i=0; i<rTable->aNum; i++) {
        if (strstr(rTable->oscAddr[i], "/Data")) {
            module_send(value, dataID, rTable->loAddr[i], rTable->oscAddr[i]);
        }
    }
}

void Module::sendDataTo(int value, int dataID, int outID)
{
    if(outNum <= outID) {printf("too many outID!\n"); return;}
    
    for (int i=0; i<rts[outID]->aNum; i++) {
        module_send(value, dataID, rts[outID]->loAddr[i], rts[outID]->oscAddr[i]);
    }
}

void Module::sendDataTo(float value, int dataID, int outID)
{
    for (int i=0; i<rts[outID]->aNum; i++) {
        module_send(value, dataID, rts[outID]->loAddr[i], rts[outID]->oscAddr[i]);
    }
}

void Module::sendData(Data *d, lo_arg **argv, int argc)
{
    if (d) {
        switch (argc) {
            case 0:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        module_send(d, rTable->loAddr[i], rTable->oscAddr[i]);
                    }
                }
                break;
            
            case 2:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "iiii",
                                argv[0]->i,
                                argv[1]->i,
                                d->value,
                                d->dataID);
                    }
                } 
                break;
            
            default:
                break;
        }
    }else {
        switch (argc) {
            case 2:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "ii", 
                                argv[0]->i,
                                argv[1]->i);
                    }
                }
                break;
                
            case 4:
                for (int i=0; i<rTable->aNum; i++) {
                    if (strstr(rTable->oscAddr[i], "/Data")) {
                        lo_send(rTable->loAddr[i], 
                                rTable->oscAddr[i],
                                "iiii",
                                argv[0]->i,
                                argv[1]->i,
                                argv[2]->i,
                                argv[3]->i);
                    }
                } 
                break;
                
            case 0:
                break;
                
            default:
                break;
        }
    }
}

void Module::sendAudio(Audio *a)
{
    Module::sendAudio(a->audio, a->length);
}

void Module::sendAudio(short *a, unsigned long l)
{
    lo_blob b = lo_blob_new((int)l, a);
    for (int i=0; i<rTable->aNum; i++) {
        if (strstr(rTable->oscAddr[i], "/Audio")) {
            module_send(b, rTable->loAddr[i], rTable->oscAddr[i]);
        }
    }
    lo_blob_free(b);
}

void Module::sendAudioTo(short *a, unsigned long l, int outID) {
    lo_blob b = lo_blob_new((int)l, a);
    for (int i=0; i<rts[outID]->aNum; i++)
        module_send(b, rts[outID]->loAddr[i], rts[outID]->oscAddr[i]);
    lo_blob_free(b);
}

void Module::sendAudio(lo_blob b1, lo_blob b2) {

    for (int i=0; i<rTable->aNum; i++) {
        if (strstr(rTable->oscAddr[i], "/Audio")) {
            lo_send(rTable->loAddr[i], 
                    rTable->oscAddr[i],
                    "bb", 
                    b1,b2);
        }
    }
}

void Module::sendAudio(Audio *a1, Audio *a2) {
    
    lo_blob b1 = lo_blob_new((int)a1->length, a1->audio);
    lo_blob b2 = lo_blob_new((int)a2->length, a2->audio);
    
    for (int i=0; i<rTable->aNum; i++) {
        if (strstr(rTable->oscAddr[i], "/Audio")) {
            lo_send(rTable->loAddr[i],
                    rTable->oscAddr[i],
                    "bb",
                    b1,b2);
        }
    }
    lo_blob_free(b1);
    lo_blob_free(b2);

}

void Module::getAddr()
{
    struct ifaddrs *ifa_list;
    struct ifaddrs *ifa;
    int n;
    char addrstr[256], netmaskstr[256];
    
    /* (1) */
    n = getifaddrs(&ifa_list);
    if (n != 0) {
        return;
    }
    
    /* (2) */
    for(ifa = ifa_list; ifa != NULL; ifa=ifa->ifa_next) {
        
        /* (3) */
        //printf("%s\n", ifa->ifa_name);
        //printf("  0x%.8x\n", ifa->ifa_flags);
        
        memset(addrstr, 0, sizeof(addrstr));
        memset(netmaskstr, 0, sizeof(netmaskstr));
        
        if (ifa->ifa_addr->sa_family == AF_INET) {  /* (4) */
            inet_ntop(AF_INET,
                      &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr,
                      addrstr, sizeof(addrstr));
            
            inet_ntop(AF_INET,
                      &((struct sockaddr_in *)ifa->ifa_netmask)->sin_addr,
                      netmaskstr, sizeof(netmaskstr));
            
            //printf("  IPv4: %s netmask %s\n", addrstr, netmaskstr);
            if (strstr(ifa->ifa_name, "en")) {
                strcpy(IPAddr, addrstr);
                //printf("  IPv4: %s netmask %s\n", addrstr, netmaskstr);
        }
            
        } else if (ifa->ifa_addr->sa_family == AF_INET6) { /* (5) */
            inet_ntop(AF_INET6,
                      &((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr,
                      addrstr, sizeof(addrstr));
            
            inet_ntop(AF_INET6,
                      &((struct sockaddr_in6 *)ifa->ifa_netmask)->sin6_addr,
                      netmaskstr, sizeof(netmaskstr));
            
            //printf("  IPv6: %s netmask %s\n", addrstr, netmaskstr);
        } else { /* (6) */
            //printf("  else:%d\n", ifa->ifa_addr->sa_family);
        }
    }
    
    /* (7) */
    freeifaddrs(ifa_list);
}

void Module::setOSCAddr(const char *osc)
{
    strcpy(OSCAddr, osc);
}

void Module::addMethodToServer(const char *path, const char *type, lo_method_handler h, void *user_data)
{
    char p[64];
    strcpy(p, OSCAddr);
    strcat(p, path);
    lo_server_thread_add_method(st->st, p, type, h, user_data);
}

void Module::addMethodToTCPServer(const char *path, const char *type, lo_method_handler h, void *user_data)
{
    char p[64];
    strcpy(p, OSCAddr);
    strcat(p, path);
    lo_server_thread_add_method(st->st_tcp, p, type, h, user_data);
}

void Module::deleteMethodFromServer(const char *path, const char *type)
{
    char p[64];
    strcpy(p, OSCAddr);
    strcat(p, path);
    lo_server_thread_del_method(st->st, p, type);
}

void Module::deleteMethodFromTCPServer(const char *path, const char *type)
{
    char p[64];
    strcpy(p, OSCAddr);
    strcat(p, path);
    lo_server_thread_del_method(st->st_tcp, p, type);
}

void Module::setRoute(char *ip, char *osc)
{
    if (rTable->isRouting(ip, osc) && rTable->numRoute(osc) == 1) return;
    
    rTable->deleteAllRoute(osc);
    rTable->setRoute(ip, osc);
}

void Module::addRoute(char *ip, char *osc)
{
    rTable->setRoute(ip, osc);
}

void Module::addRoute(char *ip, char *osc, int outID)
{
    rts[outID]->setRoute(ip, osc);
}

void Module::deleteRoute(char *ip,char *osc)
{
    rTable->deleteRoute(ip, osc);
}

void Module::deleteRoute(char *ip,char *osc, int outID)
{
    rts[outID]->deleteRoute(ip, osc);
}

void Module::deleteAllRoute(char *osc)
{
    rTable->deleteAllRoute(osc);
}

void Module::connectTo(Module *m, const char *t)
{
	char p[64];
	strcpy(p, m->OSCAddr);
    strcat(p, t);
	addRoute(m->IPAddr, p);
}

void Module::connectTo(Module *m, const char *t, int outID)
{
    char p[64];
    strcpy(p, m->OSCAddr);
    strcat(p, t);
    addRoute(m->IPAddr, p, outID);
}

void Module::disconnectFrom(Module *m, const char *t)
{
	char p[64];
	strcpy(p, m->OSCAddr);
    strcat(p, t);
	deleteRoute(m->IPAddr, p);
}

void Module::disconnectFrom(Module *m, const char *t, int outID)
{
    char p[64];
    strcpy(p, m->OSCAddr);
    strcat(p, t);
    deleteRoute(m->IPAddr, p, outID);
}

char* Module::getSenderIP() {
    lo_server_thread lst = st->st;
    lo_server ls = lo_server_thread_get_server(lst);
    struct sockaddr_in *sin = (struct sockaddr_in *)lo_server_get_addr(ls);
    
    char ip[16];
    strcpy(ip, inet_ntoa(sin->sin_addr));
    return inet_ntoa(sin->sin_addr);
}

char* Module::getSenderTCPIP() {
    lo_server_thread lst = st->st_tcp;
    lo_server ls = lo_server_thread_get_server(lst);
    struct sockaddr_in *sin = (struct sockaddr_in *)lo_server_get_addr(ls);
    
    char ip[16];
    strcpy(ip, inet_ntoa(sin->sin_addr));
    return inet_ntoa(sin->sin_addr);
}

void Module::setCoIP() {
    
    strcpy(CoIP, getSenderTCPIP());
}

Module::~Module()
{
    sendDeleteMdtkn();
	deleteMethodFromTCPServer("/SetRoute", "ss");
	deleteMethodFromTCPServer("/AddRoute", "ss");
    deleteMethodFromTCPServer("/AddRoute", "ssi");
    deleteMethodFromTCPServer("/DeleteRoute", "ss");
    deleteMethodFromTCPServer("/DeleteRoute", "ssi");
	deleteMethodFromTCPServer("/DeleteAllRoute", "s");
    //delete rTable;
    auto it = rts.begin();
    //it++;
    while(it != rts.end()) {
        RoutingTable *r = *it;
        delete r;
        it = rts.erase(it);
    }
}