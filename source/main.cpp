#include <iostream>
#include "SaveDataFactory.h"

#ifdef __SWITCH__
#include <switch.h>
#endif

int splatsave(std::string path, std::string command) {
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

#ifndef __SWITCH__
        std::ifstream fileExistCheck(save_output_path);
        if (fileExistCheck.good()) {
            std::cout << "Overwrite existing " << save_output_path
                      << "? (y or n) ";
            std::string userInput;
            getline(std::cin, userInput);
            char firstLetter;
            if (userInput.length() < 1 or
                ((firstLetter = userInput.at(0)) != 'y' and
                 firstLetter != 'Y')) {
                std::cout << "Aborted." << std::endl;
                return 0;
            }
        }
#endif

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
        if (e.m_version == 6) {
            std::cout << "Save version 6 was never seen before. If your save "
                         "was previously unmodified, please submit an issue "
                         "on the project's GitHub page or send me an email at "
                         "contact@the3096.com! Thank you."
                      << std::endl;
        } else {
            std::cout << "Save size incorrect: 0x" << std::hex << e.m_size
                  << std::endl;
        }
        return -4;
    } catch (SaveDataFactory::UnsupportedSaveVersion& e) {
        std::cout << "Save version unsupported: " << e.m_version << std::endl;
        return -3;
    } catch (SaveDataFactory::DecodeFailToVerify& e) {
        std::cout << "Cannot verify save. If you believe this is a mistake, "
                     "and your save is intact, please submit an issue "
                     "on the project's GitHub page or send me an email at "
                     "contact@the3096.com! Thank you."
                  << std::endl;
        return -2;
    } catch (SaveDataFactory::CouldNotOpenFile& e) {
        std::cout << "Could not open " << path << std::endl;
        std::cout << "Usage: " << std::endl;
        std::cout << command
                  << " {encrypted save file path} {decrypted output path}"
                  << std::endl;
        std::cout << command
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
    consoleInit(NULL);
#endif

    const char* DEFAULT_PATH = "save.dat";
    const char** paths = argv;
    int paths_count;
    if (argc < 2) {
        paths = &DEFAULT_PATH;
        paths_count = 1;
    } else {
        paths = &argv[1];
        paths_count = argc - 1;
    }
    int ret;
    for (int i = 0; i < paths_count; i++) {
        ret = splatsave(paths[i], *argv);
        if (ret != 0) {
            break;
        }
    }

#ifdef __SWITCH__
    std::cout << "Press + to exit..." << std::endl;
    while (appletMainLoop()) {
        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
        if (kDown & KEY_PLUS) break;
        consoleUpdate(NULL);
    }
    consoleExit(NULL);
#endif

    return ret;
}
