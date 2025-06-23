//Cout the block code running time
#include <chrono>
//Before starting record the start time
//auto start = std::chrono::high_resolution_clock::now();
//Run the encryption/decryption function 1000 times in a loop
//After the loop, record the end time
//auto end = std::chrono::high_resolution_clock::now();
//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//double averageTime = static_cast<double>(duration) / 1000.0; // average time per round in milliseconds
//Print the results std::cout << "Average time for encryption/decryption over 1000 rounds: " << averageTime << " ms" << std::endl;

// ... [other code]

switch (aescipher) {
    case 1: {
        // Key and IV generation logic here
        break;
    }
    case 2: {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < 1000; ++i) {
            // Encryption logic here
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        double averageTime = static_cast<double>(duration) / 1000.0;
        std::cout << "Average time for encryption over 1000 rounds: " << averageTime << " ms" << std::endl;
        break;
    }
    case 3: {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < 1000; ++i) {
            // Decryption logic here
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        double averageTime = static_cast<double>(duration) / 1000.0;
        std::cout << "Average time for decryption over 1000 rounds: " << averageTime << " ms" << std::endl;
        break;
    }
    default:
        cout << "Invalid input\n";
        break;
}

// ... [other code]
