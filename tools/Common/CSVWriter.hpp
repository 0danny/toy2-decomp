#pragma once

#include <string>
#include <memory>
#include <fstream>
#include <print>
#include <vector>

class CSVWriter final
{
public:
	CSVWriter(const std::string& fileName) : m_fileName(fileName) {};

	bool open()
	{
		if ( m_stream )
		{
			std::println("[CSVWriter]: Destroying original stream");
			m_stream.reset();
		}

		m_stream = std::make_unique<std::ofstream>(m_fileName, std::ios::binary);

		if ( ! m_stream )
		{
			std::println("[CSVWriter]: Failed to open {}", m_fileName);
			m_stream.reset();
			return false;
		}

		return true;
	}

	template <typename... Args>
	void writeLine(Args&&... args)
	{
		if ( ! m_stream || ! *m_stream )
		{
			std::println("[CSVWriter]: No valid stream is open!");
			return;
		}

		bool first = true;

		((*m_stream << (first ? (first = false, "") : ",") << args), ...);

		*m_stream << '\n';
	}

	void testWrite()
	{
		if ( ! open() )
			return;

		writeLine("Hello", "World", "Header");
		writeLine("1", "2", "3");
		writeLine("4", "5", "6");
	}

private:
	std::string m_fileName;
	std::unique_ptr<std::ofstream> m_stream;
};
