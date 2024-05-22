#ifndef AMX_INTRO_UTILS_H
#define AMX_INTRO_UTILS_H

#include <random>

namespace util {
    class RandomByte {
    public:
        RandomByte() : engine(std::random_device{}()), dist(0, 255) {}

        uint8_t Value() {
            return static_cast<uint8_t>(dist(engine));
        }

    private:
        std::mt19937 engine;
        std::uniform_int_distribution<uint16_t> dist;
    };

    inline uint8_t GetRandomByte() {
        static RandomByte random{};
        return random.Value();
    }
}

#endif //AMX_INTRO_UTILS_H
