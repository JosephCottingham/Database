// Basic_Database.cpp : Defines the entry point for the application.
//

#include "BasicDatabase.hpp"

int main()
{
	std::cout << "Starting Basic Database" << std::endl;

	// Create a storage engine
	StorageEngine storageEngine("C:\\Users\\josep\\Projects\\Database\\db");
	Column c1, c2;
	c1.name = "id";
	c1.datatype = DataType::INT;
	c2.name = "name";
	c2.datatype = DataType::STRING;

	std::vector<Column> tableSchema{
        c1,
		c2
    };
	std::string primaryKey = "id";
	std::string tableName = "users";
	storageEngine.createTable(tableName, primaryKey, tableSchema);
	std::vector<uint8_t> data(255+4);

	data[3] = 1;
	data[4] = 'j';
	data[5] = 'o';
	data[6] = 's';
	data[7] = 'e';
	data[8] = 'p';
	data[9] = 'h';

	storageEngine.insertRecord(tableName, data);
	return 0;
}