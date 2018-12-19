#include "SaveDataFactory.h"

const int SaveDataFactory::ENCODED_FILE_SIZE[] = {
    0x483B0, 0x483E0, 0x86840, 0x88D90, 0x88D90, 0x88D90, -1, 0x88D90
};
const int SaveDataFactory::BODY_SIZE[] = {
    0x483A0, 0x483A0, 0x86800, 0x88D50, 0x88D50, 0x88D50, -1, 0x88D50
};

const uint32_t SaveDataFactory::CRYPT_TAB1[] = {
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
    0x0B11FBD2, 0x6D17658A, 0x12975EE9, 0xAAAA885D,
    0x2EE139B1, 0xF80C32D2, 0x50C45B2C, 0xAED2D909,
    0x89D951D3, 0x10DA7898, 0x62B55C94, 0x5A97DB5A,
    0x19834E12, 0xDA5EEB02, 0x12C75D21, 0xF66E1928,
    0xF59E2A29, 0xD8B5C963, 0xF0B3A901, 0x76924966,
    0x7259EAFF, 0x04D1CB06, 0x6947913C, 0x8D7A3C0B,
    0x30C93149, 0xBACA45AD, 0xEF51EDB9, 0xB3970E3C,
    0xCAAD227E, 0x22A2F1AB, 0x42FEEC63, 0x4913ECEE,
    0xEA0E8960, 0xDF141223, 0x6A3C6056, 0xBC9E8563,
    0x22455E19, 0x5DBFD578, 0xD4083049, 0x9CF50236,
    0x94483227, 0x37FF21C3, 0xA2E0D50D, 0x963097D5,
    0x433C563E, 0x8D0396A3, 0x29EF72B3, 0x3B73C664,
    0x9A5FEA92, 0x0B78C1F4, 0x595050C2, 0xFCB5FD6A,
    0x99DB9314, 0x546196BA, 0x8F6C0AB4, 0xDC61E89E,
    0x99678335, 0xC1EB02E5, 0x2E3D89C6, 0xFDCBB187,
    0x5810FB20, 0xC17526ED, 0x03C4F4BC, 0xA3233ACA
};

const uint32_t SaveDataFactory::CRYPT_TAB7[]  = {
    0x266715A6, 0x89D12790, 0x6A2FB118, 0x781CE44A,
    0x66C804CA, 0x66254224, 0x71959889, 0xEE341C45,
    0xC8FCBB1D, 0x8484F444, 0x5908658D, 0x17908CDD,
    0x3A99F961, 0x991C0CA2, 0x83223601, 0x1047B4DC,
    0xE5E01544, 0x9CE06C46, 0x0EE0D3E5, 0x232C67B1,
    0xC6831CA5, 0x599EDA6F, 0x51E95542, 0x11B8A121,
    0xCF00F187, 0x60E207AA, 0x7BBA0DB1, 0xAFDF1A4E,
    0x679B4AFB, 0x1734CEF5, 0xBC489E27, 0xEB12EE8A,
    0x5663AD81, 0xE79182DE, 0x1FD081AD, 0x63CA5A63,
    0x288EFC3E, 0x43620C83, 0xD82430AB, 0xE90C708E,
    0xB63C0E9D, 0xE5690ACD, 0x1C31DFA3, 0x963354AB,
    0xC02DE816, 0x6DBFADBC, 0xFFE1AB6F, 0xF37B2E34,
    0x5CB90515, 0xCCFB9173, 0x3EB3E150, 0x1493623C,
    0x80965C16, 0x90F6357E, 0x31EEFF51, 0xBEE3F9D2,
    0x8CA774F4, 0xADDBFDDB, 0xEB898B46, 0x4A05D28C,
    0xFDB20FEC, 0x8FF500A4, 0x288179A7, 0x4C745F89
};

