#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <vector>

/* 
APIs to be accessed by the script.
*/

void foo_api(std::ostream &os, const std::string &arg);
void bar_api(std::ostream &os, const std::string &arg);

/* end API section */

/* return true if val is set, false for EOF */
bool read_uint32(int read_fd, uint32_t &val);
void send_msg(int write_fd, std::string msg);
void send_msg(int read_fd, int write_fd, unsigned char* buf, int datalen);
std::string read_string(int read_fd, uint32_t sz);

std::string check_pipe_status(int read_fd);
void transmit_buf(int read_fd, int write_fd, unsigned char* buf, int datalen);
void transmit_msg(int read_fd, int write_fd, std::string msg);