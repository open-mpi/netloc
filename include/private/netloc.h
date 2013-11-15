/*
 * Copyright (c) 2013 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2013      University of Wisconsin-La Crosse.
 *                         All rights reserved.
 *
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 * See COPYING in top-level directory.
 *
 * $HEADER$
 */

#ifndef _NETLOC_PRIVATE_H_
#define _NETLOC_PRIVATE_H_

#include <netloc.h>

/**
 * Topology state used by the API functions.
 */
struct netloc_topology {
    /** Copy of the network structure */
    netloc_network_t *network;

    /** Lazy load the node list */
    bool nodes_loaded;

    /** Node List */
    int num_nodes;
    netloc_node_t **nodes;

    /** Lookup table for all edge information */
    netloc_dt_lookup_table_t *edges;
};

#endif // _NETLOC_PRIVATE_H_
