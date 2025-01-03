<a href="https://community-extensions.duckdb.org/extensions/chsql.html" target="_blank">
<img src="https://github.com/user-attachments/assets/9003897d-db6f-4a79-9443-9b72766b511b" width=200>
</a>

# DuckDB Clickhouse System Table emulator for [chsql](https://github.com/quackscience/duckdb-extension-clickhouse-sql)

> This project emulates ClickHouse system tables within DuckDB

The extension provides system table functions:
- [x] `system_databases`
- [x] `system_tables`
- [x] `system_columns`
- [x] `system_functions`.

## Installation

> coming soon

## Usage
```sql
D -- List all databases in scope
D SELECT * FROM system.databases;
┌─────────┬─────────┬─────────┐
│  name   │ engine  │  type   │
│ varchar │ varchar │ varchar │
├─────────┼─────────┼─────────┤
│ memory  │ DuckDB  │ memory  │
└─────────┴─────────┴─────────┘

D -- List all tables in scope
D SELECT * FROM system.tables;
┌──────────┬─────────┬───────────┬──────────────────────┬────────────┬───┬──────────────────────┬──────────────────────┬────────────────────┬─────────┐
│ database │ schema  │   name    │         uuid         │   engine   │ … │     column_names     │     column_types     │ create_table_query │ comment │
│ varchar  │ varchar │  varchar  │         uuid         │  varchar   │   │      varchar[]       │      varchar[]       │      varchar       │ varchar │
├──────────┼─────────┼───────────┼──────────────────────┼────────────┼───┼──────────────────────┼──────────────────────┼────────────────────┼─────────┤
│ memory   │ main    │ test2     │ 80000000-0000-0000…  │ BASE TABLE │ … │ [value]              │ [DOUBLE]             │                    │         │
│ memory   │ system  │ columns   │ 80000000-0000-000c…  │ BASE TABLE │ … │ [database, table, …  │ [VARCHAR, VARCHAR,…  │                    │         │
│ memory   │ system  │ databases │ 80000000-0000-00da…  │ BASE TABLE │ … │ [name, engine, type] │ [VARCHAR, VARCHAR,…  │                    │         │
│ memory   │ system  │ functions │ 80000000-0000-000f…  │ BASE TABLE │ … │ [name, is_aggregat…  │ [VARCHAR, BOOLEAN,…  │                    │         │
│ memory   │ system  │ tables    │ 80000000-0000-0000…  │ BASE TABLE │ … │ [database, schema,…  │ [VARCHAR, VARCHAR,…  │                    │         │
├──────────┴─────────┴───────────┴──────────────────────┴────────────┴───┴──────────────────────┴──────────────────────┴────────────────────┴─────────┤
│ 5 rows                                                                                                                         10 columns (9 shown) │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
D 
D -- List all columns in scope
D SELECT * FROM system_columns;
┌──────────┬───────────┬────────────────────┬───────────┬──────────┬─────────┐
│ database │   table   │        name        │   type    │ position │ comment │
│ varchar  │  varchar  │      varchar       │  varchar  │  int32   │ varchar │
├──────────┼───────────┼────────────────────┼───────────┼──────────┼─────────┤
│ memory   │ test2     │ value              │ DOUBLE    │        0 │         │
│ memory   │ columns   │ database           │ VARCHAR   │        0 │         │
│ memory   │ columns   │ table              │ VARCHAR   │        1 │         │
│ memory   │ columns   │ name               │ VARCHAR   │        2 │         │
│ memory   │ columns   │ type               │ VARCHAR   │        3 │         │
│ memory   │ columns   │ position           │ INTEGER   │        4 │         │
│ memory   │ columns   │ comment            │ VARCHAR   │        5 │         │
│ memory   │ databases │ name               │ VARCHAR   │        0 │         │
│ memory   │ databases │ engine             │ VARCHAR   │        1 │         │
│ memory   │ databases │ type               │ VARCHAR   │        2 │         │
│ memory   │ functions │ name               │ VARCHAR   │        0 │         │
│ memory   │ functions │ is_aggregate       │ BOOLEAN   │        1 │         │
│ memory   │ functions │ case_insensitive   │ BOOLEAN   │        2 │         │
│ memory   │ functions │ description        │ VARCHAR   │        3 │         │
│ memory   │ tables    │ database           │ VARCHAR   │        0 │         │
│ memory   │ tables    │ schema             │ VARCHAR   │        1 │         │
│ memory   │ tables    │ name               │ VARCHAR   │        2 │         │
│ memory   │ tables    │ uuid               │ UUID      │        3 │         │
│ memory   │ tables    │ engine             │ VARCHAR   │        4 │         │
│ memory   │ tables    │ is_temporary       │ BOOLEAN   │        5 │         │
│ memory   │ tables    │ column_names       │ VARCHAR[] │        6 │         │
│ memory   │ tables    │ column_types       │ VARCHAR[] │        7 │         │
│ memory   │ tables    │ create_table_query │ VARCHAR   │        8 │         │
│ memory   │ tables    │ comment            │ VARCHAR   │        9 │         │
├──────────┴───────────┴────────────────────┴───────────┴──────────┴─────────┤
│ 24 rows                                                          6 columns │
└────────────────────────────────────────────────────────────────────────────┘
D 
D -- List all functions in scope
D SELECT * FROM system.functions;
┌──────────────────────┬──────────────┬──────────────────┬────────────────────────────────────────────────────────────────────────────────────────────────┐
│         name         │ is_aggregate │ case_insensitive │                                          description                                           │
│       varchar        │   boolean    │     boolean      │                                            varchar                                             │
├──────────────────────┼──────────────┼──────────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
│ pragma_version       │ false        │ false            │ NULL                                                                                           │
│ repeat_row           │ false        │ false            │ NULL                                                                                           │
│ duckdb_constraints   │ false        │ false            │ NULL                                                                                           │
│ duckdb_extensions    │ false        │ false            │ NULL                                                                                           │
│ duckdb_functions     │ false        │ false            │ NULL                                                                                           │
│ summary              │ false        │ false            │ NULL                                                                                           │
│ system_columns       │ false        │ false            │ NULL                                                                                           │
│ test_all_types       │ false        │ false            │ NULL                                                                                           │
│ duckdb_sequences     │ false        │ false            │ NULL                                                                                           │
│ list_indexof         │ false        │ false            │ NULL                                                                                           │
│ try_strptime         │ false        │ false            │ Converts string to timestamp using the format string (timestamp with time zone if %Z is spec…  │
│ list_sort            │ false        │ false            │ Sorts the elements of the list                                                                 │
│ list_pack            │ false        │ false            │ Create a LIST containing the argument values                                                   │
│ list_negative_inne…  │ false        │ false            │ Compute the negative inner product between two lists                                           │
│ get_current_timest…  │ false        │ false            │ Returns the current timestamp                                                                  │
│ unhex                │ false        │ false            │ Converts a value from hexadecimal representation to a blob                                     │
│ get_bit              │ false        │ false            │ Extracts the nth bit from bitstring; the first (leftmost) bit is indexed 0                     │
│ union_extract        │ false        │ false            │ Extract the value with the named tags from the union. NULL if the tag is not currently selec…  │
│ from_base64          │ false        │ false            │ Convert a base64 encoded string to a character string                                          │
│ last_day             │ false        │ false            │ Returns the last day of the month                                                              │
│  ·                   │   ·          │   ·              │                 ·                                                                              │
│  ·                   │   ·          │   ·              │                 ·                                                                              │
│  ·                   │   ·          │   ·              │                 ·                                                                              │
│ mad                  │ false        │ false            │ Returns the median absolute deviation for the values within x. NULL values are ignored. Temp…  │
│ max                  │ false        │ false            │ Returns the maximum value present in arg.                                                      │
│ arg_max_null         │ false        │ false            │ Finds the row with the maximum val. Calculates the arg expression at that row.                 │
│ disable_profiling    │ false        │ false            │ NULL                                                                                           │
│ disable_profile      │ false        │ false            │ NULL                                                                                           │
│ disable_checkpoint…  │ false        │ false            │ NULL                                                                                           │
│ enable_profiling     │ false        │ false            │ NULL                                                                                           │
│ verify_parallelism   │ false        │ false            │ NULL                                                                                           │
│ session_user         │ false        │ false            │ NULL                                                                                           │
│ list_max             │ false        │ false            │ NULL                                                                                           │
│ list_skewness        │ false        │ false            │ NULL                                                                                           │
│ pg_postmaster_star…  │ false        │ false            │ NULL                                                                                           │
│ pg_table_is_visible  │ false        │ false            │ NULL                                                                                           │
│ has_foreign_data_w…  │ false        │ false            │ NULL                                                                                           │
│ has_sequence_privi…  │ false        │ false            │ NULL                                                                                           │
│ has_tablespace_pri…  │ false        │ false            │ NULL                                                                                           │
│ pg_conversion_is_v…  │ false        │ false            │ NULL                                                                                           │
│ list_count           │ false        │ false            │ NULL                                                                                           │
│ list_first           │ false        │ false            │ NULL                                                                                           │
│ array_reverse        │ false        │ false            │ NULL                                                                                           │
├──────────────────────┴──────────────┴──────────────────┴────────────────────────────────────────────────────────────────────────────────────────────────┤
│ 692 rows (40 shown)                                                                                                                           4 columns │
└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
```
