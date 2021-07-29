#include "zlib_helper.h"
#include <stdexcept>
#include <vector>
#include <fstream>
#include <zlib.h>

zlib::vector zlib::Convert(const std::string& value)
{
	const zlib::byte* pointer = reinterpret_cast<const zlib::byte*>(value.data());
	return zlib::vector(pointer, &pointer[value.size()]);
}

std::string zlib::Convert(const zlib::vector& value)
{
	const char* pointer = reinterpret_cast<const char*>(&value[0]);
	return std::string(pointer, value.size());
}

zlib::vector zlib::ReadFile(std::string filename)
{
	// Open the file
	std::basic_ifstream<std::byte> file(filename, std::ios::binary);

	// Check if the file is open
	if (!file.is_open())
		throw std::runtime_error("Could not open the file.");

	return std::vector<std::byte>(std::istreambuf_iterator<std::byte>(file), std::istreambuf_iterator<std::byte>());
}

void zlib::WriteFile(std::string filename, const zlib::vector& data)
{
	// Open the file
	std::ofstream fout(filename, std::ios::out | std::ios::binary);

	if (!fout.is_open())
		throw std::runtime_error("Could not open file.");

	fout.write(reinterpret_cast<const char*>(&data[0]), data.size());

}

zlib::vector zlib::Compress(const zlib::vector& data, int type)
{
	return zlib::Compress(reinterpret_cast<const char*>(&data[0]), data.size(), type);
}

std::string zlib::Compress(const std::string& data, int type)
{
	zlib::vector result = zlib::Compress(data.data(), data.size(), type);
	return std::string(reinterpret_cast<char*>(&result[0]), result.size());
}

//This is the preferred method of Compressing data.
//It does not assume to much, and gives plents of options.
zlib::vector zlib::Compress(const char* data, size_t size, int type)
{
	z_stream strm{};
	memset(&strm, 0, sizeof(strm));
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	switch (type)
	{
	case zlib::ZLIB:
		type = 15;
		break;
	case zlib::GZIP:
		type = 31;
		break;
	default:
		type = 15;
		break;
	}

	if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, type, 9, Z_DEFAULT_STRATEGY) != Z_OK)
	{
		throw std::runtime_error("deflateInit2 failed.");
	}

	strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(data));
	strm.avail_in = size;

	int ret;
	std::unique_ptr<std::byte[]> outbuffer = std::make_unique<std::byte[]>(zlib::CHUNK_SIZE);
	std::vector<std::byte> result;
	do {
		strm.next_out = reinterpret_cast<Bytef*>(&outbuffer[0]);
		strm.avail_out = zlib::CHUNK_SIZE;

		ret = deflate(&strm, Z_FINISH);

		if (result.size() < strm.total_out)
		{
			int write_size = strm.total_out - result.size();
			result.insert(result.end(), &outbuffer[0], &outbuffer[write_size]);
		}

	} while (ret == Z_OK);

	deflateEnd(&strm);

	if (ret != Z_STREAM_END) {
		throw std::runtime_error("Failed at end or something.");
	}
	//result.insert(result.end(), data.begin(), data.end());
	return result;
}

zlib::vector zlib::Decompress(const zlib::vector& data, int type)
{
	return zlib::Decompress(reinterpret_cast<const char*>(&data[0]), data.size(), type);
}

std::string zlib::Decompress(const std::string& data, int type)
{
	zlib::vector result = zlib::Decompress(data.data(), data.size(), type);
	return std::string(reinterpret_cast<char*>(&result[0]), result.size());
}

zlib::vector zlib::Decompress(const char* data, size_t size, int type)
{
	z_stream strm{};
	memset(&strm, 0, sizeof(strm));
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	switch (type)
	{
	case zlib::ZLIB:
	case zlib::GZIP:
		break;
	default:
		type = zlib::ZLIB;
	}

	if (inflateInit2(&strm, type) != Z_OK)
	{
		throw std::runtime_error("inflateInit2 failed.");
	}

	strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(data));
	strm.avail_in = size;

	int ret;
	std::unique_ptr<std::byte[]> outbuffer = std::make_unique<std::byte[]>(zlib::CHUNK_SIZE);
	std::string outstring;
	std::vector<std::byte> result;
	do
	{
		strm.next_out = reinterpret_cast<Bytef*>(&outbuffer[0]);
		strm.avail_out = zlib::CHUNK_SIZE;

		ret = inflate(&strm, 0);

		if (result.size() < strm.total_out)
		{
			int write_size = strm.total_out - result.size();
			result.insert(result.end(), &outbuffer[0], &outbuffer[write_size]);
		}
	} while (ret == Z_OK);

	inflateEnd(&strm);

	if (ret != Z_STREAM_END)
	{
		throw std::runtime_error("Failed at the end or something.");
	}


	return result;
}

