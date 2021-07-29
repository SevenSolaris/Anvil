#ifndef ZLIB_HELPER_H
#define ZLIB_HELPER_H

#include <vector>
#include <iostream>
#include <fstream>

// 256k
#define BUFFER_CHUNK_SIZE 262144

namespace zlib
{

	enum { ZLIB = 47, GZIP = 31 };

	constexpr size_t CHUNK_SIZE = 64 * 1024;

	typedef std::byte byte;
	typedef std::vector<byte> vector;

	// We should have the following functions:
	// std::vector<std::byte> ReadFile

	zlib::vector Convert(const std::string& value);
	std::string Convert(const zlib::vector& value);

	zlib::vector ReadFile(std::string filename);
	void WriteFile(std::string filename, const zlib::vector& data);

	zlib::vector Compress(const char* data, size_t size, int type = zlib::ZLIB);
	zlib::vector Decompress(const char* data, size_t size, int type = zlib::ZLIB);

	zlib::vector Compress(const zlib::vector& data, int type = zlib::ZLIB);
	zlib::vector Decompress(const zlib::vector& data, int type = zlib::ZLIB);

	std::string Compress(const std::string& data, int type = zlib::ZLIB);
	std::string Decompress(const std::string& data, int type = zlib::ZLIB);
}

#endif // ZLIB_HELPER_H
