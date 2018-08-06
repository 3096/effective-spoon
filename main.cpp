#include <stdint.h>
#include <fstream>
#include <cstring>
#include <iostream>
#include <mbedtls/aes.h>
#include <mbedtls/cipher.h>
#include <mbedtls/cmac.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <zlib.h>

const int FILE_SIZE[4] = {0x483B0, 0x483B0, 0x86840, 0x88D90};

const int BODY_SIZE[4] = {0x483A0, 0x483A0, 0x86800, 0x88D50};

const uint32_t CRYPT_TAB2[64] = {
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

const uint32_t CRYPT_TAB3[64] = {
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

typedef struct {
    uint8_t key0[0x10];
    uint8_t key1[0x10];
} KeyPack;

typedef struct {
    uint8_t iv[0x10];
    uint32_t key_seed[4];
    uint8_t mac[0x10];
} SaveFooter;

class SeedRand {
private:
    uint32_t mSeed;
    uint32_t mState[4];
public:
    SeedRand(uint32_t seed);
    SeedRand(uint32_t seed, const uint32_t* state);

    uint32_t getU32();
};

SeedRand::SeedRand(uint32_t seed = 0) {
    mSeed = seed;
    uint32_t priv_seed = seed;
    for(int i = 0; i < 4; i++) {
        mState[i] =
            (uint32_t)(0x6C078965 * (priv_seed ^ (priv_seed >> 30)) + i+1);
        priv_seed = mState[i];
    }
}

SeedRand::SeedRand(uint32_t seed, const uint32_t* state) {
    mSeed = seed;
    std::memcpy(mState, state, sizeof(mState));
}

uint32_t SeedRand::getU32() {
    uint32_t a = mState[0] ^ (mState[0] << 11);
    uint32_t b = mState[3];
    uint32_t c = a ^ (a >> 8) ^ b ^ (b >> 19);
    mState[0] = mState[1];
    mState[1] = mState[2];
    mState[2] = mState[3];
    mState[3] = c;
    return c;
}

KeyPack getKeys(const uint32_t* crypt_tab, const uint32_t* key_seed) {
    SeedRand rnd(0, key_seed);
    KeyPack ret;
    for(int i = 0; i < 8; i++) {
        uint32_t k = 0;
        for(int j = 0; j < 4; j++) {
            k <<= 8;
            k |= (uint32_t)((crypt_tab[rnd.getU32() >> 26]
                >> ((rnd.getU32() >> 27) & 0x18)) & 0xFF);
        }
        ((uint32_t*)&ret)[i] = k;
    }

    return ret;
}

int calcAes128Cmac(uint8_t* msg, size_t msg_len, uint8_t* key, uint8_t* mac) {
    mbedtls_cipher_info_t cipher_info = 
        *mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_ECB);
    return mbedtls_cipher_cmac(&cipher_info, key, 128, msg, msg_len, mac);
}

int rand_bytes(uint8_t* output, size_t output_len) {
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_context entropy;
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    std::string pers = "Flow best waifu";

    int ret;
    if((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
        (unsigned char*)pers.c_str(), pers.size())) != 0) {
        return ret;
    }
    if((ret = mbedtls_ctr_drbg_random(&ctr_drbg, output, output_len)) != 0) {
        return ret;
    }
}

SaveFooter encryptBody(uint8_t* save_file, const uint32_t* crypt_tab, 
                        uint8_t* body_enc) {
    uint32_t vers = *(uint32_t*)save_file;
    uint8_t* body = &save_file[0x10];
    SaveFooter footer;
    rand_bytes((uint8_t*)&footer, sizeof(footer.iv)+sizeof(footer.key_seed));

    uint8_t iv[0x10];
    std::memcpy(iv, footer.iv, sizeof(iv));
    KeyPack keys = getKeys(crypt_tab, footer.key_seed);

    mbedtls_aes_context ctx; mbedtls_aes_init(&ctx);
    mbedtls_aes_setkey_enc(&ctx, keys.key0, 128);
   
    mbedtls_aes_crypt_cbc(
        &ctx, MBEDTLS_AES_ENCRYPT, BODY_SIZE[vers], iv, body, body_enc);
    calcAes128Cmac(body, BODY_SIZE[vers], (uint8_t*)&keys.key1, footer.mac);

    mbedtls_aes_free(&ctx);
    return footer;
}