SaveDataFactory::SaveDataFactory(const std::string save_inPath) {
    std::ifstream saveFileIS(save_inPath.c_str(),
                             std::ios::binary | std::ios::ate);
    std::streamsize saveFileInSize = saveFileIS.tellg();
    saveFileIS.seekg(0, std::ios::beg);

    if (saveFileInSize < 0) {
        saveFileIS.close();
        throw CouldNotOpenFile();
    }

    uint8_t* saveFileInBuffer = new uint8_t[saveFileInSize];
    try {
        saveFileIS.read((char*)saveFileInBuffer, saveFileInSize);
        saveFileIS.close();

        m_version = *(int*)saveFileInBuffer;

        // check version
        if (m_version > LATEST_SUPPORT_VERS || m_version < 0) {
            throw UnsupportedSaveVersion(m_version);
        }

        // check size to set m_initial_encodeState and sizes
        if (saveFileInSize == HEADER_SIZE + BODY_SIZE[m_version]) {
            m_initial_encodeState = DECODED;
        } else if (saveFileInSize == ENCODED_FILE_SIZE[m_version]) {
            m_initial_encodeState = ENCODED;
        } else {
            throw SaveSizeUnknown(saveFileInSize, m_version);
        }

        // select crypto look up table and save sizes
        switch (m_version) {
            case 1:
            case 2:
                m_cryptTab = CRYPT_TAB1;
                break;
            case 3:
                m_cryptTab = CRYPT_TAB3;
                break;
            case 4:
            case 5:
                m_cryptTab = CRYPT_TAB4;
                break;
            case 7:
                m_cryptTab = CRYPT_TAB7;
                break;
            case 6:
            default:
                throw UnsupportedSaveVersion(m_version);
        }

        m_fileSize_encoded = ENCODED_FILE_SIZE[m_version];
        m_bodySize = BODY_SIZE[m_version];

    } catch (UnsupportedSaveVersion& e) {
        delete[] saveFileInBuffer;
        throw e;
    } catch (SaveSizeUnknown& e) {
        delete[] saveFileInBuffer;
        throw e;
    }

    // initialize save buffers and pointers
    if (m_initial_encodeState == ENCODED) {
        m_saveData = new uint8_t[HEADER_SIZE + BODY_SIZE[m_version]];
        m_saveData_encoded = saveFileInBuffer;
    } else {
        m_saveData = saveFileInBuffer;
        m_saveData_encoded = new uint8_t[ENCODED_FILE_SIZE[m_version]];
    }
    m_saveBody = &m_saveData[HEADER_SIZE];
    m_saveBody_encoded = &m_saveData_encoded[HEADER_SIZE];
    if (m_version != 0) {
        m_saveFooter =
            (SaveFooter*)&m_saveData_encoded[m_fileSize_encoded - FOOTER_SIZE];
    } else {
        m_saveFooter = nullptr;
    }

    try {
        if (m_initial_encodeState == ENCODED) {
            decode();
        }
    } catch (DecodeFailToVerify& e) {
        this->~SaveDataFactory();
        throw e;
    }

    // in case crc is not fixed
    updateCRC();
}

SaveDataFactory::~SaveDataFactory() {
    delete[] m_saveData;
    delete[] m_saveData_encoded;
}

std::array<uint8_t, 0x10> SaveDataFactory::getKey(SeedRand& RNG) {
    std::array<uint8_t, 0x10> key;
    for (int i = 0; i < 4; i++) {
        uint32_t k = 0;
        for (int j = 0; j < 4; j++) {
            k <<= 8;
            k |= (uint32_t)((m_cryptTab[RNG.getU32() >> 26] >>
                             ((RNG.getU32() >> 27) & 0x18)) &
                            0xFF);
        }
        ((uint32_t*)&key)[i] = k;
    }

    return key;
}

