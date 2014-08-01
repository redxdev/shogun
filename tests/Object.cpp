#include <cstdlib>

#include <gtest/gtest.h>

#include <ShogunVM.h>
#include <SVM_Object.h>
#include <SVM_Exception.h>
#include <SVM_StringUtils.h>

using namespace Shogun;

namespace Shogun {
	namespace Tests {
		TEST(ObjectTests, NilObject)
		{
			ObjectPtr object = createObject();
			ASSERT_EQ(Object::NIL, object->getNativeType());
			ASSERT_TRUE(object->isNil());
			ASSERT_EQ(false, object->getBoolean());
			ASSERT_EQ(0.f, object->getNumber());
			ASSERT_TRUE(object->getString().empty());
			ASSERT_EQ(0, object->getUserdata());
			ASSERT_STREQ("nil", object->getReadableString().c_str());
		}

		TEST(ObjectTests, BooleanObject)
		{
			ObjectPtr object = createObject(false);
			ASSERT_EQ(Object::BOOLEAN, object->getNativeType());
			ASSERT_FALSE(object->isNil());
			ASSERT_EQ(false, object->getBoolean());
			ASSERT_EQ(0.f, object->getNumber());
			ASSERT_STREQ("false", object->getString().c_str());
			ASSERT_THROW(object->getUserdata(), ObjectConversionException);
			ASSERT_STREQ("false", object->getReadableString().c_str());

			object = createObject(true);
			ASSERT_EQ(Object::BOOLEAN, object->getNativeType());
			ASSERT_FALSE(object->isNil());
			ASSERT_EQ(true, object->getBoolean());
			ASSERT_EQ(1.f, object->getNumber());
			ASSERT_STREQ("true", object->getString().c_str());
			ASSERT_THROW(object->getUserdata(), ObjectConversionException);
			ASSERT_STREQ("true", object->getReadableString().c_str());
		}

		TEST(ObjectTests, NumberObject)
		{
			Number low = -1000;
			Number high = 1000;
			Number value = low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (high - low)));
			
			std::stringstream ss;
			ss << value;

			RecordProperty("value", ss.str());

