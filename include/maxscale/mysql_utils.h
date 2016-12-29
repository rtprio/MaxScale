#pragma once
/*
 * Copyright (c) 2016 MariaDB Corporation Ab
 *
 * Use of this software is governed by the Business Source License included
 * in the LICENSE.TXT file and at www.mariadb.com/bsl.
 *
 * Change Date: 2019-07-01
 *
 * On the date above, in accordance with the Business Source License, use
 * of this software will be governed by version 2 or later of the General
 * Public License.
 */

#include <maxscale/cdefs.h>
#include <stdlib.h>
#include <stdint.h>
#include <mysql.h>
#include <maxscale/server.h>

MXS_BEGIN_DECLS

/** Length-encoded integers */
size_t mxs_leint_bytes(const uint8_t* ptr);
uint64_t mxs_leint_value(const uint8_t* c);
uint64_t mxs_leint_consume(uint8_t ** c);

/** Length-encoded strings */
char* mxs_lestr_consume_dup(uint8_t** c);
char* mxs_lestr_consume(uint8_t** c, size_t *size);

MYSQL *mxs_mysql_real_connect(MYSQL *mysql, SERVER *server, const char *user, const char *passwd);

/**
 * Trim MySQL quote characters surrounding a string.
 *
 *   'abcd' => abcd
 *   "abcd" => abcd
 *   `abcd` => abcd
 *
 * @param s  The string to be trimmed.
 *
 * @note The string is modified in place.
 */
bool mxs_mysql_trim_quotes(char *s);


typedef enum mxs_pcre_quote_approach
{
    MXS_PCRE_QUOTE_VERBATIM, /*<! Quote all PCRE characters. */
    MXS_PCRE_QUOTE_QUERY     /*<! Quote all PCRE characters, except % that is converted into .*. */
} mxs_pcre_quote_approach_t;

typedef enum mxs_mysql_account_kind
{
    MXS_MYSQL_ACCOUNT_WITH_WILDCARD,   /*<! The input string contains a %. */
    MXS_MYSQL_ACCOUNT_WITHOUT_WILDCARD /*<! The input string does not contain a %. */
} mxs_mysql_account_kind_t;

/**
 * Convert MySQL/MariaDB account string to a pcre compatible one.
 *
 * In principle:
 *   - Quote all characters that have a special meaning in a PCRE context.
 *   - Optionally convert "%" into ".*".
 *
 * @param pcre     The string to which the conversion should be copied.
 *                 To be on the safe size, the buffer should be twice the
 *                 size of 'mysql'.
 * @param mysql    The mysql account string.
 * @param approach Whether % should be converted or not.
 *
 * @return Whether or not the account contains a wildcard.
 */
mxs_mysql_account_kind_t mxs_mysql_account_to_pcre(char *pcre,
                                                   const char *mysql,
                                                   mxs_pcre_quote_approach_t approach);

MXS_END_DECLS
