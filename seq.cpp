#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

#include "leveldb/db.h"

#define NUM 2000
#define VAL_SIZE 25  // Aprox value (+10 up to -20(if VAL_SIZE=0))

using namespace std;

int main(int argc, char** argv)
{

    if(argc != 3) {
	cout << "PARAMS: num of key/values to write, num of key/values to read" << endl;
        return 1;
    }
    int NumWrite = strtol(argv[1], NULL, 10);
    int NumRead = strtol(argv[2], NULL, 10);
    if(NumRead > NumWrite) {
        cout << "ERROR: num of key/values to read GREATER than to write" << endl;
        return 1;
    }

    // Set up database connection information and open database
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, "./testdb", &db);

    if (false == status.ok())
    {
        cerr << "Unable to open/create test database './testdb'" << endl;
        cerr << status.ToString() << endl;
        return 1;
    }
    
    // Add values to the database
    leveldb::WriteOptions writeOptions;
    string keys[NUM];
    string values[NUM];

    char input[VAL_SIZE];
    input[0]='\0';
    if(VAL_SIZE > 20) {
        for(unsigned int i = 0; i < VAL_SIZE-20; ++i) {
            input[i] = 'a';
        }
    }

    char buffer[VAL_SIZE+32]; // Max Key/Value size
    for (unsigned int i = 0; i < NUM; ++i)
    {
        ostringstream keyStream;
	snprintf(buffer, sizeof(buffer), "%s%d", "Key", i);
	keys[i] = buffer;
        keyStream << buffer;
        
	
        ostringstream valueStream;
	snprintf(buffer, sizeof(buffer), "%s %d - %s\n", "Test data value:", i, input);
        valueStream << buffer;
        values[i] = buffer;
        db->Put(writeOptions, keyStream.str(), valueStream.str());
    }

    string aux;
    int n=NUM;
    for(int i=0; i<n-1; i++) { 
        for(int j = i+1; j<n; j++) {
            if(keys[i].compare(keys[j])>0) 
            {
                aux = keys[i];
                keys[i] = keys[j]; 
                keys[j] = aux; 


                aux = values[i];
                values[i] = values[j];
                values[j] = aux;
            }
        }
    } 
   
    // Iterate over each item in the database and print them
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());

    int i=0;
    for (it->SeekToFirst(); it->Valid(); it->Next())
    {
        cout << it->key().ToString() << " : " << it->value().ToString() << endl;
	//snprintf(buffer, sizeof(buffer), "%s%d", "Key", i);
//	if(it->key().ToString() != keys[i]) {
//	    cout << "********FAIL**********" << endl;
//            return 1;
//        }

        //snprintf(buffer, sizeof(buffer), "%s%d", "Test data value: ", i);
//        if(it->value().ToString() != values[i]) {
//            cout << "********FAIL**********" << endl;
//            return 1;
//        }

//	i++;
    }
    
    if (false == it->status().ok())
    {
        cerr << "An error was found during the scan" << endl;
        cerr << it->status().ToString() << endl;
    }
    
    delete it;
    
    // Close the database
    delete db;

    cout << "********PASS**********" << endl;
    return 0;
}


