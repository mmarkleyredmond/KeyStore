//
//  KeyStoreValue.hpp
//  KeyValueStore
//
//  Created by Michael Markley on 8/23/24.
//

#ifndef KeyStore_hpp
#define KeyStore_hpp

#include <stdio.h>
#include <iostream>
#include <cxxabi.h>
#include <map>
#include <any>
#include <mutex>


struct serialized {
    const char* key;
    const char* type;
    void* data;
};

class KeyStore {
public:
    void initializeWithDefaults() {
        int value = 10;
        float value2 = 1.1f;
        double value3 = 3.14;
        unsigned char bytes[] = {0xF0, 0xF1, 0xF2, 0xF3, 0x00};

        put("keyOne", value);
        put("keyTwo", value2);
        put("keyThree", value3);
        put("keyFour", bytes);
    }
    
    void put(std::string key, std::any value) {
        std::unique_lock<std::mutex> lock(mutex);
        valueMap[key] = value;
        lock.unlock();
    };
    
    std::any get(std::string key) {
        std::unique_lock<std::mutex> lock(mutex);
        std::any value;
        value.reset();
        try {
            value = valueMap.at(key);
        } catch(const std::out_of_range) {
            std::cout << "Key `" << key << "` not found\n";
        }
        lock.unlock();
        return value;
    }
    
private:
    std::map<std::string, std::any> valueMap;
    std::mutex mutex;
    std::unique_lock<std::mutex> lock;
    
    // We need to save the data to a persistent store. So we'll create a thread
    // to write the data, 
    std::mutex persist_mutex;
    void persist(std::byte* bytes) {
        
    }
    
    std::byte* toByteArray() {
        for(std::map<std::string, std::any>::iterator iter = valueMap.begin(); iter != valueMap.end(); ++iter) {
            std::string key = iter->first;
            std::any value = valueMap[key];
            serialized data;
            data.key = key.data();
            data.type = value.type().name();
            
        }
        return NULL;
    }
};


#endif /* KeyStore_hpp */
