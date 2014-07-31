#include "SVM_Object.h"
#include "SVM_Exception.h"
#include "SVM_StringUtils.h"

#include <algorithm>

namespace Shogun
{
	Object::Object()
	{
		setNil();
	}

	Object::Object(Bool value)
	{
		setBoolean(value);
	}

	Object::Object(Number value)
	{
		setNumber(value);
	}

	Object::Object(const String& value)
	{
		setString(value);
	}

	Object::Object(const char* value)
	{
		setString(value);
	}

	Object::Object(void* value)
	{
		setUserdata(value);
	}

	Object::Object(const Object& other)
	{
		switch (other.getNativeType())
		{
		case NIL:
			this->nativeType = NIL;

		case BOOLEAN:
			this->nativeType = BOOLEAN;
			this->data.boolean = other.data.boolean;

		case NUMBER:
			this->nativeType = NUMBER;
			this->data.number = other.data.number;

		case STRING:
			this->nativeType = STRING;
			this->data.string = other.data.string;

		case USERDATA:
			this->nativeType = USERDATA;
			this->data.userdata = other.data.userdata;

		default:
			throw ObjectTypeException(FORMAT("Unknown DataType %u", other.getNativeType()));
		}
	}

	Object::~Object()
	{
		this->cleanup();
	}

	Object::DataType Object::getNativeType() const
	{
		if (this->nativeType == USERDATA && this->data.userdata == 0)
			return NIL;

		return this->nativeType;
	}

	const char* Object::getTypeName() const
	{
		switch (this->getNativeType())
		{
		case NIL:
			return "NIL";

		case BOOLEAN:
			return "BOOLEAN";

		case NUMBER:
			return "NUMBER";

		case STRING:
			return "STRING";

		case USERDATA:
			return "USERDATA";

		default:
			return FORMAT("UNKNOWN[%u]", this->getNativeType()).c_str();
		}
	}

	Bool Object::isNil() const
	{
		return this->getNativeType() == NIL;
	}

	Bool Object::getBoolean() const
	{
		switch (this->getNativeType())
		{
		default:
			throw ObjectConversionException(FORMAT("Unknown DataType %u", this->getNativeType()));

		case NIL:
			return false;

		case BOOLEAN:
			return this->data.boolean;

		case NUMBER:
			return this->data.number != 0;

		case STRING:
		{
			String lower = this->data.string;
			std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
			return lower == "true";
		}

		case USERDATA:
			return this->data.userdata != 0;
		}
	}

	Number Object::getNumber() const
	{
		switch (this->getNativeType())
		{
		default:
			throw ObjectConversionException(FORMAT("Unknown DataType %u", this->getNativeType()));

		case NIL:
			return 0;

		case BOOLEAN:
			return this->data.boolean ? 1 : 0;

		case NUMBER:
			return this->data.number;

		case STRING:
		{
			std::stringstream ss;
			ss << this->data.string;
			Number number;
			ss >> number;
			return number;
		}

		case USERDATA:
			return 0;
		}
	}

	String Object::getString() const
	{
		switch (this->getNativeType())
		{
		default:
			throw ObjectConversionException(FORMAT("Unknown DataType %u", this->getNativeType()));

		case NIL:
			return "";

		case BOOLEAN:
			return this->data.boolean ? "true" : "false";

		case NUMBER:
			return FORMAT("%f", this->data.number);

		case STRING:
			return this->data.string;

		case USERDATA:
		{
			std::stringstream ss;
			ss << this->data.userdata;
			return ss.str();
		}
		}
	}

	void* Object::getUserdata() const
	{
		switch (this->getNativeType())
		{
		default:
			throw ObjectConversionException(FORMAT("Cannot convert DataType %u to USERDATA", this->getNativeType()));

		case NIL:
			return 0;

		case USERDATA:
			return this->data.userdata;
		}
	}

	void Object::setNativeType(DataType type)
	{
		switch (type)
		{
		default:
			throw ObjectConversionException(FORMAT("Unknown DataType %u", this->getNativeType()));

		case NIL:
			setNil();
			break;

		case BOOLEAN:
			setBoolean(getBoolean());
			break;

		case NUMBER:
			setNumber(getNumber());
			break;

		case STRING:
			setString(getString());
			break;

		case USERDATA:
			throw ObjectTypeException("Cannot set native type to USERDATA");
		}
	}

	void Object::setNil()
	{
		cleanup();
		this->nativeType = NIL;
	}

	void Object::setBoolean(Bool value)
	{
		cleanup();
		this->nativeType = BOOLEAN;
		this->data.boolean = value;
	}

	void Object::setNumber(Number value)
	{
		cleanup();
		this->nativeType = NUMBER;
		this->data.number = value;
	}

	void Object::setString(const String& value)
	{
		cleanup();
		this->nativeType = STRING;
		this->data.string = strdup(value.c_str());
	}

	void Object::setUserdata(void* value)
	{
		cleanup();
		this->nativeType = USERDATA;
		this->data.userdata = value;
	}

