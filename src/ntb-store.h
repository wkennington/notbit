/*
 * Notbit - A Bitmessage client
 * Copyright (C) 2013  Neil Roberts
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NTB_STORE_H
#define NTB_STORE_H

#include <stdint.h>

#include "ntb-blob.h"
#include "ntb-error.h"
#include "ntb-netaddress.h"
#include "ntb-key.h"

/* The store is used to do all of the disk I/O. The actions are stored
 * in a queue and then executed in a separate thread */

struct ntb_store;

extern struct ntb_error_domain
ntb_store_error;

enum ntb_store_error {
        NTB_STORE_ERROR_INVALID_STORE_DIRECTORY,
        NTB_STORE_ERROR_INVALID_MAILDIR
};

struct ntb_store_cookie;

struct ntb_store_addr {
        int64_t timestamp;
        uint32_t stream;
        uint64_t services;
        struct ntb_netaddress address;
};

typedef void (* ntb_store_for_each_blob_func)(enum ntb_proto_inv_type type,
                                              const uint8_t *hash,
                                              int64_t timestamp,
                                              void *user_data);

typedef void (* ntb_store_for_each_addr_func)(const struct ntb_store_addr *addr,
                                              void *user_data);

typedef void (* ntb_store_for_each_key_func)(struct ntb_key *key,
                                             void *user_data);

/* This is called when a load is complete. If the load succeeded then
 * blob will point to the contents. If it failed the callback will
 * still be called but blob will be NULL. The callback won't be called
 * at all if the task is cancelled. The callback will always be
 * invoked from an idle handler in the main thread */
typedef void (* ntb_store_load_callback)(struct ntb_blob *blob,
                                         void *user_data);

struct ntb_store *
ntb_store_new(const char *store_directory,
              const char *maildir,
              struct ntb_error **error);

void
ntb_store_start(struct ntb_store *store);

struct ntb_store *
ntb_store_get_default(void);

void
ntb_store_set_default(struct ntb_store *store);

void
ntb_store_save_blob(struct ntb_store *store,
                    const uint8_t *hash,
                    struct ntb_blob *blob);

void
ntb_store_delete_object(struct ntb_store *store,
                        const uint8_t *hash);

void
ntb_store_save_addr_list(struct ntb_store *store,
                         struct ntb_store_addr *addrs,
                         int n_addrs);

void
ntb_store_save_keys(struct ntb_store *store,
                    struct ntb_key * const *keys,
                    int n_keys);

void
ntb_store_save_message(struct ntb_store *store,
                       const char *from_address,
                       const char *to_address,
                       struct ntb_blob *blob);

void
ntb_store_for_each_blob(struct ntb_store *store,
                        ntb_store_for_each_blob_func func,
                        void *user_data);

void
ntb_store_for_each_addr(struct ntb_store *store,
                        ntb_store_for_each_addr_func func,
                        void *user_data);

void
ntb_store_for_each_key(struct ntb_store *store,
                       ntb_store_for_each_key_func func,
                       void *user_data);

struct ntb_store_cookie *
ntb_store_load_blob(struct ntb_store *store,
                    const uint8_t *hash,
                    ntb_store_load_callback func,
                    void *user_data);

void
ntb_store_cancel_task(struct ntb_store_cookie *cookie);

void
ntb_store_free(struct ntb_store *store);

#endif /* NTB_STORE_H */
