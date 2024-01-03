#ifndef LEX_INCLUDED
#define LEX_INCLUDED

/* Copyright (c) 2000, 2010, Oracle and/or its affiliates.
   Copyright (c) 2009, 2015, MariaDB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1335  USA */


/* This file includes all reserved words and functions */

#include "lex_symbol.h"

SYM_GROUP sym_group_common= {"", ""};
SYM_GROUP sym_group_geom= {"Spatial extensions", "HAVE_SPATIAL"};
SYM_GROUP sym_group_rtree= {"RTree keys", "HAVE_RTREE_KEYS"};

/* We don't want to include sql_yacc.h into gen_lex_hash */
#ifdef NO_YACC_SYMBOLS
#define SYM_OR_NULL(A) 0
#else
#define SYM_OR_NULL(A) A
#endif

#define SYM(A) SYM_OR_NULL(A),0,&sym_group_common

/*
  Symbols are broken into separated arrays to allow field names with
  same name as functions.
  These are kept sorted for human lookup (the symbols are hashed).

  NOTE! The symbol tables should be the same regardless of what features
  are compiled into the server. Don't add ifdef'ed symbols to the
  lists
  NOTE!!
  If you add or delete symbols from this file, you must also update results for
  the perfschema.start_server_low_digest_sql_length test!
*/