void SaveDataFactory::encode() {
    // ver 0 save is plaintext
    if (m_version == 0) {
        std::memcpy(m_saveData_encoded, m_saveData, m_fileSize_encoded);
        return;
    }

    std::memcpy(m_saveData_encoded, m_saveData, HEADER_SIZE);

    // gen random iv and key seed
    randBytes((uint8_t*)m_saveFooter,
              sizeof(m_saveFooter->iv) + sizeof(m_saveFooter->keySeed));

    std::array<uint8_t, 0x10> iv;
    iv = m_saveFooter->iv;
    SeedRand keyRNG(m_saveFooter->keySeed);

    uint8_t* tmpOriginalBody = nullptr;
    if (m_version == 7) {
        tmpOriginalBody = new uint8_t[m_bodySize];
        std::memcpy(tmpOriginalBody, m_saveBody, m_bodySize);
        shuffle(false);
    }

    std::array<uint8_t, 0x10> encKey = getKey(keyRNG);
    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);
    mbedtls_aes_setkey_enc(&ctx, encKey.data(), 128);
    mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, m_bodySize, iv.data(),
                          m_saveBody, m_saveBody_encoded);
    mbedtls_aes_free(&ctx);

    if (m_version == 7) {
        std::memcpy(m_saveBody, tmpOriginalBody, m_bodySize);
        delete[] tmpOriginalBody;
    }

    if (m_version == 4 or m_version == 7) {
        shuffle();
    }

    std::array<uint8_t, 0x10> cmacKey = getKey(keyRNG);
    mbedtls_cipher_info_t cipher_info =
        *mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
    mbedtls_cipher_cmac(&cipher_info, cmacKey.data(), 128, m_saveBody,
                        m_bodySize, m_saveFooter->mac.data());
}

void SaveDataFactory::decode() {
    // copy header
    std::memcpy(m_saveData, m_saveData_encoded, HEADER_SIZE);

    if (m_version == 4 or m_version == 7) {
        unshuffle();
    }
    std::array<uint8_t, 0x10> iv;
    iv = m_saveFooter->iv;
    SeedRand keyRNG(m_saveFooter->keySeed);

    std::array<uint8_t, 0x10> encKey = getKey(keyRNG);
    mbedtls_aes_context ctx;
    mbedtls_aes_init(&ctx);
    mbedtls_aes_setkey_dec(&ctx, encKey.data(), 128);
    mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_DECRYPT, m_bodySize, iv.data(),
                          m_saveBody_encoded, m_saveBody);
    mbedtls_aes_free(&ctx);

    if (m_version == 7) {
        unshuffle(false);
    }

    std::array<uint8_t, 0x10> calced_mac;
    std::array<uint8_t, 0x10> cmacKey = getKey(keyRNG);
    mbedtls_cipher_info_t cipher_info =
        *mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
    mbedtls_cipher_cmac(&cipher_info, cmacKey.data(), 128, m_saveBody,
                        m_bodySize, calced_mac.data());

    if (m_saveFooter->mac != calced_mac) {
        throw DecodeFailToVerify();
    }
}

void SaveDataFactory::updateCRC() {
    *(uint32_t*)&m_saveData[0x8] =
        crc32(0L, &m_saveData[HEADER_SIZE], m_bodySize);
}

void SaveDataFactory::shuffle(bool onEncoded) {
    uint8_t* tmpBodyUnshuffled = new uint8_t[m_bodySize];
    uint8_t* bodyToShuffle = onEncoded ? m_saveBody_encoded : m_saveBody;
    uint32_t* crc = onEncoded ? (uint32_t*)&m_saveData_encoded[0x8]
                              : (uint32_t*)&m_saveData[0x8];

    std::memcpy(tmpBodyUnshuffled, bodyToShuffle, m_bodySize);

    std::vector<ShuffleBlock> shuffleBlocks =
        getShuffleBlocks(m_bodySize, *crc);

    for (size_t i = 0; i < shuffleBlocks.size(); i++) {
        std::memcpy(&bodyToShuffle[shuffleBlocks[i].shuffled_offset],
                    &tmpBodyUnshuffled[shuffleBlocks[i].unshuffled_offset],
                    shuffleBlocks[i].block_size);
    }

    delete[] tmpBodyUnshuffled;
}

