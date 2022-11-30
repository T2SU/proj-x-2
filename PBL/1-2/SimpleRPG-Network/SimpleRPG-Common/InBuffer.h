#pragma once
class InBuffer
{
private:
	std::istringstream	_stream;
	Serializer<char>	_serializer;

public:
	InBuffer(const char* buf, size_t len);

	template<typename T>
	T		Read();

	void	ReadBuffer(char* buf, size_t len);

	std::string	ReadString();
};

template<typename T>
T InBuffer::Read()
{
	return _serializer.Read<T>();
}

inline void	InBuffer::ReadBuffer(char* buf, size_t len)
{
	_serializer.ReadBuffer(buf, len);
}

inline std::string InBuffer::ReadString()
{
	const auto size = Read<uint16_t>();
	auto buf = reinterpret_cast<char*>(_malloca(size));
	ReadBuffer(buf, size);
	std::string ret(buf, size);
	return ret;
}