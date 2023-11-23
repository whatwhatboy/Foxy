#pragma once
#include "Rage.hpp"

class CHeaders {
public:
    char m_header_data[465]; //0x0000
}; //Size: 0x01D1
static_assert(sizeof(CHeaders) == 0x1D1);

class CHttpRequest {
public:
    char pad_0000[112]; //0x0000
    void* m_allocator; //0x0070
    char pad_0078[16]; //0x0078
    CHeaders* m_http_headers; //0x0088
    char pad_0090[1144]; //0x0090
    char* m_protocol; //0x0508
    char* m_base_url; //0x0510
    char* m_endpoint_data; //0x0518
}; //Size: 0x0520
static_assert(sizeof(CHttpRequest) == 0x520);