void SaveDataFactory::unshuffle(bool onEncoded) {
    uint8_t* tmpBodyShuffled = new uint8_t[m_bodySize];
    uint8_t* bodyToUnshuffle = onEncoded ? m_saveBody_encoded : m_saveBody;
    uint32_t* crc = onEncoded ? (uint32_t*)&m_saveData_encoded[0x8]
                              : (uint32_t*)&m_saveData[0x8];

    std::memcpy(tmpBodyShuffled, bodyToUnshuffle, m_bodySize);

    std::vector<ShuffleBlock> shuffleBlocks =
        getShuffleBlocks(m_bodySize, *crc);

    for (size_t i = 0; i < shuffleBlocks.size(); i++) {
        std::memcpy(&bodyToUnshuffle[shuffleBlocks[i].unshuffled_offset],
                    &tmpBodyShuffled[shuffleBlocks[i].shuffled_offset],
                    shuffleBlocks[i].block_size);
    }

    delete[] tmpBodyShuffled;
}

std::vector<SaveDataFactory::ShuffleBlock> SaveDataFactory::getShuffleBlocks(
    size_t total_size, uint32_t seed) {
    SeedRand RNG(seed);
    size_t min_block_size = total_size / 0x10;
    size_t max_block_size = min_block_size * 2;

    std::vector<size_t> block_indices;
    std::vector<size_t> block_sizes;
    std::vector<size_t> unshuffled_offsets;
    size_t cur_offset = 0;
    size_t cur_block_idx = 0;
    while (total_size - cur_offset > min_block_size) {
        // annoyed squid sound
        if (max_block_size > total_size - cur_offset) {
            max_block_size = total_size - cur_offset;
        }

        uint64_t rand_num = RNG.getU32();
        size_t block_size =
            (((max_block_size - min_block_size + 1)) * rand_num >> 0x20) +
            min_block_size;

        block_indices.push_back(cur_block_idx);
        block_sizes.push_back(block_size);
        unshuffled_offsets.push_back(cur_offset);

        cur_offset += block_size;
        cur_block_idx++;
    }
    // add last block if needed?
    if (total_size > cur_offset) {
        block_indices.push_back(cur_block_idx);
        block_sizes.push_back(total_size - cur_offset);
        unshuffled_offsets.push_back(cur_offset);
    }

    uint64_t remaining_blocks_to_shuffle = block_indices.size();
    while (remaining_blocks_to_shuffle > 1) {
        size_t switch_to_idx = remaining_blocks_to_shuffle - 1;
        size_t switch_from_idx =
            remaining_blocks_to_shuffle * RNG.getU32() >> 0x20;
        size_t to_back = block_indices[switch_from_idx];
        block_indices[switch_from_idx] = block_indices[switch_to_idx];
        block_indices[switch_to_idx] = to_back;
        remaining_blocks_to_shuffle--;
    }

    std::vector<ShuffleBlock> shuffle_blocks;
    size_t cur_shuffled_offset = 0;
    for (size_t i = 0; i < block_indices.size(); i++) {
        shuffle_blocks.push_back({block_sizes[block_indices[i]],
                                  unshuffled_offsets[block_indices[i]],
                                  cur_shuffled_offset});
        cur_shuffled_offset += block_sizes[block_indices[i]];
    }
    return shuffle_blocks;
}

// used in save encoding
int SaveDataFactory::randBytes(uint8_t* output, const size_t output_len) {
    int ret;
#ifndef __SWITCH__
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    std::string pers = "Flow best waifu";

    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                                     (unsigned char*)pers.c_str(),
                                     pers.size())) != 0) {
        return ret;
    }
    ret = mbedtls_ctr_drbg_random(&ctr_drbg, output, output_len);
#else
    if ((ret = csrngInitialize()) != 0) {
        return ret;
    }
    ret = csrngGetRandomBytes(output, output_len);
    csrngExit();
#endif
    return ret;
}

const uint8_t* SaveDataFactory::getSaveDecodedPtr() { return m_saveData; }

const uint8_t* SaveDataFactory::getSaveEncodedPtr() {
    encode();
    return m_saveData_encoded;
}

size_t SaveDataFactory::getDecodedSaveFileSize() {
    return HEADER_SIZE + m_bodySize;
}
size_t SaveDataFactory::getEncodedSaveFileSize() { return m_fileSize_encoded; }

int SaveDataFactory::getSaveVersion() { return m_version; }

SaveDataFactory::EncodeState SaveDataFactory::getInitialEncodeState() {
    return m_initial_encodeState;
}
