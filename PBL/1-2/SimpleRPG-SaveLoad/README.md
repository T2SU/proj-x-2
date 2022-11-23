# SimpleRPG - SaveLoad
C++로 개발된 저장과 로드를 지원하는 간단한 텍스트 기반 RPG 게임입니다.

## 1. 사용자 입력 및 출력
사용자 입력 및 출력은 콘솔 메시지를 사용합니다. 그 중, 핵심적으로 사용되는 방식은 다음과 같습니다.
### 1-1. std::cout
- 메시지를 출력하기 위해 `std::cout` 스트림을 사용하고 있습니다.
- `std::cout << "메시지" << std::endl;` 처럼, 가장 간단하고 기본적인 방식입니다.
### 1-2. 입력 메뉴
- 사용자로부터 입력받는 메시지를 개발 편의상 최대한 제한시켰습니다.
- 사용자는 정해진 메뉴에서만 프로그램에 입력을 수행할 수 있습니다.

```
[Cool Fantasy] 에 오신 것을 환영합니다.
[1]:  새로운 게임
[2]:  불러오기
무엇을 하시겠습니까?: 1


이름을 선택하십시오.
[1]:  로아
[2]:  타티아나
[3]:  제임스
[4]:  이안
무엇을 하시겠습니까?:
```

결과적으로, 사용자는 위와 같은 메뉴를 볼 수 있으며, 숫자 입력을 통해 프로그램과 상호작용할 수 있습니다.

### 1-2-1. 입력 메뉴 에러 처리
```
이름을 선택하십시오.
[1]:  로아
[2]:  타티아나
[3]:  제임스
[4]:  이안
무엇을 하시겠습니까?: -
잘못된 입력입니다.
```
1. 메뉴 파싱엔 `std::stoi` 함수를 활용하고 있습니다. 따라서 `std::stoi`가 실패할 경우에, 입력 실패 메시지가 나옵니다.


```
이름을 선택하십시오.
[1]:  로아
[2]:  타티아나
[3]:  제임스
[4]:  이안
무엇을 하시겠습니까?: 99
잘못된 입력입니다.
```
2. 또한, 범위를 넘어가는 값에 대해서도 예외로 처리합니다.

## 2. 파일 저장 및 불러오기

- 파일 저장 및 불러오기를 수행하기 위해, `Serializer.h` 에서 직렬화 컴포넌트를 구현하고 있습니다. 직렬화 컴포넌트는 다음과 같이 사용합니다. (간단하게 이해를 돕기 위해, 예외 처리 코드는 생략하였습니다.)

	```c++
	GameData data;
	std::ifstream ifs("save.dat", std::ios::binary);
	Serializer<GameData> s(ifs);
	s.Load(data);
	```

- GameData는 다음과 같이, `ISerializable<GameData>` 인터페이스를 상속받아 구현하고 있습니다. 
- `Serialize` 및 `Deserialize` 메서드는 각각 직렬화 및 역직렬화에 사용됩니다. - `Serializer` 클래스의 `Save` 및 `Load` 메서드는, `Serializer` 클래스의 생성자로 전달되었던 스트림을 사용하여 쓰기 또는 읽기를 수행합니다. 
- 여기에서 **실질적인 쓰기 및 읽기 로직**은 `ISerializable<GameData>` 를 상속받아 구현했던 `Serialize` 및 `Deserialize` 메서드를 통해 구현되어집니다.

	```c++
	void	GameData::Serialize(Serializer<GameData>& serializer) const
	{
		serializer.Write(Version);
		serializer.Write(Seed);
		serializer.Write(EnemyHealth);
		serializer.Write(CharacterData);
	}

	void	GameData::Deserialize(Serializer<GameData>& serializer)
	{
		Version = serializer.Read<decltype(Version)>();
		Seed = serializer.Read<decltype(Seed)>();
		EnemyHealth = serializer.Read<decltype(EnemyHealth)>();
		CharacterData = serializer.Read<decltype(CharacterData)>();
	}
	```

## 3. 직렬화 (Serializable.h)
- `Serializer`의 `Save` 및 `Load` 메서드는 `ISerializable` 인터페이스를 구현하는 타입의 인스턴스로만 호출될 수 있도록 다음과 같이 템플릿을 통해 제한하고 있습니다.

	```c++
	template <typename U = T,
		std::enable_if_t<std::is_convertible_v<U*, ISerializable<U>* >, bool> = true
	>
	void Save(const T& obj)
	{
		obj.Serialize(*this);
	}

	template <typename U = T,
		std::enable_if_t<std::is_convertible_v<U*, ISerializable<U>* >, bool> = true
	>
	void Load(T& obj)
	{
		obj.Deserialize(*this);
	}
	```

- `Serialize` 및 `Deserialize` 과정에서, `ISerializable` 인터페이스를 구현하는 경우와 아닌 경우에 대해서 별도로 처리하고 있습니다.
- 읽기 작업을 예로 들면, 다음과 같이 됩니다.
- `Read<int32_t>` 처럼, `ISerializable` 인터페이스를 구현하지 않는 타입을 읽어내려고 한다면, `else` 분기가 실행되어, 해당 타입을 이진 데이터로 읽어낸 후, 반환합니다.
- 그러나 `Read<CharacterData>` 처럼 `ISerializable` 인터페이스를 구현하는 타입을 읽어내려고 한다면, `if` 의 분기가 실행되어, 해당 타입에 맞는 새로운 `Serializer` 인스턴스를 생성한 후 `Deserialize`를 호출하여, 올바르게 해당 타입을 역직렬화 할 수 있게 합니다.

	```c++
	template <typename T>
	template <typename U>
	inline U	Serializer<T>::Read()
	{
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
	```
