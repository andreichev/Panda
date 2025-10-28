#include <iostream>
#include <sstream>

#include "Rain/Rain.hpp"
#include "Rain/Coders/JsonEncoder.hpp"
#include "Rain/Coders/YamlEncoder.hpp"
#include "Rain/Codable.hpp"
#include "Rain/Coders/JsonDecoder.hpp"
#include "Rain/Coders/YamlDecoder.hpp"

struct C : public Rain::Codable {
    std::string text;
    int num;

    RAIN_FIELDS_BEGIN(C)
    RAIN_FIELD(text)
    RAIN_FIELD(num)
    RAIN_FIELDS_END
};

struct B : public Rain::Codable {
    std::string text;
    std::unordered_map<std::string, C> objs;
    float value;

    RAIN_FIELDS_BEGIN(B)
    RAIN_FIELD(text)
    RAIN_FIELD(objs)
    RAIN_FIELD(value)
    RAIN_FIELDS_END
};

struct A : public Rain::Codable {
    std::vector<B> array;
    std::string message;

    RAIN_FIELDS_BEGIN(A)
    RAIN_FIELD(array)
    RAIN_FIELD(message)
    RAIN_FIELDS_END
};

int main() {
    C c1;
    c1.text = "test1";
    c1.num = -1;
    C c2;
    c2.text = "test2";
    c2.num = 1;
    C c3;
    c3.text = "test3";
    c3.num = 10;
    B b1;
    b1.objs["key1"] = c1;
    b1.objs["key2"] = c2;
    b1.objs["key3"] = c3;
    b1.text = "B TEXT";
    b1.value = 3.14;
    A a1;
    a1.array = {b1};
    a1.message = "TEST MESSAGE";

    std::stringstream stream;

    // JSON TEST
    {
        Rain::Encoder *encoder = new Rain::JsonEncoder(true);
        encoder->encode(stream, a1);
        std::cout << stream.str() << std::endl;

        A a2;
        Rain::Decoder *decoder = new Rain::JsonDecoder;
        decoder->decode(stream, a2);

        std::cout << (a1.array[0].objs["key1"].text == a2.array[0].objs["key1"].text) << std::endl;
        std::cout << (a1.array[0].objs["key2"].num == a2.array[0].objs["key2"].num) << std::endl;

        delete encoder;
        delete decoder;
    }

    // YAML TEST
    {
        Rain::Encoder *encoder = new Rain::YamlEncoder;
        encoder->encode(stream, a1);
        std::cout << stream.str() << std::endl;

        A a2;
        Rain::Decoder *decoder = new Rain::YamlDecoder;
        decoder->decode(stream, a2);

        std::cout << (a1.array[0].objs["key1"].text == a2.array[0].objs["key1"].text) << std::endl;
        std::cout << (a1.array[0].objs["key2"].num == a2.array[0].objs["key2"].num) << std::endl;

        delete encoder;
        delete decoder;
    }

    return 0;
}
