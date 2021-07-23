#ifndef NBT_HEADER_FILE
#define NBT_HEADER_FILE

#include<iostream>>

//╔════════════════════════════════════════════════════════╗
//║ Includes                                               ║
//╚════════════════════════════════════════════════════════╝
#pragma region [Includes]

// Things that a sub-class of NbtTag needs to be able to do:
// - Write to stream
// - Read from stream
// - Convert to bytes
// - Convert from bytes

class NbtTag
{
	private:
		unsigned char type_code = 0;
	public:
		union
		{
			unsigned int _uint;
			std::string String;
		};

		NbtTag& operator=(const std::string& str)
		{
			this->String = str;
			this->type_code = 2;
			return *this;
		}

		NbtTag& operator=(unsigned int value)
		{
			this->_uint = value;
			this->type_code = 1;
			return *this;
		}

};


#pragma endregion [Includes]

#endif // NBT_HEADER_FILE

