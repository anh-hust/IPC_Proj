/*
 * @Author: Alfie Bui
 * @Date: 2024-08-24
 *
 * @Last Modified by: Alfie Bui
 * @Last Modified time: 2024-08-24
 *
 * @Version: 0.1
 *
 * @Description: Definite the common macros and declaration
 *
 * @License: MIT License
 */

#ifndef __COMMON_DEF__
#define __COMMON_DEF__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH_OIF 16            // max length of interface string
#define IP_ADDR_LENGTH_UINT_TYPE 32  // use 32 bit to identify IPv4 address
#define IP_OCTET_BITS_MASK 8         // IPv4 is composed of 4 octets, each of them 8 bits

#define LOG_WITH_TAG(TAG, LOG_CONTENT, ...) \
    printf("{%s, func: %s, line: %d} -- [%s] " LOG_CONTENT "\n", __FILE__, __func__, __LINE__, TAG, ##__VA_ARGS__)

#define GEN_IP_TEMPLATE(UINT_IP) \
    (UINT_IP >> 24) & 0xFF,      \
        (UINT_IP >> 16) & 0xFF,  \
        (UINT_IP >> 8) & 0xFF,   \
        UINT_IP & 0xFF

#define GEN_ENTRY_TEMPLATE

// Log tag
#define DEBUG_TAG "DEBUG"
#define INFO_TAG "INFO"
#define ERROR_TAG "ERROR"

// return value
typedef enum {
    STATUS_ERROR = -1,
    STATUS_OK
} STATUS;

// CUD
typedef enum {
    CREATE = 1,
    UPDATE,
    DELETE
} OPCODE;

// body message (server -> client)
typedef struct {
    uint32_t destIP;
    uint8_t CIDR_prefix;
    uint32_t gwIP;
    char oif[MAX_LENGTH_OIF];
} entry_info_t;

// whole message
typedef struct {
    OPCODE cud_code;
    entry_info_t bodyMsg;
} sync_msg_t;

// using linked list to show the routing table
typedef struct {
    entry_info_t info;
    routing_table_ll* next;
} routing_table_ll;

/**
 * @brief These API help to do operations on Routing table
 *
 */
routing_table_ll* help_gen_entry(const uint32_t dest_ip, const uint8_t mask, const uint32_t gw_ip, const char* oif);
STATUS help_delete_entry(const routing_table_ll* table, const uint32_t dest_ip, const uint8_t prefix);
routing_table_ll* help_insert_new_entry(routing_table_ll* the_tail, const routing_table_ll* const new_entry);
routing_table_ll* help_update_existed_entry(routing_table_ll* const table, const uint32_t currentDestIP, const uint32_t currentPrefix, const uint32_t newGW_IP, const char* newOif);

/**
 * @brief These API to print some complicated data type
 *
 */
void print_uint_IP(uint32_t ipv4);
void print_entry_info(const entry_info_t* const entry);
void print_routing_table(const routing_table_ll* const head);

#endif