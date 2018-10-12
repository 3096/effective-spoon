#include "SaveDataFactory.h"

const int SaveDataFactory::ENCODED_FILE_SIZE[] = {
    -1, 0x483E0, 0x86840, 0x88D90, 0x88D90
};
const int SaveDataFactory::BODY_SIZE[] = {
    0x483A0, 0x483A0, 0x86800, 0x88D50, 0x88D50
};

const uint32_t SaveDataFactory::CRYPT_TAB2[] = {
    0x476BC5E7, 0x76DF4A26, 0x2BD714E2, 0x632623A5,
    0xD36838ED, 0x7583042F, 0x32B3A6CE, 0x301180A2,
    0x63479874, 0xB0EEF148, 0x4144F37F, 0x717EDD13,
    0xA7C02D5F, 0xDF535C64, 0x1816AA04, 0xC3E911A8,
    0x07E6D14A, 0xFAE665ED, 0xE0A4AA9E, 0xB271F407,
    0xE57CCA4F, 0xD2CBFF86, 0x33AC5C1A, 0x10867E0A,
    0x42E43493, 0x8CCBC746, 0x1E82ECBC, 0xFDE23A9C,
    0x52A93E3D, 0x617A7898, 0x0753255C, 0xDFF1DEC9,
    0x310E0F37, 0x1C4BA740, 0x8E02AB06, 0xDDBE8580,
    0xC714785E, 0x0B24EB7B, 0xE6951F2F, 0x728EBF81,
    0xA10ABBBF, 0x47F5244F, 0xF96AD9DA, 0x8B8472CD,
    0xEE47B55B, 0xA1E97980, 0xF30B7FDA, 0xFD230EE1,
    0x7BF84A0E, 0x705A2AFC, 0x6685E6A1, 0x98AAB220,
    0x2B307047, 0x551804EB, 0x183A95BB, 0x4531F3E8,
    0xFDCB1756, 0xF0387032, 0x1F27AC7D, 0x5AD014E2,
    0x6508E3B3, 0xF13D7C92, 0xD7DA45D4, 0xA01D9485
};

const uint32_t SaveDataFactory::CRYPT_TAB3[] = {
    0xDFD0A132, 0x1537D7E5, 0xC8B0F6D5, 0x6C31FED3,
    0xB7A1221A, 0x9B9DC40C, 0x44315579, 0xF239E05A,
    0x87E4D9AF, 0x59EF5961, 0xF5AF2DC9, 0xD1521C02,
    0x68405262, 0x9864C589, 0x98F5F8BE, 0x0C90FE24,
    0x9B3FC02A, 0x31E4FD02, 0xEC747B2D, 0x5FC1C04E,
    0x80D6B732, 0x32BA6CB7, 0x961A5683, 0x33025098,
    0xD5676789, 0xAB622A5A, 0xF651F93A, 0x130D6D68,
    0xEFFEDA48, 0x24E2C2D3, 0xEA89C5DD, 0xF04AFA58,
    0x767D3A19, 0xBF67B888, 0x54218F00, 0x6F461AFF,
    0x9A216E37, 0x5861AAD0, 0x3B44CEAE, 0xCD6C9A42,
    0x0610ECD2, 0x2A894F76, 0x23D72BE9, 0x63FA2D93,
    0x2ADC8A10, 0xFB0E9F6A, 0x3BE0CE91, 0x80BB93A1,
    0xDB8D8FF3, 0x72E21DC7, 0x93B0C670, 0x2907C541,
    0x3DBF1C6D, 0x62D8924F, 0x3205E36F, 0x041C44D5,
    0xDACB2490, 0x01D905A9, 0x6C8C579B, 0xE3C54DC2,
    0xF4583808, 0x76459488, 0x1E5F7C61, 0x2876F360
};

const uint32_t SaveDataFactory::CRYPT_TAB4[]  = {
    0x0b11fbd2, 0x6d17658a, 0x12975ee9, 0xaaaa885d,
    0x2ee139b1, 0xf80c32d2, 0x50c45b2c, 0xaed2d909,
    0x89d951d3, 0x10da7898, 0x62b55c94, 0x5a97db5a,
    0x19834e12, 0xda5eeb02, 0x12c75d21, 0xf66e1928,
    0xf59e2a29, 0xd8b5c963, 0xf0b3a901, 0x76924966,
    0x7259eaff, 0x04d1cb06, 0x6947913c, 0x8d7a3c0b,
    0x30c93149, 0xbaca45ad, 0xef51edb9, 0xb3970e3c,
    0xcaad227e, 0x22a2f1ab, 0x42feec63, 0x4913ecee,
    0xea0e8960, 0xdf141223, 0x6a3c6056, 0xbc9e8563,
    0x22455e19, 0x5dbfd578, 0xd4083049, 0x9cf50236,
    0x94483227, 0x37ff21c3, 0xa2e0d50d, 0x963097d5,
    0x433c563e, 0x8d0396a3, 0x29ef72b3, 0x3b73c664,
    0x9a5fea92, 0x0b78c1f4, 0x595050c2, 0xfcb5fd6a,
    0x99db9314, 0x546196ba, 0x8f6c0ab4, 0xdc61e89e,
    0x99678335, 0xc1eb02e5, 0x2e3d89c6, 0xfdcbb187,
    0x5810fb20, 0xc17526ed, 0x03c4f4bc, 0xa3233aca
};

