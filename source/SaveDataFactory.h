#pragma once

#include <mbedtls/aes.h>
#include <mbedtls/cipher.h>
#include <mbedtls/cmac.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <zlib.h>
#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

#ifdef __SWITCH__
#include <switch.h>
#endif

#include "SeedRand.h"

class SaveDataFactory {
   public:
    enum EncodeState { ENCODED, DECODED };

   private:
    static const int ENCODED_FILE_SIZE[];
    static const int BODY_SIZE[];

    static const uint8_t HEADER_SIZE = 0x10;
    static const uint8_t FOOTER_SIZE = 0x30;
    
    typedef struct {
        std::array<uint8_t, 0x10> iv;
        std::array<uint32_t, 4> keySeed;
        std::array<uint8_t, 0x10> mac;
    } SaveFooter;
    
    uint8_t* m_saveData;
    uint8_t* m_saveData_encoded;
    uint8_t* m_saveBody;
    uint8_t* m_saveBody_encoded;
    SaveFooter* m_saveFooter;
    size_t m_bodySize;
    size_t m_fileSize_encoded;

    // save content
    int m_version;
    static const int LATEST_SUPPORT_VERS = 7;

    // save crypto
    static const uint32_t CRYPT_TAB1[];
    static const uint32_t CRYPT_TAB3[];
    static const uint32_t CRYPT_TAB4[];
    static const uint32_t CRYPT_TAB7[];
    const uint32_t* m_cryptTab;

    std::array<uint8_t, 0x10> getKey(SeedRand& RNG);

    void encode();
    void decode();
    int randBytes(uint8_t* output, const size_t output_len);
    void updateCRC();

    // save shuffle
    typedef struct {
        size_t block_size;
        size_t unshuffled_offset;
        size_t shuffled_offset;
    } ShuffleBlock;

    void unshuffle(bool onEncoded = true);
    void shuffle(bool onEncoded = true);
    std::vector<ShuffleBlock> getShuffleBlocks(size_t total_size,
                                               uint32_t seed);

    EncodeState m_initial_encodeState;

   public:
    SaveDataFactory(const std::string save_inPath);
    ~SaveDataFactory();

    const uint8_t* getSaveDecodedPtr();
    const uint8_t* getSaveEncodedPtr();
    size_t getDecodedSaveFileSize();
    size_t getEncodedSaveFileSize();

    int getSaveVersion();
    EncodeState getInitialEncodeState();

    class SaveSizeUnknown : public std::exception {
    public:
        size_t m_size;
        int m_version;
        SaveSizeUnknown(const size_t size, const int version)
            : m_size(size), m_version(version) {}
    };
    class DecodeFailToVerify : public std::exception {};
    class CouldNotOpenFile : public std::exception {};
    class UnsupportedSaveVersion : public std::exception {
       public:
        int m_version;
        UnsupportedSaveVersion(const int version) : m_version(version) {}
    };
};
