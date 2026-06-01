#pragma once

#include <print>
#include <filesystem>
#include <fstream>
#include <exception>

class FileReader final
{
public:
	FileReader(const std::string& filePath)
	{
		std::filesystem::path inputFilePath(filePath);

		m_stream = std::ifstream(inputFilePath, std::ios::binary);

		if (! m_stream)
		    throw std::runtime_error("[FileReader]: Failed to open file!");

		m_stream.seekg(0, std::ios::end);
		m_fileSize = getOffset();
		m_stream.seekg(0, std::ios::beg);

		std::println("[FileReader]: File {} opened, size {}", inputFilePath.stem().string(), m_fileSize);
	}

	~FileReader() = default;

	template <typename T>
	T read()
	{
		if (! m_stream)
		    throw std::runtime_error("[FileReader]: Stream is invalid!");

		if (getOffset() + sizeof(T) > m_fileSize)
			throw std::runtime_error("[FileReader]: Read will go past file bounds!");

		T output;

		if (! m_stream.read(reinterpret_cast<char*>(&output), sizeof(T)))
        {
            std::println("[Dump]: Offset {}, File Size {}, Size Of Read {}", getOffset(), getFileSize(), sizeof(T));
            throw std::runtime_error("[FileReader]: Read failed!");
        }

		return output;
	}

	void seek(uint64_t bytes)
	{
		if (! m_stream)
		    throw std::runtime_error("[FileReader]: Stream is invalid!");

		m_stream.seekg(bytes, std::ios::cur);
	}

	bool atEOF()
	{
		if (! m_stream)
		    throw std::runtime_error("[FileReader]: Stream is invalid!");

		return m_stream.eof();
	}

	uint64_t getOffset() { return m_stream.tellg(); }
	uint64_t getFileSize() { return m_fileSize; }

private:
	std::ifstream m_stream;
	uint64_t m_fileSize;
};
