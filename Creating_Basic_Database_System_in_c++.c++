#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

class Database {
public:
    Database() {}

    void createTable(const string& tableName, const vector<string>& columnNames, const vector<string>& columnTypes) {
        Table newTable;
        newTable.name = tableName;
        for (int i = 0; i < columnNames.size(); ++i) {
            newTable.columns.push_back({columnNames[i], columnTypes[i]});
        }
        tables.push_back(newTable);
    }

    void insertRecord(const string& tableName, const vector<string>& values) {
        for (Table& table : tables) {
            if (table.name == tableName) {
                if (values.size() != table.columns.size()) {
                    throw runtime_error("Invalid number of values for table: " + tableName);
                }
                table.data.push_back(values);
                return;
            }
        }
        throw runtime_error("Table not found: " + tableName);
    }

    void updateRecord(const string& tableName, int rowIndex, int columnIndex, const string& newValue) {
        for (Table& table : tables) {
            if (table.name == tableName) {
                if (rowIndex >= 0 && rowIndex < table.data.size() && columnIndex >= 0 && columnIndex < table.columns.size()) {
                    table.data[rowIndex][columnIndex] = newValue;
                    return;
                }
                throw runtime_error("Invalid row or column index: " + tableName);
            }
        }
        throw runtime_error("Table not found: " + tableName);
    }

    void deleteRecord(const string& tableName, int rowIndex) {
        for (Table& table : tables) {
            if (table.name == tableName) {
                if (rowIndex >= 0 && rowIndex < table.data.size()) {
                    table.data.erase(table.data.begin() + rowIndex);
                    return;
                }
                throw runtime_error("Invalid row index: " + tableName);
            }
        }
        throw runtime_error("Table not found: " + tableName);
    }

    void viewRecords(const string& tableName) {
        for (Table& table : tables) {
            if (table.name == tableName) {
                cout << table.name << endl;
                cout << "\t";
                for (const Column& column : table.columns) {
                    cout << column.name << "\t";
                }
                cout << endl;
                for (const vector<string>& row : table.data) {
                    cout << "\t";
                    for (const string& value : row) {
                        cout << value << "\t";
                    }
                    cout << endl;
                }
                return;
            }
        }
        throw runtime_error("Table not found: " + tableName);
    }

    void dumpToFile(const string& filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("Failed to open file for writing: " + filename);
    }

    for (const Table& table : tables) {
        file << "Table: " << table.name << endl;
        for (const Column& column : table.columns) {
            file << "\t" << column.name << " (" << column.type << ")" << endl;
        }
        for (const vector<string>& row : table.data) {
            //file << "\t";
            for (const string& value : row) {
                file << value << "\t";
            }
            file << endl;
        }
        file << endl;
    }

    file.close();
}

    void loadFromFile(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("Failed to open file for reading: " + filename);
    }

    while (!file.eof()) {
        string line;
        getline(file, line);
        if (line.substr(0, 7) == "Table: ") {
            string tableName = line.substr(7);
            createTable(tableName, {}, {});  // Create an empty table initially
            while (getline(file, line) && !line.empty()) {
                if (line.substr(0, 1) == "\t") {
                    stringstream ss(line.substr(1));
                    string columnName, columnType;
                    ss >> columnName >> columnType;
                   
                    tables.back().columns.push_back({columnName, columnType});
                } else {
                    stringstream ss(line);
                    vector<string> values;
                    string value;
                    while (ss >> value) {
                        values.push_back(value);
                    }
                    insertRecord(tableName, values);
                }
            }
        }
    }

    file.close();
}

private:
    struct Column {
        string name;
        string type;
    };

    struct Table {
        string name;
        vector<Column> columns;
        vector<vector<string>> data;
    };

    vector<Table> tables;
};




int main() {
    Database db;
    string studentsdb="studentsdb";
    //Load db from file
    //db.loadFromFile(studentsdb);
    
    // Create a table
    //db.createTable("students", {"id", "name", "age"}, {"int", "string", "int"});

    // Insert records
    //db.insertRecord("students", {"1", "Alice", "20"});
    //db.insertRecord("students", {"2", "Bob", "22"});

    // Update a record
    //db.updateRecord("students", 0, 2, "21");

    // Delete a record
    //db.deleteRecord("students", 1);

    // View records
    db.viewRecords("students");

    //dump database into file
    //db.dumpToFile(studentsdb);

    return 0;
}