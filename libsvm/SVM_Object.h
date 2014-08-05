#ifndef _SVM_OBJECT_H_
#define _SVM_OBJECT_H_

#include <memory>
#include <iostream>

#include "SVM_Types.h"
#include "SVM_Exception.h"

namespace Shogun
{
	SVM_EXCEPTION_AUTO(ObjectException, Exception);

	SVM_EXCEPTION_AUTO(ObjectTypeException, ObjectException);

	SVM_EXCEPTION_AUTO(ObjectConversionException, ObjectTypeException);

	SVM_EXCEPTION_AUTO(ObjectReadException, ObjectException);

	class Object;

	typedef std::shared_ptr<Object> ObjectPtr; // TODO: The VM should take ownership of all Objects.

	class Object
	{
	public:
		enum
		{
			NIL,
			BOOLEAN,
			NUMBER,
			ADDRESS,
			STRING,
			USERDATA
		};

		typedef UInt8 DataType;

	public:
		/**
		 * Initialize with nil.
		 */
		Object();

		Object(Bool value);

		Object(Number value);

		Object(UInt32 value);

		Object(const String& value);

		Object(const char* value);

		Object(void* value);

		Object(const Object& other);

		~Object();

		DataType getNativeType() const;

		const char* getTypeName() const;

		Bool isNil() const;

		Bool getBoolean() const;

		Number getNumber() const;

		UInt32 getAddress() const;

		String getString() const;

		void* getUserdata() const;

		void setNativeType(DataType type);

		void setNil();

		void setBoolean(Bool value);

		void setNumber(Number value);

		void setAddress(UInt32 value);

		void setString(const String& value);

		void setUserdata(void* value);

		Bool equals(Object* other) const;

		Bool equals(ObjectPtr other) const;

		Bool tequals(Object* other) const;

		Bool tequals(ObjectPtr other) const;

		String getReadableString() const;

		void writeBinary(std::ostream& stream);

		void readBinary(std::istream& stream);

	private:
		union
		{
			Bool boolean;
			Number number;
			UInt32 address;
			char* string;
			void* userdata;
		} data;

		DataType nativeType;

		void cleanup();
	};

	ObjectPtr createObject();

	ObjectPtr createObject(Bool value);
	
	ObjectPtr createObject(Number value);

	ObjectPtr createObject(UInt32 value);

	ObjectPtr createObject(const String& value);

	ObjectPtr createObject(const char* value);

	ObjectPtr createObject(void* userdata);

	ObjectPtr copyObject(ObjectPtr other);
}

#endif