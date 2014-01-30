/*
 * Copyright (c) 2014 Cisco Systems, Inc.  All rights reserved.
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
#include <jansson.h>

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

/**
 * JSON Encode the data
 *
 * \param network A pointer to the network to process
 *
 * Returns
 *   A valid json object representing the network information
 */
NETLOC_DECLSPEC json_t* netloc_dt_network_t_json_encode(netloc_network_t *network);

/**
 * JSON Decode the data
 *
 * User is responsible for calling _destruct on the returned pointer.
 *
 * \param json_nw A point to a valid json object representing the network information
 *
 * Returns
 *   A newly allocated network type filled in with the stored information
 */
NETLOC_DECLSPEC netloc_network_t* netloc_dt_network_t_json_decode(json_t *json_nw);

/*************************************************/

/**
 * JSON Encode the data
 *
 * \param edge A pointer to the edge to process
 *
 * Returns
 *   A valid json object representing the edge information
 */
NETLOC_DECLSPEC json_t* netloc_dt_edge_t_json_encode(netloc_edge_t *edge);

/**
 * JSON Decode the data
 *
 * User is responsible for calling _destruct on the returned pointer.
 *
 * \param json_edge A point to a valid json object representing the edge information
 *
 * Returns
 *   A newly allocated edge type filled in with the stored information
 */
NETLOC_DECLSPEC netloc_edge_t* netloc_dt_edge_t_json_decode(json_t *json_edge);

/*************************************************/

/**
 * JSON Encode the data
 *
 * This will -not- encode the path information
 *
 * \param node A pointer to the node to process
 *
 * Returns
 *   A valid json object representing the node information
 */
NETLOC_DECLSPEC json_t* netloc_dt_node_t_json_encode(netloc_node_t *node);

/**
 * JSON Decode the data
 *
 * This will -not- decode the path information
 * User is responsible for calling _destruct on the returned pointer.
 *
 * \param json_node A point to a valid json object representing the node information
 *
 * Returns
 *   A newly allocated node type filled in with the stored information
 */
NETLOC_DECLSPEC netloc_node_t* netloc_dt_node_t_json_decode(netloc_dt_lookup_table_t *edge_table, json_t *json_node);

/**
 * JSON Encode the paths in the data structure
 *
 * This -only- encodes the path information specified.
 *
 * \param node A pointer to the node to process
 * \param paths A pointer to the paths data to process
 *
 * Returns
 *   A valid json object representing the paths information for this node
 */
NETLOC_DECLSPEC json_t* netloc_dt_node_t_json_encode_paths(netloc_node_t *node, netloc_dt_lookup_table_t *paths);

/**
 * JSON Decode the paths in the data structure
 *
 * This will -only- decode the path information
 * User is responsible for calling _destruct on the returned pointer.
 *
 * \param json_all_paths A point to a valid json object representing the path information for a node
 *
 * Returns
 *   A newly allocated lookup table for the path information stored in the json object
 */
NETLOC_DECLSPEC netloc_dt_lookup_table_t * netloc_dt_node_t_json_decode_paths(netloc_dt_lookup_table_t *edge_table, json_t *json_all_paths);

/*************************************************/

/**
 * JSON Encode the data
 *
 * \param table A pointer to a lookup table
 * \param (*func) A function to encode the individual values
 *
 * Returns
 *   A valid json object representing the lookup table information
 */
NETLOC_DECLSPEC json_t* netloc_dt_lookup_table_t_json_encode(netloc_dt_lookup_table_t *table,
                                                             json_t* (*func)(const char * key, void *value));

/**
 * JSON Decode the data
 *
 * User is responsible for calling _destruct on the returned pointer.
 *
 * \param json_lt A pointer to a valid json object representing the lookup table information
 * \param (*func) A function to decode the individual values
 *
 * Returns
 *   A newly allocated lookup table type filled in with the stored information
 */
NETLOC_DECLSPEC netloc_dt_lookup_table_t* netloc_dt_lookup_table_t_json_decode(json_t *json_lt,
                                                                               void * (*func)(const char *key, json_t* json_obj));

/*************************************************/

#endif // _NETLOC_PRIVATE_H_
