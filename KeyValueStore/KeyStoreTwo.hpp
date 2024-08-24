//
//  KeyStoreTwo.hpp
//  KeyValueStore
//
//  Created by Michael Markley on 8/23/24.
//

#ifndef KeyStoreTwo_hpp
#define KeyStoreTwo_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <any>
#include <map>
#include <mutex>
#include "sys/stat.h"
#include "sys/types.h"
#include <sstream>

class KeyStoreTwo {
public:
    /*
     Store integer values
     */
    void put(std::string key, int value) {
        union {
            int intValue = INT_MIN;
            std::byte bytes[4];
        } data;
        
        data.intValue = value;
        put(key, "int", data.bytes, 4);
    }
    
    /*
     Store float values
     */
    void put(std::string key, float value) {
        union {
            float floatValue = NAN;
            std::byte bytes[4];
        } data;
        
        data.floatValue = value;
        put(key, "float", data.bytes, 4);
    }

    /*
     Store double values
     */
    void put(std::string key, double value) {
        union {
            double doubleValue = NAN;
            std::byte bytes[8];
        } data;
        
        data.doubleValue = value;
        put(key, "double", data.bytes, 8);
    }
    
    /*
     Store arbitrary byte arrays.
     */
    void put(std::string key, std::byte* value, int size) {
#ifdef DEBUG
        std::cout << "writing " << size << " bytes to file\n";
#endif
        put(key, "byteArray", value, size);
    }
    
    /*
     Retrieve an integer value
     */
    int getInt(std::string key) {
        union {
            int value;
            std::byte bytes[4];
        } data;
        
        get(key, "int", data.bytes, 4, 1);
        return data.value;
    }
    
    /*
     Retrieve a float value
     */
    float getFloat(std::string key) {
        union {
            float value;
            std::byte bytes[4];
        } data;
        
        get(key, "float", data.bytes, 4, 1);
        return data.value;
    }

    /*
     Retrieve a double value
     */
    double getDouble(std::string key) {
        std::string path = buildPath("double", key);
        union {
            double value;
            std::byte bytes[8];
        } data;
        
        get(key, "double", data.bytes, 8, 1);
        return data.value;
    }
    
    /*
     Retrieve a byte array.
     */
    std::byte* getBytes(std::string key) {
        std::string path = buildPath("byteArray", key);
        std::byte * bytes = NULL;
        int byteCount = getSize(path);
        if(0 < byteCount) {
            bytes = (std::byte*)malloc(byteCount + 1);
            memset(bytes, 0, byteCount);
            get(key, "byteArray", bytes, byteCount, 1);
        }

#ifdef DEBUG
//        std::cout << "getBytes " << sizeof(bytes) << "\n";
#endif

        return bytes;
    }
    
private:
    std::mutex readMutex;
    std::mutex writeMutex;
    std::unique_lock<std::mutex> lock;
    
    
    /*
     Method to create a path
     */
    std::string buildPath(std::string first, std::string fileName) {
        std::string path = first + std::filesystem::path::preferred_separator + fileName;
//        std::cout << "Built " << path << "\n";
        return path;
    }
    
    /*
     Write the specified bytes to a file.
     The file is created in a directory based on the type, plus the key
     The file contains:
        1. the size of the bytes. Stored as an integer
        2. The data.
     */
    void put(std::string key, std::string type, std::byte* value, int length) {
        std::unique_lock<std::mutex> lock(writeMutex);
        try {
            std::string path = buildPath(type, key);
//            std::cout << "Path " << path + "\n";
            
            // Make sure the directory exists, if not create it.
            if(!std::filesystem::is_directory(path)) {
                std::filesystem::create_directory(type);
            }
            
            FILE* file = fopen(path.data(), "w");
#ifdef DEBUG
//            std::cout << "Writing " << length << " bytes\n";
#endif
            
            // Write out the number of bytes we are writing.
            if(NULL != file) {
                fwrite(&length, sizeof(int), 1, file);
                fwrite(value, sizeof(std::byte*), length, file);
                fflush(file);
                fclose(file);
            } else {
                std::cout << "Failed to open " + path + " errorno " << errno << "\n";
            }
        } catch(...) {
            std::cout << "Something went wrong writing the bytes";
        }
        lock.unlock();
    }

    /*
     Retrieve the size count from the file, returns 0 if the file doesn't exist.
     */
    int getSize(std::string path) {
        int size = 0;
        std::unique_lock<std::mutex> lock(readMutex);
        try {
            FILE* file = fopen(path.data(), "r");
            if(NULL != file) {
                fread(&size, sizeof(int), 1, file);
                fclose(file);
            }
        } catch (...) {
            std::cout << "Something went wrong getting the size of " << path << "\n";
        }
        lock.unlock();
        return size;
    }
    
    /*
     Retrieve the bytes from the file.
     */
    void get(std::string key, std::string type, std::byte* result, int size, int count) {
        std::unique_lock<std::mutex> lock(readMutex);
        try {
            std::string path = buildPath(type, key);
            FILE* file = fopen(path.data(), "r");
            int byteCount = 0;
            if(NULL != file) {
                // Read the byte count
                fread(&byteCount, sizeof(int), 1, file);
#ifdef DEBUG
//                std::cout << "Num bytes " << byteCount << "\n";
#endif
                if(NULL != result) {
                    fread(result, size, count, file);
                }
                
                fclose(file);
            }
        } catch(...) {
            std::cout << "Something went wrong retrieving bytes from " << key << "\n";
        }
        lock.unlock();
    }

};

#endif /* KeyStoreTwo_hpp */
