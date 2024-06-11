/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
 * Copyright (c) 2024 Dennis P Paul.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>
#include <string>
#include <string.h>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class SDCard {
public:
    static const uint8_t READ          = 0x01; // opens the file for reading. If this flag is set, you can read data from the file.
    static const uint8_t WRITE         = 0x02; // opens the file for writing. If this flag is set, you can write data to the file.
    static const uint8_t OPEN_EXISTING = 0x00; // opens an existing file. If the file does not exist, the function will fail.
    static const uint8_t CREATE_NEW    = 0x04; // creates a new file. If the file already exists, the function will fail.
    static const uint8_t CREATE_ALWAYS = 0x08; // creates a new file. If the file already exists, it will be truncated to zero length (all previous data will be lost).
    static const uint8_t OPEN_ALWAYS   = 0x10; // opens a file if it exists. If the file does not exist, it will create a new file.
    static const uint8_t OPEN_APPEND   = 0x30; // opens a file for appending. If the file exists, the file write pointer will be set to the end of the file. If the file does not exist, it will create a new file. It is a combination of FA_WRITE and setting the file pointer to the end of the file.

    SDCard() {}
    ~SDCard() {}
    bool     init();
    bool     deinit();
    bool     reinit();
    bool     detected();
    bool     status();
    bool     mount(bool immediately=true);
    bool     unmount();
    bool     format();
    bool     list(std::string path, std::vector<std::string>& files, std::vector<std::string>& directories);
    bool     open_file(std::string filepath, uint8_t flags);
    uint32_t write(uint8_t* bytes, uint32_t bytes_to_write);
    uint32_t read(uint8_t* bytes, uint32_t bytes_to_read);
    bool     close_file();
};

#endif // __cplusplus
