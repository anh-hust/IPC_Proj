#include "common.h"

/**
 * @brief generate a new entry of Routing table
 *
 * @param dest_ip
 * @param mask
 * @param gw_ip
 * @param oif
 * @return routing_table_ll* the pointer to new entry dynamic allocation
 *         NULL if fail to allocate
 */
routing_table_ll* help_gen_entry(const uint32_t dest_ip, const uint8_t mask, const uint32_t gw_ip, const char* oif) {
    routing_table_ll* new_entry = (routing_table_ll*)malloc(sizeof(routing_table_ll));
    if (new_entry == NULL) {
        LOG_WITH_TAG(ERROR_TAG, "Fail to allocate new entry !!!");
        return NULL;
    }

    new_entry->info.destIP = dest_ip;
    new_entry->info.CIDR_prefix = mask;
    new_entry->info.gwIP = gw_ip;
    strncpy(new_entry->info.oif, oif, MAX_LENGTH_OIF);

    new_entry->next = NULL;
    return new_entry;
}

/**
 * @brief Delete the existed entry, search the matching destination and CIDR prefix
 *          (the function will change table pointer to new head if matching entry is first entry)
 *
 * @param table
 * @param dest_ip
 * @param prefix
 * @return STATUS OK if found at least 1 matching entry
 *                ERROR if cannot found any matching entry
 */
STATUS help_delete_entry(const routing_table_ll* table, const uint32_t dest_ip, const uint8_t prefix) {
    STATUS ret = STATUS_ERROR;
    int del = 0;
    routing_table_ll* traverse = table;
    routing_table_ll* pre_traverse = NULL;

    while (traverse != NULL) {
        if (traverse->info.destIP == dest_ip && traverse->info.CIDR_prefix == prefix) {
            LOG_WITH_TAG(DEBUG_TAG, "Found the matching entry:");
            print_entry_info(&traverse->info);

            // Delete the entry of linked list
            if (pre_traverse = NULL) {
                // if head --> move the pointer table b/c we delete them
                pre_traverse = traverse->next;
                free(traverse);
                table = pre_traverse;  // update new head for table
                // back to loop after delete head
                traverse = table;
                pre_traverse = NULL;
                continue;
            }
            pre_traverse->next = traverse->next;
            free(traverse);
            traverse = pre_traverse->next;
            LOG_WITH_TAG(DEBUG_TAG, "Delte entry successfully !");
            ret = STATUS_OK;
            continue;
        }
        // if no match --> move on
        pre_traverse = traverse;
        traverse = traverse->next;
    }

    return ret;
}

/**
 * @brief Insert new entry to tail of the routing table, notice the first argument must be the tail, not the head
 *
 * @param the_tail: @NOTE is the pointer to tail of table (not the head)
 * @param new_entry
 * @return routing_table_ll* the pointer to the new tail after insert
 *         NULL if error (check log)
 */
routing_table_ll* help_insert_new_entry(routing_table_ll* the_tail, const routing_table_ll* const new_entry) {
    if (the_tail != NULL) {
        LOG_WITH_TAG(ERROR_TAG, "The tail pointer has already point to other entry:");
        print_entry_info(&the_tail->info);
        return NULL;
    }

    if (new_entry->next != NULL) {
        LOG_WITH_TAG(ERROR_TAG, "The new entry -> next should point to NULL");
        return NULL;
    }

    the_tail->next = new_entry;
    // update the tail
    the_tail = new_entry;
    return the_tail;
}

/**
 * @brief To update the entry, search by Destnation IP and CIDR prefix
 *
 * @param table
 * @param currentDestIP
 * @param currentPrefix
 * @param newGW_IP
 * @param newOif
 * @return routing_table_ll* pointer to the updated entry
 *         NULL if cannot found any matching entry
 */
routing_table_ll* help_update_existed_entry(routing_table_ll* const table, const uint32_t currentDestIP, const uint32_t currentPrefix, const uint32_t newGW_IP, const char* newOif) {
    routing_table_ll* traverse = table;
    int8_t found = 0;

    // find the matching entry
    while (traverse != NULL) {
        if (traverse->info.destIP == currentDestIP && traverse->info.CIDR_prefix == currentPrefix) {
            found = 1;
            break;
        }
        traverse = traverse->next;
    }

    if (found) {
        // update
        traverse->info.gwIP = newGW_IP;
        memset(traverse->info.oif, 0x0, MAX_LENGTH_OIF);
        strncpy(traverse->info.oif, newOif, MAX_LENGTH_OIF);
        LOG_WITH_TAG(DEBUG_TAG, "Update entry success, the new entry now:");
        print_entry_info(&traverse->info);
    } else {
        LOG_WITH_TAG(ERROR_TAG, "Cannot found any entry match with input IP !!!");
        return NULL;
    }

    return traverse;
}

/* =================================================================================================================== */
/* ========================================== PRINT FUNCTION ========================================================= */
/* =================================================================================================================== */

void print_uint_IP(uint32_t ipv4) {
    printf("%u.%u.%u.%u\n", GEN_IP_TEMPLATE(ipv4));
}

void print_entry_info(const entry_info_t* const entry) {
    printf("%3u.%3u.%3u.%3u/%2u\t%3u.%3u.%3u.%3u\t%s\n",
           GEN_IP_TEMPLATE(entry->destIP), entry->CIDR_prefix, GEN_IP_TEMPLATE(entry->gwIP), entry->oif);
}

void print_routing_table(const routing_table_ll* const head) {
    if (head == NULL) {
        LOG_WITH_TAG(INFO_TAG, "The routing table is just empty !!!");
        return;
    }

    const routing_table_ll* traverse = head;
    while (traverse != NULL) {
        printf("\n********** Routing table **********\n");
        printf("<Destination/Subnet Mask>\t<Gateway IP>\t<OIF>\n");
        printf("%3u.%3u.%3u.%3u/%2u\t%3u.%3u.%3u.%3u\t%s\n",
               GEN_IP_TEMPLATE(head->info.destIP), head->info.CIDR_prefix, GEN_IP_TEMPLATE(head->info.gwIP), head->info.oif);
        printf("*************************************\n");
        traverse = traverse->next;
    };
}