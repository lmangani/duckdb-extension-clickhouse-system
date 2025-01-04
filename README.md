<a href="https://community-extensions.duckdb.org/extensions/chsql.html" target="_blank">
<img src="https://github.com/user-attachments/assets/9003897d-db6f-4a79-9443-9b72766b511b" width=200>
</a>

# DuckDB Clickhouse System Table emulator for [chsql](https://github.com/quackscience/duckdb-extension-clickhouse-sql)

> This project emulates ClickHouse system tables within DuckDB

The extension provides system functions:
### Table Views
- [x] `system.databases`
- [x] `system.tables`
- [x] `system.columns`
- [x] `system.functions`
- [x] `system.uptime`
### Scalar
- [x] `uptime()`


## Installation

> coming soon

## Usage
```sql
D -- List all databases in scope
D SELECT * FROM system.databases;
┌─────────┬─────────┬───────────┬───────────────┬──────────────────────────────────────┬─────────────┬─────────┐
│  name   │ engine  │ data_path │ metadata_path │                 uuid                 │ engine_full │ comment │
│ varchar │ varchar │  varchar  │    varchar    │                 uuid                 │   varchar   │ varchar │
├─────────┼─────────┼───────────┼───────────────┼──────────────────────────────────────┼─────────────┼─────────┤
│ memory  │ duckdb  │ NULL      │               │ 80000000-0000-0000-0000-000000000000 │ DuckDB      │ NULL    │
│ test    │ duckdb  │ test.db   │               │ 80000000-0000-0000-0000-000000000000 │ DuckDB      │ NULL    │
│ system  │ duckdb  │ NULL      │               │ 80000000-0000-0000-0000-000000000000 │ DuckDB      │ NULL    │
│ temp    │ duckdb  │ NULL      │               │ 80000000-0000-0000-0000-000000000000 │ DuckDB      │ NULL    │
└─────────┴─────────┴───────────┴───────────────┴──────────────────────────────────────┴─────────────┴─────────┘

D -- List all tables in scope
D SELECT * FROM system.tables;
┌──────────┬──────────────┬──────────────────────────────────────┬────────────┬──────────────┬───────────┬───────────────┬────────────────────────────┬──────────────────┬────────────────────────────────────────┐
│ database │     name     │                 uuid                 │   engine   │ is_temporary │ data_path │ metadata_path │ metadata_modification_time │ metadata_version │           create_table_query           │
│ varchar  │   varchar    │                 uuid                 │  varchar   │   boolean    │  varchar  │    varchar    │         timestamp          │      int32       │                varchar                 │
├──────────┼──────────────┼──────────────────────────────────────┼────────────┼──────────────┼───────────┼───────────────┼────────────────────────────┼──────────────────┼────────────────────────────────────────┤
│ test     │ remote_table │ 80000000-0000-0000-0000-000000000000 │ BASE TABLE │ false        │           │               │ 1970-01-01 00:00:00        │                0 │ CREATE TABLE remote_table(id INTEGER); │
└──────────┴──────────────┴──────────────────────────────────────┴────────────┴──────────────┴───────────┴───────────────┴────────────────────────────┴──────────────────┴────────────────────────────────────────┘
 
D -- List all columns in scope
D SELECT * FROM system.columns;
┌──────────┬───────────┬────────────────────────────┬───────────┬──────────┬─────────┐
│ database │   table   │            name            │   type    │ position │ comment │
│ varchar  │  varchar  │          varchar           │  varchar  │  int32   │ varchar │
├──────────┼───────────┼────────────────────────────┼───────────┼──────────┼─────────┤
│ memory   │ columns   │ database                   │ VARCHAR   │        0 │         │
│ memory   │ columns   │ table                      │ VARCHAR   │        1 │         │
│ memory   │ columns   │ name                       │ VARCHAR   │        2 │         │
│ memory   │ columns   │ type                       │ VARCHAR   │        3 │         │
│ memory   │ columns   │ position                   │ INTEGER   │        4 │         │
│ memory   │ columns   │ comment                    │ VARCHAR   │        5 │         │
│ memory   │ databases │ name                       │ VARCHAR   │        0 │         │
│ memory   │ databases │ engine                     │ VARCHAR   │        1 │         │
│ memory   │ databases │ data_path                  │ VARCHAR   │        2 │         │
│ memory   │ databases │ metadata_path              │ VARCHAR   │        3 │         │
│ memory   │ databases │ uuid                       │ UUID      │        4 │         │
│ memory   │ databases │ engine_full                │ VARCHAR   │        5 │         │
│ memory   │ databases │ comment                    │ VARCHAR   │        6 │         │
│ memory   │ functions │ name                       │ VARCHAR   │        0 │         │
│ memory   │ functions │ is_aggregate               │ BOOLEAN   │        1 │         │
│ memory   │ functions │ case_insensitive           │ BOOLEAN   │        2 │         │
│ memory   │ functions │ description                │ VARCHAR   │        3 │         │
│ memory   │ tables    │ database                   │ VARCHAR   │        0 │         │
│ memory   │ tables    │ name                       │ VARCHAR   │        1 │         │
│ memory   │ tables    │ uuid                       │ UUID      │        2 │         │
│ memory   │ tables    │ engine                     │ VARCHAR   │        3 │         │
│ memory   │ tables    │ is_temporary               │ BOOLEAN   │        4 │         │
│ memory   │ tables    │ data_path                  │ VARCHAR   │        5 │         │
│ memory   │ tables    │ metadata_path              │ VARCHAR   │        6 │         │
│ memory   │ tables    │ metadata_modification_time │ TIMESTAMP │        7 │         │
│ memory   │ tables    │ metadata_version           │ INTEGER   │        8 │         │
│ memory   │ tables    │ create_table_query         │ VARCHAR   │        9 │         │
├──────────┴───────────┴────────────────────────────┴───────────┴──────────┴─────────┤
│ 27 rows                                                                  6 columns │
└────────────────────────────────────────────────────────────────────────────────────┘

 
D -- List all functions in scope
D SELECT * FROM system.functions;
┌──────────────────────────────────────────┬──────────────┬──────────────────┬─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                   name                   │ is_aggregate │ case_insensitive │                                                                   description                                                                   │
│                 varchar                  │   boolean    │     boolean      │                                                                     varchar                                                                     │
├──────────────────────────────────────────┼──────────────┼──────────────────┼─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ test_all_types                           │ false        │ false            │ NULL                                                                                                                                            │
│ pragma_version                           │ false        │ false            │ NULL                                                                                                                                            │
│ duckdb_sequences                         │ false        │ false            │ NULL                                                                                                                                            │
│ duckdb_functions                         │ false        │ false            │ NULL                                                                                                                                            │
│ duckdb_extensions                        │ false        │ false            │ NULL                                                                                                                                            │
│ duckdb_constraints                       │ false        │ false            │ NULL                                                                                                                                            │
│ system_columns                           │ false        │ false            │ NULL                                                                                                                                            │
│ summary                                  │ false        │ false            │ NULL                                                                                                                                            │
│ repeat_row                               │ false        │ false            │ NULL                                                                                                                                            │
│ __internal_decompress_integral_usmallint │ false        │ false            │ NULL                                                                                                                                            │
│ md5                                      │ false        │ false            │ Returns the MD5 hash of the value as a string                                                                                                   │
│ list_negative_inner_product              │ false        │ false            │ Compute the negative inner product between two lists                                                                                            │
│ list_sort                                │ false        │ false            │ Sorts the elements of the list                                                                                                                  │
│ __internal_decompress_integral_smallint  │ false        │ false            │ NULL                                                                                                                                            │
│ list_unique                              │ false        │ false            │ Counts the unique elements of a list                                                                                                            │
│ last_day                                 │ false        │ false            │ Returns the last day of the month                                                                                                               │
│ isnan                                    │ false        │ false            │ Returns true if the floating point value is not a number, false otherwise                                                                       │
│ isinf                                    │ false        │ false            │ Returns true if the floating point value is infinite, false otherwise                                                                           │
│ list_pack                                │ false        │ false            │ Create a LIST containing the argument values                                                                                                    │
│ list_indexof                             │ false        │ false            │ NULL                                                                                                                                            │
│  ·                                       │   ·          │   ·              │  ·                                                                                                                                              │
│  ·                                       │   ·          │   ·              │  ·                                                                                                                                              │
│  ·                                       │   ·          │   ·              │  ·                                                                                                                                              │
│ list                                     │ false        │ false            │ Returns a LIST containing all the values of a column.                                                                                           │
│ listagg                                  │ false        │ false            │ Concatenates the column string values with an optional separator.                                                                               │
│ bit_xor                                  │ false        │ false            │ Returns the bitwise XOR of all bits in a given expression.                                                                                      │
│ kurtosis                                 │ false        │ false            │ Returns the excess kurtosis (Fisher’s definition) of all input values, with a bias correction according to the sample size                      │
│ any_value                                │ false        │ false            │ NULL                                                                                                                                            │
│ product                                  │ false        │ false            │ Calculates the product of all tuples in arg.                                                                                                    │
│ quantile                                 │ false        │ false            │ Returns the exact quantile number between 0 and 1 . If pos is a LIST of FLOATs, then the result is a LIST of the corresponding exact quantiles. │
│ metadata_info                            │ false        │ false            │ NULL                                                                                                                                            │
│ user_agent                               │ false        │ false            │ NULL                                                                                                                                            │
│ array_to_string_comma_default            │ false        │ false            │ NULL                                                                                                                                            │
│ list_reverse                             │ false        │ false            │ NULL                                                                                                                                            │
│ list_mode                                │ false        │ false            │ NULL                                                                                                                                            │
│ list_approx_count_distinct               │ false        │ false            │ NULL                                                                                                                                            │
│ list_var_samp                            │ false        │ false            │ NULL                                                                                                                                            │
│ pg_opfamily_is_visible                   │ false        │ false            │ NULL                                                                                                                                            │
│ pg_ts_parser_is_visible                  │ false        │ false            │ NULL                                                                                                                                            │
│ format_pg_type                           │ false        │ false            │ NULL                                                                                                                                            │
│ format_type                              │ false        │ false            │ NULL                                                                                                                                            │
│ has_column_privilege                     │ false        │ false            │ NULL                                                                                                                                            │
│ inet_client_port                         │ false        │ false            │ NULL                                                                                                                                            │
├──────────────────────────────────────────┴──────────────┴──────────────────┴─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ 692 rows (40 shown)                                                                                                                                                                                                4 columns │
└──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
```
