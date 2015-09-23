//
//  SimpleHash.h
//  MITAI
//
//  Created by Musashi Nakajima on 2015/09/15.
//  Copyright (c) 2015年 Musashi Nakajima. All rights reserved.
//

#ifndef MITAI_SimpleHash_h
#define MITAI_SimpleHash_h

#include <stdio.h>
#include <stdint.h>

uint32_t fnv_1_hash_32(uint8_t *bytes, size_t length);
uint64_t fnv_1_hash_64(uint8_t *bytes, size_t length);

#endif