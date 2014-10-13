#include "SVM_Object.h"
#include "SVM_Exception.h"
#include "SVM_StringUtils.h"
#include "SVM_Opcodes.h"

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

	Object::Object(UInt32 value)
	{
		setAddress(value);
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
			break;

		case NUMBER:
			this->nativeType = NUMBER;
			this->data.number = other.data.number;
			break;

		case ADDRESS:
			this->nativeType = ADDRESS;
			this->data.address = other.data.address;
			break;

		case STRING:
			this->nativeType = STRING;
			this->data.string = strdup(other.data.string);
			break;

		case USERDATA:
			this->nativeType = USERDATA;
			this->data.userdata = other.data.userdata;
			break;

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

		case ADDRESS:
			return false;

		case STRING:
		{
			String lower = this->data.string;
			std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
			return !String(this->data.string).empty();
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
			return this->data.boolean ? 1.f : 0.f;

		case NUMBER:
			return this->data.number;

		case ADDRESS:
			return 0;

		case STRING:
		{
			std::stringstream ss;
			ss << this->data.string;
			Number number;
			ss >> number;
			if (ss.fail() || ss.bad())
				throw ObjectConversionException(FORMAT("Unable to convert from STRING %s to NUMBER", this->data.string));
			return number;
		}

		case USERDATA:
			if (this->data.userdata == 0)
				return 0.f;

			throw ObjectConversionException("Cannot convert from USERDATA to ADDRESS");
		}
	}

	UInt32 Object::getAddress() const
	{
		switch (this->getNativeType())
		{
		default:
			throw ObjectConversionException(FORMAT("Unknown DataType %u", this->getNativeType()));

		case NIL:
			return 0;

		case BOOLEAN:
			throw ObjectConversionException(FORMAT("Unable to convert from BOOLEAN to ADDRESS", this->data.string));

		case NUMBER:
			throw ObjectConversionException(FORMAT("Unable to convert from NUMBER to ADDRESS", this->data.string));

		case ADDRESS:
			return this->data.address;

		case STRING:
		{
			std::stringstream ss;
			ss << this->data.string;
			UInt32 number;
			ss >> number;
			if (ss.fail() || ss.bad())
				throw ObjectConversionException(FORMAT("Unable to convert from STRING %s to ADDRESS", this->data.string));
			return number;
		}

		case USERDATA:
			if (this->data.userdata == 0)
				return 0u;

			throw ObjectConversionException("Cannot convert from USERDATA to ADDRESS");
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

		case ADDRESS:
			return FORMAT("%u", this->data.address);

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

		case ADDRESS:
			setAddress(getAddress());
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

	void Object::setAddress(UInt32 value)
	{
		cleanup();
		this->nativeType = ADDRESS;
		this->data.address = value;
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

			case ADDRESS:
				return this->data.address == 0;

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

			case ADDRESS:
				return false;

			case STRING:
			{
				return this->getBoolean() == other->data.boolean;
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

			case ADDRESS:
				return false;

			case STRING:
			{
				std::stringstream ss;
				ss << this->data.string;
				Number num;
				ss >> num;
				if (ss.fail() || ss.bad())
					return false;
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
				return this->data.boolean == other->getBoolean();
			}

			case NUMBER:
			{
				std::stringstream ss;
				ss << this->data.number;
				return ss.str().compare(other->data.string) == 0;
			}

			case ADDRESS:
				return false;

			case STRING:
				return strcmp(this->data.string, other->data.string) == 0;

			case USERDATA:
				return false;
			}
		}

		case USERDATA:
		{
			switch (myType)
			{
			default:
				return false;

			case NIL:
				return other->data.userdata == 0;

			case USERDATA:
				return this->data.userdata == other->data.userdata;
			}
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

		case ADDRESS:
			return this->data.address == other->data.address;

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
		String result;

		switch (this->getNativeType())
		{
		default:
			throw ObjectConversionException(FORMAT("Unknown DataType %u", this->getNativeType()));

		case NIL:
			result = "nil";
			break;

		case BOOLEAN:
		case NUMBER:
		case USERDATA:
			result = getString();
			break;

		case ADDRESS:
		{
			String op = isOpcode(getAddress()) ? opcodeToString((Opcode)getAddress()) : "?";

			result = FORMAT("%u[%s]", getAddress(), op.c_str());
			break;
		}

		case STRING:
			result = "\"" + getString() + "\"";
			break;
		}

		if (this->debug != 0)
		{
			result.append(FORMAT(" %%%s", this->debug->string.c_str()));
		}

		return result;
	}

	void Object::writeBinary(std::ostream& stream, bool debug)
	{
		if (debug)
		{
			WritableBool hasDebug = this->debug != 0;
			stream.write(reinterpret_cast<char*>(&hasDebug), sizeof(hasDebug));

			if (hasDebug)
			{
				UInt32 len = this->debug->string.size() + 1;
				stream.write(reinterpret_cast<char*>(&len), sizeof(len));
				stream.write(this->debug->string.c_str(), len);
			}
		}

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

		case ADDRESS:
		{
			stream.write(reinterpret_cast<char*>(&(this->data.address)), sizeof(this->data.address));
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

	void Object::readBinary(std::istream& stream, bool debug)
	{
		if (debug)
		{
			WritableBool hasDebug = false;
			stream.read(reinterpret_cast<char*>(&hasDebug), sizeof(hasDebug));

			if (hasDebug)
			{
				UInt32 len;
				stream.read(reinterpret_cast<char*>(&len), sizeof(len));
				char* data = new char[len];
				stream.read(data, len);
				data[len - 1] = '\0';
				this->debug = new DebugInfo();
				this->debug->string = String(data);
				delete[] data;
			}
			else
			{
				this->debug = 0;
			}
		}

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
			setBoolean(value != 0 ? 1 : 0);
			break;
		}

		case NUMBER:
		{
			Number value = 0;
			stream.read(reinterpret_cast<char*>(&value), sizeof(value));
			setNumber(value);
			break;
		}

		case ADDRESS:
		{
			UInt32 value = 0;
			stream.read(reinterpret_cast<char*>(&value), sizeof(value));
			setAddress(value);
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

	void Object::setDebug(Object::DebugInfo* debug)
	{
		this->debug = debug;
	}

	Object::DebugInfo* Object::getDebug()
	{
		return this->debug;
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

	ObjectPtr createObject()
	{
		return std::make_shared<Object>();
	}

	ObjectPtr createObject(Bool value)
	{
		return std::make_shared<Object>(value);
	}

	ObjectPtr createObject(Number value)
	{
		return std::make_shared<Object>(value);
	}

	ObjectPtr createObject(UInt32 value)
	{
		return std::make_shared<Object>(value);
	}

	ObjectPtr createObject(const String& value)
	{
		return std::make_shared<Object>(value);
	}

	ObjectPtr createObject(const char* value)
	{
		return std::make_shared<Object>(value);
	}

	ObjectPtr createObject(void* value)
	{
		return std::make_shared<Object>(value);
	}

	ObjectPtr copyObject(ObjectPtr other)
	{
		Object& obj = *other.get();
		return std::make_shared<Object>(obj);
	}
}