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
	/// ����ȭ ������ �����ؾ� �ϴ� ���� �Լ� �Դϴ�.
	/// </summary>
	/// <param name="serializer">����ȭ ���� �ν��Ͻ�</param>
	virtual void Serialize(Serializer<T>& serializer) const = 0;

	/// <summary>
	/// ������ȭ ������ �����ؾ� �ϴ� ���� �Լ� �Դϴ�.
	/// </summary>
	/// <param name="serializer">����ȭ ���� �ν��Ͻ�</param>
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
	/// ��Ʈ���� �����͸� �ۼ��ϱ� ���� ������ �Դϴ�.
	/// </summary>
	/// <param name="os">�����͸� �ۼ��� ��� ��Ʈ��</param>
	Serializer(std::basic_ostream<CharT>& os)
		: _mode(Mode::WRITE), _out(&os), _in(nullptr) {}

	/// <summary>
	/// ��Ʈ������ �����͸� �б� ���� ������ �Դϴ�.
	/// </summary>
	/// <param name="os">�����͸� �о�� �Է� ��Ʈ��</param>
	Serializer(std::basic_istream<CharT>& is)
		: _mode(Mode::READ), _in(&is), _out(nullptr) {}

	Serializer(const Serializer&) = delete;
	Serializer(Serializer&&) = delete;
	Serializer& operator=(const Serializer&) = delete;
	Serializer&& operator=(Serializer&&) = delete;

	/// <summary>
	/// ���ڷ� ���޹��� ��ü�� ���� ��Ʈ���� ����ȭ�Ͽ� ����մϴ�.
	/// </summary>
	/// <typeparam name="U">��ü Ÿ��</typeparam>
	/// <param name="obj">��Ʈ������ ����ȭ�� ��ü</param>
	template <typename U = T,
		std::enable_if_t<std::is_convertible_v<U*, ISerializable<U>* >, bool> = true
	>
	void Save(const T& obj)
	{
		obj.Serialize(*this);
	}

	/// <summary>
	/// ���� ��Ʈ������ ������ȭ�Ͽ� ���ڷ� ���޹��� ��ü�� ���� �ۼ��մϴ�.
	/// </summary>
	/// <typeparam name="U">��ü Ÿ��</typeparam>
	/// <param name="obj">��Ʈ������ ������ȭ�� ��ü</param>
	template <typename U = T,
		std::enable_if_t<std::is_convertible_v<U*, ISerializable<U>* >, bool> = true
	>
	void Load(T& obj)
	{
		obj.Deserialize(*this);
	}

	/// <summary>
	/// ��Ʈ������ ���� �о� ��ȯ�մϴ�.
	/// </summary>
	/// <typeparam name="U">���� ���� Ÿ��</typeparam>
	/// <returns>��Ʈ������ ���� ��</returns>
	template <typename U>
	U		Read();

	/// <summary>
	/// ��Ʈ������ ����Ʈ �迭�� �н��ϴ�.
	/// </summary>
	/// <param name="buffer">�����͸� �о� ������ �迭�Դϴ�.</param>
	/// <param name="count">���� ����Ʈ �� �Դϴ�.</param>
	void	ReadBuffer(CharT buffer[], size_t count);

	/// <summary>
	/// ��Ʈ���� ���� ����ȭ �Ͽ� ��ȯ�մϴ�.
	/// </summary>
	/// <typeparam name="U">�ۼ��� ���� Ÿ��</typeparam>
	template <typename U>
	void	Write(const U&);

	/// <summary>
	/// ��Ʈ���� ����Ʈ �迭�� �ۼ��մϴ�.
	/// </summary>
	/// <param name="buffer">��Ʈ���� ������ �������� �迭�Դϴ�.</param>
	/// <param name="count">�ۼ��� ����Ʈ �� �Դϴ�.</param>
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