#include <iostream>
#include "SaveDataFactory.h"

#ifdef __SWITCH__
#include <switch.h>
#endif

int splatsave(int argc, char const* argv[]) {
    std::string path;
    if (argc < 2)
        path = "save.dat";
    else
        path = argv[1];

    try {
        SaveDataFactory svFactory(path);

        const uint8_t* save_write = nullptr;
        size_t save_write_size = 0;
        std::string save_output_path;
        bool isDecode = false;
        if (svFactory.getInitialEncodeState() == SaveDataFactory::ENCODED) {
            isDecode = true;
            save_output_path = path + ".dec";
            save_write = svFactory.getSaveDecodedPtr();
            save_write_size = svFactory.getDecodedSaveFileSize();
        } else {
            save_output_path = path + ".enc";
            save_write = svFactory.getSaveEncodedPtr();
            save_write_size = svFactory.getEncodedSaveFileSize();
        }

        std::ofstream fileos(save_output_path, std::fstream::binary);
        if (!fileos) {
            std::cout << "Could not open " << save_output_path << std::endl;
            return -5;
        }
        fileos.write((char*)save_write, save_write_size);
        fileos.close();

        if (isDecode) {
            std::cout << "Decrypted " << path << " to " << save_output_path
                      << "!" << std::endl;
        } else {
            std::cout << "Encrypted " << path << " to " << save_output_path
                      << "!" << std::endl;
        }
    } catch (SaveDataFactory::SaveSizeUnknown& e) {
        std::cout << "Save size incorrect: 0x" << std::hex << e.m_size
                  << std::endl;
        return -4;
    } catch (SaveDataFactory::UnsupportedSaveVersion& e) {
        std::cout << "Save version unsupported: " << e.m_version << std::endl;
        return -3;
    } catch (SaveDataFactory::DecodeFailToVerify& e) {
        std::cout << "Cannot verify save." << std::endl;
        return -2;
    } catch (SaveDataFactory::CouldNotOpenFile& e) {
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

    return 0;
}

int main(int argc, char const* argv[]) {
#ifdef __SWITCH__
    gfxInitDefault();
    consoleInit(NULL);
#endif

    int ret = splatsave(argc, argv);

#ifdef __SWITCH__
    std::cout << "Press + to exit..." << std::endl;
    while (appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown & KEY_PLUS) break;
        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }
    gfxExit();
#endif

    return ret;
}