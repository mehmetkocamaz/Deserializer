#pragma once
#include <vector>
#include <cstdint>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>

class EraEncryptor {
public:
	EraEncryptor(const CryptoPP::SecByteBlock& key, const CryptoPP::SecByteBlock& iv)
		: key_(key), iv_(iv) {}

	std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) const {
		CryptoPP::AES::Encryption aesEncryption(key_, CryptoPP::AES::DEFAULT_KEYLENGTH);
		CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv_);

		std::vector<uint8_t> paddedData = data;
		padPKCS7(paddedData);

		std::vector<uint8_t> ciphertext(paddedData.size());
		CryptoPP::ArraySink cs(&ciphertext[0], ciphertext.size());

		CryptoPP::ArraySource(paddedData.data(), paddedData.size(), true,
			new CryptoPP::StreamTransformationFilter(cbcEncryption,
				new CryptoPP::Redirector(cs)));

		return ciphertext;
	}

	std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext) const {
		CryptoPP::AES::Decryption aesDecryption(key_, CryptoPP::AES::DEFAULT_KEYLENGTH);
		CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv_);

		std::vector<uint8_t> data(ciphertext.size());
		CryptoPP::ArraySink ds(&data[0], data.size());

		CryptoPP::ArraySource(ciphertext.data(), ciphertext.size(), true,
			new CryptoPP::StreamTransformationFilter(cbcDecryption,
				new CryptoPP::Redirector(ds)));

		unpadPKCS7(data);
		return data;
	}

private:
	void padPKCS7(std::vector<uint8_t>& data) const {
		size_t blockSize = CryptoPP::AES::BLOCKSIZE;
		size_t paddingSize = blockSize - (data.size() % blockSize);
		data.insert(data.end(), paddingSize, static_cast<uint8_t>(paddingSize));
	}

	void unpadPKCS7(std::vector<uint8_t>& data) const {
		if (data.empty()) {
			throw std::runtime_error("Data is empty, cannot unpad");
		}
		size_t paddingSize = data.back();
		if (paddingSize > data.size()) {
			throw std::runtime_error("Invalid padding size");
		}
		data.resize(data.size() - paddingSize);
	}
private:

	CryptoPP::SecByteBlock key_;
	CryptoPP::SecByteBlock iv_;
};
