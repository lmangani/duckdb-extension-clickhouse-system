#define DUCKDB_EXTENSION_MAIN

#include "chsql_system_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/function/table_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include "duckdb/main/client_context.hpp"
#include "duckdb/catalog/catalog.hpp"
#include "duckdb/catalog/catalog_entry/schema_catalog_entry.hpp"
#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "duckdb/main/connection.hpp"

namespace duckdb {

struct SystemDatabasesData : public TableFunctionData {
    vector<string> databases;
    idx_t offset = 0;
};

struct SystemTablesData : public TableFunctionData {
    vector<Value> databases;
    vector<Value> schemas;
    vector<Value> names;
    vector<Value> column_names;
    vector<Value> column_types;
    vector<Value> temporary;
    idx_t offset = 0;
};

static void SystemDatabasesFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output) {
    auto &data = (SystemDatabasesData &)*data_p.bind_data;
    if (data.offset >= data.databases.size()) {
        return;
    }

    idx_t count = 0;
    while (data.offset < data.databases.size() && count < STANDARD_VECTOR_SIZE) {
        auto &db_name = data.databases[data.offset];
        
        output.SetValue(0, count, Value(db_name));        // name
        output.SetValue(1, count, Value("DuckDB"));       // engine
        output.SetValue(2, count, Value(db_name == "memory" ? "memory" : "attached")); // type
        
        count++;
        data.offset++;
    }

    output.SetCardinality(count);
}

static void SystemTablesFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output) {
    auto &data = (SystemTablesData &)*data_p.bind_data;
    if (data.offset >= data.databases.size()) {
        return;
    }

    idx_t count = 0;
    while (data.offset < data.databases.size() && count < STANDARD_VECTOR_SIZE) {
        output.SetValue(0, count, data.databases[data.offset]);    // database
        output.SetValue(1, count, data.schemas[data.offset]);      // schema
        output.SetValue(2, count, data.names[data.offset]);        // name
        output.SetValue(3, count, data.column_names[data.offset]); // column_names
        output.SetValue(4, count, data.column_types[data.offset]); // column_types
        output.SetValue(5, count, data.temporary[data.offset]);    // temporary
        output.SetValue(6, count, Value(data.temporary[data.offset].GetValue<bool>() ? "TEMPORARY" : "BASE TABLE")); // engine
        
        count++;
        data.offset++;
    }

    output.SetCardinality(count);
}

static unique_ptr<FunctionData> SystemDatabasesBind(ClientContext &context, TableFunctionBindInput &input,
                                                  vector<LogicalType> &return_types, vector<string> &names) {
    names.emplace_back("name");
    names.emplace_back("engine");
    names.emplace_back("type");

    return_types.emplace_back(LogicalType::VARCHAR);
    return_types.emplace_back(LogicalType::VARCHAR);
    return_types.emplace_back(LogicalType::VARCHAR);

    auto result = make_uniq<SystemDatabasesData>();

    // Get databases from catalog
    Connection con(Catalog::GetSystemCatalog(context).GetDatabase());
    auto db_result = con.Query("SELECT DISTINCT database FROM (SHOW ALL TABLES)");
    
    if (!db_result->HasError()) {
        for (auto &row : *db_result) {
            result->databases.push_back(row.GetValue<string>(0));
        }
    }

    return std::move(result);
}

static unique_ptr<FunctionData> SystemTablesBind(ClientContext &context, TableFunctionBindInput &input,
                                               vector<LogicalType> &return_types, vector<string> &names) {
    // Core columns
    names.emplace_back("database");
    names.emplace_back("schema");
    names.emplace_back("name");
    names.emplace_back("column_names");
    names.emplace_back("column_types");
    names.emplace_back("temporary");
    names.emplace_back("engine");

    return_types.emplace_back(LogicalType::VARCHAR);
    return_types.emplace_back(LogicalType::VARCHAR);
    return_types.emplace_back(LogicalType::VARCHAR);
    return_types.emplace_back(LogicalType::LIST(LogicalType::VARCHAR));
    return_types.emplace_back(LogicalType::LIST(LogicalType::VARCHAR));
    return_types.emplace_back(LogicalType::BOOLEAN);
    return_types.emplace_back(LogicalType::VARCHAR);

    auto result = make_uniq<SystemTablesData>();
    
    // Get tables using SHOW ALL TABLES
    Connection con(Catalog::GetSystemCatalog(context).GetDatabase());
    auto table_result = con.Query("SHOW ALL TABLES");
    
    if (!table_result->HasError()) {
        for (auto &row : *table_result) {
            result->databases.push_back(row.GetValue<std::string>(0));     // database
            result->schemas.push_back(row.GetValue<std::string>(1));       // schema
            result->names.push_back(row.GetValue<std::string>(2));         // name
            result->column_names.push_back(row.GetValue<Value>(3));        // column_names
            result->column_types.push_back(row.GetValue<Value>(4));        // column_types
            result->temporary.push_back(row.GetValue<bool>(5));            // temporary
        }
    }

    return std::move(result);
}

void ChsqlSystemExtension::Load(DuckDB &db) {
    Connection con(db);
    con.BeginTransaction();

    // Register system.databases table function
    auto databases_func = TableFunction("system_databases", {}, SystemDatabasesFunction, SystemDatabasesBind);
    ExtensionUtil::RegisterFunction(*db.instance, databases_func);

    // Register system.tables table function
    auto tables_func = TableFunction("system_tables", {}, SystemTablesFunction, SystemTablesBind);
    ExtensionUtil::RegisterFunction(*db.instance, tables_func);

    // Create system schema if it doesn't exist
    con.Query("CREATE SCHEMA IF NOT EXISTS system;");
    
    // Create views in system schema
    con.Query("CREATE OR REPLACE VIEW system.databases AS SELECT * FROM system_databases();");
    con.Query("CREATE OR REPLACE VIEW system.tables AS SELECT * FROM system_tables();");

    con.Commit();
}

std::string ChsqlSystemExtension::Name() {
    return "chsql_system";
}

std::string ChsqlSystemExtension::Version() const {
    return "v0.0.1";
}

} // namespace duckdb

extern "C" {
DUCKDB_EXTENSION_API void chsql_system_init(duckdb::DatabaseInstance &db) {
    duckdb::DuckDB db_wrapper(db);
    db_wrapper.LoadExtension<duckdb::ChsqlSystemExtension>();
}

DUCKDB_EXTENSION_API const char *chsql_system_version() {
    return duckdb::DuckDB::LibraryVersion();
}
}

#ifndef DUCKDB_EXTENSION_MAIN
#error DUCKDB_EXTENSION_MAIN not defined
#endif
