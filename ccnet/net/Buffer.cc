#include <sys/socket.h>
#include <errno.h>

#include "Buffer.h"

Buffer::Buffer(ssize_t initialsize)
:buffer_(kInitialHeadSize + kInitialSize),
write_index_(kInitialHeadSize),
read_index_(kInitialHeadSize)
{
}

Buffer::~Buffer()
{
}

size_t Buffer::readableBytes()
{
    return write_index_ - read_index_;
}

size_t Buffer::writableBytes()
{
    return buffer_.size() - write_index_;
}

const char* Buffer::peek()
{
    return begin() + read_index_;
}

void Buffer::retrieve(ssize_t len)
{
    if (len < readableBytes())
    {
        read_index_ += len;
    }else
    {
        retrieveAll();
    }
    
}

void Buffer::retrieveAll()
{
    read_index_ = kInitialHeadSize;
    write_index_ = kInitialHeadSize;
}

char* Buffer::begin()
{
    return &*buffer_.begin();
}

ssize_t Buffer::readFd(int fd , int* savedErr)
{
    ssize_t n =0;
    while (true)
    {
        size_t writeable =  writableBytes();
        n = ::recv(fd, begin()+write_index_,writeable,0);
        if (n <0)
        {
            *savedErr = errno;
            break;
        }
        if (n <= writeable)
        {
            write_index_ += n;
        }
        else
        {
            write_index_ = buffer_.size();
        }
    }
    return n;
}

ssize_t Buffer::append(const char* data, ssize_t len)
{
    //todo makespace
    std::copy(data, data+len, begin() + write_index_);
    write_index_ += len;
}
