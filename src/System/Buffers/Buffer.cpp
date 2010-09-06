#include "Buffer.h"
#include "System/Common.h"
#include <stdarg.h>
#include <stdlib.h>

Buffer::Buffer()
{
	Init();
}

Buffer::~Buffer()
{
	if (buffer != array && !preallocated)
		free(buffer);
}

void Buffer::SetPreallocated(char* buffer_, unsigned size_)
{
	preallocated = true;
	buffer = buffer_;
	size = size_;
}

bool Buffer::Cmp(Buffer& a, Buffer& b)
{
	return MEMCMP(a.buffer, a.length, b.buffer, b.length);
}

bool Buffer::Cmp(const char* buffer_, unsigned length_)
{
	return MEMCMP(buffer, length, buffer_, length_);
}

bool Buffer::Cmp(const char* str)
{
	return MEMCMP(buffer, length, str, strlen(str));
}

void Buffer::Lengthen(unsigned k)
{
	length += k;
}

void Buffer::Shorten(unsigned k)
{
	length -= k;
}

void Buffer::Allocate(unsigned size_, bool keepold)
{
	char* newbuffer;
	
	if (size_ <= size)
		return;
	
	size_ = size_ + ALLOC_GRANURALITY - 1;
	size_ -= size_ % ALLOC_GRANURALITY;

	if (buffer == array || preallocated)
		newbuffer = (char*) malloc(size_);
	else	
		newbuffer = (char*) realloc(buffer, size_);

	if (keepold && length > 0)
	{
		if (buffer == array)
			memcpy(newbuffer, buffer, length);
	}
	
	buffer = newbuffer;
	size = size_;
	preallocated = false;
}

int Buffer::Readf(const char* format, ...) const
{
	int			read;
	va_list		ap;
	
	va_start(ap, format);
	read = VReadf(buffer, length, format, ap);
	va_end(ap);
	
	return read;
}

unsigned Buffer::Writef(const char* fmt, ...)
{
	unsigned required;
	va_list ap;

	while (true)
	{
		va_start(ap, fmt);
		required = VWritef(buffer, size, fmt, ap);
		va_end(ap);
		
		if (required <= size)
		{
			length = required;
			break;
		}
		
		Allocate(required, false);
	}
	
	return length;
}

unsigned Buffer::Appendf(const char* fmt, ...)
{
	unsigned required;
	va_list ap;
	
	while (true)
	{
		va_start(ap, fmt);
		required = VWritef(GetPosition(), GetRemaining(), fmt, ap);
		va_end(ap);
		
		// snwritef returns number of bytes required
		if (required <= GetRemaining())
			break;
		
		Allocate(length + required, true);
	}
	
	length += required;
	return required;
}

void Buffer::Write(const char* buffer_, unsigned length_)
{
	if (length_ > size)
		Allocate(length + length_);
	memmove(buffer, buffer_, length_);
	length = length_;
}

void Buffer::Write(const char* str)	
{
	Write(str, strlen(str));
}

void Buffer::Write(Buffer& other)
{
	Write(other.GetBuffer(), other.GetLength());
}

void Buffer::Write(ReadBuffer& other)
{
	Write(other.GetBuffer(), other.GetLength());
}

void Buffer::Append(const char* buffer_, unsigned length_)
{
	if (length_ > GetRemaining())
		Allocate(length + length_);
	memcpy(GetPosition(), buffer_, length_);
	Lengthen(length_);
}

void Buffer::Append(const char* str)
{
	Append(str, strlen(str));
}

void Buffer::Append(Buffer& other)
{
	Append(other.GetBuffer(), other.GetLength());
}

void Buffer::Append(ReadBuffer& other)
{
	Append(other.GetBuffer(), other.GetLength());
}

void Buffer::NullTerminate()
{
	Append("", 1);
}

void Buffer::Zero()
{
	memset(buffer, 0, size);
}

void Buffer::SetLength(unsigned length_)
{
	length = length_;
	if (length < 0 || length > size)
		ASSERT_FAIL();
}

void Buffer::Init()
{
	buffer = array;
	size = SIZE(array);
	length = 0;
	preallocated = false;
}

unsigned Buffer::GetSize()
{
	return size;
}

char* Buffer::GetBuffer()
{
	return buffer;
}

unsigned Buffer::GetLength()
{
	return length;
}

unsigned Buffer::GetRemaining()
{
	return size - length;
}

char* Buffer::GetPosition()
{
	return buffer + length;
}

char Buffer::GetCharAt(unsigned i)
{
	if (i > length - 1)
		ASSERT_FAIL();
	
	return *(buffer + i);
}

uint32_t Buffer::GetChecksum()
{
	return ChecksumBuffer(buffer, length);
}

void Buffer::Clear()
{
	length = 0;
}

Buffer::Buffer(const Buffer& other)
{
	*this = other;	// call operator=()
}

Buffer& Buffer::operator=(const Buffer& other)
{
	if (other.size != size)
		Allocate(other.size, false);
	
	memcpy(buffer, other.buffer, other.size);
	length = other.length;
	
	return *this;
}
