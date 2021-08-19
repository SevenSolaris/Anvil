#ifndef NBT_TAGS_HEADER_FILE
#define NBT_TAGS_HEADER_FILE

// https://minecraft.fandom.com/wiki/NBT_format

#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <variant>
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <algorithm>


namespace nbt
{

	enum class tag : unsigned char
	{
		NONE = 0,
		BYTE = 1,
		SHORT = 2,
		INT = 3,
		LONG = 4,
		FLOAT = 5,
		DOUBLE = 6,
		BYTEARRAY = 7,
		STRING = 8,
		LIST = 9,
		COMPOUND = 10,
		INTARRAY = 11,
		LONGARRAY = 12,
	};

	using t_byte = int8_t;
	using t_short = int16_t;
	using t_int = int32_t;
	using t_long = int64_t;
	using t_float = float;
	using t_double = double;
	using t_string = std::string;
	using t_bytearray = std::vector<std::byte>;
	using t_intarray = std::vector<int32_t>;
	using t_longarray = std::vector<int64_t>;

	class t_list;
	class t_compound;

	template<typename T>
	struct is_nbt_t
	{
		constexpr static bool value =
			std::is_same<t_byte, T>::value ||		// 1
			std::is_same<t_short, T>::value ||		// 2
			std::is_same<t_int, T>::value ||		// 3
			std::is_same<t_long, T>::value ||		// 4
			std::is_same<t_float, T>::value ||		// 5
			std::is_same<t_double, T>::value ||		// 6
			std::is_same<t_bytearray, T>::value ||	// 7
			std::is_same<t_string, T>::value ||		// 8
			std::is_same<t_list, T>::value ||		// 9
			std::is_same<t_compound, T>::value ||	// 10
			std::is_same<t_intarray, T>::value ||	// 11
			std::is_same<t_longarray, T>::value;	// 12
	};

	template<typename T>
	constexpr tag TagType()
	{
		static_assert(is_nbt_t<T>::value, "Must be NBT tag type.");
		if constexpr (std::is_same<t_byte, T>::value)
			return tag::BYTE;
		if constexpr (std::is_same<t_short, T>::value)
			return tag::SHORT;
		if constexpr (std::is_same<t_int, T>::value)
			return tag::INT;
		if constexpr (std::is_same<t_long, T>::value)
			return tag::LONG;
		if constexpr (std::is_same<t_float, T>::value)
			return tag::FLOAT;
		if constexpr (std::is_same<t_double, T>::value)
			return tag::DOUBLE;
		if constexpr (std::is_same<t_bytearray, T>::value)
			return tag::BYTEARRAY;
		if constexpr (std::is_same<t_string, T>::value)
			return tag::STRING;
		if constexpr (std::is_same<t_list, T>::value)
			return tag::LIST;
		if constexpr (std::is_same<t_compound, T>::value)
			return tag::COMPOUND;
		if constexpr (std::is_same<t_intarray, T>::value)
			return tag::INTARRAY;
		if constexpr (std::is_same<t_longarray, T>::value)
			return tag::LONGARRAY;
		return tag::NONE;
	}

	class t_list
	{
	public:
		// t_list uses std::variant that combines vectors of all tag types, as well as nullptr_t (for setting to null)
		// This allows us to work directly with tag data rather than "boxed" tag data.
		// Notice the commented index on the right hand side.
		// This is also the tag ID for each type.
		// This means that we can get the tag type like so:
		//	tag id = tag(var.index());
		// where var is of type list_variant.
		using list_variant = std::variant<
			std::nullptr_t,				// 0
			std::vector<t_byte>,		// 1
			std::vector<t_short>,		// 2
			std::vector<t_int>,			// 3
			std::vector<t_long>,		// 4
			std::vector<t_float>,		// 5
			std::vector<t_double>,		// 6
			std::vector<t_bytearray>,	// 7
			std::vector<t_string>,		// 8
			std::vector<t_list>,		// 9
			std::vector<t_compound>,	// 10
			std::vector<t_intarray>,	// 11
			std::vector<t_longarray>	// 12
		>; // list_variant

		list_variant data;

		t_list() = default;

		t_list(tag type)
		{
			switch (type)
			{
			case tag::BYTE:
				data = std::vector<t_byte>();
				break;
			case tag::SHORT:
				data = std::vector<t_short>();
				break;
			case tag::INT:
				data = std::vector<t_int>();
				break;
			case tag::LONG:
				data = std::vector<t_long>();
				break;
			case tag::FLOAT:
				data = std::vector<t_float>();
				break;
			case tag::DOUBLE:
				data = std::vector<t_double>();
				break;
			case tag::BYTEARRAY:
				data = std::vector<t_bytearray>();
				break;
			case tag::STRING:
				data = std::vector<t_string>();
				break;
			case tag::LIST:
				data = std::vector<t_list>();
				break;
			case tag::COMPOUND:
				data = std::vector<t_compound>();
				break;
			case tag::INTARRAY:
				data = std::vector<t_intarray>();
				break;
			case tag::LONGARRAY:
				data = std::vector<t_longarray>();
				break;
			default:
				data = nullptr;
				break;
			}
		}

		t_list(const t_list& rhs) = default;
		t_list(t_list&& rhs) = default;

		template<typename T>
		t_list(const std::vector<T>& cdata) : data(cdata)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
		}

