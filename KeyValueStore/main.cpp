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
void threadOne(KeyStoreTwo* keyStore) {
    for(int i = 0; i < 10; i++) {
       int valueOne = keyStore->getInt("keyOne");
        std::cout << "ThreadOne Got " << valueOne << " for keyOne\n";
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }
    
}

// Thread to write values, for testing
void threadTwo(KeyStoreTwo* keyStore) {
    for(int i = 0; i < 5; i++) {
        std::cout << "Writing " << i << " to keyStore\n";
        keyStore->put("keyOne", i);
        std::this_thread::sleep_for (std::chrono::seconds(2));
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    KeyStoreTwo keyStoreTwo;
    std::byte bytes[] = {(std::byte)0xDE, (std::byte)0xAD, (std::byte)0xBE, (std::byte)0xEF, (std::byte)0x00};
 
    // Validate the initial values in the KeyStore

    keyStoreTwo.put("intkey", 10);
    
    int intTwo = keyStoreTwo.getInt("intkey");
    
    std::cout << "Retrieved " << intTwo << "\n";
    
    keyStoreTwo.put("floatkey", 1.1f);
    
    float floatVAlue = keyStoreTwo.getFloat("floatkey");

    std::cout << "Retrieved float value " << floatVAlue <<  "\n";
    
    keyStoreTwo.put("doubleKey", 3.1415926);
    
    double doubleValue = keyStoreTwo.getDouble("doubleKey");
    
    std::cout << "Retrieved double value " << doubleValue << "\n";
    
    std::cout << "Size of array " << sizeof(bytes) << "\n";
    
    keyStoreTwo.put("byteArrayKey", bytes, sizeof(bytes));
    
    std::byte* resultBytes = keyStoreTwo.getBytes("byteArrayKey");
    
    std::cout << "Size of resulting array " << sizeof(resultBytes) << "\n";
    for(int i = 0; i < sizeof(resultBytes); i++) {
        printf("0x%02x ", (unsigned char) resultBytes[i]);
    }
    std::cout << "\n";
    
    
    // Create the thread to write values
    std::thread writeThread(threadTwo, &keyStoreTwo);

    // Create thread to read values
    std::thread readThread(threadOne, &keyStoreTwo);
   
    writeThread.join();
    readThread.join();
    return 0;
}





