//
//  KeyStoreValue.hpp
//  KeyValueStore
//
//  Created by Michael Markley on 8/23/24.
//

#ifndef KeyStoreValue_hpp
#define KeyStoreValue_hpp

#include <stdio.h>
#include <iostream>
#include <cxxabi.h>
#include <map>
#include <any>
#include <mutex>

class KeyStoreValue {
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
        const char* type = value.type().name();
        std::unique_lock<std::mutex> lock(mutex);
        char* name = abi::__cxa_demangle(type,0,0,0);
        valueMap[key] = value;
        std::cout << "key is " << key << " type is " << name << "\n";
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
};

#endif /* KeyStoreValue_hpp */
