#include "dump.hpp"
#include <stdio.h>

void pkt_header_dump(
    const mctp_transport_header_t *header
) {
    printf("MCTP Transport header\n");
    printf("version:    0x%X\n",    header->version);
    printf("dest:       0x%02X\n",  header->dest);
    printf("source:     0x%02X\n",  header->source);
    printf("tag:        0x%d\n",    header->tag);
    printf("tag_owner:  0x%s\n",    header->tag_owner ? "YES" : "NO");
    printf("pkt_seq:    0x%d\n",    header->pkt_seq);
    printf("som:        0x%s\n",    header->som ? "YES" : "NO");
    printf("eom:        0x%s\n",    header->eom ? "YES" : "NO");
    printf("\n");
}

void serial_header_dump(
    const mctp_serial_header_t *header
) {
    printf("MCTP Serial binding header\n");
    printf("frame flag: 0x%02X\n",  header->framing_flag);
    printf("revision:   0x%02X\n",  header->revision);
    printf("byte count: 0x%dX\n",  header->byte_count);
    printf("\n");
}

void serial_trailer_dump(
    const mctp_serial_trailer_t *trailer
) {
    printf("MCTP Serial binding trailer\n");
    printf("FCS high:   0x%02X\n",  trailer->fcs_high);
    printf("FCS low:    0x%02X\n",  trailer->fcs_low);
    printf("frame flag: 0x%02X\n",  trailer->framing_flag);
    printf("\n");
}