SYMBOL symbols[] = {
  { "&&",		SYM(AND_AND_SYM)},
  { "<=",		SYM(LE)},
  { "<>",		SYM(NE)},
  { "!=",		SYM(NE)},
  { ">=",		SYM(GE)},
  { "<<",		SYM(SHIFT_LEFT)},
  { ">>",		SYM(SHIFT_RIGHT)},
  { "<=>",		SYM(EQUAL_SYM)},
  { "ACCESSIBLE",	SYM(ACCESSIBLE_SYM)},
  { "ACCOUNT",		SYM(ACCOUNT_SYM)},
  { "ACTION",		SYM(ACTION)},
  { "ADD",		SYM(ADD)},
  { "ADMIN",            SYM(ADMIN_SYM)},
  { "AFTER",		SYM(AFTER_SYM)},
  { "AGAINST",          SYM(AGAINST)},
  { "AGGREGATE",	SYM(AGGREGATE_SYM)},
  { "ALL",		SYM(ALL)},
  { "ALGORITHM",	SYM(ALGORITHM_SYM)},
  { "ALTER",		SYM(ALTER)},
  { "ALWAYS",           SYM(ALWAYS_SYM)},
  { "ANALYZE",		SYM(ANALYZE_SYM)},
  { "AND",		SYM(AND_SYM)},
  { "ANY",              SYM(ANY_SYM)},
  { "AS",		SYM(AS)},
  { "ASC",		SYM(ASC)},
  { "ASCII",		SYM(ASCII_SYM)},
  { "ASENSITIVE",       SYM(ASENSITIVE_SYM)},
  { "AT",		SYM(AT_SYM)},
  { "ATOMIC",		SYM(ATOMIC_SYM)},
  { "AUTHORS",	        SYM(AUTHORS_SYM)},
  { "AUTO_INCREMENT",	SYM(AUTO_INC)},
  { "AUTOEXTEND_SIZE",	SYM(AUTOEXTEND_SIZE_SYM)},
  { "AUTO",	        SYM(AUTO_SYM)},
  { "AVG",		SYM(AVG_SYM)},
  { "AVG_ROW_LENGTH",	SYM(AVG_ROW_LENGTH)},
  { "BACKUP",	        SYM(BACKUP_SYM)},
  { "BEFORE",	        SYM(BEFORE_SYM)},
  { "BEGIN",	        SYM(BEGIN_MARIADB_SYM)},
  { "BETWEEN",		SYM(BETWEEN_SYM)},
  { "BIGINT",		SYM(BIGINT)},
  { "BINARY",		SYM(BINARY)},
  { "BINLOG",		SYM(BINLOG_SYM)},
  { "BIT",		SYM(BIT_SYM)},
  { "BLOB",		SYM(BLOB_MARIADB_SYM)},
  { "BLOCK",  SYM(BLOCK_SYM)},
  { "BODY",             SYM(BODY_MARIADB_SYM)},
  { "BOOL",		SYM(BOOL_SYM)},
  { "BOOLEAN",		SYM(BOOLEAN_SYM)},
  { "BOTH",		SYM(BOTH)},
  { "BTREE",		SYM(BTREE_SYM)},
  { "BY",		SYM(BY)},
  { "BYTE",		SYM(BYTE_SYM)},
  { "CACHE",		SYM(CACHE_SYM)},
  { "CALL",             SYM(CALL_SYM)},
  { "CASCADE",		SYM(CASCADE)},
  { "CASCADED",         SYM(CASCADED)},
  { "CASE",		SYM(CASE_SYM)},
  { "CATALOG_NAME",     SYM(CATALOG_NAME_SYM)},
  { "CHAIN",		SYM(CHAIN_SYM)},
  { "CHANGE",		SYM(CHANGE)},
  { "CHANGED",		SYM(CHANGED)},
  { "CHAR",		SYM(CHAR_SYM)},
  { "CHARACTER",	SYM(CHAR_SYM)},
  { "CHARSET",		SYM(CHARSET)},
  { "CHECK",		SYM(CHECK_SYM)},
  { "CHECKPOINT",	SYM(CHECKPOINT_SYM)},
  { "CHECKSUM",		SYM(CHECKSUM_SYM)},
  { "CIPHER",		SYM(CIPHER_SYM)},
  { "CLASS_ORIGIN",     SYM(CLASS_ORIGIN_SYM)},
  { "CLIENT",		SYM(CLIENT_SYM)},
  { "CLOB",             SYM(CLOB_MARIADB_SYM)},
  { "CLOSE",		SYM(CLOSE_SYM)},
  { "COALESCE",		SYM(COALESCE)},
  { "CODE",             SYM(CODE_SYM)},
  { "COLLATE",		SYM(COLLATE_SYM)},
  { "COLLATION",	SYM(COLLATION_SYM)},
  { "COLUMN",		SYM(COLUMN_SYM)},
  { "COLUMN_NAME",      SYM(COLUMN_NAME_SYM)},
  { "COLUMNS",		SYM(COLUMNS)},
  { "COLUMN_ADD",       SYM(COLUMN_ADD_SYM)},
  { "COLUMN_CHECK",     SYM(COLUMN_CHECK_SYM)},
  { "COLUMN_CREATE",    SYM(COLUMN_CREATE_SYM)},
  { "COLUMN_DELETE",    SYM(COLUMN_DELETE_SYM)},
  { "COLUMN_GET",       SYM(COLUMN_GET_SYM)},
  { "COMMENT",		SYM(COMMENT_SYM)},
  { "COMMIT",		SYM(COMMIT_SYM)},
  { "COMMITTED",	SYM(COMMITTED_SYM)},
  { "COMPACT",		SYM(COMPACT_SYM)},
  { "COMPLETION",	SYM(COMPLETION_SYM)},
  { "COMPRESSED",	SYM(COMPRESSED_SYM)},
  { "CONCURRENT",	SYM(CONCURRENT)},
  { "CONDITION",        SYM(CONDITION_SYM)},
  { "CONNECTION",       SYM(CONNECTION_SYM)},
  { "CONSISTENT",	SYM(CONSISTENT_SYM)},
  { "CONSTRAINT",	SYM(CONSTRAINT)},
  { "CONSTRAINT_CATALOG", SYM(CONSTRAINT_CATALOG_SYM)},
  { "CONSTRAINT_NAME",  SYM(CONSTRAINT_NAME_SYM)},
  { "CONSTRAINT_SCHEMA", SYM(CONSTRAINT_SCHEMA_SYM)},
  { "CONTAINS",         SYM(CONTAINS_SYM)},
  { "CONTEXT",    SYM(CONTEXT_SYM)},
  { "CONTINUE",         SYM(CONTINUE_MARIADB_SYM)},
  { "CONTRIBUTORS",     SYM(CONTRIBUTORS_SYM)},
  { "CONVERT",		SYM(CONVERT_SYM)},
  { "CPU",        SYM(CPU_SYM)},
  { "CREATE",		SYM(CREATE)},
  { "CROSS",		SYM(CROSS)},
  { "CUBE",		SYM(CUBE_SYM)},
  { "CURRENT",          SYM(CURRENT_SYM)},
  { "CURRENT_DATE",	SYM(CURDATE)},
  { "CURRENT_POS",      SYM(CURRENT_POS_SYM)},
  { "CURRENT_ROLE",	SYM(CURRENT_ROLE)},
  { "CURRENT_TIME",	SYM(CURTIME)},
  { "CURRENT_TIMESTAMP", SYM(NOW_SYM)},
  { "CURRENT_USER",	SYM(CURRENT_USER)},
  { "CURSOR",           SYM(CURSOR_SYM)},
  { "CURSOR_NAME",      SYM(CURSOR_NAME_SYM)},
  { "CYCLE",            SYM(CYCLE_SYM)},
  { "DATA",		SYM(DATA_SYM)},
  { "DATABASE",		SYM(DATABASE)},
  { "DATABASES",	SYM(DATABASES)},
  { "DATAFILE", 	SYM(DATAFILE_SYM)},
  { "DATE",		SYM(DATE_SYM)},
  { "DATETIME",		SYM(DATETIME)},
  { "DAY",		SYM(DAY_SYM)},
  { "DAY_HOUR",		SYM(DAY_HOUR_SYM)},
  { "DAY_MICROSECOND",	SYM(DAY_MICROSECOND_SYM)},
  { "DAY_MINUTE",	SYM(DAY_MINUTE_SYM)},
  { "DAY_SECOND",	SYM(DAY_SECOND_SYM)},
  { "DEALLOCATE",       SYM(DEALLOCATE_SYM)},
  { "DEC",		SYM(DECIMAL_SYM)},
  { "DECIMAL",		SYM(DECIMAL_SYM)},
  { "DECLARE",          SYM(DECLARE_MARIADB_SYM)},
  { "DEFAULT",		SYM(DEFAULT)},
  { "DEFINER",          SYM(DEFINER_SYM)},
  { "DELAYED",		SYM(DELAYED_SYM)},
  { "DELAY_KEY_WRITE",	SYM(DELAY_KEY_WRITE_SYM)},
  { "DELETE",		SYM(DELETE_SYM)},
  { "DELETE_DOMAIN_ID", SYM(DELETE_DOMAIN_ID_SYM)},
  { "DESC",		SYM(DESC)},
  { "DESCRIBE",		SYM(DESCRIBE)},
  { "DES_KEY_FILE",	SYM(DES_KEY_FILE)},
  { "DETERMINISTIC",    SYM(DETERMINISTIC_SYM)},
  { "DIAGNOSTICS",      SYM(DIAGNOSTICS_SYM)},
  { "DIRECTORY",	SYM(DIRECTORY_SYM)},
  { "DISABLE",		SYM(DISABLE_SYM)},
  { "DISCARD",		SYM(DISCARD)},
  { "DISK",		SYM(DISK_SYM)},
  { "DISTINCT",		SYM(DISTINCT)},
  { "DISTINCTROW",	SYM(DISTINCT)},	/* Access likes this */
  { "DIV",		SYM(DIV_SYM)},
  { "DO",		SYM(DO_SYM)},
  { "DOUBLE",		SYM(DOUBLE_SYM)},
  { "DO_DOMAIN_IDS",    SYM(DO_DOMAIN_IDS_SYM)},
  { "DROP",		SYM(DROP)},
  { "DUAL",		SYM(DUAL_SYM)},
  { "DUMPFILE",		SYM(DUMPFILE)},
  { "DUPLICATE",	SYM(DUPLICATE_SYM)},
  { "DYNAMIC",		SYM(DYNAMIC_SYM)},
  { "EACH",             SYM(EACH_SYM)},
  { "ELSE",             SYM(ELSE)},
  { "ELSEIF",           SYM(ELSEIF_MARIADB_SYM)},
  { "ELSIF",            SYM(ELSIF_MARIADB_SYM)},
  { "EMPTY",		SYM(EMPTY_SYM)},
  { "ENABLE",		SYM(ENABLE_SYM)},
  { "ENCLOSED",		SYM(ENCLOSED)},
  { "END",		SYM(END)},
  { "ENDS",		SYM(ENDS_SYM)},
  { "ENGINE",		SYM(ENGINE_SYM)},
  { "ENGINES",		SYM(ENGINES_SYM)},
  { "ENUM",		SYM(ENUM)},
  { "ERROR",            SYM(ERROR_SYM)},
  { "ERRORS",		SYM(ERRORS)},
  { "ESCAPE",		SYM(ESCAPE_SYM)},
  { "ESCAPED",		SYM(ESCAPED)},
  { "EVENT",		SYM(EVENT_SYM)},
  { "EVENTS",		SYM(EVENTS_SYM)},
  { "EVERY",		SYM(EVERY_SYM)},
  { "EXAMINED",         SYM(EXAMINED_SYM)},
  { "EXCEPT",           SYM(EXCEPT_SYM)},
  { "EXCHANGE",         SYM(EXCHANGE_SYM)},
  { "EXCLUDE",          SYM(EXCLUDE_SYM)},
  { "EXECUTE",		SYM(EXECUTE_SYM)},
  { "EXCEPTION",	SYM(EXCEPTION_MARIADB_SYM)},
  { "EXISTS",		SYM(EXISTS)},
  { "EXIT",             SYM(EXIT_MARIADB_SYM)},
  { "EXPANSION",	SYM(EXPANSION_SYM)},
  { "EXPIRE",		SYM(EXPIRE_SYM)},
  { "EXPORT",           SYM(EXPORT_SYM)},
  { "EXPLAIN",		SYM(DESCRIBE)},
  { "EXTENDED",		SYM(EXTENDED_SYM)},
  { "EXTENT_SIZE",	SYM(EXTENT_SIZE_SYM)},
  { "FALSE",		SYM(FALSE_SYM)},
  { "FAST",		SYM(FAST_SYM)},
  { "FAULTS",  SYM(FAULTS_SYM)},
  { "FEDERATED", SYM(FEDERATED_SYM)},
  { "FETCH",            SYM(FETCH_SYM)},
  { "FIELDS",		SYM(COLUMNS)},
  { "FILE",		SYM(FILE_SYM)},
  { "FIRST",		SYM(FIRST_SYM)},
  { "FIXED",		SYM(FIXED_SYM)},
  { "FLOAT",		SYM(FLOAT_SYM)},
  { "FLOAT4",		SYM(FLOAT_SYM)},
  { "FLOAT8",		SYM(DOUBLE_SYM)},
  { "FLUSH",		SYM(FLUSH_SYM)},
  { "FOLLOWING",        SYM(FOLLOWING_SYM)},
  { "FOLLOWS",          SYM(FOLLOWS_SYM)},
  { "FOR",		SYM(FOR_SYM)},
  { "FORCE",		SYM(FORCE_SYM)},
  { "FOREIGN",		SYM(FOREIGN)},
  { "FORMAT",		SYM(FORMAT_SYM)},
  { "FOUND",            SYM(FOUND_SYM)},
  { "FROM",		SYM(FROM)},
  { "FULL",		SYM(FULL)},
  { "FULLTEXT",		SYM(FULLTEXT_SYM)},
  { "FUNCTION",		SYM(FUNCTION_SYM)},
  { "GENERAL",          SYM(GENERAL)},
  { "GENERATED",        SYM(GENERATED_SYM)},
  { "GET_FORMAT",       SYM(GET_FORMAT)},
  { "GET",              SYM(GET_SYM)},
  { "GLOBAL",		SYM(GLOBAL_SYM)},
  { "GOTO",             SYM(GOTO_MARIADB_SYM)},
  { "GRANT",		SYM(GRANT)},
  { "GRANTS",	        SYM(GRANTS)},
  { "GROUP",		SYM(GROUP_SYM)},
  { "HANDLER",		SYM(HANDLER_SYM)},
  { "HARD",		SYM(HARD_SYM)},
  { "HASH",		SYM(HASH_SYM)},
  { "HAVING",		SYM(HAVING)},
  { "HELP",		SYM(HELP_SYM)},
  { "HIGH_PRIORITY",	SYM(HIGH_PRIORITY)},
  { "HISTORY",      	SYM(HISTORY_SYM)},
  { "HOST",		SYM(HOST_SYM)},
  { "HOSTS",		SYM(HOSTS_SYM)},
  { "HOUR",		SYM(HOUR_SYM)},
  { "HOUR_MICROSECOND",	SYM(HOUR_MICROSECOND_SYM)},
  { "HOUR_MINUTE",	SYM(HOUR_MINUTE_SYM)},
  { "HOUR_SECOND",	SYM(HOUR_SECOND_SYM)},
  { "ID",               SYM(ID_SYM)},
  { "IDENTIFIED",	SYM(IDENTIFIED_SYM)},
  { "IF",		SYM(IF_SYM)},
  { "IGNORE",		SYM(IGNORE_SYM)},
  { "IGNORED",		SYM(IGNORED_SYM)},
  { "IGNORE_DOMAIN_IDS", SYM(IGNORE_DOMAIN_IDS_SYM)},
  { "IGNORE_SERVER_IDS", SYM(IGNORE_SERVER_IDS_SYM)},
  { "IMMEDIATE",	SYM(IMMEDIATE_SYM)},
  { "IMPORT",		SYM(IMPORT)},
  { "INTERSECT",        SYM(INTERSECT_SYM)},
  { "IN",		SYM(IN_SYM)},
  { "INCREMENT",        SYM(INCREMENT_SYM)},
  { "INDEX",		SYM(INDEX_SYM)},
  { "INDEXES",		SYM(INDEXES)},
  { "INFILE",		SYM(INFILE)},
  { "INITIAL_SIZE",	SYM(INITIAL_SIZE_SYM)},
  { "INNER",		SYM(INNER_SYM)},
  { "INOUT",            SYM(INOUT_SYM)},
  { "INSENSITIVE",      SYM(INSENSITIVE_SYM)},
  { "INSERT",		SYM(INSERT)},
  { "INSERT_METHOD",    SYM(INSERT_METHOD)},
  { "INSTALL",          SYM(INSTALL_SYM)},
  { "INT",		SYM(INT_SYM)},
  { "INT1",		SYM(TINYINT)},
  { "INT2",		SYM(SMALLINT)},
  { "INT3",		SYM(MEDIUMINT)},
  { "INT4",		SYM(INT_SYM)},
  { "INT8",		SYM(BIGINT)},
  { "INTEGER",		SYM(INT_SYM)},
  { "INTERVAL",		SYM(INTERVAL_SYM)},
  { "INVISIBLE",        SYM(INVISIBLE_SYM)},
  { "INTO",		SYM(INTO)},
  { "IO",     SYM(IO_SYM)},
  { "IO_THREAD",        SYM(RELAY_THREAD)},
  { "IPC",    SYM(IPC_SYM)},
  { "IS",		SYM(IS)},
  { "ISOLATION",	SYM(ISOLATION)},
  { "ISOPEN",           SYM(ISOPEN_SYM)},
  { "ISSUER",		SYM(ISSUER_SYM)},
  { "ITERATE",          SYM(ITERATE_SYM)},
  { "INVOKER",          SYM(INVOKER_SYM)},
  { "JOIN",		SYM(JOIN_SYM)},
  { "JSON",		SYM(JSON_SYM)},
  { "JSON_TABLE",	SYM(JSON_TABLE_SYM)},
  { "KEY",		SYM(KEY_SYM)},
  { "KEYS",		SYM(KEYS)},
  { "KEY_BLOCK_SIZE",	SYM(KEY_BLOCK_SIZE)},
  { "KILL",		SYM(KILL_SYM)},
  { "LANGUAGE",         SYM(LANGUAGE_SYM)},
  { "LAST",		SYM(LAST_SYM)},
  { "LAST_VALUE",	SYM(LAST_VALUE)},
  { "LASTVAL",		SYM(LASTVAL_SYM)},
  { "LEADING",		SYM(LEADING)},
  { "LEAVE",            SYM(LEAVE_SYM)},
  { "LEAVES",		SYM(LEAVES)},
  { "LEFT",		SYM(LEFT)},
  { "LESS",             SYM(LESS_SYM)},
  { "LEVEL",		SYM(LEVEL_SYM)},
  { "LIKE",		SYM(LIKE)},
  { "LIMIT",		SYM(LIMIT)},
  { "LINEAR",		SYM(LINEAR_SYM)},
  { "LINES",		SYM(LINES)},
  { "LIST",             SYM(LIST_SYM)},
  { "LOAD",		SYM(LOAD)},
  { "LOCAL",		SYM(LOCAL_SYM)},
  { "LOCALTIME",	SYM(NOW_SYM)},
  { "LOCALTIMESTAMP",	SYM(NOW_SYM)},
  { "LOCK",		SYM(LOCK_SYM)},
  { "LOCKED",		SYM(LOCKED_SYM)},
  { "LOCKS",		SYM(LOCKS_SYM)},
  { "LOGFILE",		SYM(LOGFILE_SYM)},
  { "LOGS",		SYM(LOGS_SYM)},
  { "LONG",		SYM(LONG_SYM)},
  { "LONGBLOB",		SYM(LONGBLOB)},
  { "LONGTEXT",		SYM(LONGTEXT)},
  { "LOOP",             SYM(LOOP_SYM)},
  { "LOW_PRIORITY",	SYM(LOW_PRIORITY)},
  { "MASTER",           SYM(MASTER_SYM)},
  { "MASTER_CONNECT_RETRY",           SYM(MASTER_CONNECT_RETRY_SYM)},
  { "MASTER_DELAY",     SYM(MASTER_DELAY_SYM)},
  { "MASTER_GTID_POS",  SYM(MASTER_GTID_POS_SYM)},
  { "MASTER_HOST",           SYM(MASTER_HOST_SYM)},
  { "MASTER_LOG_FILE",           SYM(MASTER_LOG_FILE_SYM)},
  { "MASTER_LOG_POS",           SYM(MASTER_LOG_POS_SYM)},
  { "MASTER_PASSWORD",           SYM(MASTER_PASSWORD_SYM)},
  { "MASTER_PORT",           SYM(MASTER_PORT_SYM)},
  { "MASTER_SERVER_ID",           SYM(MASTER_SERVER_ID_SYM)},
  { "MASTER_SSL",       SYM(MASTER_SSL_SYM)},
  { "MASTER_SSL_CA",    SYM(MASTER_SSL_CA_SYM)},
  { "MASTER_SSL_CAPATH",SYM(MASTER_SSL_CAPATH_SYM)},
  { "MASTER_SSL_CERT",  SYM(MASTER_SSL_CERT_SYM)},
  { "MASTER_SSL_CIPHER",SYM(MASTER_SSL_CIPHER_SYM)},
  { "MASTER_SSL_CRL",   SYM(MASTER_SSL_CRL_SYM)},
  { "MASTER_SSL_CRLPATH",SYM(MASTER_SSL_CRLPATH_SYM)},
  { "MASTER_SSL_KEY",   SYM(MASTER_SSL_KEY_SYM)},
  { "MASTER_SSL_VERIFY_SERVER_CERT", SYM(MASTER_SSL_VERIFY_SERVER_CERT_SYM)},
  { "MASTER_USER",           SYM(MASTER_USER_SYM)},
  { "MASTER_USE_GTID",  SYM(MASTER_USE_GTID_SYM)},
  { "MASTER_HEARTBEAT_PERIOD", SYM(MASTER_HEARTBEAT_PERIOD_SYM)},
  { "MATCH",		SYM(MATCH)},
  { "MAX_CONNECTIONS_PER_HOUR", SYM(MAX_CONNECTIONS_PER_HOUR)},
  { "MAX_QUERIES_PER_HOUR", SYM(MAX_QUERIES_PER_HOUR)},
  { "MAX_ROWS",		SYM(MAX_ROWS)},
  { "MAX_SIZE",		SYM(MAX_SIZE_SYM)},
  { "MAX_STATEMENT_TIME",   SYM(MAX_STATEMENT_TIME_SYM)},
  { "MAX_UPDATES_PER_HOUR", SYM(MAX_UPDATES_PER_HOUR)},
  { "MAX_USER_CONNECTIONS", SYM(MAX_USER_CONNECTIONS_SYM)},
  { "MAXVALUE",         SYM(MAXVALUE_SYM)},
  { "MEDIUM",		SYM(MEDIUM_SYM)},
  { "MEDIUMBLOB",	SYM(MEDIUMBLOB)},
  { "MEDIUMINT",	SYM(MEDIUMINT)},
  { "MEDIUMTEXT",	SYM(MEDIUMTEXT)},
  { "MEMORY",		SYM(MEMORY_SYM)},
  { "MERGE",		SYM(MERGE_SYM)},
  { "MESSAGE_TEXT",     SYM(MESSAGE_TEXT_SYM)},
  { "MICROSECOND",	SYM(MICROSECOND_SYM)},
  { "MIDDLEINT",	SYM(MEDIUMINT)},	/* For powerbuilder */
  { "MIGRATE",          SYM(MIGRATE_SYM)},
  { "MINUS",            SYM(MINUS_ORACLE_SYM)},
  { "MINUTE",		SYM(MINUTE_SYM)},
  { "MINUTE_MICROSECOND", SYM(MINUTE_MICROSECOND_SYM)},
  { "MINUTE_SECOND",	SYM(MINUTE_SECOND_SYM)},
  { "MINVALUE",		SYM(MINVALUE_SYM)},
  { "MIN_ROWS",		SYM(MIN_ROWS)},
  { "MOD",		SYM(MOD_SYM)},
  { "MODE",		SYM(MODE_SYM)},
  { "MODIFIES",		SYM(MODIFIES_SYM)},
  { "MODIFY",		SYM(MODIFY_SYM)},
  { "MONITOR",          SYM(MONITOR_SYM)},
  { "MONTH",		SYM(MONTH_SYM)},
  { "MUTEX",  SYM(MUTEX_SYM)},
  { "MYSQL",            SYM(MYSQL_SYM)},
  { "MYSQL_ERRNO",      SYM(MYSQL_ERRNO_SYM)},
  { "NAME",             SYM(NAME_SYM)},
  { "NAMES",		SYM(NAMES_SYM)},
  { "NATIONAL",		SYM(NATIONAL_SYM)},
  { "NATURAL",		SYM(NATURAL)},
  { "NCHAR",		SYM(NCHAR_SYM)},
  { "NESTED",		SYM(NESTED_SYM)},
  { "NEVER",		SYM(NEVER_SYM)},
  { "NEW",              SYM(NEW_SYM)},
  { "NEXT",		SYM(NEXT_SYM)},
  { "NEXTVAL",		SYM(NEXTVAL_SYM)},
  { "NO",		SYM(NO_SYM)},
  { "NOMAXVALUE",	SYM(NOMAXVALUE_SYM)},
  { "NOMINVALUE",	SYM(NOMINVALUE_SYM)},
  { "NOCACHE",          SYM(NOCACHE_SYM)},
  { "NOCYCLE",          SYM(NOCYCLE_SYM)},
  { "NO_WAIT",		SYM(NO_WAIT_SYM)},
  { "NOWAIT",		SYM(NOWAIT_SYM)},
  { "NODEGROUP",	SYM(NODEGROUP_SYM)},
  { "NONE",		SYM(NONE_SYM)},
  { "NOT",		SYM(NOT_SYM)},
  { "NOTFOUND",         SYM(NOTFOUND_SYM)},
  { "NO_WRITE_TO_BINLOG",  SYM(NO_WRITE_TO_BINLOG)},
  { "NULL",		SYM(NULL_SYM)},
  { "NUMBER",           SYM(NUMBER_MARIADB_SYM)},
  { "NUMERIC",		SYM(NUMERIC_SYM)},
  { "NVARCHAR",		SYM(NVARCHAR_SYM)},
  { "OF",		SYM(OF_SYM)},
  { "OFFSET",		SYM(OFFSET_SYM)},
  { "OLD_PASSWORD",	SYM(OLD_PASSWORD_SYM)},
  { "ON",		SYM(ON)},
  { "ONE",              SYM(ONE_SYM)},
  { "ONLINE",		SYM(ONLINE_SYM)},
  { "ONLY",             SYM(ONLY_SYM)},
  { "OPEN",		SYM(OPEN_SYM)},
  { "OPTIMIZE",		SYM(OPTIMIZE)},
  { "OPTIONS",		SYM(OPTIONS_SYM)},
  { "OPTION",		SYM(OPTION)},
  { "OPTIONALLY",	SYM(OPTIONALLY)},
  { "OR",		SYM(OR_SYM)},
  { "ORDER",		SYM(ORDER_SYM)},
  { "ORDINALITY",	SYM(ORDINALITY_SYM)},
  { "OTHERS",           SYM(OTHERS_MARIADB_SYM)},
  { "OUT",              SYM(OUT_SYM)},
  { "OUTER",		SYM(OUTER)},
  { "OUTFILE",		SYM(OUTFILE)},
  { "OVER",             SYM(OVER_SYM)},
  { "OVERLAPS",         SYM(OVERLAPS_SYM)},
  { "OWNER",		SYM(OWNER_SYM)},
  { "PACKAGE",          SYM(PACKAGE_MARIADB_SYM)},
  { "PACK_KEYS",	SYM(PACK_KEYS_SYM)},
  { "PAGE",	        SYM(PAGE_SYM)},
  { "PAGE_CHECKSUM",	SYM(PAGE_CHECKSUM_SYM)},
  { "PARSER",           SYM(PARSER_SYM)},
  { "PARSE_VCOL_EXPR",  SYM(PARSE_VCOL_EXPR_SYM)},
  { "PATH",		SYM(PATH_SYM)},
  { "PERIOD",		SYM(PERIOD_SYM)},
  { "PARTIAL",		SYM(PARTIAL)},
  { "PARTITION",        SYM(PARTITION_SYM)},
  { "PARTITIONING",     SYM(PARTITIONING_SYM)},
  { "PARTITIONS",       SYM(PARTITIONS_SYM)},
  { "PASSWORD",		SYM(PASSWORD_SYM)},
  { "PERSISTENT",	SYM(PERSISTENT_SYM)},
  { "PHASE",            SYM(PHASE_SYM)},
  { "PLUGIN",           SYM(PLUGIN_SYM)},
  { "PLUGINS",          SYM(PLUGINS_SYM)},
  { "PORT",		SYM(PORT_SYM)},
  { "PORTION",		SYM(PORTION_SYM)},
  { "PRECEDES",         SYM(PRECEDES_SYM)},
  { "PRECEDING",        SYM(PRECEDING_SYM)},
  { "PRECISION",	SYM(PRECISION)},
  { "PREPARE",          SYM(PREPARE_SYM)},
  { "PRESERVE",		SYM(PRESERVE_SYM)},
  { "PREV",		SYM(PREV_SYM)},
  { "PREVIOUS",		SYM(PREVIOUS_SYM)},
  { "PRIMARY",		SYM(PRIMARY_SYM)},
  { "PRIVILEGES",	SYM(PRIVILEGES)},
  { "PROCEDURE",	SYM(PROCEDURE_SYM)},
  { "PROCESS"	,	SYM(PROCESS)},
  { "PROCESSLIST",	SYM(PROCESSLIST_SYM)},
  { "PROFILE",          SYM(PROFILE_SYM)},
  { "PROFILES",         SYM(PROFILES_SYM)},
  { "PROXY",            SYM(PROXY_SYM)},
  { "PURGE",		SYM(PURGE)},
  { "QUARTER",          SYM(QUARTER_SYM)},
  { "QUERY",		SYM(QUERY_SYM)},
  { "QUICK",	        SYM(QUICK)},
  { "RAISE",            SYM(RAISE_MARIADB_SYM)},
  { "RANGE",            SYM(RANGE_SYM)},
  { "RAW",              SYM(RAW_MARIADB_SYM)},
  { "READ",		SYM(READ_SYM)},
  { "READ_ONLY",	SYM(READ_ONLY_SYM)},
  { "READ_WRITE",	SYM(READ_WRITE_SYM)},
  { "READS",		SYM(READS_SYM)},
  { "REAL",		SYM(REAL)},
  { "REBUILD",		SYM(REBUILD_SYM)},
  { "RECOVER",          SYM(RECOVER_SYM)},
  { "RECURSIVE",        SYM(RECURSIVE_SYM)},
  { "REDO_BUFFER_SIZE",	SYM(REDO_BUFFER_SIZE_SYM)},
  { "REDOFILE",         SYM(REDOFILE_SYM)},
  { "REDUNDANT",	SYM(REDUNDANT_SYM)},
  { "REFERENCES",	SYM(REFERENCES)},
  { "REGEXP",		SYM(REGEXP)},
  { "RELAY",            SYM(RELAY)},
  { "RELAYLOG",         SYM(RELAYLOG_SYM)},
  { "RELAY_LOG_FILE",   SYM(RELAY_LOG_FILE_SYM)},
  { "RELAY_LOG_POS",    SYM(RELAY_LOG_POS_SYM)},
  { "RELAY_THREAD",     SYM(RELAY_THREAD)},
  { "RELEASE",		SYM(RELEASE_SYM)},
  { "RELOAD",		SYM(RELOAD)},
  { "REMOVE",		SYM(REMOVE_SYM)},
  { "RENAME",		SYM(RENAME)},
  { "REORGANIZE",	SYM(REORGANIZE_SYM)},
  { "REPAIR",		SYM(REPAIR)},
  { "REPEATABLE",	SYM(REPEATABLE_SYM)},
  { "REPLACE",		SYM(REPLACE)},
  { "REPLAY",           SYM(REPLAY_SYM)},
  { "REPLICA",      SYM(SLAVE)},
  { "REPLICAS",     SYM(SLAVES)},
  { "REPLICA_POS",  SYM(SLAVE_POS_SYM)},
  { "REPLICATION",	SYM(REPLICATION)},
  { "REPEAT",           SYM(REPEAT_SYM)},
  { "REQUIRE",	        SYM(REQUIRE_SYM)},
  { "RESET",		SYM(RESET_SYM)},
  { "RESIGNAL",         SYM(RESIGNAL_SYM)},
  { "RESTART",		SYM(RESTART_SYM)},
  { "RESTORE",		SYM(RESTORE_SYM)},
  { "RESTRICT",		SYM(RESTRICT)},
  { "RESUME",           SYM(RESUME_SYM)},
  { "RETURNED_SQLSTATE",SYM(RETURNED_SQLSTATE_SYM)},
  { "RETURN",           SYM(RETURN_MARIADB_SYM)},
  { "RETURNING",        SYM(RETURNING_SYM)},
  { "RETURNS",		SYM(RETURNS_SYM)},
  { "REUSE",            SYM(REUSE_SYM)},
  { "REVERSE",		SYM(REVERSE_SYM)},
  { "REVOKE",		SYM(REVOKE)},
  { "RIGHT",		SYM(RIGHT)},
  { "RLIKE",		SYM(REGEXP)},	/* Like in mSQL2 */
  { "ROLE",             SYM(ROLE_SYM)},
  { "ROLLBACK",		SYM(ROLLBACK_SYM)},
  { "ROLLUP",		SYM(ROLLUP_SYM)},
  { "ROUTINE",		SYM(ROUTINE_SYM)},
  { "ROW",		SYM(ROW_SYM)},
  { "ROWCOUNT",         SYM(ROWCOUNT_SYM)}, /* Oracle-N */
  { "ROWNUM",           SYM(ROWNUM_SYM)}, /* Oracle-R */
  { "ROWS",		SYM(ROWS_SYM)},
  { "ROWTYPE",          SYM(ROWTYPE_MARIADB_SYM)},
  { "ROW_COUNT",        SYM(ROW_COUNT_SYM)},
  { "ROW_FORMAT",	SYM(ROW_FORMAT_SYM)},
  { "RTREE",		SYM(RTREE_SYM)},
  { "SAVEPOINT",	SYM(SAVEPOINT_SYM)},
  { "SCHEDULE",		SYM(SCHEDULE_SYM)},
  { "SCHEMA",		SYM(DATABASE)},
  { "SCHEMA_NAME",      SYM(SCHEMA_NAME_SYM)},
  { "SCHEMAS",          SYM(DATABASES)},
  { "SECOND",		SYM(SECOND_SYM)},
  { "SECOND_MICROSECOND", SYM(SECOND_MICROSECOND_SYM)},
  { "SECURITY",         SYM(SECURITY_SYM)},
  { "SELECT",		SYM(SELECT_SYM)},
  { "SENSITIVE",        SYM(SENSITIVE_SYM)},
  { "SEPARATOR",	SYM(SEPARATOR_SYM)},
  { "SEQUENCE",		SYM(SEQUENCE_SYM)},
  { "SERIAL",		SYM(SERIAL_SYM)},
  { "SERIALIZABLE",	SYM(SERIALIZABLE_SYM)},
  { "SESSION",		SYM(SESSION_SYM)},
  { "SERVER",           SYM(SERVER_SYM)},
  { "SET",		SYM(SET)},
  { "SETVAL",		SYM(SETVAL_SYM)},
  { "SHARE",		SYM(SHARE_SYM)},
  { "SHOW",		SYM(SHOW)},
  { "SHUTDOWN",		SYM(SHUTDOWN)},
  { "SIGNAL",           SYM(SIGNAL_SYM)},
  { "SIGNED",		SYM(SIGNED_SYM)},
  { "SIMPLE",		SYM(SIMPLE_SYM)},
  { "SKIP",             SYM(SKIP_SYM)},
  { "SLAVE",            SYM(SLAVE)},
  { "SLAVES",           SYM(SLAVES)},
  { "SLAVE_POS",        SYM(SLAVE_POS_SYM)},
  { "SLOW",             SYM(SLOW)},
  { "SNAPSHOT",         SYM(SNAPSHOT_SYM)},
  { "SMALLINT",		SYM(SMALLINT)},
  { "SOCKET",		SYM(SOCKET_SYM)},
  { "SOFT",             SYM(SOFT_SYM)},
  { "SOME",             SYM(ANY_SYM)},
  { "SONAME",		SYM(SONAME_SYM)},
  { "SOUNDS",		SYM(SOUNDS_SYM)},
  { "SOURCE",           SYM(SOURCE_SYM)},
  { "STAGE",            SYM(STAGE_SYM)},
  { "STORED",           SYM(STORED_SYM)},
  { "SPATIAL",		SYM(SPATIAL_SYM)},
  { "SPECIFIC",         SYM(SPECIFIC_SYM)},
  { "REF_SYSTEM_ID",    SYM(REF_SYSTEM_ID_SYM)},
  { "SQL",              SYM(SQL_SYM)},
  { "SQLEXCEPTION",     SYM(SQLEXCEPTION_SYM)},
  { "SQLSTATE",         SYM(SQLSTATE_SYM)},
  { "SQLWARNING",       SYM(SQLWARNING_SYM)},
  { "SQL_BIG_RESULT",	SYM(SQL_BIG_RESULT)},
  { "SQL_BUFFER_RESULT", SYM(SQL_BUFFER_RESULT)},
  { "SQL_CACHE",        SYM(SQL_CACHE_SYM)},
  { "SQL_CALC_FOUND_ROWS", SYM(SQL_CALC_FOUND_ROWS)},
  { "SQL_NO_CACHE",	SYM(SQL_NO_CACHE_SYM)},
  { "SQL_SMALL_RESULT", SYM(SQL_SMALL_RESULT)},
  { "SQL_THREAD",	SYM(SQL_THREAD)},
  { "SQL_TSI_SECOND",   SYM(SECOND_SYM)},
  { "SQL_TSI_MINUTE",   SYM(MINUTE_SYM)},
  { "SQL_TSI_HOUR",     SYM(HOUR_SYM)},
  { "SQL_TSI_DAY",      SYM(DAY_SYM)},
  { "SQL_TSI_WEEK",     SYM(WEEK_SYM)},
  { "SQL_TSI_MONTH",    SYM(MONTH_SYM)},
  { "SQL_TSI_QUARTER",  SYM(QUARTER_SYM)},
  { "SQL_TSI_YEAR",     SYM(YEAR_SYM)},
  { "SSL",		SYM(SSL_SYM)},
  { "START",		SYM(START_SYM)},
  { "STARTING",		SYM(STARTING)},
  { "STARTS",		SYM(STARTS_SYM)},
  { "STATEMENT",	SYM(STATEMENT_SYM)},
  { "STATS_AUTO_RECALC",SYM(STATS_AUTO_RECALC_SYM)},
  { "STATS_PERSISTENT",	SYM(STATS_PERSISTENT_SYM)},
  { "STATS_SAMPLE_PAGES",SYM(STATS_SAMPLE_PAGES_SYM)},
  { "STATUS",		SYM(STATUS_SYM)},
  { "STOP",		SYM(STOP_SYM)},
  { "STORAGE",		SYM(STORAGE_SYM)},
  { "STRAIGHT_JOIN",	SYM(STRAIGHT_JOIN)},
  { "STRING",		SYM(STRING_SYM)},
  { "SUBCLASS_ORIGIN",  SYM(SUBCLASS_ORIGIN_SYM)},
  { "SUBJECT",		SYM(SUBJECT_SYM)},
  { "SUBPARTITION",     SYM(SUBPARTITION_SYM)},
  { "SUBPARTITIONS",    SYM(SUBPARTITIONS_SYM)},
  { "SUPER",		SYM(SUPER_SYM)},
  { "SUSPEND",          SYM(SUSPEND_SYM)},
  { "SWAPS",      SYM(SWAPS_SYM)},
  { "SWITCHES",   SYM(SWITCHES_SYM)},
  { "SYSDATE",		SYM(SYSDATE)},
  { "SYSTEM",     SYM(SYSTEM)},
  { "SYSTEM_TIME",      SYM(SYSTEM_TIME_SYM)},
  { "TABLE",		SYM(TABLE_SYM)},
  { "TABLE_NAME",       SYM(TABLE_NAME_SYM)},
  { "TABLES",		SYM(TABLES)},
  { "TABLESPACE",	        SYM(TABLESPACE)},
  { "TABLE_CHECKSUM",	SYM(TABLE_CHECKSUM_SYM)},
  { "TEMPORARY",	SYM(TEMPORARY)},
  { "TEMPTABLE",	SYM(TEMPTABLE_SYM)},
  { "TERMINATED",	SYM(TERMINATED)},
  { "TEXT",		SYM(TEXT_SYM)},
  { "THAN",             SYM(THAN_SYM)},
  { "THEN",		SYM(THEN_SYM)},
  { "TIES",             SYM(TIES_SYM)},
  { "TIME",		SYM(TIME_SYM)},
  { "TIMESTAMP",	SYM(TIMESTAMP)},
  { "TIMESTAMPADD",     SYM(TIMESTAMP_ADD)},
  { "TIMESTAMPDIFF",    SYM(TIMESTAMP_DIFF)},
  { "TINYBLOB",		SYM(TINYBLOB)},
  { "TINYINT",		SYM(TINYINT)},
  { "TINYTEXT",		SYM(TINYTEXT)},
  { "TO",		SYM(TO_SYM)},
  { "TRAILING",		SYM(TRAILING)},
  { "TRANSACTION",	SYM(TRANSACTION_SYM)},
  { "TRANSACTIONAL",	SYM(TRANSACTIONAL_SYM)},
  { "THREADS",          SYM(THREADS_SYM)},
  { "TRIGGER",          SYM(TRIGGER_SYM)},
  { "TRIGGERS",         SYM(TRIGGERS_SYM)},
  { "TRUE",		SYM(TRUE_SYM)},
  { "TRUNCATE",		SYM(TRUNCATE_SYM)},
  { "TYPE",		SYM(TYPE_SYM)},
  { "TYPES",		SYM(TYPES_SYM)},
  { "UNBOUNDED",        SYM(UNBOUNDED_SYM)},
  { "UNCOMMITTED",	SYM(UNCOMMITTED_SYM)},
  { "UNDEFINED",	SYM(UNDEFINED_SYM)},
  { "UNDO_BUFFER_SIZE",	SYM(UNDO_BUFFER_SIZE_SYM)},
  { "UNDOFILE", 	SYM(UNDOFILE_SYM)},
  { "UNDO",             SYM(UNDO_SYM)},
  { "UNICODE",	        SYM(UNICODE_SYM)},
  { "UNION",	        SYM(UNION_SYM)},
  { "UNIQUE",		SYM(UNIQUE_SYM)},
  { "UNKNOWN",		SYM(UNKNOWN_SYM)},
  { "UNLOCK",		SYM(UNLOCK_SYM)},
  { "UNINSTALL",        SYM(UNINSTALL_SYM)},
  { "UNSIGNED",		SYM(UNSIGNED)},
  { "UNTIL",		SYM(UNTIL_SYM)},
  { "UPDATE",		SYM(UPDATE_SYM)},
  { "UPGRADE",          SYM(UPGRADE_SYM)},
  { "USAGE",		SYM(USAGE)},
  { "USE",		SYM(USE_SYM)},
  { "USER",		SYM(USER_SYM)},
  { "USER_RESOURCES",	SYM(RESOURCES)},
  { "USE_FRM",		SYM(USE_FRM)},
  { "USING",		SYM(USING)},
  { "UTC_DATE",         SYM(UTC_DATE_SYM)},
  { "UTC_TIME",         SYM(UTC_TIME_SYM)},
  { "UTC_TIMESTAMP",    SYM(UTC_TIMESTAMP_SYM)},
  { "VALUE",		SYM(VALUE_SYM)},
  { "VALUES",		SYM(VALUES)},
  { "VARBINARY",	SYM(VARBINARY)},
  { "VARCHAR",		SYM(VARCHAR)},
  { "VARCHARACTER",	SYM(VARCHAR)},
  { "VARCHAR2",         SYM(VARCHAR2_MARIADB_SYM)},
  { "VARIABLES",	SYM(VARIABLES)},
  { "VARYING",		SYM(VARYING)},
  { "VIA",              SYM(VIA_SYM)},
  { "VIEW",		SYM(VIEW_SYM)},
  { "VIRTUAL",          SYM(VIRTUAL_SYM)},
  { "VISIBLE",          SYM(VISIBLE_SYM)},
  { "VERSIONING",       SYM(VERSIONING_SYM)},
  { "WAIT",		SYM(WAIT_SYM)},
  { "WARNINGS",		SYM(WARNINGS)},
  { "WEEK",		SYM(WEEK_SYM)},
  { "WEIGHT_STRING",	SYM(WEIGHT_STRING_SYM)},
  { "WHEN",		SYM(WHEN_SYM)},
  { "WHERE",		SYM(WHERE)},
  { "WHILE",            SYM(WHILE_SYM)},
  { "WINDOW",           SYM(WINDOW_SYM)},
  { "WITH",		SYM(WITH)},
  { "WITHIN",   SYM(WITHIN)},
  { "WITHOUT",          SYM(WITHOUT)},
  { "WORK",		SYM(WORK_SYM)},
  { "WRAPPER",		SYM(WRAPPER_SYM)},
  { "WRITE",		SYM(WRITE_SYM)},
  { "X509",		SYM(X509_SYM)},
  { "XOR",		SYM(XOR)},
  { "XA",               SYM(XA_SYM)},
  { "XML",              SYM(XML_SYM)}, /* LOAD XML Arnold/Erik */
  { "YEAR",		SYM(YEAR_SYM)},
  { "YEAR_MONTH",	SYM(YEAR_MONTH_SYM)},
  { "ZEROFILL",		SYM(ZEROFILL)},
  { "||",		SYM(OR2_SYM)}
};


