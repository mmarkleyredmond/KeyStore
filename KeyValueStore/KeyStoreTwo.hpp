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

class KeyStoreTwo {
public:
    void put(std::string key, int value) {
        union {
            int intValue;
            std::byte bytes[4];
        } data;
        
        data.intValue = value;
        put(key, "int", data.bytes, 4);
    }
    
    void put(std::string key, float value) {
        union {
            float floatValue;
            std::byte bytes[4];
        } data;
        
        data.floatValue = value;
        put(key, "float", data.bytes, 4);
    }
    
    void put(std::string key, std::byte* value) {
        put(key, "byteArray", value, sizeof(value));
    }
    
    void put(std::string key, double value) {
        union {
            double doubleValue;
            std::byte bytes[8];
        } data;
        
        data.doubleValue = value;
        put(key, "double", data.bytes, 8);
    }
    
    int getInt(std::string key) {
        std::string path = "int\\" + key;
        union {
            int value;
            std::byte bytes[4];
        } data;
        
        get(path, data.bytes, 4, 1);
        return data.value;
    }
    
    float getFloat(std::string key) {
        std::string path = "float\\" + key;
        union {
            float value;
            std::byte bytes[4];
        } data;
        
        get(path, data.bytes, 4, 1);
        return data.value;
    }

    double getDouble(std::string key) {
        std::string path = "double\\" + key;
        union {
            double value;
            std::byte bytes[8];
        } data;
        
        get(path, data.bytes, 8, 1);
        return data.value;
    }
    
private:
    std::map<std::string, std::any> valueMap;
    std::mutex mutex;
    std::unique_lock<std::mutex> lock;
    
    void put(std::string key, std::string type, std::byte* value, int length) {

        std::string path = type + "\\" + key;
        std::cout << "Path " << path + "\n";
        FILE* file = fopen(path.data(), "w");
        fwrite(value, sizeof(std::byte*), length, file);
        fflush(file);
        fclose(file);
    }

    void get(std::string path, std::byte* result, int size, int count) {
        FILE* file = fopen(path.data(), "r");
        if(NULL != file) {
            fread(result, size, count, file);
        }
        fclose(file);
    }

};

#endif /* KeyStoreTwo_hpp */
