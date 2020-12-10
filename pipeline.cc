// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// A simple example to print out "Hello World!" from a MediaPipe graph.

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <ctime>

/* return true if val is set, false for EOF */
bool read_uint32(int read_fd, uint32_t &val)
{
    unsigned char msgSizeBuf[4];
    unsigned iBuf = 0;

    while (iBuf < sizeof(msgSizeBuf))
    {
        ssize_t rc = ::read(read_fd, msgSizeBuf + iBuf, sizeof(msgSizeBuf) - iBuf);

        if (rc == 0)
        {
            return false;
        }
        else if (rc < 0)
        {
            std::cout << __func__ << "@" << __LINE__ << ":::Read ERROR" << std::endl;
            exit(1);
        }
        else
        {
            iBuf += rc;
        }
    }

    val = *(static_cast<uint32_t *>(static_cast<void *>(&msgSizeBuf[0])));

    return true;
}

void send_msg(int write_fd, std::string msg)
{
    uint32_t msgSize = msg.size();
    unsigned char msgSizeBuf[4];

    ::memcpy(msgSizeBuf, &msgSize, sizeof(msgSize));

    unsigned iBuf = 0;
    while (iBuf < 4)
    {
        ssize_t rc = ::write(write_fd, msgSizeBuf + iBuf, sizeof(msgSizeBuf) - iBuf);
        if (rc < 0)
        {
            std::cout << "Error writing message size" << std::endl;
            ::exit(1);
        }
        else if (rc == 0)
        {
            std::cout << "rc == 0, what does that mean?" << std::endl;
            ::exit(1);
        }
        else
        {
            iBuf += rc;
        }
    }

    iBuf = 0;
    const char *msgBuf = msg.c_str();
    while (iBuf < msgSize)
    {
        ssize_t rc = ::write(write_fd, msgBuf + iBuf, msgSize - iBuf);
        if (rc < 0)
        {
            std::cout << "Error writing message" << std::endl;
            ::exit(1);
        }
        else if (rc == 0)
        {
            std::cout << "rc == 0, what does that mean?" << std::endl;
            ::exit(1);
        }
        else
        {
            iBuf += rc;
        }
    }
}

void send_msg(int write_fd, int datalen, unsigned char *buf)
{
    unsigned char msgSizeBuf[4];

    ::memcpy(msgSizeBuf, &datalen, sizeof(uint32_t(datalen)));

    // std::cout << "start sending : " << datalen << std::endl;

    unsigned iBuf = 0;
    while (iBuf < 4)
    {
        ssize_t rc = ::write(write_fd, msgSizeBuf + iBuf, sizeof(msgSizeBuf) - iBuf);
        if (rc < 0)
        {
            std::cout << "Error writing message size : " << errno << std::endl;
            ::exit(1);
        }
        else if (rc == 0)
        {
            std::cout << "rc == 0, what does that mean?" << std::endl;
            ::exit(1);
        }
        else
        {
            iBuf += rc;
        }
    }

    iBuf = 0;
    while (iBuf < datalen)
    {
        try
        {
            ssize_t rc = ::write(write_fd, buf + iBuf, datalen - iBuf);

            if (rc < 0)
            {
                std::cout << "Error writing message" << std::endl;
                ::exit(1);
            }
            else if (rc == 0)
            {
                std::cout << "rc == 0, what does that mean?" << std::endl;
                ::exit(1);
            }
            else
            {
                iBuf += rc;
            }
        }
        catch (const std::exception &ex)
        {
            std::cout << "exception : " << ex.what() << std::endl;
        }
        catch (...)
        {
            std::cout << "unknown exception " << std::endl;
        }
    }

    // std::cout << "end sending : " << datalen << std::endl;
}

std::string read_string(int read_fd, uint32_t sz)
{
    std::vector<char> msgBuf(sz + 1);
    msgBuf[sz] = '\0';
    unsigned iBuf = 0;

    while (iBuf < sz)
    {
        ssize_t rc = ::read(read_fd, &(msgBuf[0]) + iBuf, sz - iBuf);

        if (rc == 0)
        {
            std::cout << __func__ << "@" << __LINE__ << ":::EOF read" << std::endl;
            exit(1);
        }
        else if (rc < 0)
        {
            std::cout << __func__ << "@" << __LINE__ << ":::Read ERROR during message" << std::endl;
            exit(1);
        }
        else
        {
            iBuf += rc;
        }
    }

    return std::string(&(msgBuf[0]));
}

void test_msg(int read_fd, int write_fd)
{
    std::ostringstream os;
    auto time = std::time(nullptr);
    os << std::asctime(std::localtime(&time));

    send_msg(write_fd, os.str());
}

std::string check_pipe_status(int read_fd)
{
    try
    {
        uint32_t statusSize;
        if (!read_uint32(read_fd, statusSize))
        {
            // EOF waiting for a message, script ended
            std::cout << "EOF waiting for message, script ended" << std::endl;
            return "";
        }

        std::string status = read_string(read_fd, statusSize);
 
        // std::cout << "status : " << status << std::endl;
        return status;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return "";
}

void transmit_buf(int read_fd, int write_fd, unsigned char *buf, int datalen)
{
    send_msg(write_fd, datalen, buf);
}

void transmit_msg(int read_fd, int write_fd, std::string msg)
{
    send_msg(write_fd, msg);
}