	Bool Object::equals(Object* other) const
	{
		DataType otherType = (other == 0) ? NIL : other->getNativeType();
		DataType myType = this->getNativeType();
		switch (otherType)
		{
		case NIL:
		{
			switch (myType)
			{
			case NIL:
				return true;

			case BOOLEAN:
				return this->data.boolean == false;

			case NUMBER:
				return this->data.number == 0;

			case USERDATA:
				return this->data.userdata == 0;
			}
		}

		case BOOLEAN:
		{
			switch (myType)
			{
			case NIL:
				return other->data.boolean == false;

			case BOOLEAN:
				return this->data.boolean == other->data.boolean;

			case NUMBER:
				return (this->data.number != 0) == other->data.boolean;

			case STRING:
			{
				String lower = this->data.string;
				std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
				return (lower == "true") == other->data.boolean;
			}

			case USERDATA:
				return false;
			}
		}

		case NUMBER:
		{
			switch (myType)
			{
			case NIL:
				return other->data.number == 0;

			case BOOLEAN:
				return this->data.boolean == (other->data.number != 0);

			case NUMBER:
				return this->data.number == other->data.number;

			case STRING:
			{
				std::stringstream ss;
				ss << this->data.string;
				Number num;
				ss >> num;
				return num == other->data.number;
			}

			case USERDATA:
				return false;
			}
		}

		case STRING:
		{
			switch (myType)
			{
			case NIL:
				return strcmp("", other->data.string) == 0;

			case BOOLEAN:
			{
				String lower = other->data.string;
				std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
				return this->data.boolean == (lower == "true");
			}

			case NUMBER:
			{
				std::stringstream ss;
				ss << this->data.number;
				return ss.str().compare(other->data.string) == 0;
			}

			case STRING:
				return strcmp(this->data.string, other->data.string) == 0;

			case USERDATA:
				return false;
			}
		}

		case USERDATA:
		{
			if (myType != USERDATA)
				return false;

			return this->data.userdata == other->data.userdata;
		}
		}

		throw ObjectConversionException(FORMAT("Unknown DataType %u or %u", myType, otherType));
	}

	Bool Object::equals(ObjectPtr other) const
	{
		return this->equals(other.get());
	}

	Bool Object::tequals(Object* other) const
	{
		DataType myType = this->getNativeType();
		DataType otherType = other == 0 ? NIL : other->getNativeType();
		
		if (myType != otherType)
			return false;

		switch (myType)
		{
		case NIL:
			return true;

		case BOOLEAN:
			return this->data.boolean == other->data.boolean;

		case NUMBER:
			return this->data.number == other->data.number;

		case STRING:
			return strcmp(this->data.string, other->data.string) == 0;

		case USERDATA:
			return this->data.userdata == other->data.userdata;
		}

		throw ObjectConversionException(FORMAT("Unknown DataType %u or %u", myType, otherType));
	}

	Bool Object::tequals(ObjectPtr other) const
	{
		return this->tequals(other.get());
	}

	String Object::getReadableString() const
	{
		switch (this->getNativeType())
		{
		default:
			throw ObjectConversionException(FORMAT("Unknown DataType %u", this->getNativeType()));

		case NIL:
			return "nil";

		case BOOLEAN:
		case NUMBER:
		case USERDATA:
			return getString();

		case STRING:
			return "\"" + getString() + "\"";
		}
	}

	void Object::writeBinary(std::ostream& stream)
	{
		stream.write(reinterpret_cast<char*>(&this->nativeType), sizeof(this->nativeType)); // write type

		switch (this->getNativeType())
		{
		case NIL:
			break;

		case BOOLEAN:
		{
			WritableBool value = this->data.boolean;
			stream.write(reinterpret_cast<char*>(&value), sizeof(value));
			break;
		}

		case NUMBER:
		{
			stream.write(reinterpret_cast<char*>(&(this->data.number)), sizeof(this->data.number));
			break;
		}

		case STRING:
		{
			UInt32 len = strlen(this->data.string) + 1;
			stream.write(reinterpret_cast<char*>(&len), sizeof(len));
			stream.write(this->data.string, len);
			break;
		}

		case USERDATA:
		{
			throw ObjectTypeException("Unable to write Object of type USERDATA");
		}
		}
	}

	void Object::readBinary(std::istream& stream)
	{
		DataType type;
		stream.read(reinterpret_cast<char*>(&type), sizeof(type));

		switch (type)
		{
		default:
			throw ObjectReadException(FORMAT("Unknown DataType %u read from stream", type));
			break;

		case NIL:
			setNil();
			break;

		case BOOLEAN:
		{
			WritableBool value = false;
			stream.read(reinterpret_cast<char*>(&value), sizeof(value));
			setBoolean(value);
			break;
		}

		case NUMBER:
		{
			Number value = 0;
			stream.read(reinterpret_cast<char*>(&value), sizeof(value));
			setNumber(value);
			break;
		}

		case STRING:
		{
			UInt32 len;
			stream.read(reinterpret_cast<char*>(&len), sizeof(len));
			char* data = new char[len];
			stream.read(data, len);
			data[len - 1] = '\0';
			setString(String(data));
			delete[] data;
			break;
		}
		}
	}

	void Object::cleanup()
	{
		switch (this->nativeType)
		{
		default:
			break;

		case STRING:
			free(this->data.string);
			this->data.string = 0;
			break;
		}
	}
}