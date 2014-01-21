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

#ifndef _NETLOC_H_
#define _NETLOC_H_

#include <netloc/autogen/config.h>
#include <netloc/rename.h>

#define _GNU_SOURCE // for asprintf
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <jansson.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup netloc_api Netloc API
 * @{
 */

/**********************************************************************
 * Enumerated types
 **********************************************************************/

/**
 * Definitions for Comparators
 */
#define NETLOC_CMP_SAME     0
#define NETLOC_CMP_SIMILAR -1
#define NETLOC_CMP_DIFF    -2


/**
 * Enumerated type for the various types of supported networks
 */
typedef enum {
    NETLOC_NETWORK_TYPE_ETHERNET    = 1,
    NETLOC_NETWORK_TYPE_INFINIBAND  = 2,
    NETLOC_NETWORK_TYPE_INVALID     = 3
} netloc_network_type_t;

/**
 * Encode the network type
 *
 * \param str_val String value to parse
 *
 * Returns
 *   A valid member of the netloc_network_type_t type
 */
static inline netloc_network_type_t netloc_encode_network_type(const char * str_val) {
    if( NULL == str_val ) {
        return NETLOC_NETWORK_TYPE_INVALID;
    }
    else if( 0 == strncmp(str_val, "ETH", strlen(str_val)) ) {
        return NETLOC_NETWORK_TYPE_ETHERNET;
    }
    else if( 0 == strncmp(str_val, "IB", strlen(str_val)) ) {
        return NETLOC_NETWORK_TYPE_INFINIBAND;
    }
    else {
        return NETLOC_NETWORK_TYPE_INVALID;
    }
}

/**
 * Decode the network type
 *
 * \param A valid member of the netloc_network_type_t type
 *
 * Returns
 *   NULL if the type is invalid
 *   A string for that netloc_network_type_t type
 */
static inline const char * netloc_decode_network_type(netloc_network_type_t net_type) {
    if( NETLOC_NETWORK_TYPE_ETHERNET == net_type ) {
        return "ETH";
    }
    else if( NETLOC_NETWORK_TYPE_INFINIBAND == net_type ) {
        return "IB";
    }
    else {
        return NULL;
    }
}

/**
 * Decode the network type into a human readable string
 *
 * \param A valid member of the netloc_network_type_t type
 *
 * Returns
 *   A string for that netloc_network_type_t type
 */
static inline const char * netloc_decode_network_type_readable(netloc_network_type_t net_type) {
    if( NETLOC_NETWORK_TYPE_ETHERNET == net_type ) {
        return "Ethernet";
    }
    else if( NETLOC_NETWORK_TYPE_INFINIBAND == net_type ) {
        return "InfiniBand";
    }
    else {
        return "Invalid";
    }
}


/**
 * Enumerated type for the various types of nodes
 */
typedef enum {
    NETLOC_NODE_TYPE_SWITCH  = 1,
    NETLOC_NODE_TYPE_HOST    = 2,
    NETLOC_NODE_TYPE_INVALID = 3
} netloc_node_type_t;

/**
 * Encode the node type
 *
 * \param str_val String value to parse
 *
 * Returns
 *   A valid member of the netloc_node_type_t type
 */
static inline netloc_node_type_t netloc_encode_node_type(const char * str_val) {
    if( NULL == str_val ) {
        return NETLOC_NODE_TYPE_INVALID;
    }
    else if( 0 == strncmp(str_val, "CA", strlen(str_val)) ) {
        return NETLOC_NODE_TYPE_HOST;
    }
    else if( 0 == strncmp(str_val, "SW", strlen(str_val)) ) {
        return NETLOC_NODE_TYPE_SWITCH;
    }
    else {
        return NETLOC_NODE_TYPE_INVALID;
    }
}

/**
 * Decode the node type
 *
 * \param A valid member of the netloc_node_type_t type
 *
 * Returns
 *   NULL if the type is invalid
 *   A string for that netloc_node_type_t type
 */
static inline const char * netloc_decode_node_type(netloc_node_type_t node_type) {
    if( NETLOC_NODE_TYPE_SWITCH == node_type ) {
        return "SW";
    }
    else if( NETLOC_NODE_TYPE_HOST == node_type ) {
        return "CA";
    }
    else {
        return NULL;
    }
}

/**
 * Decode the node type into a human readable string
 *
 * \param A valid member of the netloc_node_type_t type
 *
 * Returns
 *   NULL if the type is invalid
 *   A string for that netloc_node_type_t type
 */
static inline char * netloc_decode_node_type_readable(netloc_node_type_t node_type) {
    if( NETLOC_NODE_TYPE_SWITCH == node_type ) {
        return "Switch";
    }
    else if( NETLOC_NODE_TYPE_HOST == node_type ) {
        return "Host";
    }
    else {
        return "Invalid";
    }
}

/**
 * Return codes
 */
enum {
    NETLOC_SUCCESS         =  0,
    NETLOC_ERROR           = -1,
    NETLOC_ERROR_NOTDIR    = -2,
    NETLOC_ERROR_NOENT     = -3,
    NETLOC_ERROR_EMPTY     = -4,
    NETLOC_ERROR_MULTIPLE  = -5,
    NETLOC_ERROR_NOT_IMPL  = -6,
    NETLOC_ERROR_EXISTS    = -7,
    NETLOC_ERROR_NOT_FOUND = -8,
    NETLOC_ERROR_MAX       = -9
};

/**********************************************************************
 *        Structures
 **********************************************************************/

/**
 * Netloc Topology Context
 *
 * Must be initialized with netloc_attach()
 */
struct netloc_topology;
typedef struct netloc_topology * netloc_topology_t;


/**
 * Lookup table entry type
 */
struct netloc_lookup_table_entry_t {
    /** Key */
    const char *key;
    /** Value pointer */
    void *value;
    /** Hash key */
    unsigned long __key__;
};
typedef struct netloc_lookup_table_entry_t netloc_lookup_table_entry_t;

/**
 * Lookup table type
 */
struct netloc_dt_lookup_table_t {
    /** Table entries array */
    netloc_lookup_table_entry_t **ht_entries;
    /** Number of entries in the lookup table */
    size_t   ht_size;
    /** Number of filled entried in the lookup table */
    size_t   ht_used_size;
    /** Flags */
    unsigned long flags;
};
typedef struct netloc_dt_lookup_table_t netloc_dt_lookup_table_t;

/**
 * Lookup table iterator
 */
struct netloc_dt_lookup_table_iterator_t {
    /** A pointer to the lookup table */
    netloc_dt_lookup_table_t *htp;
    /** The current location in the table */
    size_t loc;
    /** Flag if we reached the end */
    bool at_end;
};
typedef struct netloc_dt_lookup_table_iterator_t netloc_dt_lookup_table_iterator_t;


/**
 * Netloc Network Type
 * Represents a single network.
 */
struct netloc_network_t {
    /** Type of network */
    netloc_network_type_t network_type;

    /** Subnet ID */
    char * subnet_id;

    /** Data URI */
    char * data_uri;

    /** Node URI */
    char * node_uri;

    /** Physical Path URI */
    char * phy_path_uri;

    /** Path URI */
    char * path_uri;

    /** Description information from discovery (if any) */
    char * description;

    /** Metadata about network information */
    char * version;

    /**
     * Application-given private data pointer.
     * Initialized to NULL, and not used by the netloc library.
     */
    void * userdata;
};
typedef struct netloc_network_t netloc_network_t;

/**
 * Netloc Edge Type
 * Represents the concept of a directed edge within a network graph.
 *
 * Note that we do not point to the netloc_node_t structure directly to 
 * simplify the representation, and allow the information to more easily
 * be entered into the data store without circular references.
 */
struct netloc_node_t;
typedef struct netloc_node_t netloc_node_t;

#define NETLOC_EDGE_UID_START    0
#define NETLOC_EDGE_UID_NULL    -1
#define NETLOC_EDGE_UID_INVALID -2

struct netloc_edge_t {
    /** Unique Edge ID */
    int edge_uid;

    /** Source: Pointer to neloc_node_t */
    netloc_node_t *src_node;
    /** Source: Physical ID from netloc_node_t */
    char *      src_node_id;
    /** Source: Node type from netloc_node_t */
    netloc_node_type_t src_node_type;
    /** Source: Port number */
    char *      src_port_id;

    /** Dest: Pointer to neloc_node_t */
    netloc_node_t *dest_node;
    /** Dest: Physical ID from netloc_node_t */
    char *      dest_node_id;
    /** Dest: Node type from netloc_node_t */
    netloc_node_type_t dest_node_type;
    /** Dest: Port number */
    char *      dest_port_id;

    /** Metadata: Speed */
    char *      speed;
    /** Metadata: Width */
    char *      width;

    /** Description information from discovery (if any) */
    char * description;

    /**
     * Application-given private data pointer.
     * Initialized to NULL, and not used by the netloc library.
     */
    void * userdata;
};
typedef struct netloc_edge_t netloc_edge_t;

/**
 * Netloc Node Type
 * Represents the concept of a node (a.k.a., vertex, endpoint) within a network graph.
 */
struct netloc_node_t {
    /** Type of the network connection */
    netloc_network_type_t  network_type;

    /** Type of the node */
    netloc_node_type_t     node_type;

    /** Physical ID of the node (must be unique) */
    char *          physical_id;
    unsigned long   physical_id_int;

    /** Logical ID of the node (if any) */
    char *          logical_id;

    /** Internal unique ID: 0 - N */
    int __uid__;

    /** Subnet ID */
    char * subnet_id;

    /** Description information from discovery (if any) */
    char * description;

    /**
     * Application-given private data pointer.
     * Initialized to NULL, and not used by the netloc library.
     */
    void * userdata;

    /** Outgoing edges from this node */
    int num_edges;
    netloc_edge_t **edges;

    int num_edge_ids;
    int *edge_ids;

    /** Lookup table for physical paths from this node */
    int num_phy_paths;
    netloc_dt_lookup_table_t *physical_paths;

    /** Lookup table for logical paths from this node */
    int num_log_paths;
    netloc_dt_lookup_table_t *logical_paths;
};


/**********************************************************************
 * Datatype Support Functions
 **********************************************************************/
/**
 * Constructor for netloc_network_t
 *
 * User is responsible for calling the destructor on the handle.
 *
 * Returns
 *   A newly allocated pointer to the network information.
 */
NETLOC_DECLSPEC netloc_network_t * netloc_dt_network_t_construct();

/**
 * Destructor for netloc_network_t
 *
 * \param network A valid network handle
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_dt_network_t_destruct(netloc_network_t *network);

/**
 * Copy Constructor for netloc_network_t
 *
 * Allocates memory. User is responsible for calling _destruct on the returned pointer.
 * Does a shallow copy of the pointers to data.
 *
 * \param network A pointer to the network to duplicate
 *
 * Returns
 *   A newly allocated copy of the network.
 */
NETLOC_DECLSPEC netloc_network_t * netloc_dt_network_t_dup(netloc_network_t *network);

/**
 * Copy Function for netloc_network_t
 *
 * Does not allocate memory for 'to'. Does a shallow copy of the pointers to data.
 *
 * \param from A pointer to the network to duplicate
 * \param to A pointer to the network to duplicate into
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_dt_network_t_copy(netloc_network_t *from, netloc_network_t *to);

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

/**
 * Compare function for netloc_network_t
 *
 * \param a A pointer to one network object for comparison
 * \param b A pointer to the other network object for comparison
 *
 * Returns
 *  NETLOC_CMP_SAME if the same
 *  NETLOC_CMP_SIMILAR if only the metadata (e.g., version) is different
 *  NETLOC_CMP_DIFF if different
 */
NETLOC_DECLSPEC int netloc_dt_network_t_compare(netloc_network_t *a, netloc_network_t *b);


/*************************************************/


/**
 * Constructor for netloc_edge_t
 *
 * User is responsible for calling the destructor on the handle.
 *
 * Returns
 *   A newly allocated pointer to the edge information.
 */
NETLOC_DECLSPEC netloc_edge_t * netloc_dt_edge_t_construct();

/**
 * Destructor for netloc_edge_t
 *
 * \param edge A valid edge handle
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_dt_edge_t_destruct(netloc_edge_t *edge);

/**
 * Copy Constructor for netloc_edge_t
 *
 * Allocates memory. User is responsible for calling _destruct on the returned pointer.
 * Does a shallow copy of the pointers to data.
 *
 * \param edge A pointer to the edge to duplicate
 *
 * Returns
 *   A newly allocated copy of the edge.
 */
NETLOC_DECLSPEC netloc_edge_t * netloc_dt_edge_t_dup(netloc_edge_t *edge);

/**
 * Copy Function for netloc_edge_t
 *
 * Does not allocate memory for 'to'. Does a shallow copy of the pointers to data.
 *
 * \param from A pointer to the edge to duplicate
 * \param to A pointer to the edge to duplicate into
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_dt_edge_t_copy(netloc_edge_t *from, netloc_edge_t *to);

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

/**
 * Compare function for netloc_edge_t
 *
 * \param a A pointer to one edge object for comparison
 * \param b A pointer to the other edge object for comparison
 *
 * Returns
 *  NETLOC_CMP_SAME if the same
 *  NETLOC_CMP_DIFF if different
 */
NETLOC_DECLSPEC int netloc_dt_edge_t_compare(netloc_edge_t *a, netloc_edge_t *b);


/*************************************************/


/**
 * Constructor for netloc_node_t
 *
 * User is responsible for calling the destructor on the handle.
 *
 * Returns
 *   A newly allocated pointer to the network information.
 */
NETLOC_DECLSPEC netloc_node_t * netloc_dt_node_t_construct();

/**
 * Destructor for netloc_node_t
 *
 * \param node A valid node handle
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_dt_node_t_destruct(netloc_node_t *node);

/**
 * Copy Constructor for netloc_node_t
 *
 * Allocates memory. User is responsible for calling _destruct on the returned pointer.
 * Does a shallow copy of the pointers to data.
 *
 * \param node A pointer to the node to duplicate
 *
 * Returns
 *   A newly allocated copy of the node.
 */
NETLOC_DECLSPEC netloc_node_t * netloc_dt_node_t_dup(netloc_node_t *node);

/**
 * Copy Function for netloc_node_t
 *
 * Does not allocate memory for 'to'. Does a shallow copy of the pointers to data.
 *
 * \param from A pointer to the node to duplicate
 * \param to A pointer to the node to duplicate into
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_dt_node_t_copy(netloc_node_t *from, netloc_node_t *to);

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

/**
 * Compare function for netloc_node_t
 *
 * \param a A pointer to one network object for comparison
 * \param b A pointer to the other network object for comparison
 *
 * Returns
 *  NETLOC_CMP_SAME if the same
 *  NETLOC_CMP_DIFF if different
 */
NETLOC_DECLSPEC int netloc_dt_node_t_compare(netloc_node_t *a, netloc_node_t *b);


/**
 * Convert a string MAC address (':' separated) into a whole number value
 *
 * \param mac String MAC address
 *
 * Returns
 *  whole number encoding of that value
 */
NETLOC_DECLSPEC unsigned long netloc_dt_convert_mac_str_to_int(const char * mac);

/**
 * Convert a value encoding a MAC address into a string representation (':' separated)
 *
 * Caller is responsible for free'ing the pointer returned.
 *
 * \param valu encoded value MAC address
 *
 * Returns
 *  NULL on error
 *  otherwise string representation.
 */
NETLOC_DECLSPEC char * netloc_dt_convert_mac_int_to_str(const unsigned long value);

/**
 * Convert a string GUID address (':' separated) into a whole number value
 *
 * \param guid String GUID address
 *
 * Returns
 *  whole number encoding of that value
 */
NETLOC_DECLSPEC unsigned long netloc_dt_convert_guid_str_to_int(const char * guid);

/**
 * Convert a value encoding a GUID address into a string representation (':' separated)
 *
 * Caller is responsible for free'ing the pointer returned.
 *
 * \param valu encoded value GUID address
 *
 * Returns
 *  NULL on error
 *  otherwise string representation.
 */
NETLOC_DECLSPEC char * netloc_dt_convert_guid_int_to_str(const unsigned long value);


/**********************************************************************
 * Datatype Support Functions for Lookup Tables
 **********************************************************************/

/**
 * Copy a lookup table and all entries
 *
 * Note that the pointers are copied for each entry. The user is responsible for reference counting.
 *
 * \param from A pointer to the lookup table to duplicate
 * \param to A pointer to the lookup table to duplicate into
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_dt_lookup_table_t_copy(netloc_dt_lookup_table_t *from, netloc_dt_lookup_table_t *to);

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


/**
 * Constructor for a lookup table iterator
 *
 * User is responsible for calling the destructor on the handle.
 *
 * \param table The table to reference in this iterator
 *
 * Returns
 *   A newly allocated pointer to the lookup table iterator.
 */
NETLOC_DECLSPEC netloc_dt_lookup_table_iterator_t *netloc_dt_lookup_table_iterator_t_construct(netloc_dt_lookup_table_t *table);

/**
 * Destructor for a lookup table iterator
 *
 * \param table A valid lookup table iterator handle
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_dt_lookup_table_iterator_t_destruct(netloc_dt_lookup_table_iterator_t* hti);


/**********************************************************************
 * Lookup table API Functions
 **********************************************************************/

enum netloc_lookup_table_init_flag_e {
    /* Don't duplicate keys inside the table, assume the given string
     * will remain valid and unchanged during the entire table life.
     */
    NETLOC_LOOKUP_TABLE_FLAG_NO_STRDUP_KEY = (1UL<<0)
};

/**
 * Initialize the lookup table
 *
 * The lookup table must have been memset'ed to 0 before calling this function.
 *
 * \param table The lookup table to initialize
 * \param size Initial table size (will automaticly expand as necessary)
 * \param flags Flags to tune the table, OR'ed set of netloc_lookup_table_init_flag_e.
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_lookup_table_init(netloc_dt_lookup_table_t *table, size_t size, unsigned long flags);

/**
 * Destroy a lookup table.
 */
NETLOC_DECLSPEC int netloc_lookup_table_destroy(netloc_dt_lookup_table_t *table);

/**
 * Access the -allocated- size of the hash table
 *
 * \param table A valid pointer to a lookup table
 *
 * Returns
 *   The allocated size of the hash table
 */
static inline int netloc_lookup_table_size_alloc(netloc_dt_lookup_table_t *table) {
    if( NULL == table ) {
        return 0;
    } else {
        return table->ht_size;
    }
}

/**
 * Access the -used- size of the hash table
 *
 * \param table A valid pointer to a lookup table
 *
 * Returns
 *   The used size of the hash table
 */
static inline int netloc_lookup_table_size(netloc_dt_lookup_table_t *table) {
    if( NULL == table ) {
        return 0;
    } else {
        return table->ht_used_size;
    }
}

/**
 * Append an entry to the hash table
 * 
 * \param ht A valid pointer to a lookup table
 * \param key The key used to find the data
 * \param value The pointer to associate with this key
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
*/
NETLOC_DECLSPEC int netloc_lookup_table_append(netloc_dt_lookup_table_t *ht, const char *key, void *value);

/**
 * Append an entry to the hash table while specifying the integer key to use
 * (instead of calculating it)
 *
 * Warning: This interface is only used internally at the moment.
 * Warning: In order for this interface to work, you must use all of the
 *          *_with_int() methods when interacting with this loopup table.
 * 
 * \param ht A valid pointer to a lookup table
 * \param key The key used to find the data
 * \param key_int The unique integer key used to find the data
 * \param value The pointer to associate with this key
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_lookup_table_append_with_int(netloc_dt_lookup_table_t *ht, const char *key, unsigned long key_int, void *value);

/**
 * Access an entry in the hash table
 *
 * \param ht A valid pointer to a lookup table
 * \param key The key used to find the data
 *
 * Returns
 *   NULL if nothing found
 *   The pointer stored from a prior call to netloc_lookup_table_append
 */
NETLOC_DECLSPEC void * netloc_lookup_table_access(netloc_dt_lookup_table_t *ht, const char *key);

/**
 * Access an entry to the hash table while specifying the integer key to use
 * (instead of calculating it)
 *
 * Warning: This interface is only used internally at the moment.
 * Warning: In order for this interface to work, you must use all of the
 *          *_with_int() methods when interacting with this loopup table.
 * 
 * \param ht A valid pointer to a lookup table
 * \param key The key used to find the data
 * \param key_int The unique integer key used to find the data
 *
 * Returns
 *   NULL if nothing found
 *   The pointer stored from a prior call to netloc_lookup_table_append
 */
NETLOC_DECLSPEC void * netloc_lookup_table_access_with_int(netloc_dt_lookup_table_t *ht, const char *key, unsigned long key_int);

/**
 * Replace an entry in the hash table with the provided value
 *
 * \param ht A valid pointer to a lookup table
 * \param key The key used to find the data
 * \param value The pointer to associate with this key
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_lookup_table_replace(netloc_dt_lookup_table_t *ht, const char *key, void *value);

/**
 * Replace an entry in the hash table with the provided value
 * while specifying the integer key to use (instead of calculating it)
 *
 * Warning: This interface is only used internally at the moment.
 * Warning: In order for this interface to work, you must use all of the
 *          *_with_int() methods when interacting with this loopup table.
 * 
 * \param ht A valid pointer to a lookup table
 * \param key The key used to find the data
 * \param key_int The unique integer key used to find the data
 * \param value The pointer to associate with this key
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_lookup_table_replace_with_int(netloc_dt_lookup_table_t *ht, const char *key, unsigned long key_int, void *value);

/**
 * Remove an entry from the hash table.
 *
 * \param ht A valid pointer to a lookup table
 * \param key The key used to find the data
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_lookup_table_remove(netloc_dt_lookup_table_t *ht, const char *key);

/**
 * Remove an entry from the hash table.
 * while specifying the integer key to use (instead of calculating it)
 *
 * Warning: This interface is only used internally at the moment.
 * Warning: In order for this interface to work, you must use all of the
 *          *_with_int() methods when interacting with this loopup table.
 * 
 * \param ht A valid pointer to a lookup table
 * \param key The key used to find the data
 * \param key_int The unique integer key used to find the data
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR on error
 */
NETLOC_DECLSPEC int netloc_lookup_table_remove_with_int(netloc_dt_lookup_table_t *ht, const char *key, unsigned long key_int);

/**
 * Pretty print the lookup table to stdout (Debugging Support)
 *
 * \param ht A valid pointer to a lookup table
 */
NETLOC_DECLSPEC void netloc_lookup_table_pretty_print(netloc_dt_lookup_table_t *ht);

/**
 * Get the next key and advance the iterator
 *
 * The user should -not- call free() on the string returned.
 *
 * \param hti A valid pointer to a lookup table iterator
 *
 * Returns
 *   NULL if error or at end
 *   A newly allocated string copy of the key.
 */
NETLOC_DECLSPEC const char * netloc_lookup_table_iterator_next_key(netloc_dt_lookup_table_iterator_t* hti);

/**
 * Get the next key and advance the iterator
 *
 * \param hti A valid pointer to a lookup table iterator
 *
 * Returns
 *   internal hash key, 0 if error
 */
NETLOC_DECLSPEC unsigned long netloc_lookup_table_iterator_next_key_int(netloc_dt_lookup_table_iterator_t* hti);

/**
 * Get the next entry and advance the iterator
 *
 * Similar to netloc_lookup_table_iterator_next_key() except the caller is
 * given the next value directly. So they do not need to call the 
 * netloc_lookup_table_access() function to access the value.
 *
 * \param hti A valid pointer to a lookup table iterator
 *
 * Returns
 *   NULL if error or at end
 *   The pointer stored from a prior call to netloc_lookup_table_append
 */
NETLOC_DECLSPEC void * netloc_lookup_table_iterator_next_entry(netloc_dt_lookup_table_iterator_t* hti);

/**
 * Check if we are at the end of the iterator
 *
 * \param hti A valid pointer to a lookup table iterator
 *
 * Returns
 *   true if at the end of the data, false otherwise
 */
static inline bool netloc_lookup_table_iterator_at_end(netloc_dt_lookup_table_iterator_t* hti) {
    return hti->at_end;
}

/**
 * Reset the iterator back to the start
 *
 * \param hti A valid pointer to a lookup table iterator
 *
 */
static inline void netloc_lookup_table_iterator_reset(netloc_dt_lookup_table_iterator_t* hti) {
    hti->loc = 0;
    hti->at_end = false;
}



/**********************************************************************
 * Network Metadata API Functions
 **********************************************************************/
/**
 * Pretty print the network (Debugging Support)
 *
 * The user is responsible for calling free() on the string returned.
 *
 * \param network A valid pointer to a network
 *
 * Returns
 *   A newly allocated string representation of the network.
 */
NETLOC_DECLSPEC char * netloc_pretty_print_network_t(netloc_network_t* network);

/**
 * Pretty print the edge (Debugging Support)
 *
 * The user is responsible for calling free() on the string returned.
 *
 * \param edge A valid pointer to an edge
 *
 * Returns
 *   A newly allocated string representation of the edge.
 */
NETLOC_DECLSPEC char * netloc_pretty_print_edge_t(netloc_edge_t* edge);

/**
 * Pretty print the node (Debugging Support)
 *
 * The user is responsible for calling free() on the string returned.
 *
 * \param node A valid pointer to a node
 *
 * Returns
 *   A newly allocated string representation of the node.
 */
NETLOC_DECLSPEC char * netloc_pretty_print_node_t(netloc_node_t* node);

/**
 * Find a specific network at the URI specified.
 *
 * \param network_topo_uri URI to search for the specified network.
 * \param network  Netloc network handle (IN/OUT)
 *                 A network handle with the data structure fields set to
 *                 specify the search. For example, the user can set 'IB'
 *                 and nothing else, if they do not know the subnet or any
 *                 of the other necessary information. If the method
 *                 returns success then the network handle will be filled
 *                 out with the rest of the information found. If the method
 *                 returns some error then the network handle is not modified.
 *
 * Returns
 *   NETLOC_SUCCESS if exactly one network matches the specification,
 *                  and updates the network handle.
 *   NETLOC_ERROR_MULTIPLE if more than one network matches the spec.
 *   NETLOC_ERROR_EMPTY if no networks match the specification.
 *   NETLOC_ERROR_NOENT if the directory does not exist.
 *   NETLOC_ERROR_NOTDIR if the data_dir is not a directory.
 *   NETLOC_ERROR if something else is wrong.
 */
NETLOC_DECLSPEC int netloc_find_network(const char * network_topo_uri,
                                        netloc_network_t* network);

/**
 * Find all available networks in the specified URIs
 *
 * User is responsible for calling the destructor for each element of the networks array
 * paramater, then free() on the entire array.
 *
 * \param search_uris Array of URIs. file:// syntax is the only supported mechanism
 *                    at the moment. Array is searched for .dat files. All uris will
 *                    be searched. If NULL is supplied then the default search path
 *                    will be used (currently the CWD).
 * \param num_uris Size of the search_uris array.
 * \param (*func) A callback function triggered for each network found the user
 *                is provided an opportunity to decide if it should be included
 *                in the "networks" array or not. "net" is a handle to the
 *                network information (includes uri where it was found).
 *                If the callback returns non-zero then the entry is added to
 *                the networks array. If the callback returns 0 then the entry
 *                is not added to the networks array. If NULL is supplied as 
 *                an argument for this function pointer then all networks are
 *                included in the array.
 * \param funcdata User specified data pointer to be passed to the callback function.
 * \param num_networks Size of the networks array.
 * \param networks An array of networks discovered.
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR otherwise
 */
NETLOC_DECLSPEC int netloc_foreach_network(const char * const * search_uris,
                                             int num_uris,
                                             int (*func)(const netloc_network_t *network, void *funcdata),
                                             void *funcdata,
                                             int *num_networks,
                                             netloc_network_t ***networks);


/**********************************************************************
 * Topology API Functions
 **********************************************************************/
/**
 * Attach to the specified network, and allocate a topology handle.
 *
 * User is responsible for calling netloc_detach on the topology handle.
 * The network parameter information is deep copied into the topology handle, so the
 * user may destruct the network handle after calling this function and/or reuse
 * the network handle.
 *
 * \param topology A pointer to a netloc_topology_t handle.
 * \param network The netloc_network_t handle from a prior call to either:
 *                - netloc_find_network()
 *                - netloc_foreach_network()
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR upon an error.
 */
NETLOC_DECLSPEC int netloc_attach(netloc_topology_t * topology, netloc_network_t network);

/**
 * Detach from a topology handle
 *
 * \param topology A valid pointer to a netloc_topology_t handle created from a prior call to netloc_attach().
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR upon an error.
 */
NETLOC_DECLSPEC int netloc_detach(netloc_topology_t topology);

/**
 * Refresh the data associated with the topology.
 *
 * Note: This interface is not currently implemented.
 *
 * \param topology A valid pointer to a netloc_topology_t handle created from a prior call to netloc_attach().
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR upon an error.
 */
NETLOC_DECLSPEC int netloc_refresh(netloc_topology_t topology);


/**********************************************************************
 * Query API Functions
 **********************************************************************/
/**
 * Access a reference to the netloc_network_t associated with the netloc_topology_t
 *
 * The user should -not- call the netloc_network_t's destructor on the reference returned.
 *
 * \param topology A valid pointer to a topology handle
 *
 * Returns
 *   A reference to the netloc_network_t associtated with the topology
 *   NULL on error.
 */
NETLOC_DECLSPEC netloc_network_t* netloc_access_network_ref(netloc_topology_t topology);

/**
 * Get all nodes in the network topology
 * 
 * The user is responsible for calling the lookup table destructor on the nodes table.
 * The user should -not- call the netloc_node_t's destructor on the elements in the lookup table.
 *
 * \param topology A valid pointer to a topology handle
 * \param nodes A lookup table of the nodes requested
 *              Keys in the table are the physical_id's of the netloc_node_t's
 *              The values are pointers to netloc_node_t's
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR upon an error.
 */
NETLOC_DECLSPEC int netloc_get_all_nodes(netloc_topology_t topology, netloc_dt_lookup_table_t **nodes);

/**
 * Get only switch nodes in the network topology
 * 
 * The user is responsible for calling the lookup table destructor on the nodes table.
 * The user should -not- call the netloc_node_t's destructor on the elements in the lookup table.
 *
 * \param topology A valid pointer to a topology handle
 * \param nodes A lookup table of the nodes requested
 *              Keys in the table are the physical_id's of the netloc_node_t's
 *              The values are pointers to netloc_node_t's
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR upon an error.
 */
NETLOC_DECLSPEC int netloc_get_all_switch_nodes(netloc_topology_t topology, netloc_dt_lookup_table_t **nodes);

/**
 * Get only host nodes in the network topology
 * 
 * The user is responsible for calling the lookup table destructor on the nodes table.
 * The user should -not- call the netloc_node_t's destructor on the elements in the lookup table.
 * 
 * \param topology A valid pointer to a topology handle
 * \param nodes A lookup table of the nodes requested
 *              Keys in the table are the physical_id's of the netloc_node_t's
 *              The values are pointers to netloc_node_t's
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR upon an error.
 */
NETLOC_DECLSPEC int netloc_get_all_host_nodes(netloc_topology_t topology, netloc_dt_lookup_table_t **nodes);

/**
 * Get all of the edges from the specified node in the network topology.
 * There should be one edge for every active port on this node.
 *
 * The user should not free the array, neither its elements.
 *
 * \param topology A valid pointer to a topology handle
 * \param node A valid pointer to a netloc_node_t from which to get the edges.
 * \param num_edges The number of edges in the edges array.
 * \param edges An array of netloc_edge_t's
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR upon an error.
 */
NETLOC_DECLSPEC int netloc_get_all_edges(netloc_topology_t topology,
                                         netloc_node_t *node,
                                         int *num_edges,
                                         netloc_edge_t ***edges);

/**
 * Access the netloc_node_t pointer given a physical_id
 *
 * The user should -not- call the destructor on the returned value.
 *
 * \param topology A valid pointer to a topology handle
 * \param phy_id The physical_id to search for
 *
 * Returns
 *   A pointer to the netloc_node_t with the specified physical_id
 *   or NULL if the physical_id is not found.
 */
NETLOC_DECLSPEC netloc_node_t * netloc_get_node_by_physical_id(netloc_topology_t topology, const char * phy_id);

/**
 * Get the "path" from the source to the destination as an ordered array of netloc_edge_t's
 *
 * The user is responsible for calling free() on the allocated array, but -not- the elements in the array.
 *
 * Note: A large API change is in the works for v1.0 that will change how we represent path data.
 *
 * \param topology A valid pointer to a topology handle
 * \param src_node A valid pointer to the source node
 * \param dst_node A valid pointer to the destination node
 * \param num_edges The number of edges in the path array.
 * \param path An ordered array of netloc_edge_t's from the source to the destination
 * \param is_logical If the path should represent the logical or the physical path information.
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR upon an error.
 */
NETLOC_DECLSPEC int netloc_get_path(const netloc_topology_t topology,
                                    netloc_node_t *src_node,
                                    netloc_node_t *dst_node,
                                    int *num_edges,
                                    netloc_edge_t ***path,
                                    bool is_logical);


/**********************************************************************
 * Export API Functions
 **********************************************************************/
/**
 * Exports the network topology to a GraphML formatted file.
 *
 * \param topology A valid pointer to a topology handle
 * \param filename The filename to write the data to
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR upon an error.
 */
NETLOC_DECLSPEC int netloc_topology_export_graphml(netloc_topology_t topology, const char * filename);

/**
 * Exports the network topology to a GEXF formatted file.
 *
 * \param topology A valid pointer to a topology handle
 * \param filename The filename to write the data to
 *
 * Returns
 *   NETLOC_SUCCESS on success
 *   NETLOC_ERROR upon an error.
 */
NETLOC_DECLSPEC int netloc_topology_export_gexf(netloc_topology_t topology, const char * filename);


#ifdef __cplusplus
} /* extern "C" */
#endif

/** @} */

#endif // _NETLOC_H_
