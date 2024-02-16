#pragma once
#include <vector>
#include <array>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include "Zlw/Zlw.h"



namespace Utils {
	template <typename T>
	static void ReadFromBuffer(const std::vector<uint8_t>& buffer, T& destination, size_t& offset) {
		static_assert(std::is_pod<T>::value, "Read only supports POD types.");

		// Ensure there is enough data to read
		if (offset + sizeof(T) > buffer.size()) {
			throw std::runtime_error("Attempt to read beyond the buffer size.");
		}

		// Read the data from the buffer into the destination
		std::memcpy(&destination, buffer.data() + offset, sizeof(T));

		// Adjust for endianness if necessary
		if constexpr (std::endian::native != std::endian::little) {
			ReverseBytes(destination);
		}

		// Update the offset by the size of T
		offset += sizeof(T);
	}

	template <typename T>
	static void ReverseBytes(T& value) {
		std::array<uint8_t, sizeof(T)> bytes;
		std::memcpy(bytes.data(), &value, sizeof(T));
		std::reverse(bytes.begin(), bytes.end());
		std::memcpy(&value, bytes.data(), sizeof(T));
	}

	static uint8_t* ReadBytes(const std::filesystem::path& filepath, size_t* outSize) {
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
		if (!stream) {
			return nullptr;
		}
		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0)
		{
			return nullptr;
		}
		uint8_t* buffer = new uint8_t[size];
		stream.read((char*)buffer, size);
		stream.close();
		*outSize = (uint32_t)size;
		return buffer;
	}

	static std::vector<uint8_t> ReadBytesToVector(const std::filesystem::path& filepath) {
		size_t size = 0;
		uint8_t* buffer = ReadBytes(filepath, &size);

		if (buffer == nullptr) {
			throw std::runtime_error("Failed to read bytes or file is empty.");
		}

		std::vector<uint8_t> dataVector(buffer, buffer + size);

		delete[] buffer;

		return dataVector;
	}

	static bool WriteBytes(const std::filesystem::path& filepath, uint8_t* buffer, size_t size) {
		std::ofstream stream(filepath, std::ios::binary | std::ios::out);
		if (!stream) {
			return false;
		}
		stream.write((char*)buffer, size);
		stream.close();
		return true;
	}


	static bool Compress(std::vector<uint8_t>& dataBufferToCompress, std::vector<uint8_t>& compressedDataBuffer)
	{
		// Check if the data buffer is large enough
		if (dataBufferToCompress.size() < 4)
			return false; // Not enough data to compress

		// Reserve space for the uncompressed data
		uLongf uncompressedSize = dataBufferToCompress.size(); // Exclude first 4 bytes
		uLongf compressedSize = Zlw::CompressBound(uncompressedSize);
		compressedDataBuffer.resize(compressedSize); // Reserve space for the 4 bytes to be appended later

		// Compress the data (excluding first 4 bytes)
		if (Zlw::Compress(dataBufferToCompress.data(), uncompressedSize, compressedDataBuffer.data(), compressedSize)) {
			// Write the first 4 bytes to the beginning of the compressed buffer
			std::copy(dataBufferToCompress.begin(), dataBufferToCompress.begin(), compressedDataBuffer.begin());
			// Resize the compressed buffer to the actual size of the compressed data plus the first 4 bytes
			compressedDataBuffer.resize(compressedSize + 4);
			return true;
		}

		return false;
	}


	//static bool Compress(std::vector<uint8_t>& dataBufferToCompress, std::vector<uint8_t>& compressedDataBuffer)
	//{
	//	uLongf compressedSize = Zlw::CompressBound(dataBufferToCompress.size());
	//	compressedDataBuffer.resize(compressedSize);

	//	// Compress the data
	//	if (Zlw::Compress(dataBufferToCompress.data(), dataBufferToCompress.size(), compressedDataBuffer.data(), compressedSize)) {
	//		// Resize the compressed buffer to the actual size of the compressed data
	//		compressedDataBuffer.resize(compressedSize);
	//		return true;
	//	}

	//	return false;
	//}

	static bool Decompress(std::vector<uint8_t>& compressedDataBuffer, std::vector<uint8_t>& decompressedDataBuffer)
	{

		// Decompress the data
		uLongf decompressedSize = decompressedDataBuffer.size();
		decompressedDataBuffer.resize(decompressedSize);
		if (Zlw::Decompress(compressedDataBuffer.size(), compressedDataBuffer.data(), decompressedSize, decompressedDataBuffer.data())) {
			decompressedDataBuffer.resize(decompressedSize);
			return true;
		}

		return false;

	}

	template<typename T>
	static void ApplyXorFilter(std::vector<T>& rawBuffer, uint32_t key)
	{
		for (T& data : rawBuffer)
			data = data ^ key;
	}
}