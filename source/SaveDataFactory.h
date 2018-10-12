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

#include "SeedRand.h"

class SaveDataFactory {
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

    static const uint32_t CRYPT_TAB2[];
    static const uint32_t CRYPT_TAB3[];
    static const uint32_t CRYPT_TAB4[];

    const uint32_t* m_cryptTab;

    typedef struct {
        std::array<uint8_t, 0x10> key0;
        std::array<uint8_t, 0x10> key1;
    } KeyPack;

    KeyPack getKeys(const std::array<uint32_t, 4> key_seed);

    int m_version;
    static const int LATEST_SUPPORT_VERS = 4;

   public:
    enum EncodeState { ENCODED, DECODED };
    EncodeState m_initial_encodeState;

    uint8_t* m_saveData;
    uint8_t* m_saveDataEncoded;

    SaveDataFactory(const std::string save_inPath);
    ~SaveDataFactory();

    uint8_t* getSavePtr();
    uint8_t* getSaveEncodedPtr();

    void encode();
    void decode();
    void unshuffle();
    void shuffle();
    void getShufflePtrArray();

    class SaveSizeUnknown : public std::exception {};
    class UnsupportedSaveVersion : public std::exception {};
    class DecodeFailToVerify : public std::exception {};
    class CouldNotOpenFile : public std::exception {};
};
