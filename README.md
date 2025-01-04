<a href="https://community-extensions.duckdb.org/extensions/chsql.html" target="_blank">
<img src="https://github.com/user-attachments/assets/9003897d-db6f-4a79-9443-9b72766b511b" width=200>
</a>

# DuckDB Clickhouse System Table emulator for [chsql](https://github.com/quackscience/duckdb-extension-clickhouse-sql)

> This project loosely emulates ClickHouse system tables within DuckDB for client compatibility

The `chsql_system` extension provides a subset of system functions:

### Table Views
- [x] `system.databases`
- [x] `system.tables`
- [x] `system.columns`
- [x] `system.functions`
- [x] `system.uptime`
- [x] `system.disks`
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
│ system_columns                           │ false        │ false            │ NULL                                                                                                                                            │                         │
│  ·                                       │   ·          │   ·              │  ·                                                                                                                                              │
│  ·                                       │   ·          │   ·              │  ·                                                                                                                                              │
│  ·                                       │   ·          │   ·              │  ·                                                                                                                                              │
│ format_type                              │ false        │ false            │ NULL                                                                                                                                            │
│ has_column_privilege                     │ false        │ false            │ NULL                                                                                                                                            │
│ inet_client_port                         │ false        │ false            │ NULL                                                                                                                                            │
├──────────────────────────────────────────┴──────────────┴──────────────────┴─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤
│ 692 rows (40 shown)                                                                                                                                                                                                4 columns │
└──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘

D -- List all attached databases as disks
D SELECT * FROM system.disks;
┌──────────┬──────────────┬────────────┬─────────────┬──────────────────┬─────────────────┬─────────┬─────────────────────┬───────────────┬──────────────┬──────────────┬───────────────┬───────────┬───────────┬────────────┐
│   name   │     path     │ free_space │ total_space │ unreserved_space │ keep_free_space │  type   │ object_storage_type │ metadata_type │ is_encrypted │ is_read_only │ is_write_once │ is_remote │ is_broken │ cache_path │
│ varchar  │   varchar    │   int64    │    int64    │      int64       │      int64      │ varchar │       varchar       │    varchar    │   boolean    │   boolean    │    boolean    │  boolean  │  boolean  │  varchar   │
├──────────┼──────────────┼────────────┼─────────────┼──────────────────┼─────────────────┼─────────┼─────────────────────┼───────────────┼──────────────┼──────────────┼───────────────┼───────────┼───────────┼────────────┤
│ localdb  │ test.db      │          0 │      262144 │                0 │               0 │ Local   │ None                │ None          │ false        │ false        │ false         │ false     │ false     │            │
│ memory   │ NULL         │          0 │           0 │                0 │               0 │ Local   │ None                │ None          │ false        │ false        │ false         │ false     │ false     │            │
│ testduck │ /tmp/duck.db │     262144 │      786432 │           262144 │               0 │ Local   │ None                │ None          │ false        │ false        │ false         │ false     │ false     │            │
└──────────┴──────────────┴────────────┴─────────────┴──────────────────┴─────────────────┴─────────┴─────────────────────┴───────────────┴──────────────┴──────────────┴───────────────┴───────────┴───────────┴────────────┘
```

### Note
Don't use this unless you need ClickHouse client compatibility. Rely on pure DuckDB functions to build your applications.