SYMBOL sql_functions[] = {
  { "ADDDATE",		SYM(ADDDATE_SYM)},
  { "ADD_MONTHS",	SYM(ADD_MONTHS_SYM)},
  { "BIT_AND",		SYM(BIT_AND)},
  { "BIT_OR",		SYM(BIT_OR)},
  { "BIT_XOR",		SYM(BIT_XOR)},
  { "CAST",		SYM(CAST_SYM)},
  { "COUNT",		SYM(COUNT_SYM)},
  { "CUME_DIST",        SYM(CUME_DIST_SYM)},
  { "CURDATE",		SYM(CURDATE)},
  { "CURTIME",		SYM(CURTIME)},
  { "DATE_ADD",		SYM(DATE_ADD_INTERVAL)},
  { "DATE_SUB",		SYM(DATE_SUB_INTERVAL)},
  { "DATE_FORMAT",      SYM(DATE_FORMAT_SYM)},
  { "DECODE",           SYM(DECODE_MARIADB_SYM)},
  { "DENSE_RANK",       SYM(DENSE_RANK_SYM)},
  { "EXTRACT",		SYM(EXTRACT_SYM)},
  { "FIRST_VALUE",      SYM(FIRST_VALUE_SYM)},
  { "GROUP_CONCAT",	SYM(GROUP_CONCAT_SYM)},
  { "JSON_ARRAYAGG",	SYM(JSON_ARRAYAGG_SYM)},
  { "JSON_OBJECTAGG",	SYM(JSON_OBJECTAGG_SYM)},
  { "LAG",              SYM(LAG_SYM)},
  { "LEAD",             SYM(LEAD_SYM)},
  { "MAX",		SYM(MAX_SYM)},
  { "MEDIAN", SYM(MEDIAN_SYM)},
  { "MID",		SYM(SUBSTRING)},	/* unireg function */
  { "MIN",		SYM(MIN_SYM)},
  { "NOW",		SYM(NOW_SYM)},
  { "NTH_VALUE",        SYM(NTH_VALUE_SYM)},
  { "NTILE",            SYM(NTILE_SYM)},
  { "POSITION",		SYM(POSITION_SYM)},
  { "PERCENT_RANK",     SYM(PERCENT_RANK_SYM)},
  { "PERCENTILE_CONT",  SYM(PERCENTILE_CONT_SYM)},
  { "PERCENTILE_DISC",  SYM(PERCENTILE_DISC_SYM)},
  { "RANK",             SYM(RANK_SYM)},
  { "ROW_NUMBER",       SYM(ROW_NUMBER_SYM)},
  { "SESSION_USER",     SYM(USER_SYM)},
  { "STD",		SYM(STD_SYM)},
  { "STDDEV",		SYM(STD_SYM)},
  { "STDDEV_POP",	SYM(STD_SYM)},
  { "STDDEV_SAMP",	SYM(STDDEV_SAMP_SYM)},
  { "SUBDATE",		SYM(SUBDATE_SYM)},
  { "SUBSTR",		SYM(SUBSTRING)},
  { "SUBSTRING",	SYM(SUBSTRING)},
  { "SUM",		SYM(SUM_SYM)},
  { "SYSTEM_USER",      SYM(USER_SYM)},
  { "TRIM",		SYM(TRIM)},
  { "TRIM_ORACLE",	SYM(TRIM_ORACLE)},
  { "VARIANCE",		SYM(VARIANCE_SYM)},
  { "VAR_POP",		SYM(VARIANCE_SYM)},
  { "VAR_SAMP",		SYM(VAR_SAMP_SYM)},
};

size_t symbols_length= sizeof(symbols) / sizeof(SYMBOL);
size_t sql_functions_length= sizeof(sql_functions) / sizeof(SYMBOL);

#endif /* LEX_INCLUDED */
