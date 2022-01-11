/*
 * Wazuh Module for SQLite database syncing
 * Copyright (C) 2015, Wazuh Inc.
 * November 29, 2016
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#ifndef WM_DATABASE
#define WM_DATABASE

#define WM_DATABASE_LOGTAG ARGV0 ":database"

typedef struct wm_database {
    int sync_agents;
    int real_time;
    int interval;
    int max_queued_events;
} wm_database;

extern int wdb_wmdb_sock;

// Read configuration and return a module (if enabled) or NULL (if disabled)
wmodule* wm_database_read();

/**
 * @brief Synchronizes a keystore with the agent table of global.db. It will insert
 *        the agents that are in the keystore and are not in global.db. It also
 *        will create all the agent artifacts.
 *        In addition it will remove from global.db in wazuh-db all the agents that
 *        are not in the keystore. Also it will remove all the artifacts for those
 *        agents.
 *
 * @param keys The keystore structure to be synchronized
 */
void sync_keys_with_wdb(keystore *keys);

/**
 * @brief Synchronizes a keystore with the legacy agents databases in var/db/agents.
 *        It will create a database for the agents in the keystore that doesn't
 *        have it created.
 *
 * @param keys The keystore structure to be synchronized
 */
void sync_keys_with_agents_db(keystore *keys);

/**
 * @brief Synchronizes the agents artifacts with wazuh-db. It will remove
 *        the databases of agents that are not in the agent table of
 *        global.db.
 */
void sync_agents_artifacts_dbs_with_wdb();

/**
 * @brief This function removes the legacy agent DB, the wazuh-db agent DB
 *        and the diff folder of an agent.
 *
 * @param agent_id The ID of the agent.
 * @param agent_name The name of the agent.
 */
void wm_clean_agent_artifacts(int agent_id, const char* agent_name);

#endif /* WM_DATABASE */