		template<typename T>
		t_list(std::vector<T>&& rdata) : data(rdata)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
		}

		t_list& operator=(const t_list& rhs) = default;
		t_list& operator=(t_list&& rhs) = default;

		template<typename T>
		t_list& operator=(const std::vector<T>& rhs)
		{
			this->data = rhs;
			return *this;
		}

		template<typename T>
		t_list& operator=(std::vector<T>&& rhs)
		{
			this->data = rhs;
			return *this;
		}

		bool operator==(const t_list& rhs) const
		{
			return this->data == rhs.data;
		}

		template<typename T>
		[[nodiscard]] std::vector<T>* as_vector()
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
				return &std::get<std::vector<T>>(this->data);
			return nullptr;
		}

		template<typename T>
		[[nodiscard]] const std::vector<T>* as_vector() const
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
				return &std::get<std::vector<T>>(this->data);
			return nullptr;
		}

		// Returns a nullptr upon any kind of failure, including out of range failure.
		template<typename T>
		[[nodiscard]] T* get(size_t index)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
			{
				std::vector<T>& ref = std::get<std::vector<T>>(this->data);
				if (index < ref.size())
					return &ref[index];
			}
			return nullptr;
		}

		template<typename T>
		[[nodiscard]] const T* get(size_t index)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
			{
				std::vector<T>& ref = std::get<std::vector<T>>(this->data);
				if (index < ref.size())
					return &ref[index];
			}
			return nullptr;
		}

		// This function is designed to silently fail.
		template<typename T>
		void set(size_t index, const T& cvalue)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
			{
				std::vector<T>& ref = std::get<std::vector<T>>(this->data);
				if (index < ref.size())
				{
					ref[index] = cvalue;
				}
			}
		}

		// This function is designed to silently fail.
		template<typename T>
		void set(size_t index, T&& rvalue)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
			{
				std::vector<T>& ref = std::get<std::vector<T>>(this->data);
				if (index < ref.size())
				{
					ref[index] = rvalue;
				}
			}
		}

		template<typename ItT, typename T>
		void insert(ItT at, const T& value)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
			{
				std::vector<T>& ref = std::get<std::vector<T>>(this->data);
				// TODO: Finish
				ref.insert(at, value);
			}
		}

		template<typename ItT, typename T>
		void insert(ItT at, T&& value)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
			{
				std::vector<T>& ref = std::get<std::vector<T>>(this->data);
				// TODO: Finish
				ref.insert(at, value);
			}
		}

		template<typename T>
		void insert(size_t index, const T& value)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
			{
				std::vector<T>& ref = std::get<std::vector<T>>(this->data);
				if (index > ref.size())
					return;
				// TODO: Finish
				ref.insert(ref.begin() + index, value);
			}
		}

		template<typename T>
		void insert(size_t index, T&& value)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
			{
				std::vector<T>& ref = std::get<std::vector<T>>(this->data);
				if (index > ref.size())
					return;
				// TODO: Finish
				ref.insert(ref.begin() + index, value);
			}
		}

		template<typename T>
		void push_back(const T& value)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
			{
				std::vector<T>& ref = std::get<std::vector<T>>(this->data);
				ref.push_back(value);
			}
		}

		template<typename T>
		void push_back(T&& value)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
			{
				std::vector<T>& ref = std::get<std::vector<T>>(this->data);
				ref.push_back(value);
			}
		}

		void erase(size_t index);

		void erase(size_t start, size_t end);

		void clear();

		[[nodiscard]] size_t size() const;

		[[nodiscard]] tag type() const;


	}; // t_list

	using t_variant = std::variant<
		std::nullptr_t, // 0
		t_byte,			// 1
		t_short,		// 2
		t_int,			// 3
		t_long,			// 4
		t_float,		// 5
		t_double,		// 6
		t_bytearray,	// 7
		t_string,		// 8
		t_list,			// 9
		t_compound,		// 10
		t_intarray,		// 11
		t_longarray		// 12
	>; // t_variant

	class t_compound
	{
	public:
		using map = std::vector<std::pair<std::string, t_variant>>;
		map data;

		t_compound() = default;
		t_compound(const map& cdata) : data(cdata) {}
		t_compound(map&& rdata) : data(rdata) {}
		t_compound(const t_compound& rhs) = default;
		t_compound(t_compound&& rhs) = default;

		t_compound& operator=(const map& cdata);

		t_compound& operator=(map&& rdata);
		t_compound& operator=(const t_compound& rhs) = default;
		t_compound& operator=(t_compound&& rhs) = default;

		bool operator==(const t_compound& rhs) const
		{
			return this->data == rhs.data;
		}

		map::iterator operator[](std::string_view key);

		map::const_iterator operator[](std::string_view key) const;

		[[nodiscard]] size_t size() const;

		[[nodiscard]] map::iterator begin()
		{
			return data.begin();
		}

		[[nodiscard]] map::iterator end()
		{
			return data.end();
		}

		[[nodiscard]] map::const_iterator begin() const
		{
			return data.begin();
		}

		[[nodiscard]] map::const_iterator end() const
		{
			return data.end();
		}

		void clear();

		void remove(std::string_view key);

		[[nodiscard]] tag get_type(std::string_view key) const;

		[[nodiscard]] map::iterator find(std::string_view key);

		[[nodiscard]] map::const_iterator find(std::string_view key) const;

		template<typename T>
		[[nodiscard]] T* get_if(std::string_view key)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");

			map::iterator found = this->find(key);
			if (found != data.end() && tag(found->second.index()) == TagType<T>())
			{
				T& result = std::get<T>(found->second);
				return &result;
			}
			return nullptr;
		}

		template<typename T>
		void set(std::string_view key, const T& value)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			map::iterator found = this->find(key);
			if (found != this->data.end())
			{
				found->second = value;
			}
			else
			{
				this->data.push_back({ key, value });
			}
		}

		template<typename T>
		void set(std::string_view key, T&& value)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			map::iterator found = this->find(key);
			if (found != this->data.end())
			{
				found->second = value;
			}
			else
			{
				this->data.push_back({ key, value });
			}
		}

	};

	template<typename T>
	T cast_t_var(const t_variant& var)
	{
		static_assert(is_nbt_t<T>::value, "Must be NBT type.");
		if (tag(var.index()) == TagType<T>())
			return std::get<T>(var);
		if constexpr (
			std::is_same<T, t_byte>::value ||
			std::is_same<T, t_short>::value ||
			std::is_same<T, t_int>::value ||
			std::is_same<T, t_long>::value ||
			std::is_same<T, t_float>::value ||
			std::is_same<T, t_double>::value)
		{
			switch (tag(var.index()))
			{
			case tag::BYTE:
				return static_cast<T>(std::get<t_byte>(var));
			case tag::SHORT:
				return static_cast<T>(std::get<t_short>(var));
			case tag::INT:
				return static_cast<T>(std::get<t_int>(var));
			case tag::LONG:
				return static_cast<T>(std::get<t_long>(var));
			case tag::FLOAT:
				return static_cast<T>(std::get<t_float>(var));
			case tag::DOUBLE:
				return static_cast<T>(std::get<t_double>(var));
			}
		}
		return T{};
	}

	t_compound& t_compound::operator=(const map& cdata)
	{
		this->data = cdata;
		return *this;
	}

	t_compound& t_compound::operator=(map&& rdata)
	{
		this->data = rdata;
		return *this;
	}

	t_compound::map::iterator t_compound::operator[](std::string_view key)
	{
		return this->find(key);
	}

	t_compound::map::const_iterator t_compound::operator[](std::string_view key) const
	{
		return this->find(key);
	}

	size_t t_compound::size() const
	{
		return this->data.size();
	}

	void t_compound::clear()
	{
		this->data.clear();
	}

	void t_compound::remove(std::string_view key)
	{
		auto it = this->find(key);
		if (it != this->data.end())
			this->data.erase(it);
	}

	tag t_compound::get_type(std::string_view key) const
	{
		t_compound::map::const_iterator found = this->find(key);
		if (found != this->data.end())
			return tag(found->second.index());
		return tag::NONE;
	}

	t_compound::map::iterator t_compound::find(std::string_view key)
	{
		return std::find_if(data.begin(), data.end(), [key](const std::pair<std::string, t_variant>& val) { return val.first == key; });
	}

	t_compound::map::const_iterator t_compound::find(std::string_view key) const
	{
		return std::find_if(data.begin(), data.end(), [key](const std::pair<std::string, t_variant>& val) { return val.first == key; });
	}

	void t_list::erase(size_t index)
	{

		switch (this->type())
		{
		case tag::BYTE:
		{
			std::vector<t_byte>& ref = std::get<std::vector<t_byte>>(this->data);
			if (index < ref.size())
				ref.erase(ref.begin() + index);
			break;
		}
		case tag::SHORT:
		{
			std::vector<t_short>& ref = std::get<std::vector<t_short>>(this->data);
			if (index < ref.size())
				ref.erase(ref.begin() + index);
			break;
		}
		case tag::INT:
		{
			std::vector<t_int>& ref = std::get<std::vector<t_int>>(this->data);
			if (index < ref.size())
				ref.erase(ref.begin() + index);
			break;
		}
		case tag::LONG:
		{
			std::vector<t_long>& ref = std::get<std::vector<t_long>>(this->data);
			if (index < ref.size())
				ref.erase(ref.begin() + index);
			break;
		}
		case tag::FLOAT:
		{
			std::vector<t_float>& ref = std::get<std::vector<t_float>>(this->data);
			if (index < ref.size())
				ref.erase(ref.begin() + index);
			break;
		}
		case tag::DOUBLE:
		{
			std::vector<t_double>& ref = std::get<std::vector<t_double>>(this->data);
			if (index < ref.size())
				ref.erase(ref.begin() + index);
			break;
		}
		case tag::BYTEARRAY:
		{
			std::vector<t_bytearray>& ref = std::get<std::vector<t_bytearray>>(this->data);
			if (index < ref.size())
				ref.erase(ref.begin() + index);
			break;
		}
		case tag::STRING:
		{
			std::vector<t_string>& ref = std::get<std::vector<t_string>>(this->data);
			if (index < ref.size())
				ref.erase(ref.begin() + index);
			break;
		}
		case tag::LIST:
		{
			std::vector<t_list>& ref = std::get<std::vector<t_list>>(this->data);
			if (index < ref.size())
				ref.erase(ref.begin() + index);
			break;
		}
		case tag::COMPOUND:
		{
			std::vector<t_compound>& ref = std::get<std::vector<t_compound>>(this->data);
			if (index < ref.size())
				ref.erase(ref.begin() + index);
			break;
		}
		case tag::INTARRAY:
		{
			std::vector<t_intarray>& ref = std::get<std::vector<t_intarray>>(this->data);
			if (index < ref.size())
				ref.erase(ref.begin() + index);
			break;
		}
		case tag::LONGARRAY:
		{
			std::vector<t_longarray>& ref = std::get<std::vector<t_longarray>>(this->data);
			if (index < ref.size())
				ref.erase(ref.begin() + index);
			break;
		}
		}
	}

	void t_list::erase(size_t start, size_t end)
	{
		switch (this->type())
		{
		case tag::BYTE:
		{
			std::vector<t_byte>& ref = std::get<std::vector<t_byte>>(this->data);
			if (start < ref.size() && end > start && end <= ref.size())
				ref.erase(ref.begin() + start, ref.begin() + end);
			break;
		}
		case tag::SHORT:
		{
			std::vector<t_short>& ref = std::get<std::vector<t_short>>(this->data);
			if (start < ref.size() && end > start && end <= ref.size())
				ref.erase(ref.begin() + start, ref.begin() + end);
			break;
		}
		case tag::INT:
		{
			std::vector<t_int>& ref = std::get<std::vector<t_int>>(this->data);
			if (start < ref.size() && end > start && end <= ref.size())
				ref.erase(ref.begin() + start, ref.begin() + end);
			break;
		}
		case tag::LONG:
		{
			std::vector<t_long>& ref = std::get<std::vector<t_long>>(this->data);
			if (start < ref.size() && end > start && end <= ref.size())
				ref.erase(ref.begin() + start, ref.begin() + end);
			break;
		}
		case tag::FLOAT:
		{
			std::vector<t_float>& ref = std::get<std::vector<t_float>>(this->data);
			if (start < ref.size() && end > start && end <= ref.size())
				ref.erase(ref.begin() + start, ref.begin() + end);
			break;
		}
		case tag::DOUBLE:
		{
			std::vector<t_double>& ref = std::get<std::vector<t_double>>(this->data);
			if (start < ref.size() && end > start && end <= ref.size())
				ref.erase(ref.begin() + start, ref.begin() + end);
			break;
		}
		case tag::BYTEARRAY:
		{
			std::vector<t_bytearray>& ref = std::get<std::vector<t_bytearray>>(this->data);
			if (start < ref.size() && end > start && end <= ref.size())
				ref.erase(ref.begin() + start, ref.begin() + end);
			break;
		}
		case tag::STRING:
		{
			std::vector<t_string>& ref = std::get<std::vector<t_string>>(this->data);
			if (start < ref.size() && end > start && end <= ref.size())
				ref.erase(ref.begin() + start, ref.begin() + end);
			break;
		}
		case tag::LIST:
		{
			std::vector<t_list>& ref = std::get<std::vector<t_list>>(this->data);
			if (start < ref.size() && end > start && end <= ref.size())
				ref.erase(ref.begin() + start, ref.begin() + end);
			break;
		}
		case tag::COMPOUND:
		{
			std::vector<t_compound>& ref = std::get<std::vector<t_compound>>(this->data);
			if (start < ref.size() && end > start && end <= ref.size())
				ref.erase(ref.begin() + start, ref.begin() + end);
			break;
		}
		case tag::INTARRAY:
		{
			std::vector<t_intarray>& ref = std::get<std::vector<t_intarray>>(this->data);
			if (start < ref.size() && end > start && end <= ref.size())
				ref.erase(ref.begin() + start, ref.begin() + end);
			break;
		}
		case tag::LONGARRAY:
		{
			std::vector<t_longarray>& ref = std::get<std::vector<t_longarray>>(this->data);
			if (start < ref.size() && end > start && end <= ref.size())
				ref.erase(ref.begin() + start, ref.begin() + end);
			break;
		}
		}
	}

	void t_list::clear()
	{
		this->data = nullptr;
	}

	tag t_list::type() const
	{
		return tag(this->data.index());
	}

	size_t t_list::size() const
	{
		switch (this->type())
		{
		case tag::NONE:
			// Since the type is None, the length is 0.
			return 0;
		case tag::BYTE:
			return std::get<std::vector<t_byte>>(this->data).size();
		case tag::SHORT:
			return std::get<std::vector<t_short>>(this->data).size();
		case tag::INT:
			return std::get<std::vector<t_int>>(this->data).size();
		case tag::LONG:
			return std::get<std::vector<t_long>>(this->data).size();
		case tag::FLOAT:
			return std::get<std::vector<t_float>>(this->data).size();
		case tag::DOUBLE:
			return std::get<std::vector<t_double>>(this->data).size();
		case tag::BYTEARRAY:
			return std::get<std::vector<t_bytearray>>(this->data).size();
		case tag::STRING:
			return std::get<std::vector<t_string>>(this->data).size();
		case tag::LIST:
			return std::get<std::vector<t_list>>(this->data).size();
		case tag::COMPOUND:
			return std::get<std::vector<t_compound>>(this->data).size();
		case tag::INTARRAY:
			return std::get<std::vector<t_intarray>>(this->data).size();
		case tag::LONGARRAY:
			return std::get<std::vector<t_longarray>>(this->data).size();
		}
		return 0;
	}

	class nbtin
	{
	public:
		const std::byte* begin;
		const std::byte* end;
		const std::byte* scan;

		nbtin(const std::vector<std::byte>& buffer) : begin(&*buffer.begin()), end(&*buffer.begin() + buffer.size()), scan(&*buffer.begin()) {}
		nbtin(const void* begin, const void* end)
			: begin(static_cast<const std::byte*>(begin)),
			end(static_cast<const std::byte*>(end)),
			scan(static_cast<const std::byte*>(begin)) {}
		nbtin(const void* begin, size_t size)
			: begin(static_cast<const std::byte*>(begin)),
			end(static_cast<const std::byte*>(begin) + size),
			scan(static_cast<const std::byte*>(begin)) {}

		nbtin(const nbtin& cvalue) = default;
		nbtin(nbtin&& rvalue) = delete;

		nbtin& operator=(const nbtin& cvalue) = default;
		nbtin& operator=(nbtin&& rvalue) = delete;

		[[nodiscard]] inline size_t tellg() const
		{
			return scan - begin;
		}

		[[nodiscard]] inline size_t size() const
		{
			return end - begin;
		}

		[[nodiscard]] inline bool ensure(size_t count) const
		{
			return scan + count <= end;
		}

		inline void advance(size_t count)
		{
			scan += count;
		}

		[[nodiscard]] inline bool good() const
		{
			return scan < end;
		}

		nbtin& seekg(size_t count, std::ios_base::seekdir dir = std::ios_base::beg)
		{
			switch (dir)
			{
			case std::ios_base::beg:
				scan = begin + count;
				break;
			case std::ios_base::cur:
				scan = scan + count;
				break;
			case std::ios_base::end:
				scan = end + count;
				break;
			}
			return *this;
		}

		[[nodiscard]] inline uint8_t read_u8()
		{
			if (!ensure(1))
				throw std::runtime_error("Reached end of buffer.");
			uint8_t result = static_cast<uint8_t>(*scan);
			advance(1);
			return result;
		}

		[[nodiscard]] inline int8_t read_i8()
		{
			if (!ensure(1))
				throw std::runtime_error("Reached end of buffer.");
			int8_t result = static_cast<int8_t>(*scan);
			advance(1);
			return result;
		}

		[[nodiscard]] inline uint16_t read_u16()
		{
			if (!ensure(2))
				throw std::runtime_error("Reached end of buffer.");
			uint16_t result = static_cast<uint16_t>(scan[0]) << 8 | static_cast<uint16_t>(scan[1]);
			advance(2);
			return result;
		}

		[[nodiscard]] inline int16_t read_i16()
		{
			if (!ensure(2))
				throw std::runtime_error("Reached end of buffer.");
			int16_t result = static_cast<int16_t>(scan[0]) << 8 | static_cast<int8_t>(scan[1]);
			advance(2);
			return result;
		}

		[[nodiscard]] inline uint32_t read_u32()
		{
			if (!ensure(4))
				throw std::runtime_error("Reached end of buffer.");
			uint32_t result =
				static_cast<uint32_t>(scan[0]) << 24 |
				static_cast<uint32_t>(scan[1]) << 16 |
				static_cast<uint32_t>(scan[2]) << 8 |
				static_cast<uint32_t>(scan[3]);
			advance(4);
			return result;
		}

		[[nodiscard]] inline int32_t read_i32()
		{
			return static_cast<int32_t>(read_u32());
		}

		[[nodiscard]] inline uint64_t read_u64()
		{
			if (!ensure(8))
				throw std::runtime_error("Reached end of buffer.");
			uint64_t result =
				static_cast<uint64_t>(scan[0]) << 56 |
				static_cast<uint64_t>(scan[1]) << 48 |
				static_cast<uint64_t>(scan[2]) << 40 |
				static_cast<uint64_t>(scan[3]) << 32 |
				static_cast<uint64_t>(scan[4]) << 24 |
				static_cast<uint64_t>(scan[5]) << 16 |
				static_cast<uint64_t>(scan[6]) << 8 |
				static_cast<uint64_t>(scan[7]);
			advance(8);
			return result;
		}

		[[nodiscard]] inline int64_t read_i64()
		{
			return static_cast<int64_t>(read_u64());
		}

		[[nodiscard]] inline float read_f32()
		{
			const auto as_int = read_u32();
			float result;
			std::memcpy(&result, &as_int, 4);
			return result;
		}

		[[nodiscard]] inline double read_f64()
		{
			const auto as_int = read_u64();
			double result;
			std::memcpy(&result, &as_int, 8);
			return result;
		}

		[[nodiscard]] inline std::string read_str()
		{
			const auto length = read_u16();
			if (!ensure(length))
				throw std::runtime_error("Reached end of buffer.");
			std::string result(
				reinterpret_cast<const char*>(scan),
				reinterpret_cast<const char*>(scan + length));
			advance(length);
			return result;
		}

		[[nodiscard]] inline tag read_type()
		{
			const auto val = read_u8();
			return tag(val);
		}

	};

	class nbtout
	{
	public:
		std::vector<std::byte> buffer;

		nbtout() { buffer.reserve(64 * 1024); }
		nbtout(size_t reservation) { buffer.reserve(reservation); }

		nbtout(const std::vector<std::byte>& cvalue) : buffer(cvalue) {}
		nbtout(std::vector<std::byte>&& rvalue) : buffer(rvalue) {}

		nbtout(const nbtout& cvalue) = default;
		nbtout(nbtout&& rvalue) = default;

		nbtout& operator=(const nbtout& cvalue) = default;
		nbtout& operator=(nbtout&& rvalue) = default;

		[[nodiscard]] inline size_t size() const
		{
			return buffer.size();
		}

		inline void write(std::byte value)
		{
			buffer.push_back(value);
		}

		inline void write(uint8_t value)
		{
			buffer.push_back(static_cast<std::byte>(value));
		}

		inline void write(int8_t value)
		{
			buffer.push_back(static_cast<std::byte>(value));
		}

		inline void write(uint16_t value)
		{
			buffer.push_back(static_cast<std::byte>((value >> 8) & 0xFF));
			buffer.push_back(static_cast<std::byte>(value & 0xFF));
		}

		inline void write(int16_t value)
		{
			buffer.push_back(static_cast<std::byte>((value >> 8) & 0xFF));
			buffer.push_back(static_cast<std::byte>(value & 0xFF));
		}

		inline void write(uint32_t value)
		{
			buffer.push_back(static_cast<std::byte>((value >> 24) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 16) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 8) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value) & 0xFF));
		}

		inline void write(int32_t value)
		{
			buffer.push_back(static_cast<std::byte>((value >> 24) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 16) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 8) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value) & 0xFF));
		}

		inline void write(uint64_t value)
		{
			buffer.push_back(static_cast<std::byte>((value >> 56) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 48) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 40) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 32) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 24) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 16) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 8) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value) & 0xFF));
		}

		inline void write(int64_t value)
		{
			buffer.push_back(static_cast<std::byte>((value >> 56) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 48) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 40) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 32) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 24) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 16) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value >> 8) & 0xFF));
			buffer.push_back(static_cast<std::byte>((value) & 0xFF));
		}

		inline void write(float value)
		{
			uint32_t as_int;
			std::memcpy(&as_int, &value, 4);
			write(as_int);
		}

		inline void write(double value)
		{
			uint64_t as_int;
			std::memcpy(&as_int, &value, 8);
			write(as_int);
		}

		inline void write(std::string_view value)
		{
			uint16_t length = value.size();
			write(length);
			const char* beg = value.data();
			buffer.insert(buffer.end(),
				reinterpret_cast<const std::byte*>(beg),
				reinterpret_cast<const std::byte*>(beg + length));
		}

		inline void write(tag value)
		{
			write(static_cast<uint8_t>(value));
		}

	};

	enum class proxytype : uint8_t
	{
		NONE = 0,
		BYTE = 1,
		SHORT = 2,
		INT = 3,
		LONG = 4,
		FLOAT = 5,
		DOUBLE = 6,
		BYTEARRAY = 7,
		STRING = 8,
		LIST = 9,
		COMPOUND = 10,
		INTARRAY = 11,
		LONGARRAY = 12,
		PAIR = 13 // If the type is PAIR, that means that it is a std::pair<std::string, t_variant>
	};

	using key_pair = std::pair<std::string, t_variant>;

	// TODO:
	//	Add methods for appending, erasing, and finding.
	class Tag
	{
	private:
		void* ptr;
		proxytype _type;
	public:

		[[nodiscard]] inline tag type() const
		{
			if (_type == proxytype::PAIR)
			{
				pair* p = static_cast<pair*>(ptr);
				return tag(p->second.index());
			}
			return tag(_type);
		}

		[[nodiscard]] inline bool is_pair() const
		{
			return _type == proxytype::PAIR;
		}

		using pair = key_pair;

		Tag() : ptr(nullptr), _type(proxytype::NONE) {}
		Tag(std::byte& value) : ptr(&value), _type(proxytype::BYTE) {}
		Tag(pair& value) : ptr(&value), _type(proxytype::PAIR) {}


		template<typename T>
		Tag(T& value) : ptr(&value), _type(proxytype(TagType<T>()))
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
		}

		Tag(t_variant& value)
		{
			_type = proxytype(value.index());
			switch (_type)
			{
			case proxytype::BYTE:
			{
				auto& ref = std::get<t_byte>(value);
				ptr = &ref;
				break;
			}
			case proxytype::SHORT:
			{
				auto& ref = std::get<t_short>(value);
				ptr = &ref;
				break;
			}
			case proxytype::INT:
			{
				auto& ref = std::get<t_int>(value);
				ptr = &ref;
				break;
			}
			case proxytype::LONG:
			{
				auto& ref = std::get<t_long>(value);
				ptr = &ref;
				break;
			}
			case proxytype::FLOAT:
			{
				auto& ref = std::get<t_float>(value);
				ptr = &ref;
				break;
			}
			case proxytype::DOUBLE:
			{
				auto& ref = std::get<t_double>(value);
				ptr = &ref;
				break;
			}
			case proxytype::BYTEARRAY:
			{
				auto& ref = std::get<t_bytearray>(value);
				ptr = &ref;
				break;
			}
			case proxytype::STRING:
			{
				auto& ref = std::get<t_string>(value);
				ptr = &ref;
				break;
			}
			case proxytype::LIST:
			{
				auto& ref = std::get<t_list>(value);
				ptr = &ref;
				break;
			}
			case proxytype::COMPOUND:
			{
				auto& ref = std::get<t_compound>(value);
				ptr = &ref;
				break;
			}
			case proxytype::INTARRAY:
			{
				auto& ref = std::get<t_intarray>(value);
				ptr = &ref;
				break;
			}
			case proxytype::LONGARRAY:
			{
				auto& ref = std::get<t_longarray>(value);
				ptr = &ref;
				break;
			}
			default:
				ptr = nullptr;
				_type = proxytype::NONE;
				break;
			}
		}

		template<typename T>
		[[nodiscard]] inline T* cast()
		{
			static_assert(is_nbt_t<T>::value || std::is_same<pair, T>::value, "Must be NBT type.");
			if (_type == proxytype::PAIR)
			{
				pair* p = static_cast<pair*>(ptr);
				if constexpr (std::is_same<pair, T>::value)
				{
					return p;
				}
				else if constexpr (is_nbt_t<T>::value)
				{
					if (tag(p->second.index()) == TagType<T>())
						return &std::get<T>(p->second);
				}
			}
			if constexpr (is_nbt_t<T>::value)
			{
				if (_type == TagType<T>())
					return static_cast<T*>(ptr);
			}
			return nullptr;
		}

		// This function will operate on the follow types:
		//	t_bytearray
		//	t_intarray
		//	t_longarray
		//	t_list
		[[nodiscard]] inline Tag operator[](size_t index)
		{
			switch (_type)
			{
			case proxytype::BYTEARRAY:
			{
				t_bytearray* p = static_cast<t_bytearray*>(ptr);
				if (index < p->size())
					return Tag(p->at(index));
				break;
			}
			case proxytype::INTARRAY:
			{
				t_intarray* p = static_cast<t_intarray*>(ptr);
				if (index < p->size())
					return Tag(p->at(index));
				break;
			}
			case proxytype::LONGARRAY:
			{
				t_longarray* p = static_cast<t_longarray*>(ptr);
				if (index < p->size())
					return Tag(p->at(index));
				break;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				// We can create a temporary tag that takes the t_variant
				// pointed to by p->second.
				// This allows us to do tmp[index] to get our final tag.
				// Aren't shortcuts amazing?
				Tag tmp(p->second);
				return tmp[index];
			}
			case proxytype::LIST:
			{
				t_list* p = static_cast<t_list*>(ptr);
				switch (p->type())
				{
				case tag::BYTE:
				{
					auto& ref = std::get<std::vector<t_byte>>(p->data);
					if (index < ref.size())
						return Tag(ref.at(index));
					break;
				}
				case tag::SHORT:
				{
					auto& ref = std::get<std::vector<t_short>>(p->data);
					if (index < ref.size())
						return Tag(ref.at(index));
					break;
				}
				case tag::INT:
				{
					auto& ref = std::get<std::vector<t_int>>(p->data);
					if (index < ref.size())
						return Tag(ref.at(index));
					break;
				}
				case tag::LONG:
				{
					auto& ref = std::get<std::vector<t_long>>(p->data);
					if (index < ref.size())
						return Tag(ref.at(index));
					break;
				}
				case tag::FLOAT:
				{
					auto& ref = std::get<std::vector<t_float>>(p->data);
					if (index < ref.size())
						return Tag(ref.at(index));
					break;
				}
				case tag::DOUBLE:
				{
					auto& ref = std::get<std::vector<t_double>>(p->data);
					if (index < ref.size())
						return Tag(ref.at(index));
					break;
				}
				case tag::BYTEARRAY:
				{
					auto& ref = std::get<std::vector<t_bytearray>>(p->data);
					if (index < ref.size())
						return Tag(ref.at(index));
					break;
				}
				case tag::STRING:
				{
					auto& ref = std::get<std::vector<t_string>>(p->data);
					if (index < ref.size())
						return Tag(ref.at(index));
					break;
				}
				case tag::LIST:
				{
					auto& ref = std::get<std::vector<t_list>>(p->data);
					if (index < ref.size())
						return Tag(ref.at(index));
					break;
				}
				case tag::COMPOUND:
				{
					auto& ref = std::get<std::vector<t_compound>>(p->data);
					if (index < ref.size())
						return Tag(ref.at(index));
					break;
				}
				case tag::INTARRAY:
				{
					auto& ref = std::get<std::vector<t_intarray>>(p->data);
					if (index < ref.size())
						return Tag(ref.at(index));
					break;
				}
				case tag::LONGARRAY:
				{
					auto& ref = std::get<std::vector<t_longarray>>(p->data);
					if (index < ref.size())
						return Tag(ref.at(index));
					break;
				}
				}
				break;
			}
			}
			return Tag();
		}

		[[nodiscard]] inline Tag operator[](const char* key)
		{
			return this->operator[](std::string_view(key));
		}

		// This function will operate on compound tags.
		[[nodiscard]] inline Tag operator[](std::string_view key)
		{
			if (_type == proxytype::COMPOUND)
			{
				t_compound* p = static_cast<t_compound*>(ptr);
				auto it = p->find(key);
				if (it != p->end())
				{
					return Tag(*it);
				}
			}
			else if (_type == proxytype::PAIR)
			{
				pair* p = static_cast<pair*>(ptr);
				Tag tmp(p->second);
				return tmp[key];
			}
			return Tag();
		}

		[[nodiscard]] inline size_t size() const
		{
			switch (_type)
			{
			case proxytype::BYTEARRAY:
			{
				t_bytearray* p = static_cast<t_bytearray*>(ptr);
				return p->size();
			}
			case proxytype::INTARRAY:
			{
				t_intarray* p = static_cast<t_intarray*>(ptr);
				return p->size();
			}
			case proxytype::LONGARRAY:
			{
				t_longarray* p = static_cast<t_longarray*>(ptr);
				return p->size();
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				Tag tmp(p->second);
				return tmp.size();
			}
			case proxytype::COMPOUND:
			{
				t_compound* p = static_cast<t_compound*>(ptr);
				return p->data.size();
			}
			case proxytype::LIST:
			{
				t_list* p = static_cast<t_list*>(ptr);
				switch (p->type())
				{
				case tag::BYTE:
				{
					auto& ref = std::get<std::vector<t_byte>>(p->data);
					return ref.size();
				}
				case tag::SHORT:
				{
					auto& ref = std::get<std::vector<t_short>>(p->data);
					return ref.size();
				}
				case tag::INT:
				{
					auto& ref = std::get<std::vector<t_int>>(p->data);
					return ref.size();
				}
				case tag::LONG:
				{
					auto& ref = std::get<std::vector<t_long>>(p->data);
					return ref.size();
				}
				case tag::FLOAT:
				{
					auto& ref = std::get<std::vector<t_float>>(p->data);
					return ref.size();
				}
				case tag::DOUBLE:
				{
					auto& ref = std::get<std::vector<t_double>>(p->data);
					return ref.size();
				}
				case tag::BYTEARRAY:
				{
					auto& ref = std::get<std::vector<t_bytearray>>(p->data);
					return ref.size();
				}
				case tag::STRING:
				{
					auto& ref = std::get<std::vector<t_string>>(p->data);
					return ref.size();
				}
				case tag::LIST:
				{
					auto& ref = std::get<std::vector<t_list>>(p->data);
					return ref.size();
				}
				case tag::COMPOUND:
				{
					auto& ref = std::get<std::vector<t_compound>>(p->data);
					return ref.size();
				}
				case tag::INTARRAY:
				{
					auto& ref = std::get<std::vector<t_intarray>>(p->data);
					return ref.size();
				}
				case tag::LONGARRAY:
				{
					auto& ref = std::get<std::vector<t_longarray>>(p->data);
					return ref.size();
				}
				}
				break;
			}
			}
			return 0;
		}

		/// <summary>
		/// The name (if any) of this Tag.
		/// </summary>
		/// <returns>Name of tag or empty string.</returns>
		[[nodiscard]] inline std::string_view name() const
		{
			if (_type == proxytype::PAIR)
			{
				pair* p = static_cast<pair*>(ptr);
				return p->first;
			}
			return std::string_view();
		}

		/// <summary>
		/// Checks whether this Tag is valid.
		/// </summary>
		[[nodiscard]] inline operator bool() const
		{
			return ptr != nullptr && _type != proxytype::NONE;
		}

		[[nodiscard]] inline operator t_byte() const
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				t_byte* p = static_cast<t_byte*>(ptr);
				return *p;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				return static_cast<t_byte>(*p);
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				return static_cast<t_byte>(*p);
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				return static_cast<t_byte>(*p);
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				return static_cast<t_byte>(*p);
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				return cast_t_var<t_byte>(p->second);
			}
			}
			return t_byte{};
		}

		[[nodiscard]] inline operator t_short() const
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				std::byte* p = static_cast<std::byte*>(ptr);
				return static_cast<t_short>(*p);
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				return static_cast<t_short>(*p);
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				return static_cast<t_short>(*p);
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				return static_cast<t_short>(*p);
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				return static_cast<t_short>(*p);
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				return cast_t_var<t_short>(p->second);
			}
			}
			return t_short{};
		}

		[[nodiscard]] inline operator t_int() const
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				std::byte* p = static_cast<std::byte*>(ptr);
				return static_cast<t_int>(*p);
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				return static_cast<t_int>(*p);
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				return static_cast<t_int>(*p);
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				return static_cast<t_int>(*p);
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				return static_cast<t_int>(*p);
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				return cast_t_var<t_int>(p->second);
			}
			}
			return t_int{};
		}

		[[nodiscard]] inline operator t_long() const
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				std::byte* p = static_cast<std::byte*>(ptr);
				return static_cast<t_long>(*p);
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				return static_cast<t_long>(*p);
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				return static_cast<t_long>(*p);
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				return static_cast<t_long>(*p);
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				return static_cast<t_long>(*p);
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				return cast_t_var<t_long>(p->second);
			}
			}
			return t_long{};
		}

		[[nodiscard]] inline operator t_float() const
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				std::byte* p = static_cast<std::byte*>(ptr);
				return static_cast<t_float>(*p);
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				return static_cast<t_float>(*p);
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				return static_cast<t_float>(*p);
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				return static_cast<t_float>(*p);
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				return static_cast<t_float>(*p);
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				return cast_t_var<t_float>(p->second);
			}
			}
			return t_float{};
		}

		[[nodiscard]] inline operator t_double() const
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				std::byte* p = static_cast<std::byte*>(ptr);
				return static_cast<t_double>(*p);
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				return static_cast<t_double>(*p);
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				return static_cast<t_double>(*p);
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				return static_cast<t_double>(*p);
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				return static_cast<t_double>(*p);
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				return cast_t_var<t_double>(p->second);
			}
			}
			return t_double{};
		}

		[[nodiscard]] inline operator std::string_view() const
		{
			if (_type == proxytype::STRING)
			{
				std::string* p = static_cast<std::string*>(ptr);
				return *p;
			}
			else if (_type == proxytype::PAIR)
			{
				pair* p = static_cast<pair*>(ptr);
				if (tag(p->second.index()) == tag::STRING)
				{
					return std::get<std::string>(p->second);
				}
			}
			return std::string_view();
		}

		[[nodiscard]] inline operator std::string() const
		{
			if (_type == proxytype::STRING)
			{
				std::string* p = static_cast<std::string*>(ptr);
				return *p;
			}
			else if (_type == proxytype::PAIR)
			{
				pair* p = static_cast<pair*>(ptr);
				if (tag(p->second.index()) == tag::STRING)
				{
					return std::get<std::string>(p->second);
				}
			}
			return std::string();
		}

		template<typename T>
		inline Tag& operator=(const T& rhs)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type");
			if (this->_type == TagType<T>())
			{
				T* p = static_cast<T*>(ptr);
				*p = rhs;
			}
			else if (this->_type == proxytype::PAIR)
			{
				pair* p = static_cast<T*>(ptr);
				p->second = rhs;
			}
			return *this;
		}

		template<typename T>
		inline Tag& operator=(T&& rhs)
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type");
			if (this->_type == TagType<T>())
			{
				T* p = static_cast<T*>(ptr);
				*p = rhs;
			}
			else if (this->_type == proxytype::PAIR)
			{
				pair* p = static_cast<T*>(ptr);
				p->second = rhs;
			}
			return *this;
		}

		inline Tag& operator=(std::byte value)
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				std::byte* p = static_cast<std::byte*>(ptr);
				*p = value;
				break;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				*p = static_cast<t_short>(value);
				break;
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				*p = static_cast<t_int>(value);
				break;
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				*p = static_cast<float>(value);
				break;
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				*p = static_cast<double>(value);
				break;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				p->second = static_cast<t_byte>(value);
				break;
			}
			}
			return *this;
		}

		inline Tag& operator=(t_byte value)
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				t_byte* p = static_cast<t_byte*>(ptr);
				*p = static_cast<t_byte>(value);
				break;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				*p = static_cast<t_short>(value);
				break;
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				*p = static_cast<t_int>(value);
				break;
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				*p = static_cast<float>(value);
				break;
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				*p = static_cast<double>(value);
				break;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				p->second = value;
				break;
			}
			}
			return *this;
		}

		inline Tag& operator=(t_short value)
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				t_byte* p = static_cast<t_byte*>(ptr);
				*p = static_cast<t_byte>(value);
				break;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				*p = static_cast<t_short>(value);
				break;
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				*p = static_cast<t_int>(value);
				break;
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				*p = static_cast<float>(value);
				break;
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				*p = static_cast<double>(value);
				break;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				p->second = value;
				break;
			}
			}
			return *this;
		}

		inline Tag& operator=(t_int value)
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				t_byte* p = static_cast<t_byte*>(ptr);
				*p = static_cast<t_byte>(value);
				break;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				*p = static_cast<t_short>(value);
				break;
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				*p = static_cast<t_int>(value);
				break;
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				*p = static_cast<float>(value);
				break;
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				*p = static_cast<double>(value);
				break;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				p->second = value;
				break;
			}
			}
			return *this;
		}

		inline Tag& operator=(t_long value)
		{
			{
				switch (_type)
				{
				case proxytype::BYTE:
				{
					t_byte* p = static_cast<t_byte*>(ptr);
					*p = static_cast<t_byte>(value);
					break;
				}
				case proxytype::SHORT:
				{
					t_short* p = static_cast<t_short*>(ptr);
					*p = static_cast<t_short>(value);
					break;
				}
				case proxytype::INT:
				{
					t_int* p = static_cast<t_int*>(ptr);
					*p = static_cast<t_int>(value);
					break;
				}
				case proxytype::FLOAT:
				{
					t_float* p = static_cast<t_float*>(ptr);
					*p = static_cast<float>(value);
					break;
				}
				case proxytype::DOUBLE:
				{
					t_double* p = static_cast<t_double*>(ptr);
					*p = static_cast<double>(value);
					break;
				}
				case proxytype::PAIR:
				{
					pair* p = static_cast<pair*>(ptr);
					p->second = value;
					break;
				}
				}
				return *this;
			}
		}

		inline Tag& operator=(t_float value)
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				t_byte* p = static_cast<t_byte*>(ptr);
				*p = static_cast<t_byte>(value);
				break;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				*p = static_cast<t_short>(value);
				break;
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				*p = static_cast<t_int>(value);
				break;
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				*p = static_cast<float>(value);
				break;
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				*p = static_cast<double>(value);
				break;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				p->second = value;
				break;
			}
			}
			return *this;
		}

		inline Tag& operator=(t_double value)
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				t_byte* p = static_cast<t_byte*>(ptr);
				*p = static_cast<t_byte>(value);
				break;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				*p = static_cast<t_short>(value);
				break;
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				*p = static_cast<t_int>(value);
				break;
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				*p = static_cast<float>(value);
				break;
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				*p = static_cast<double>(value);
				break;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				p->second = value;
				break;
			}
			}
			return *this;
		}

		inline Tag& operator=(std::string_view value)
		{
			if (_type == proxytype::STRING)
			{
				t_string* p = static_cast<t_string*>(ptr);
				*p = value;
			}
			else if (_type == proxytype::PAIR)
			{
				pair* p = static_cast<pair*>(ptr);
				p->second = std::string(value);
			}
			return *this;
		}

		[[nodiscard]] inline bool operator==(std::nullptr_t) const
		{
			return ptr == nullptr;
		}

		[[nodiscard]] inline bool operator==(t_byte value) const
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				t_byte* p = static_cast<t_byte*>(ptr);
				return *p == value;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				return *p == value;
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				return *p == value;
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				return *p == value;
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				return *p == value;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				return cast_t_var<t_byte>(p->second) == value;
			}
			}
			return false;
		}

		[[nodiscard]] inline bool operator==(t_short value) const
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				t_byte* p = static_cast<t_byte*>(ptr);
				return *p == value;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				return *p == value;
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				return *p == value;
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				return *p == value;
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				return *p == value;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				return cast_t_var<t_short>(p->second) == value;
			}
			}
			return false;
		}

		[[nodiscard]] inline bool operator==(t_int value) const
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				t_byte* p = static_cast<t_byte*>(ptr);
				return *p == value;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				return *p == value;
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				return *p == value;
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				return *p == value;
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				return *p == value;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				return cast_t_var<t_int>(p->second) == value;
			}
			}
			return false;
		}

		[[nodiscard]] inline bool operator==(t_long value) const
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				t_byte* p = static_cast<t_byte*>(ptr);
				return *p == value;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				return *p == value;
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				return *p == value;
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				return *p == value;
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				return *p == value;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				return cast_t_var<t_long>(p->second) == value;
			}
			}
			return false;
		}

		[[nodiscard]] inline bool operator==(t_float value) const
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				t_byte* p = static_cast<t_byte*>(ptr);
				return *p == value;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				return *p == value;
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				return *p == value;
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				return *p == value;
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				return *p == value;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				return cast_t_var<t_float>(p->second) == value;
			}
			}
			return false;
		}

		[[nodiscard]] inline bool operator==(t_double value) const
		{
			switch (_type)
			{
			case proxytype::BYTE:
			{
				t_byte* p = static_cast<t_byte*>(ptr);
				return *p == value;
			}
			case proxytype::SHORT:
			{
				t_short* p = static_cast<t_short*>(ptr);
				return *p == value;
			}
			case proxytype::INT:
			{
				t_int* p = static_cast<t_int*>(ptr);
				return *p == value;
			}
			case proxytype::FLOAT:
			{
				t_float* p = static_cast<t_float*>(ptr);
				return *p == value;
			}
			case proxytype::DOUBLE:
			{
				t_double* p = static_cast<t_double*>(ptr);
				return *p == value;
			}
			case proxytype::PAIR:
			{
				pair* p = static_cast<pair*>(ptr);
				return cast_t_var<t_double>(p->second) == value;
			}
			}
			return false;
		}

		[[nodiscard]] inline bool operator==(std::string_view value) const
		{
			if (_type == proxytype::STRING)
			{
				std::string* p = static_cast<std::string*>(ptr);
				return *p == value;
			}
			else if (_type == proxytype::PAIR)
			{
				pair* p = static_cast<pair*>(ptr);
				if (tag(p->second.index()) == tag::STRING)
					return std::get<std::string>(p->second) == value;
			}
			return false;
		}

		template<typename T>
		[[nodiscard]] inline bool operator==(const T& rhs) const
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (this->type() == TagType<T>())
			{
				if (this->is_pair())
				{
					const pair* p = static_cast<const pair*>(ptr);
					return std::get<T>(p->second) == rhs;
				}
			}
			return false;
		}

	};

	class NBTree
	{
	public:
		t_variant root;
		Tag root_tag;
		std::string name;


		NBTree() : root(nullptr), name(""), root_tag(Tag()) {}

		NBTree(const t_variant& value, std::string_view tag_name = "") : root(value), name(tag_name), root_tag(Tag(root)) {}
		NBTree(t_variant&& value, std::string_view tag_name = "") : root(value), name(tag_name), root_tag(Tag(root)) {}
		NBTree(t_byte value, std::string_view tag_name = "") : root(value), name(tag_name), root_tag(Tag(root)) {}
		NBTree(t_short value, std::string_view tag_name = "") : root(value), name(tag_name), root_tag(Tag(root)) {}
		NBTree(t_int value, std::string_view tag_name = "") : root(value), name(tag_name), root_tag(Tag(root)) {}
		NBTree(t_long value, std::string_view tag_name = "") : root(value), name(tag_name), root_tag(Tag(root)) {}
		NBTree(t_float value, std::string_view tag_name = "") : root(value), name(tag_name), root_tag(Tag(root)) {}
		NBTree(t_double value, std::string_view tag_name = "") : root(value), name(tag_name), root_tag(Tag(root)) {}

		template<typename T>
		NBTree(const T& value, std::string_view tag_name = "")
			: root(value), name(tag_name), root_tag(Tag(root))
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
		}

		template<typename T>
		NBTree(T&& value, std::string_view tag_name = "")
			: root(value), name(tag_name), root_tag(Tag(root))
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
		}

		NBTree(const NBTree& rhs) = default;
		NBTree(NBTree&& rhs) = default;

		NBTree& operator=(const NBTree& rhs) = default;
		NBTree& operator=(NBTree&& rhs) = default;

		[[nodiscard]] inline size_t size() const
		{
			return this->root_tag.size();
		}

		// We need to be able to get the tag type easily.
		[[nodiscard]] inline tag type()
		{
			return tag(root.index());
		}

		template<typename T>
		[[nodiscard]] inline T* get_if()
		{
			static_assert(is_nbt_t<T>::value, "Must be NBT type.");
			if (type() == TagType<T>())
				return &std::get<T>(root);
			return nullptr;
		}

		[[nodiscard]] inline Tag operator[](size_t index)
		{
			return this->root_tag[index];
		}

		[[nodiscard]] inline Tag operator[](const char* key)
		{
			return this->operator[](std::string_view(key));
		}

		[[nodiscard]] inline Tag operator[](std::string_view key)
		{
			return this->root_tag[key];
		}

	};

	t_byte read_byte(nbtin& in);
	t_short read_short(nbtin& in);
	t_int read_int(nbtin& in);
	t_long read_long(nbtin& in);
	t_float read_float(nbtin& in);
	t_double read_double(nbtin& in);
	t_bytearray read_bytearray(nbtin& in);
	t_string read_string(nbtin& in);
	t_list read_list(nbtin& in);
	t_compound read_compound(nbtin& in);
	t_intarray read_intarray(nbtin& in);
	t_longarray read_longarray(nbtin& in);
	t_variant read_tag(nbtin& in, tag type);

	[[nodiscard]] inline t_byte read_byte(nbtin& in)
	{
		return in.read_i8();
	}

	[[nodiscard]] inline t_short read_short(nbtin& in)
	{
		return in.read_i16();
	}

	[[nodiscard]] inline t_int read_int(nbtin& in)
	{
		return in.read_i32();
	}

	[[nodiscard]] inline t_long read_long(nbtin& in)
	{
		return in.read_i64();
	}

	[[nodiscard]] inline t_float read_float(nbtin& in)
	{
		return in.read_f32();
	}

	[[nodiscard]] inline t_double read_double(nbtin& in)
	{
		return in.read_f64();
	}

	[[nodiscard]] inline t_string read_string(nbtin& in)
	{
		return in.read_str();
	}

	[[nodiscard]] inline t_bytearray read_bytearray(nbtin& in)
	{
		int length = in.read_i32();
		if (!in.ensure(length))
			throw std::runtime_error("Reached end of buffer.");
		t_bytearray result;
		result.reserve(length);
		for (int i = 0; i < length; i++)
		{
			result.push_back(static_cast<std::byte>(in.read_u8()));
		}
		return result;
	}

	[[nodiscard]] inline t_intarray read_intarray(nbtin& in)
	{
		int length = in.read_i32();
		if (!in.ensure(static_cast<size_t>(length) * 4))
			throw std::runtime_error("Reached end of buffer.");
		t_intarray result(length);

		return result;
	}

	[[nodiscard]] inline t_longarray read_longarray(nbtin& in)
	{
		int length = in.read_i32();
		if (!in.ensure(static_cast<size_t>(length) * 8))
			throw std::runtime_error("Reached end of buffer.");
		t_longarray result;
		result.reserve(length);
		for (int i = 0; i < length; i++)
		{
			result.push_back(in.read_i64());
		}
		return result;
	}

	[[nodiscard]] inline t_list read_list(nbtin& in)
	{
		tag list_type = in.read_type();
		int length = in.read_i32();

		if (length == 0 || list_type == tag::NONE)
			return t_list(tag::NONE);

		switch (list_type)
		{
		case tag::BYTE:
		{
			auto data = std::vector<t_byte>();
			data.reserve(length);
			for (int i = 0; i < length; i++)
			{
				data.push_back(in.read_i8());
			}
			return t_list(std::move(data));
		}
		case tag::SHORT:
		{
			auto data = std::vector<t_short>();
			data.reserve(length);
			for (int i = 0; i < length; i++)
			{
				data.push_back(in.read_i16());
			}
			return t_list(std::move(data));
		}
		case tag::INT:
		{
			auto data = std::vector<t_int>();
			data.reserve(length);
			for (int i = 0; i < length; i++)
			{
				data.push_back(in.read_i32());
			}
			return t_list(std::move(data));
		}
		case tag::LONG:
		{
			auto data = std::vector<t_long>();
			data.reserve(length);
			for (int i = 0; i < length; i++)
			{
				data.push_back(in.read_i64());
			}
			return t_list(std::move(data));
		}
		case tag::FLOAT:
		{
			auto data = std::vector<t_float>();
			data.reserve(length);
			for (int i = 0; i < length; i++)
			{
				data.push_back(in.read_f32());
			}
			return t_list(std::move(data));
		}
		case tag::DOUBLE:
		{
			auto data = std::vector<t_double>();
			data.reserve(length);
			for (int i = 0; i < length; i++)
			{
				data.push_back(in.read_f64());
			}
			return t_list(std::move(data));
		}
		case tag::BYTEARRAY:
		{
			auto data = std::vector<t_bytearray>();
			data.reserve(length);
			for (int i = 0; i < length; i++)
			{
				data.push_back(read_bytearray(in));
			}
			return t_list(std::move(data));
		}
		case tag::STRING:
		{
			auto data = std::vector<t_string>();
			data.reserve(length);
			for (int i = 0; i < length; i++)
			{
				data.push_back(in.read_str());
			}
			return t_list(std::move(data));
		}
		case tag::LIST:
		{
			auto data = std::vector<t_list>();
			data.reserve(length);
			for (int i = 0; i < length; i++)
			{
				data.push_back(read_list(in));
			}
			return t_list(std::move(data));
		}
		case tag::COMPOUND:
		{
			auto data = std::vector<t_compound>();
			data.reserve(length);
			for (int i = 0; i < length; i++)
			{
				data.push_back(read_compound(in));
			}
			return t_list(std::move(data));
		}
		case tag::INTARRAY:
		{
			auto data = std::vector<t_intarray>();
			data.reserve(length);
			for (int i = 0; i < length; i++)
			{
				data.push_back(read_intarray(in));
			}
			return t_list(std::move(data));
		}
		case tag::LONGARRAY:
		{
			auto data = std::vector<t_longarray>();
			data.reserve(length);
			for (int i = 0; i < length; i++)
			{
				data.push_back(read_longarray(in));
			}
			return t_list(std::move(data));
		}
		default:
			return t_list(tag::NONE);
		}
	}

	[[nodiscard]] inline t_compound read_compound(nbtin& in)
	{
		std::vector<std::pair<std::string, t_variant>> map;
		tag intype = in.read_type();
		while (intype != tag::NONE)
		{
			std::string key = in.read_str();
			switch (intype)
			{
			case nbt::tag::BYTE:
				map.push_back(std::make_pair(std::move(key), in.read_i8()));
				break;
			case nbt::tag::SHORT:
				map.push_back(std::make_pair(std::move(key), in.read_i16()));
				break;
			case nbt::tag::INT:
				map.push_back(std::make_pair(std::move(key), in.read_i32()));
				break;
			case nbt::tag::LONG:
				map.push_back(std::make_pair(std::move(key), in.read_i64()));
				break;
			case nbt::tag::FLOAT:
				map.push_back(std::make_pair(std::move(key), in.read_f32()));
				break;
			case nbt::tag::DOUBLE:
				map.push_back(std::make_pair(std::move(key), in.read_f64()));
				break;
			case nbt::tag::BYTEARRAY:
				map.push_back(std::make_pair(std::move(key), read_bytearray(in)));
				break;
			case nbt::tag::STRING:
				map.push_back(std::make_pair(std::move(key), in.read_str()));
				break;
			case nbt::tag::LIST:
				map.push_back(std::make_pair(std::move(key), read_list(in)));
				break;
			case nbt::tag::COMPOUND:
				map.push_back(std::make_pair(std::move(key), read_compound(in)));
				break;
			case nbt::tag::INTARRAY:
				map.push_back(std::make_pair(std::move(key), read_intarray(in)));
				break;
			case nbt::tag::LONGARRAY:
				map.push_back(std::make_pair(std::move(key), read_longarray(in)));
				break;
			}
			intype = in.read_type();
		}
		return t_compound(std::move(map));
	}

	[[nodiscard]] inline t_variant read_tag(nbtin& in, tag type)
	{
		switch (type)
		{
		case tag::BYTE:
			return in.read_i8();
		case tag::SHORT:
			return in.read_i16();
		case tag::INT:
			return in.read_i32();
		case tag::LONG:
			return in.read_i64();
		case tag::FLOAT:
			return in.read_f32();
		case tag::DOUBLE:
			return in.read_f64();
		case tag::STRING:
			return in.read_str();
		case tag::BYTEARRAY:
			return read_bytearray(in);
		case tag::LIST:
			return read_list(in);
		case tag::COMPOUND:
			return read_compound(in);
		case tag::INTARRAY:
			return read_intarray(in);
		case tag::LONGARRAY:
			return read_longarray(in);
		}
		return nullptr;
	}

	void write_tag(t_byte value, nbtout& out);
	void write_tag(t_short value, nbtout& out);
	void write_tag(t_int value, nbtout& out);
	void write_tag(t_long value, nbtout& out);
	void write_tag(t_float value, nbtout& out);
	void write_tag(t_double value, nbtout& out);
	void write_tag(const t_bytearray& value, nbtout& out);
	void write_tag(const t_string& value, nbtout& out);
	void write_tag(const t_list& value, nbtout& out);
	void write_tag(const t_compound& value, nbtout& out);
	void write_tag(const t_intarray& value, nbtout& out);
	void write_tag(const t_longarray& value, nbtout& out);
	void write_tag(const t_variant& value, nbtout& out);

	inline void write_tag(t_byte value, nbtout& out)
	{
		out.write(value);
	}

	inline void write_tag(t_short value, nbtout& out)
	{
		out.write(value);
	}

	inline void write_tag(t_int value, nbtout& out)
	{
		out.write(value);
	}

	inline void write_tag(t_long value, nbtout& out)
	{
		out.write(value);
	}

	inline void write_tag(t_float value, nbtout& out)
	{
		out.write(value);
	}

	inline void write_tag(t_double value, nbtout& out)
	{
		out.write(value);
	}

	inline void write_tag(const t_string& value, nbtout& out)
	{
		out.write(value);
	}

	inline void write_tag(const t_bytearray& value, nbtout& out)
	{
		out.write(static_cast<int>(value.size()));
		for (auto v : value)
		{
			out.write(v);
		}
	}

	inline void write_tag(const t_intarray& value, nbtout& out)
	{
		out.write(static_cast<int>(value.size()));
		for (auto v : value)
		{
			out.write(v);
		}
	}

	inline void write_tag(const t_longarray& value, nbtout& out)
	{
		out.write(static_cast<int>(value.size()));
		for (auto v : value)
		{
			out.write(v);
		}
	}

	inline void write_tag(const t_list& value, nbtout& out)
	{
		out.write(value.type());
		switch (value.type())
		{
		case tag::BYTE:
		{
			const std::vector<t_byte>& sub = std::get<std::vector<t_byte>>(value.data);
			out.write(static_cast<int>(sub.size()));
			for (auto v : sub)
			{
				out.write(v);
			}
		}
		case tag::SHORT:
		{
			const std::vector<t_short>& sub = std::get<std::vector<t_short>>(value.data);
			out.write(static_cast<int>(sub.size()));
			for (auto v : sub)
			{
				out.write(v);
			}
		}
		case tag::INT:
		{
			const std::vector<t_int>& sub = std::get<std::vector<t_int>>(value.data);
			out.write(static_cast<int>(sub.size()));
			for (auto v : sub)
			{
				out.write(v);
			}
		}
		case tag::LONG:
		{
			const std::vector<t_long>& sub = std::get<std::vector<t_long>>(value.data);
			out.write(static_cast<int>(sub.size()));
			for (auto v : sub)
			{
				out.write(v);
			}
		}
		case tag::FLOAT:
		{
			const std::vector<t_float>& sub = std::get<std::vector<t_float>>(value.data);
			out.write(static_cast<int>(sub.size()));
			for (auto v : sub)
			{
				out.write(v);
			}
		}
		case tag::DOUBLE:
		{
			const std::vector<t_double>& sub = std::get<std::vector<t_double>>(value.data);
			out.write(static_cast<int>(sub.size()));
			for (auto v : sub)
			{
				out.write(v);
			}
		}
		case tag::BYTEARRAY:
		{
			const std::vector<t_bytearray>& sub = std::get<std::vector<t_bytearray>>(value.data);
			out.write(static_cast<int>(sub.size()));
			for (auto& v : sub)
			{
				write_tag(v, out);
			}
			break;
		}
		case tag::STRING:
		{
			const std::vector<t_string>& sub = std::get<std::vector<t_string>>(value.data);
			out.write(static_cast<int>(sub.size()));
			for (auto& v : sub)
			{
				out.write(v);
			}
			break;
		}
		case tag::LIST:
		{
			const std::vector<t_list>& sub = std::get<std::vector<t_list>>(value.data);
			out.write(static_cast<int>(sub.size()));
			for (auto& v : sub)
			{
				write_tag(v, out);
			}
			break;
		}
		case tag::COMPOUND:
		{
			const std::vector<t_compound>& sub = std::get<std::vector<t_compound>>(value.data);
			out.write(static_cast<int>(sub.size()));
			for (auto& v : sub)
			{
				write_tag(v, out);
			}
			break;
		}
		case tag::INTARRAY:
		{
			const std::vector<t_intarray>& sub = std::get<std::vector<t_intarray>>(value.data);
			out.write(static_cast<int>(sub.size()));
			for (auto& v : sub)
			{
				write_tag(v, out);
			}
			break;
		}
		case tag::LONGARRAY:
		{
			const std::vector<t_longarray>& sub = std::get<std::vector<t_longarray>>(value.data);
			out.write(static_cast<int>(sub.size()));
			for (auto& v : sub)
			{
				write_tag(v, out);
			}
			break;
		}
		default:
			out.write(int(0));
			break;
		}
	}

	inline void write_tag(const t_compound& value, nbtout& out)
	{
		for (auto& v : value.data)
		{
			tag v_type = tag(v.second.index());
			out.write(v_type);
			out.write(v.first);
			write_tag(v.second, out);
		}
		out.write(tag::NONE);
	}

	inline void write_tag(const t_variant& node, nbtout& out)
	{
		switch (tag(node.index()))
		{
		case tag::BYTE:
			out.write(std::get<t_byte>(node));
			break;
		case tag::SHORT:
			out.write(std::get<t_short>(node));
			break;
		case tag::INT:
			out.write(std::get<t_int>(node));
			break;
		case tag::LONG:
			out.write(std::get<t_long>(node));
			break;
		case tag::FLOAT:
			out.write(std::get<t_float>(node));
			break;
		case tag::DOUBLE:
			out.write(std::get<t_double>(node));
			break;
		case tag::BYTEARRAY:
			write_tag(std::get<t_bytearray>(node), out);
			break;
		case tag::STRING:
			out.write(std::get<t_string>(node));
			break;
		case tag::LIST:
			write_tag(std::get<t_list>(node), out);
			break;
		case tag::COMPOUND:
			write_tag(std::get<t_compound>(node), out);
			break;
		case tag::INTARRAY:
			write_tag(std::get<t_intarray>(node), out);
			break;
		case tag::LONGARRAY:
			write_tag(std::get<t_longarray>(node), out);
			break;
		}
	}

	[[nodiscard]] inline NBTree load(nbtin& in)
	{
		tag node_type = in.read_type();
		std::string node_name = in.read_str();
		if (node_type == tag::COMPOUND)
		{
			t_compound node = read_compound(in);
			return NBTree(std::move(node), std::move(node_name));
		}
		else
		{
			t_variant node = read_tag(in, node_type);
			return NBTree(std::move(node), std::move(node_name));
		}
	}

	inline void dump(const NBTree& tree, nbtout& out)
	{
		tag node_type = tag(tree.root.index());
		out.write(node_type);
		out.write(tree.name);
		write_tag(tree.root, out);
	}

}

#endif // NBT_TAGS_HEADER_FILE