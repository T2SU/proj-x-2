#pragma once
#include <type_traits>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <array>
#include <bit>

template <typename T> class Serializer;

template <typename T>
class ISerializable
{
public:
	/// <summary>
	/// 직렬화 로직을 구현해야 하는 가상 함수 입니다.
	/// </summary>
	/// <param name="serializer">직렬화 헬퍼 인스턴스</param>
	virtual void Serialize(Serializer<T>& serializer) const = 0;

	/// <summary>
	/// 역직렬화 로직을 구현해야 하는 가상 함수 입니다.
	/// </summary>
	/// <param name="serializer">직렬화 헬퍼 인스턴스</param>
	virtual void Deserialize(Serializer<T>& serializer) = 0;
};

template <typename T>
class Serializer
{
private:
	enum class Mode { READ, WRITE };
	Mode _mode;

	typedef std::fstream::char_type	CharT;

	std::basic_ostream<CharT>* _out;
	std::basic_istream<CharT>* _in;

public:
	/// <summary>
	/// 스트림에 데이터를 작성하기 위한 생성자 입니다.
	/// </summary>
	/// <param name="os">데이터를 작성할 출력 스트림</param>
	Serializer(std::basic_ostream<CharT>& os)
		: _mode(Mode::WRITE), _out(&os), _in(nullptr) {}

	/// <summary>
	/// 스트림에서 데이터를 읽기 위한 생성자 입니다.
	/// </summary>
	/// <param name="os">데이터를 읽어올 입력 스트림</param>
	Serializer(std::basic_istream<CharT>& is)
		: _mode(Mode::READ), _in(&is), _out(nullptr) {}

	Serializer(const Serializer&) = delete;
	Serializer(Serializer&&) = delete;
	Serializer& operator=(const Serializer&) = delete;
	Serializer&& operator=(Serializer&&) = delete;

	/// <summary>
	/// 인자로 전달받은 객체를 현재 스트림에 직렬화하여 출력합니다.
	/// </summary>
	/// <typeparam name="U">객체 타입</typeparam>
	/// <param name="obj">스트림으로 직렬화할 객체</param>
	template <typename U = T,
		std::enable_if_t<std::is_convertible_v<U*, ISerializable<U>* >, bool> = true
	>
	void Save(const T& obj)
	{
		obj.Serialize(*this);
	}

	/// <summary>
	/// 현재 스트림에서 역직렬화하여 인자로 전달받은 객체에 값을 작성합니다.
	/// </summary>
	/// <typeparam name="U">객체 타입</typeparam>
	/// <param name="obj">스트림에서 역직렬화될 객체</param>
	template <typename U = T,
		std::enable_if_t<std::is_convertible_v<U*, ISerializable<U>* >, bool> = true
	>
	void Load(T& obj)
	{
		obj.Deserialize(*this);
	}

	/// <summary>
	/// 스트림에서 값을 읽어 반환합니다.
	/// </summary>
	/// <typeparam name="U">읽을 값의 타입</typeparam>
	/// <returns>스트림에서 읽은 값</returns>
	template <typename U>
	U		Read();

	/// <summary>
	/// 스트림에서 바이트 배열을 읽습니다.
	/// </summary>
	/// <param name="buffer">데이터를 읽어 저장할 배열입니다.</param>
	/// <param name="count">읽을 바이트 수 입니다.</param>
	void	ReadBuffer(CharT buffer[], size_t count);

	/// <summary>
	/// 스트림에 값을 직렬화 하여 반환합니다.
	/// </summary>
	/// <typeparam name="U">작성할 값의 타입</typeparam>
	template <typename U>
	void	Write(const U&);

	/// <summary>
	/// 스트림에 바이트 배열을 작성합니다.
	/// </summary>
	/// <param name="buffer">스트림에 저장할 데이터의 배열입니다.</param>
	/// <param name="count">작성할 바이트 수 입니다.</param>
	void	WriteBuffer(const CharT buffer[], size_t count);
};

template <typename T>
template <typename U>
inline U	Serializer<T>::Read()
{
	if (_mode != Mode::READ)
	{
		throw std::runtime_error("invalid operation");
	}
	if constexpr (std::is_convertible_v<U*, ISerializable<U>* >)
	{
		U ret{};
		Serializer<U> s(*_in);
		ret.Deserialize(s);
		return ret;
	}
	else
	{
		std::array<CharT, sizeof(U)> tmp;
		_in->read(tmp.data(), sizeof(U));
		return std::bit_cast<U>(tmp);
	}
}

template <typename T>
inline void		Serializer<T>::ReadBuffer(CharT buffer[], size_t count)
{
	_in->read(buffer, count);
}

template <typename T>
template <typename U>
inline void		Serializer<T>::Write(const U& val)
{
	if (_mode != Mode::WRITE)
	{
		throw std::runtime_error("invalid operation");
	}
	if constexpr (std::is_convertible_v<U*, ISerializable<U>* >)
	{
		Serializer<U> s(*_out);
		val.Serialize(s);
	}
	else
	{
		std::array<CharT, sizeof(U)> tmp = std::bit_cast<decltype(tmp)>(val);
		_out->write(tmp.data(), sizeof(U));
	}
}

template <typename T>
inline void		Serializer<T>::WriteBuffer(const CharT buffer[], size_t count)
{
	_out->write(buffer, count);
}