			ObjectPtr object = createObject(value);
			ASSERT_EQ(Object::NUMBER, object->getNativeType());
			ASSERT_FALSE(object->isNil());
			ASSERT_EQ(value == 0 ? false : true, object->getBoolean());
			ASSERT_EQ(value, object->getNumber());
			ASSERT_EQ(FORMAT("%f", value), object->getString());
			ASSERT_THROW(object->getUserdata(), ObjectConversionException);
			ASSERT_EQ(FORMAT("%f", value), object->getReadableString());
		}

		TEST(ObjectTests, AddressObject)
		{
			ObjectPtr object = createObject(55u);
			ASSERT_EQ(Object::ADDRESS, object->getNativeType());
			ASSERT_EQ(55u, object->getAddress());
			ASSERT_EQ("@55", object->getReadableString());
			// we don't care about any other comparisons, as the rest are
			// undefined behavior.
		}

		TEST(ObjectTests, StringObject)
		{
			String value = "abcdefghijklmnopqrstuvwxyz1234567890";

			RecordProperty("value", value);

			ObjectPtr object = createObject(value);
			ASSERT_EQ(Object::STRING, object->getNativeType());
			ASSERT_FALSE(object->isNil());
			ASSERT_TRUE(object->getBoolean());
			ASSERT_THROW(object->getNumber(), ObjectConversionException);
			ASSERT_EQ(value, object->getString());
			ASSERT_THROW(object->getUserdata(), ObjectConversionException);
			ASSERT_EQ(FORMAT("\"%s\"", value.c_str()), object->getReadableString());
		}

		TEST(ObjectTests, UserdataObject)
		{
			void* memory = malloc(32);

			ObjectPtr object = createObject(memory);
			ASSERT_EQ(Object::USERDATA, object->getNativeType());
			ASSERT_FALSE(object->isNil());
			ASSERT_TRUE(object->getBoolean());
			ASSERT_THROW(object->getNumber(), ObjectConversionException);
			// TODO: Check getString()
			ASSERT_EQ(memory, object->getUserdata());
			// TODO: Check getReadableString()

			free(memory);
		}

		TEST(ObjectTests, ObjectEquality)
		{
			ObjectPtr nil = createObject();
			ObjectPtr boolean = createObject(true);
			ObjectPtr number = createObject(1.f);
			ObjectPtr string = createObject("true");

			void* memory = malloc(32);
			ObjectPtr userdata = createObject(memory);

			ASSERT_TRUE(nil->equals(nil));
			ASSERT_FALSE(nil->equals(boolean));
			ASSERT_FALSE(nil->equals(number));
			ASSERT_FALSE(nil->equals(string));
			ASSERT_FALSE(nil->equals(userdata));

			ASSERT_FALSE(boolean->equals(nil));
			ASSERT_TRUE(boolean->equals(boolean));
			ASSERT_TRUE(boolean->equals(number));
			ASSERT_TRUE(boolean->equals(string));
			ASSERT_FALSE(boolean->equals(userdata));

			ASSERT_FALSE(number->equals(nil));
			ASSERT_TRUE(number->equals(boolean));
			ASSERT_TRUE(number->equals(number));
			ASSERT_FALSE(number->equals(string));
			ASSERT_FALSE(number->equals(userdata));

			ASSERT_FALSE(string->equals(nil));
			ASSERT_TRUE(string->equals(boolean));
			ASSERT_FALSE(string->equals(number));
			ASSERT_TRUE(string->equals(string));
			ASSERT_FALSE(string->equals(userdata));

			ASSERT_FALSE(userdata->equals(nil));
			ASSERT_FALSE(userdata->equals(boolean));
			ASSERT_FALSE(userdata->equals(number));
			ASSERT_FALSE(userdata->equals(string));
			ASSERT_TRUE(userdata->equals(userdata));

			free(memory);
		}

		TEST(ObjectTests, ObjectTypeEquality)
		{
			ObjectPtr nil = createObject();
			ObjectPtr boolean = createObject(true);
			ObjectPtr number = createObject(1.f);
			ObjectPtr string = createObject("true");

			void* memory = malloc(32);
			ObjectPtr userdata = createObject(memory);

			ASSERT_TRUE(nil->tequals(nil));
			ASSERT_FALSE(nil->tequals(boolean));
			ASSERT_FALSE(nil->tequals(number));
			ASSERT_FALSE(nil->tequals(string));
			ASSERT_FALSE(nil->tequals(userdata));

			ASSERT_FALSE(boolean->tequals(nil));
			ASSERT_TRUE(boolean->tequals(boolean));
			ASSERT_FALSE(boolean->tequals(number));
			ASSERT_FALSE(boolean->tequals(string));
			ASSERT_FALSE(boolean->tequals(userdata));

			ASSERT_FALSE(number->tequals(nil));
			ASSERT_FALSE(number->tequals(boolean));
			ASSERT_TRUE(number->tequals(number));
			ASSERT_FALSE(number->tequals(string));
			ASSERT_FALSE(number->tequals(userdata));

			ASSERT_FALSE(string->tequals(nil));
			ASSERT_FALSE(string->tequals(boolean));
			ASSERT_FALSE(string->tequals(number));
			ASSERT_TRUE(string->tequals(string));
			ASSERT_FALSE(string->tequals(userdata));

			ASSERT_FALSE(userdata->tequals(nil));
			ASSERT_FALSE(userdata->tequals(boolean));
			ASSERT_FALSE(userdata->tequals(number));
			ASSERT_FALSE(userdata->tequals(string));
			ASSERT_TRUE(userdata->tequals(userdata));

			free(memory);
		}
	}
}