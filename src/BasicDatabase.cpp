// Basic_Database.cpp : Defines the entry point for the application.
//

#include "BasicDatabase.hpp"

int main()
{
	std::cout << "Starting Basic Database" << std::endl;

	// Create a storage engine
	StorageEngine storageEngine("C:\\Users\\josep\\Projects\\Database\\db");
	ColumnMetadata c1, c2;
	// c1.name = "id";
	// c1.datatype = DataType::INT;
	c2.name = "name";
	c2.datatype = DataType::STRING;

	std::vector<ColumnMetadata> tableSchema;
	tableSchema.push_back(c1);
	tableSchema.push_back(c2);

	std::string primaryKey = "id";
	std::string tableName = "users";
	storageEngine.createTable(tableName, tableSchema);
	std::vector<uint8_t> data(255);

	// data[3] = 1;
	data[0] = 'j';
	data[1] = 'o';
	data[2] = 's';
	data[3] = 'e';
	data[4] = 'p';
	data[5] = 'h';

	int response = storageEngine.insertRecord(tableName, data);
	std::cout << "Response: " << response << std::endl;
	TableData responseData = storageEngine.readRecord(tableName, 1);
	return 0;
}