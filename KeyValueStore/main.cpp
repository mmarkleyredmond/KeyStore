//
//  main.cpp
//  KeyValueStore
//
//  Created by Michael Markley on 8/23/24.
//

#include <iostream>
#include "KeyStore.hpp"
#include "KeyStoreTwo.hpp"
#include <thread>

// Thread to read values, for testing
void threadOne(KeyStore* keyStore) {
    for(int i = 0; i < 10; i++) {
        std::any retrieved = keyStore->get("keyOne");
        int valueOne = std::any_cast<int>(retrieved);
        std::cout << "ThreadOne Got " << valueOne << " for keyOne\n";
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }
    
}

// Thread to write values, for testing
void threadTwo(KeyStore* keyStore) {
    for(int i = 0; i < 5; i++) {
        std::cout << "Writing " << i << " to keyStore\n";
        keyStore->put("keyOne", i);
        std::this_thread::sleep_for (std::chrono::seconds(2));
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    KeyStore keyStore;
    KeyStoreTwo keyStoreTwo;

    // Call the initialization method for the KeyStore
    keyStore.initializeWithDefaults();
    std::any retrieved = keyStore.get("keyOne");
    
    // Validate the initial values in the KeyValueStore
    
    keyStoreTwo.put("intkey", 10);
    
    int intTwo = keyStoreTwo.getInt("intkey");
    
    std::cout << "Retrieved " << intTwo << "\n";
    
    keyStoreTwo.put("floatkey", 1.1f);
    
    float floatVAlue = keyStoreTwo.getFloat("floatkey");

    std::cout << "Retrieved float value " << floatVAlue <<  "\n";
    
    keyStoreTwo.put("doubleKey", 3.1415926);
    
    double doubleValue = keyStoreTwo.getDouble("doubleKey");
    
    std::cout << "Retrieved double value " << doubleValue << "\n";
    
    
    try {
        int valueOne = std::any_cast<int>(retrieved);
        std::cout << "Got " << valueOne << " for keyOne\n";
        retrieved = keyStore.get("keyTwo");
        float valueTwo = std::any_cast<float>(retrieved);
        std::cout << "Float value is " << valueTwo << "\n";
        retrieved = keyStore.get("keyThree");
        double valueThree = std::any_cast<double>(retrieved);
        std::cout << "Double value is " << valueThree << "\n";
        retrieved = keyStore.get("keyFour");
        unsigned char* byteValues = std::any_cast<unsigned char*>(retrieved);
        std::cout << "Byte values are " << byteValues << "\n";
        retrieved = keyStore.get("undefined key");
        if(!retrieved.has_value()) {
            std::cout << "Got no value for undefined key, as expected\n";
        } else {
            std::cout << "Weird, this should not have a value\n";
        }
       
    } catch(const std::bad_any_cast& e) {
        std::cout << "Something went wrong process retrieved value\n";
    }
    
    // Create the thread to write values
    std::thread writeThread(threadTwo, &keyStore);

    // Create thread to read values
    std::thread readThread(threadOne, &keyStore);
    
    writeThread.join();
    readThread.join();
    return 0;
}