int decryptBody(uint8_t* save_file, const uint32_t* crypt_tab, 
                uint8_t* body_dec) {
    uint32_t vers = *(uint32_t*)save_file;
    size_t size = FILE_SIZE[vers];
    uint8_t* body = &save_file[0x10];
    SaveFooter footer;
    memcpy(&footer, &save_file[size-0x30], sizeof(footer));
    
    KeyPack keys = getKeys(crypt_tab, footer.key_seed);
    
    mbedtls_aes_context ctx; mbedtls_aes_init(&ctx);
    mbedtls_aes_setkey_dec(&ctx, keys.key0, 128);

    mbedtls_aes_crypt_cbc(
        &ctx, MBEDTLS_AES_DECRYPT, BODY_SIZE[vers], footer.iv, body, body_dec);

    uint8_t calced_mac[0x10];
    calcAes128Cmac(body_dec, BODY_SIZE[vers], (uint8_t*)&keys.key1, calced_mac);

    for(int i = 0; i < 0x10; i++) {
        if(footer.mac[i] != calced_mac[i]) {
            std::cout << "Warning: Stored MAC is invalid, save may have been "
                << "modified or corrupted and decryption may produce garbage."
                << std::endl;
            break;
        }
    }

    mbedtls_aes_free(&ctx);
    return 0;
}

int main(int argc, char const *argv[]) {
    std::string path;
    if (argc < 2) 
        path = "save.dat";
    else
        path = argv[1];

    std::ifstream fileis(path, std::ios::binary | std::ios::ate);
    std::streamsize size = fileis.tellg();
    fileis.seekg(0, std::ios::beg);

    uint8_t save_file[size];
    if (!fileis.read((char*)save_file, size)) {

        std::cout << "Could not open " << path << std::endl;
        std::cout << "Usage: " << std::endl;
        std::cout << *argv 
            << " {encrypted save file path} {decrypted output path}"
            << std::endl;
        std::cout << *argv 
            << " {decrypted save file path} {re-encrypted output path}"
            << std::endl;
        std::cout << "Decrypt or re-encrypt mode is determined by file size."
            << std::endl;
        return -1;
    }

    uint32_t vers = *(uint32_t*)save_file;
    
    const uint32_t* crypt_tab;
    bool isEncrypt = false;
    switch (vers) {
    case 2:
        if(size == FILE_SIZE[2]-0x30) {
            isEncrypt = true;
        } else if(size != FILE_SIZE[2]) {
            std::cout << "Version 2 file size incorrect." << std::endl;
            return -3;
        }
        crypt_tab = CRYPT_TAB2;
        break;
    case 3:
        if(size == FILE_SIZE[3]-0x30) {
            isEncrypt = true;
        } else if(size != FILE_SIZE[3]) {
            std::cout << "Version 3 file size incorrect." << std::endl;
            return -3;
        }
        crypt_tab = CRYPT_TAB3;
        break;
    default:
        std::cout << "Only save version 2 or 3 supported." << std::endl;
        return -3;
    }

    std::string out_path;
    if(argc < 3)
        if(isEncrypt)
            out_path = path + ".enc";
        else
            out_path = path + ".dec";
    else
        out_path = argv[2];

    uint8_t new_body[BODY_SIZE[vers]];
    SaveFooter footer;

    if(isEncrypt) {
        footer = encryptBody(save_file, crypt_tab, new_body);
        uint32_t crc = crc32(0L, &save_file[0x10], BODY_SIZE[vers]);
        if(crc != *(uint32_t*)&save_file[0x8]) {
            *(uint32_t*)&save_file[0x8] = crc;
            std::cout << "CRC fixed to " << std::hex << crc << std::endl;
        }
    } else {
        decryptBody(save_file, crypt_tab, new_body);
        uint32_t crc = *(uint32_t*)&save_file[0x8];
        if(crc != crc32(0L, new_body, BODY_SIZE[vers])) {
            std::cout<< "Warning: Stored CRC is invalid. "
                <<"(Can be auto fixed when re-encrypting)" << std::endl;
        }
    }

    std::ofstream fileos(out_path, std::fstream::binary);
    if(!fileos) {
        std::cout << "Could not open " << out_path << std::endl;
        return -2;
    }
    fileos.write((char*)save_file, 0x10);
    fileos.write((char*)new_body, BODY_SIZE[vers]);

    if(isEncrypt) {
        fileos.write((char*)&footer, sizeof(footer));
        std::cout << "Encrypted " << path << " to " << out_path << "!" 
            << std::endl;
    } else {
        std::cout << "Decrypted " << path << " to " << out_path << "!" 
            << std::endl;
    } 
    return 0;
}
