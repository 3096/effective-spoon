#include <stdint.h>
#include <fstream>
#include <cstring>
#include <iostream>
#include <openssl/aes.h>
#include <openssl/cmac.h>
#include <openssl/rand.h>

const int FILE_SIZE[4] = {0x483B0, 0x483B0, 0x86840, 0x88D90};

const int BODY_SIZE[4] = {0x483A0, 0x483A0, 0x86800, 0x88D50};

const uint32_t crc32_tab[] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

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
    uint8_t buf[0x20];
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

int calcAesCmac(uint8_t* msg, size_t msg_len, uint8_t* key, uint8_t* mac) {
    size_t maclen;
    CMAC_CTX *ctx = CMAC_CTX_new();
    
    CMAC_Init(ctx, key, 16, EVP_aes_128_cbc(), NULL);
    CMAC_Update(ctx, msg, msg_len);
    CMAC_Final(ctx, mac, &maclen);
    CMAC_CTX_free(ctx);

    return 0;
}

uint32_t calcCRC32(uint8_t* msg, size_t msg_len) {
    uint32_t ret = 0xFFFFFFFF;
    for (size_t i = 0; i < msg_len; i++) {
        ret = crc32_tab[(ret ^ msg[i]) & 0xFF] ^ (ret >> 8);
    }
    return ret ^ 0xFFFFFFFF;
}

SaveFooter encryptBody(uint8_t* save_file, const uint32_t* crypt_tab, 
                        uint8_t* body_enc) {
    uint32_t vers = *(uint32_t*)save_file;
    uint8_t* body = &save_file[0x10];
    SaveFooter footer;
    RAND_bytes((uint8_t*)&footer, sizeof(footer.iv)+sizeof(footer.key_seed));

    uint8_t iv[0x10];
    std::memcpy(iv, footer.iv, sizeof(iv));
    KeyPack keys = getKeys(crypt_tab, footer.key_seed);
    AES_KEY key;
    AES_set_encrypt_key(keys.key0, 128, &key);    

    AES_cbc_encrypt(body, body_enc, BODY_SIZE[vers], &key, iv, AES_ENCRYPT);
    calcAesCmac(body, BODY_SIZE[vers], (uint8_t*)&keys.key1, footer.mac);

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
    AES_KEY key;
    AES_set_decrypt_key(keys.key0, 128, &key);

    AES_cbc_encrypt(body, body_dec, BODY_SIZE[vers], &key, footer.iv, AES_DECRYPT);

    uint8_t calced_mac[0x10];
    calcAesCmac(body_dec, BODY_SIZE[vers], (uint8_t*)&keys.key1, calced_mac);

    for(int i = 0; i < 0x10; i++) {
        if(footer.mac[i] != calced_mac[i]) {
            std::cout << "Warning: Stored MAC is invalid, save may have been "
                << "modified or corrupted and decryption may produce garbage."
                << std::endl;
            break;
        }
    }

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
        uint32_t crc = calcCRC32(&save_file[0x10], BODY_SIZE[vers]);
        if(crc != *(uint32_t*)&save_file[0x8]) {
            *(uint32_t*)&save_file[0x8] = crc;
            std::cout << "CRC fixed to " << std::hex << crc << std::endl;
        }
    } else {
        decryptBody(save_file, crypt_tab, new_body);
        uint32_t crc = *(uint32_t*)&save_file[0x8];
        if(crc != calcCRC32(new_body, BODY_SIZE[vers])) {
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