SaveDataFactory::SaveDataFactory(const std::string save_inPath) {
    std::ifstream saveFileIS(save_inPath.c_str(),
                             std::ios::binary | std::ios::ate);
    std::streamsize saveFileInSize = saveFileIS.tellg();
    saveFileIS.seekg(0, std::ios::beg);
    
    if (saveFileInSize < 0) {
        throw CouldNotOpenFile();
    }

    uint8_t* saveFileInBuffer = new uint8_t[saveFileInSize];
    try {
        saveFileIS.read((char*)saveFileInBuffer, saveFileInSize);

        m_version = *(int*)saveFileInBuffer;

        // check version
        if (m_version > LATEST_SUPPORT_VERS || m_version < 0) {
            throw UnsupportedSaveVersion();
        }
        switch (m_version) {
            case 1:
            case 2:
                m_cryptTab = CRYPT_TAB2;
                break;
            case 3:
                m_cryptTab = CRYPT_TAB3;
                break;
            case 4:
                m_cryptTab = CRYPT_TAB4;
                break;
            default:
                m_cryptTab = nullptr;
        }

        // check size to set m_initial_encodeState
        if (saveFileInSize == ENCODED_FILE_SIZE[m_version]) {
            m_initial_encodeState = ENCODED;
        } else if (saveFileInSize == HEADER_SIZE + BODY_SIZE[m_version]) {
            m_initial_encodeState = DECODED;
        } else {
            throw SaveSizeUnknown();
        }
    } catch (std::exception& e) {
        delete[] saveFileInBuffer;
        throw e;
    }

    // initialize save buffers
    try {
        if (m_initial_encodeState == ENCODED) {
            m_saveDataEncoded = saveFileInBuffer;
            m_saveData = new uint8_t[HEADER_SIZE + BODY_SIZE[m_version]];
            decode();
        } else {
            m_saveData = saveFileInBuffer;
            m_saveDataEncoded = new uint8_t[ENCODED_FILE_SIZE[m_version]];
        }
    } catch (std::exception& e) {
        this->~SaveDataFactory();
        throw e;
    }
}

SaveDataFactory::~SaveDataFactory() {
    delete[] m_saveData;
    delete[] m_saveDataEncoded;
}

SaveDataFactory::KeyPack SaveDataFactory::getKeys(
    const std::array<uint32_t, 4> key_seed) {
    SeedRand rnd(0, key_seed);
    KeyPack keys;
    for (int i = 0; i < 8; i++) {
        uint32_t k = 0;
        for (int j = 0; j < 4; j++) {
            k <<= 8;
            k |= (uint32_t)((m_cryptTab[rnd.getU32() >> 26] >>
                             ((rnd.getU32() >> 27) & 0x18)) &
                            0xFF);
        }
        ((uint32_t*)&keys)[i] = k;
    }

    return keys;
}

void SaveDataFactory::encode() {}
void SaveDataFactory::shuffle() {}
void SaveDataFactory::unshuffle() {}
void SaveDataFactory::getShufflePtrArray() {}

void SaveDataFactory::decode() {
    // copy header
    std::memcpy(m_saveData, m_saveDataEncoded, HEADER_SIZE);

    if (m_version == 4) {
        unshuffle();
    }

    const size_t enc_file_size = ENCODED_FILE_SIZE[m_version];
    const size_t body_size = BODY_SIZE[m_version];
    const uint8_t* encoded_body = &m_saveDataEncoded[HEADER_SIZE];
    uint8_t* decoded_body = &m_saveData[HEADER_SIZE];
    const SaveFooter* footer =
        (SaveFooter*)&m_saveDataEncoded[enc_file_size - FOOTER_SIZE];
    KeyPack keys = getKeys(footer->keySeed);
    std::array<uint8_t, 0x10> iv;
    iv = footer->iv;

    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);
    mbedtls_aes_setkey_dec(&ctx, keys.key0.data(), 128);
    mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_DECRYPT, body_size, iv.data(),
                          encoded_body, decoded_body);
    mbedtls_aes_free(&ctx);

    std::array<uint8_t, 0x10> calced_mac;
    mbedtls_cipher_info_t cipher_info =
        *mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
    mbedtls_cipher_cmac(&cipher_info, keys.key1.data(), 128, decoded_body,
                        body_size, calced_mac.data());

    if (footer->mac != calced_mac) {
        throw DecodeFailToVerify();
    }
}
