#import <Foundation/Foundation.h>
#import <iostream>

#import "engine.hpp"

int main(int argc, char** argv) {
    @autoreleasepool {
        Engine engine;
        if (!engine.startup()) {
            std::cerr << "Failed to initialize engine" << std::endl;
            return -1;
        }

        engine.run();

        engine.shutdown();
    }
    return 0;
}