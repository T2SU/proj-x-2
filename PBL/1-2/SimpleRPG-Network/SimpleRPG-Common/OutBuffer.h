#pragma once

class OutBuffer
{
private:
	std::ostringstream	_stream;
	Serializer<char>	_serializer;

	mutable std::string	_cacheBuffer;
	mutable bool		_dirtyCache;

	void	UpdateBuffer() const;
public:
	OutBuffer();

	template<typename T>
	void	Write(const T& val);

	void	WriteBuffer(const char* buf, size_t len);
	void	WriteString(const std::string& str);

	const char* GetBytes() const { UpdateBuffer(); return _cacheBuffer.c_str(); }
	size_t		GetSize() const { UpdateBuffer(); return _cacheBuffer.size(); }
};

template<typename T>
void OutBuffer::Write(const T& val)
{
	_serializer.Write(val);
	_dirtyCache = true;
}

inline void OutBuffer::WriteBuffer(const char* buf, size_t len)
{
	_serializer.WriteBuffer(buf, len);
	_dirtyCache = true;
}

inline void OutBuffer::WriteString(const std::string& str)
{
	Write(static_cast<uint16_t>(str.size()));
	WriteBuffer(str.data(), str.size());
}